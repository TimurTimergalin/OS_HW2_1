#ifndef IHW2_1_HANDSHAKE_H
#define IHW2_1_HANDSHAKE_H

#include <csignal>
#include <iostream>

#include "pipe/get.h"
#include "constants.h"
#include "empty.h"


Pipe<pid_t> master_pipe = pipes::get<pid_t>(master_pipe_seed);
Pipe<None> response_master_pipe = pipes::get<None>(response_master_pipe_seed);

void handle_sigint(int) {
    master_pipe->write(getpid());
    exit(0);
}

void handle_sigusr1(int) {
    exit(0);
}


void handshake(void (*action)()) {
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);
    signal(SIGQUIT, handle_sigint);
    signal(SIGUSR1, handle_sigusr1);
    master_pipe->write(getpid());
    response_master_pipe->read();
    try {
        action();
    } catch (const std::exception& e) {
        std::cerr << e.what();
        handle_sigint(SIGINT);
    }
}

#endif //IHW2_1_HANDSHAKE_H
