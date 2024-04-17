#include "get.h"
#include "../empty.h"
#include "../constants.h"

#include <fcntl.h>
#include <array>
#include <sys/mman.h>
#include <semaphore.h>


namespace shm {
    template<class T>
    class Posix : public virtual Base<T> {
    private:
        std::string name;
        int shm;
        T *mem;
        bool closed = false;
    public:
        explicit Posix(const std::string &name) : name(name) {
            shm = shm_open(name.c_str(), O_RDWR | O_CREAT, 0666);
            if (shm < 0) {
                throw std::runtime_error("Unable to open shared memory");
            }

            if (ftruncate(shm, sizeof(T)) < 0) {
                throw std::runtime_error("Unable to truncate shared memory");
            }

            void *mapped = mmap(nullptr, sizeof(T), PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
            if (mapped == MAP_FAILED) {
                throw std::runtime_error("Unable to map shared memory");
            }
            mem = reinterpret_cast<T *>(mapped);
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
            _close();
        }

        void unlink() override {
            if (shm_unlink(name.c_str()) < 0) {
                throw std::runtime_error("Unable to unlink shared memory");
            }
        }

        ~Posix() override {
            _close();
        }

    private:
        void _close() {
            if (!closed) {
                ::close(shm);
                closed = true;
            }
        }
    };

    template<class T>
    SharedMemory<T> get(const std::string &name) {
        return std::make_unique<Posix<T>>(name);
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
    class Posix<int>;

    template
    class Posix<None>;

    template
    class Posix<std::array<int, smokers_count>>;

    template
    class Posix<sem_t>;

    template SharedMemory<int> get(const std::string &);

    template SharedMemory<None> get(const std::string &);

    template SharedMemory<std::array<int, smokers_count>> get(const std::string &);

    template SharedMemory<sem_t> get(const std::string &);
}