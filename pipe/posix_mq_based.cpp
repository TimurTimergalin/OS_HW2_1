#include "get.h"
#include "../name_generator/NameGenerator.h"
#include "../constants.h"
#include "../empty.h"

#include <array>

#include <mqueue.h>

namespace pipes {
    template<class MT>
    class PosixMqBased : public virtual Base<MT> {
    private:
        std::string name;
        mqd_t des;
    public:
        explicit PosixMqBased(const std::string &name, bool new_) : name(name) {
            mq_attr attr{.mq_maxmsg = 5, .mq_msgsize = sizeof(MT)};
            des = mq_open(name.c_str(), O_RDWR | (new_ ? O_CREAT | O_EXCL : 0), 0666, &attr);
            if (des == -1) {
                throw std::runtime_error("Unable to open mq");
            }
        }

        MT read() override {
            char buf[sizeof(MT)];
            int res = mq_receive(des, buf, sizeof(MT), nullptr);
            if (res < 0) {
                throw std::runtime_error("Unable to read from mq");
            }
            return *reinterpret_cast<MT *>(buf);
        }

        void write(const MT &m) override {
            int res = mq_send(
                    des,
                    reinterpret_cast<const char *>(&m),
                    sizeof(MT),
                    0
            );

            if (res < 0) {
                throw std::runtime_error("Unable to write to mq");
            }
        }

        void close() override {
            mq_close(des);
        }

        void unlink() override {
            int res = mq_unlink(name.c_str());
            if (res < 0) {
                throw std::runtime_error("Unable to unlink mq");
            }
        }

        ~PosixMqBased() override {
            mq_close(des);
        }
    };

    template<class MT>
    Pipe<MT> get(const std::string& name, bool new_) {
        return std::make_unique<PosixMqBased<MT>>(join('/', name), new_);
    }

    template class Base<int>;
    template class Base<None>;
    template class Base<std::array<int, smokers_count>>;

    template class PosixMqBased<int>;
    template class PosixMqBased<None>;
    template class PosixMqBased<std::array<int, smokers_count>>;

    template Pipe<int>get(const std::string&, bool);
    template Pipe<None>get(const std::string&, bool);
    template Pipe<std::array<int, smokers_count>>get(const std::string&, bool);
}
