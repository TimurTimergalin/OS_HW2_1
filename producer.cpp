#include "handshake.h"

#include <random>
#include <chrono>
#include "thread"  // for waiting only, no high-level thread tools were used

const std::string header = "Producer process: ";

Pipe<int> request_pipe = pipes::get<int>(request_pipe_seed);
Pipe<std::array<int, smokers_count>> resources_pipe = pipes::get<std::array<int, smokers_count>>(resources_pipe_seed);

std::mt19937 rnd(std::random_device{}());
int duration = std::uniform_int_distribution(500, 5000)(rnd);

std::uniform_int_distribution<int> resources_dist(0, smokers_count - 1);

void wait_() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(duration * 1ms);
}

void work() {
    std::ostream& cout = std::cout;
    cout << header << "initialization finished\n";
    while (true) {
        cout << header << "producing resources\n";
        wait_();
        std::array<int, smokers_count> created{};
        created.fill(1);
        int exc = resources_dist(rnd);
        cout << header << "production finished, produced everything except " << exc << "\n";
        created[exc] = 0;
        cout << header << "sending request for rendezvous\n";
        request_pipe->write(producer_resource);
        cout << header << "delivering produced resources\n";
        resources_pipe->write(created);
        cout << header << "delivery finished\n";
    }
}

int main() {
    handshake(work);
}
