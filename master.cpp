#include "pipe/get.h"
#include "empty.h"
#include "constants.h"
#include "name_generator/NameGenerator.h"
#include "semaphore/get.h"
#include "choose_out.h"

#include <unordered_set>
#include <array>
#include <csignal>
#include <iostream>
#include <memory>
#include <ctime>

#include <fcntl.h>

int ignore = close(creat(system_v_keyfile.c_str(), 0666));

std::unique_ptr<std::ostream> cout_ptr;
struct Header {
    friend std::ostream& operator<<(std::ostream& os, const Header&) {
        return os << time(nullptr) << " Master process: ";
    }
} header;

const int total = smokers_count + 2;
std::unordered_set<pid_t> registered{};

Pipe<int> request_pipe = pipes::get<int>(request_pipe_seed, true);
Pipe<std::array<int, smokers_count>> resources_pipe = pipes::get<std::array<int, smokers_count>>(resources_pipe_seed, true);
Pipe<pid_t> master_pipe = pipes::get<pid_t>(master_pipe_seed, true);
Pipe<None> response_master_pipe = pipes::get<None>(response_master_pipe_seed, true);

Pipe<None> response_pipes[smokers_count];
Semaphore sync_cout_sem = sem::get(sync_cout_sem_name, 1, true);

void finish() {
    request_pipe->close();
    request_pipe->unlink();

    resources_pipe->close();
    resources_pipe->unlink();

    master_pipe->close();
    master_pipe->unlink();

    response_master_pipe->close();
    response_master_pipe->unlink();

    for (const auto & response_pipe : response_pipes) {
        response_pipe->close();
        response_pipe->unlink();
    }

    sync_cout_sem->close();
    sync_cout_sem->unlink();
    unlink(system_v_keyfile.c_str());
    exit(0);
}

void handle_subprocess_finish(pid_t finished) {
    std::ostream &cout = *cout_ptr;
    cout << header << "process " << finished << " terminated\n";
    registered.erase(finished);
    if (registered.empty()) {
        cout << header << "all processes have finished, unlinking shared memory\n";
        finish();
    }
}

void handle_sigint(int) {
    std::ostream &cout = *cout_ptr;
    cout << header << "premature termination, terminating other processes and unlinking shared memory\n";
    for (pid_t pid : registered) {
        kill(pid, SIGUSR1);
    }
    finish();
}

int main(int argc, char *argv[]) {
    cout_ptr = choose_out(argc, argv);
    std::ostream &cout = *cout_ptr;
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);
    signal(SIGQUIT, handle_sigint);

    for (int i = 0; i < smokers_count; ++i) {
        response_pipes[i] = pipes::get<None>(join(response_pipe_seed_template, i), true);
    }
    cout << header << "initialization complete\n";

    cout << header << "awaiting registration, expected number of processes: " << total << "\n";
    for (int i = 0; i < total; ++i) {
        pid_t new_ = master_pipe->read();
        cout << header << "process " << new_ << " registered, " << total - i - 1 << " remaining\n";
        registered.insert(new_);
    }

    cout << header << "registration complete, signaling for start\n";
    for (int i = 0; i < total; ++i) {
        response_master_pipe->write(None{});
    }

    cout << header << "finished signaling, waiting for processes to finish\n";

    while (!registered.empty()) {
        handle_subprocess_finish(master_pipe->read());
    }
    return 0;
}