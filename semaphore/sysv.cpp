#include "get.h"
#include "../constants.h"

#include <sys/sem.h>

#include <stdexcept>
#include <limits>


namespace sem {
    union semun {
        int val;
        semid_ds *buf;
        unsigned short *array;
        seminfo *buff;
    };

    class SystemV : public virtual Base {
    private:
        int sem;
    public:
        SystemV(key_t key, int init, bool new_) {
            if ((sem = semget(key, 1, (new_ ? IPC_CREAT | IPC_EXCL : 0) | 0666)) < 0) {
                throw std::runtime_error("Unable to open sem");
            }

            if (new_) {
                semctl(sem, 0, SETVAL, semun{.val = init});
            }
        }

        void post() override {
            sembuf arg{0, 1, 0};
            semop(sem, &arg, 1);
        }

        void wait() override {
            sembuf arg{0, -1, 0};
            semop(sem, &arg, 1);
        }

        void close() override {

        }

        void unlink() override {
            int res = semctl(sem, -1, IPC_RMID);
            if (res < 0) {
                throw std::runtime_error("Unable to unlink sem");
            }
        }
    };

    Semaphore get(const std::string& name, int init, bool new_) {
        int val = static_cast<int>(std::hash<std::string>{}(name) % std::numeric_limits<int>::max());
        key_t key = ftok(system_v_keyfile.c_str(), val);
        return std::make_unique<SystemV>(key, init, new_);
    }
}