#include "get.h"
#include "../semaphore/get.h"
#include "../name_generator/NameGenerator.h"
#include "../shared_memory/get.h"
#include "../empty.h"
#include "../constants.h"

namespace pipes {
    template<class MT>
    class SemaphoreBased : public virtual Base<MT> {
    private:
        Semaphore read_sem;
        Semaphore write_sem;
        SharedMemory<MT> buf;
    public:
        explicit SemaphoreBased(const std::string &seed, bool new_ = false) {
            NameGenerator ng(seed);
            read_sem = sem::get(ng.get(), 0, new_);
            write_sem = sem::get(ng.get(), 1, new_);
            buf = shm::get<MT>(ng.get());
        }

        MT read() override {
            read_sem->wait();
            const MT& res = **buf;
            write_sem->post();
            return res;
        }

        void write(const MT &m) override {
            write_sem->wait();
            **buf = m;
            read_sem->post();
        }

        void close() override {
            _close();
        }

        void unlink() override {
            read_sem->unlink();
            write_sem->unlink();
            buf->unlink();
        }

        ~SemaphoreBased() override {
            _close();
        }

    private:
        void _close() {
            read_sem->close();
            write_sem->close();
            buf->close();
        }
    };

    template<class MT>
    Pipe<MT> get(const std::string& seed, bool new_) {
        return std::make_unique<SemaphoreBased<MT>>(seed, new_);
    }

    template class Base<int>;
    template class Base<None>;
    template class Base<std::array<int, smokers_count>>;

    template class SemaphoreBased<int>;
    template class SemaphoreBased<None>;
    template class SemaphoreBased<std::array<int, smokers_count>>;

    template Pipe<int>get(const std::string&, bool);
    template Pipe<None>get(const std::string&, bool);
    template Pipe<std::array<int, smokers_count>>get(const std::string&, bool);
}
