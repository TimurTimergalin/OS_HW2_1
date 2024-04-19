#include "get.h"
#include "../constants.h"
#include "../empty.h"

#include <sys/msg.h>

#include <cstring>
#include <limits>

namespace pipes {
    template<class MT>
    class SystemVMqBased : public virtual Base<MT> {
    private:
        int des;
        static const long type = 1;
        struct msg_t {
            long mtype;
            char mtext[sizeof(MT)];

            MT& operator*() {
                return *reinterpret_cast<MT*>(mtext);
            }
        };
    public:
        explicit SystemVMqBased(key_t key, bool new_) {
            des = msgget(key, (new_ ? IPC_CREAT | IPC_EXCL : 0) | 0666);
            if (des < 0) {
                throw std::runtime_error("Unable to open mq");
            }
        }

        MT read() override {
            msg_t res;
            int m = msgrcv(des, &res, sizeof(MT), type, 0);
            if (m < 0) {
                throw std::runtime_error("Unable to read from mq");
            }
            return *res;
        }

        void write(const MT &m) override {
            msg_t res{.mtype = type};
            *res = m;
            int me = msgsnd(des, &res, sizeof(MT), 0);
            if (me < 0) {
                throw std::runtime_error("Unable to write to mq");
            }
        }

        void close() override {

        }

        void unlink() override {
            if (msgctl(des, IPC_RMID, nullptr) < 0) {
                throw std::runtime_error("Unable to unlink mq");
            }
        }
    };

    template<class MT>
    Pipe<MT> get(const std::string& name, bool new_) {
        int val = static_cast<int>(std::hash<std::string>{}(name) % std::numeric_limits<int>::max());
        key_t key = ftok(system_v_keyfile.c_str(), val);
        return std::make_unique<SystemVMqBased<MT>>(key, new_);
    }

    template class Base<int>;
    template class Base<None>;
    template class Base<std::array<int, smokers_count>>;

    template class SystemVMqBased<int>;
    template class SystemVMqBased<None>;
    template class SystemVMqBased<std::array<int, smokers_count>>;

    template Pipe<int>get(const std::string&, bool);
    template Pipe<None>get(const std::string&, bool);
    template Pipe<std::array<int, smokers_count>>get(const std::string&, bool);
}
