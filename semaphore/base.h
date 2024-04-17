#ifndef IHW2_1_SEMAPHORE_BASE_H
#define IHW2_1_SEMAPHORE_BASE_H

namespace sem {
    class Base {
    public:
        virtual void post() = 0;
        virtual void wait() = 0;
        virtual void close() = 0;
        virtual void unlink() = 0;
        virtual ~Base() = default;
    };
}
#endif //IHW2_1_SEMAPHORE_BASE_H
