#include "handshake.h"
#include "name_generator/NameGenerator.h"
#include "SyncCout.h"
#include "choose_out.h"

#include <ctime>
#include <iostream>
#include <random>
#include <chrono>
#include <thread>  // for waiting only, no high-level thread tools were used
#include <sys/wait.h>

int res;
int duration;

struct Header {
    friend std::ostream &operator<<(std::ostream& os, const Header&) {
        return os << time(nullptr) << " Smoker with resource " << res << ": ";
    }
} header;

std::unique_ptr<std::ostream> cout_ptr;

void wait_() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(duration * 1ms);
}


void work() {
    SyncCout cout(*cout_ptr, sync_cout_sem_name);
    cout << sync_flags::lock << header << "initialization is complete\n" << sync_flags::unlock;
    Pipe<int> request_pipe = pipes::get<int>(request_pipe_seed);
    Pipe<None> response_pipe = pipes::get<None>(join(response_pipe_seed_template, res));

    while (true) {
        cout << sync_flags::lock << header << "requesting resources\n" << sync_flags::unlock;
        request_pipe->write(res);
        cout << sync_flags::lock << header << "waiting for response\n" << sync_flags::unlock;
        response_pipe->read();
        cout << sync_flags::lock << header << "resources granted, starting smoking\n" << sync_flags::unlock;
        wait_();
        cout << sync_flags::lock << header << "finished smoking\n" << sync_flags::unlock;
    }
}


int main(int argc, char *argv[]) {
    cout_ptr = choose_out(argc, argv);
    for (int i = 0; i < smokers_count; ++i) {
        pid_t p = fork();
        if (p == 0) {
            res = i;
            std::mt19937 rnd(std::random_device{}());
            duration = std::uniform_int_distribution<int>(500, 5000)(rnd);
            handshake(work);
            return 0;
        }
    }

    pid_t wpid;
    int status = 0;
    while ((wpid = wait(&status)) > 0);
    return 0;
}