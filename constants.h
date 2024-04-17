#ifndef IHW2_1_SMOKERS_COUNT_H
#define IHW2_1_SMOKERS_COUNT_H

#include <string>

const int smokers_count = 3;
const int producer_resource = -1;
const std::string sync_cout_sem_name = "/sync_cout_sem_name";
const std::string master_pipe_seed = "master_pipe_seed";
const std::string response_master_pipe_seed = "response_master_pipe_seed";
const std::string request_pipe_seed = "request_pipe_seed";
const std::string resources_pipe_seed = "resources_pipe_seed";
const std::string response_pipe_seed_template = "response_pipe_seed";
const std::string system_v_keyfile = "/tmp/os_hw2_file";

#endif //IHW2_1_SMOKERS_COUNT_H
