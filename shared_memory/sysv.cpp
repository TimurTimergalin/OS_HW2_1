#include "get.h"
#include "../constants.h"
#include "../empty.h"

#include <sys/shm.h>
#include <limits>
#include <semaphore.h>

namespace shm {
    template<class T>
    class SystemV : public virtual Base<T>{
    private:
        int des;
        T* mem;
    public:
        explicit SystemV(key_t key) {
            des = shmget(key, sizeof(T), IPC_CREAT | 0666);
            if (des < 0) {
                throw std::runtime_error("Unable to open shared memory");
            }

            mem = reinterpret_cast<T*>(shmat(des, nullptr, 0));
            if (mem == (T *)-1) {
                throw std::runtime_error("Unable to map shared memory");
            }
        }

        T &operator*() override {
            return *mem;
        }

        T *operator->() override {
            return mem;
        }

        T *get_raw() override {
            return mem;
        }

        void close() override {
            shmdt(mem);
        }

        void unlink() override {
            if (shmctl(des, IPC_RMID, nullptr) < 0) {
                throw std::runtime_error("Unable to unlink shared memory");
            }
        }

        ~SystemV() override {
            shmdt(mem);
        }
    };

    template<class T>
    SharedMemory<T> get(const std::string &name) {
        int val = static_cast<int>(std::hash<std::string>{}(name) % std::numeric_limits<int>::max());
        key_t key = ftok(system_v_keyfile.c_str(), val);
        return std::make_unique<SystemV<T>>(key);
    }

    template
    class Base<int>;

    template
    class Base<None>;

    template
    class Base<std::array<int, smokers_count>>;

    template
    class Base<sem_t>;

    template
    class SystemV<int>;

    template
    class SystemV<None>;

    template
    class SystemV<std::array<int, smokers_count>>;

    template
    class SystemV<sem_t>;

    template SharedMemory<int> get(const std::string &);

    template SharedMemory<None> get(const std::string &);

    template SharedMemory<std::array<int, smokers_count>> get(const std::string &);

    template SharedMemory<sem_t> get(const std::string &);
}
