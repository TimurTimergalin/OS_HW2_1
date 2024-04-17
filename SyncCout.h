#ifndef IHW2_1_SYNCCOUT_H
#define IHW2_1_SYNCCOUT_H

#include <ostream>
#include "semaphore/get.h"

namespace sync_flags {
    struct Lock {
    };
    const Lock lock;

    struct Unlock {
    };
    const Unlock unlock;
}

class SyncCout {
private:
    std::ostream& inner;
    Semaphore sem;
public:
    SyncCout(std::ostream& os, const std::string& sem_name) : inner(os){
        sem = sem::get(sem_name, 1);
    }

    template<class T>
    SyncCout &operator<<(const T &o) {
        inner << o;
        return *this;
    }

    SyncCout &operator<<(const sync_flags::Lock&) {
        sem->wait();
        return *this;
    }

    SyncCout &operator<<(const sync_flags::Unlock&) {
        inner << std::flush;
        sem->post();
        return *this;
    }

    void close() {
        sem->close();
    }

    void unlink() {
        sem->unlink();
    }

    ~SyncCout() {
        close();
    }
};


#endif //IHW2_1_SYNCCOUT_H
