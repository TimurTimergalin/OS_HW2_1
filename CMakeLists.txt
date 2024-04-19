cmake_minimum_required(VERSION 3.27)
project(ihw2_1)

set(CMAKE_CXX_STANDARD 20)

add_executable(45_master
        semaphore/posix_named.cpp
        pipe/semaphore_based.cpp
        shared_memory/posix.cpp
        master.cpp
        name_generator/NameGenerator.cpp
)

add_executable(45_producer
        semaphore/posix_named.cpp
        pipe/semaphore_based.cpp
        shared_memory/posix.cpp
        producer.cpp
        name_generator/NameGenerator.cpp
)

add_executable(45_smokers
        semaphore/posix_named.cpp
        pipe/semaphore_based.cpp
        shared_memory/posix.cpp
        smokers.cpp
        name_generator/NameGenerator.cpp
)

add_executable(45_resources
        semaphore/posix_named.cpp
        pipe/semaphore_based.cpp
        shared_memory/posix.cpp
        resources.cpp
        name_generator/NameGenerator.cpp
)

add_executable(67_master
        semaphore/posix_unnamed.cpp
        pipe/semaphore_based.cpp
        shared_memory/posix.cpp
        master.cpp
        name_generator/NameGenerator.cpp
)

add_executable(67_producer
        semaphore/posix_unnamed.cpp
        pipe/semaphore_based.cpp
        shared_memory/posix.cpp
        producer.cpp
        name_generator/NameGenerator.cpp
)

add_executable(67_smokers
        semaphore/posix_unnamed.cpp
        pipe/semaphore_based.cpp
        shared_memory/posix.cpp
        smokers.cpp
        name_generator/NameGenerator.cpp
)

add_executable(67_resources
        semaphore/posix_unnamed.cpp
        pipe/semaphore_based.cpp
        shared_memory/posix.cpp
        resources.cpp
        name_generator/NameGenerator.cpp
)

add_executable(8_master
        semaphore/sysv.cpp
        pipe/semaphore_based.cpp
        shared_memory/sysv.cpp
        master.cpp
        name_generator/NameGenerator.cpp
)

add_executable(8_producer
        semaphore/sysv.cpp
        pipe/semaphore_based.cpp
        shared_memory/sysv.cpp
        producer.cpp
        name_generator/NameGenerator.cpp
)

add_executable(8_smokers
        semaphore/sysv.cpp
        pipe/semaphore_based.cpp
        shared_memory/sysv.cpp
        smokers.cpp
        name_generator/NameGenerator.cpp
)

add_executable(8_resources
        semaphore/sysv.cpp
        pipe/semaphore_based.cpp
        shared_memory/sysv.cpp
        resources.cpp
        name_generator/NameGenerator.cpp
)

add_executable(9_master
        semaphore/posix_named.cpp
        pipe/posix_mq_based.cpp
        master.cpp
        name_generator/NameGenerator.cpp
)

add_executable(9_producer
        semaphore/posix_named.cpp
        pipe/posix_mq_based.cpp
        producer.cpp
        name_generator/NameGenerator.cpp
)

add_executable(9_smokers
        semaphore/posix_named.cpp
        pipe/posix_mq_based.cpp
        smokers.cpp
        name_generator/NameGenerator.cpp
)

add_executable(9_resources
        semaphore/posix_named.cpp
        pipe/posix_mq_based.cpp
        resources.cpp
        name_generator/NameGenerator.cpp
)

add_executable(10_master
        semaphore/sysv.cpp
        pipe/sysv_mq_based.cpp
        master.cpp
        name_generator/NameGenerator.cpp
)

add_executable(10_producer
        semaphore/sysv.cpp
        pipe/sysv_mq_based.cpp
        producer.cpp
        name_generator/NameGenerator.cpp
)

add_executable(10_smokers
        semaphore/sysv.cpp
        pipe/sysv_mq_based.cpp
        smokers.cpp
        name_generator/NameGenerator.cpp
)

add_executable(10_resources
        semaphore/sysv.cpp
        pipe/sysv_mq_based.cpp
        resources.cpp
        name_generator/NameGenerator.cpp
)

