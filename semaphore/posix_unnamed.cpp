#include <stdexcept>

#include <semaphore.h>
#include <fcntl.h>

#include "get.h"
#include "../shared_memory/get.h"

namespace sem {
    class PosixUnnamed : public virtual Base {
    private:
        SharedMemory<sem_t> mem;
        sem_t *inner;

    public:
        PosixUnnamed(const std::string& shm_name, int init, bool new_) {
            mem = shm::get<sem_t>(shm_name);
            inner = mem->get_raw();
            if (new_ && sem_init(inner, 1, init) < 0) {
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
            mem->close();
        }

        void unlink() override {
            if (sem_destroy(inner) < 0) {
                throw std::runtime_error("Unable to destroy sem");
            }
            mem->unlink();
        }

        ~PosixUnnamed() override {
            mem->close();
        }
    };

    Semaphore get(const std::string& shm_name, int init, bool new_) {
        return std::make_unique<PosixUnnamed>(shm_name, init, new_);
    }
}