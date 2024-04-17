#ifndef IHW2_1_QUEUE_BASE_H
#define IHW2_1_QUEUE_BASE_H

namespace pipes {
    template<class MT>
    class Base {
    public:
        virtual MT read() = 0;
        virtual void write(const MT& m) = 0;
        virtual void close() = 0;
        virtual void unlink() = 0;
        virtual ~Base() = default;
    };
}

#endif //IHW2_1_QUEUE_BASE_H
