#ifndef IHW2_1_BASE_H
#define IHW2_1_BASE_H

namespace shm {
    template<class T>
    class Base {
    public:
        virtual T& operator*() = 0;
        virtual T* operator->() = 0;
        virtual T* get_raw() = 0;
        virtual void close() = 0;
        virtual void unlink() = 0;
        virtual ~Base() = default;
    };
}

#endif //IHW2_1_BASE_H
