#include "handshake.h"
#include "name_generator/NameGenerator.h"
#include "choose_out.h"

#include <ctime>
#include <queue>

struct Header {
    friend std::ostream& operator<<(std::ostream &os, const Header&) {
        return os << time(nullptr) << " Resource process: ";
    }
} header;

std::unique_ptr<std::ostream> cout_ptr;

Pipe<int> request_pipe = pipes::get<int>(request_pipe_seed);
Pipe<std::array<int, smokers_count>> resources_pipe = pipes::get<std::array<int, smokers_count>>(resources_pipe_seed);
Pipe<None> response_pipes[smokers_count];

bool check_resources(const std::array<int, smokers_count> &resources, int res) {
    for (int i = 0; i < smokers_count; ++i) {
        if (i == res) {
            continue;
        }
        if (resources[i] == 0) {
            return false;
        }
    }
    return true;
}

void work() {
    std::ostream& cout = *cout_ptr;
    cout << header << "initialization complete\n";

    std::array<int, smokers_count> total{};
    total.fill(0);
    std::queue<int> awaiting_smokers{};

    while (true) {
        cout << header << "waiting for request\n";
        int res = request_pipe->read();
        if (res == producer_resource) {
            cout << header << "producer process is about to deliver new resources\n";
            std::array<int, smokers_count> new_ = resources_pipe->read();
            for (int i = 0; i < smokers_count; ++i) {
                total[i] += new_[i];
            }
            cout << header << "new resources received\n";
        } else {
            cout << header << "smoker with resource " << res << " requested resources\n";
            awaiting_smokers.push(res);
        }

        std::queue<int> not_served{};
        while (!awaiting_smokers.empty()) {
            int r = awaiting_smokers.front();
            awaiting_smokers.pop();
            if (check_resources(total, r)) {
                cout << header << "smoker with resource " << r << " is being served\n";
                response_pipes[r]->write(None{});
                for (int i = 0; i < smokers_count; ++i) {
                    if (i == res) {
                        continue;
                    }
                    --total[i];
                }
            } else {
                not_served.push(r);
            }
        }
        while (!not_served.empty()) {
            awaiting_smokers.push(not_served.front());
            not_served.pop();
        }
    }
}

int main(int argc, char *argv[]) {
    cout_ptr = choose_out(argc, argv);
    for (int i = 0; i < smokers_count; ++i) {
        response_pipes[i] = pipes::get<None>(join(response_pipe_seed_template, i));
    }
    handshake(work);
    return 0;
}
