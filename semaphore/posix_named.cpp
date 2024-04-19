#include <stdexcept>

#include <semaphore.h>
#include <fcntl.h>

#include "get.h"

namespace sem {
    class PosixNamed : public virtual Base {
    private:
        std::string name;
        sem_t *inner;
        bool closed;
    public:
        PosixNamed(const std::string& name, int init, bool new_) : name(name), closed(false) {
            inner = sem_open(name.c_str(), (new_ ? O_CREAT | O_EXCL : 0), 0666, init);
            if (inner == SEM_FAILED) {
                throw std::runtime_error("Unable to open sem");
            }
        }

        void post() override {
            if (sem_post(inner) < 0) {
                throw std::runtime_error("Unable to increment sem");
            }
        }

        void wait() override {
            if (sem_wait(inner) < 0) {
                throw std::runtime_error("Unable to wait for sem");
            }
        }

        void close() override {
            _close();
        }

        void unlink() override {
            if (sem_unlink(name.c_str()) < 0) {
                throw std::runtime_error("Unable to unlink sem");
            }
        }

        ~PosixNamed() override {
            _close();
        }

    private:
        void _close() {
            if (!closed) {
                sem_close(inner);
                closed = true;
            }
        }
    };

    Semaphore get(const std::string& name, int init, bool new_) {
        return std::make_unique<PosixNamed>(name, init, new_);
    }
}
