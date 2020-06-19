include_directories(
    ./include
    )

set(source_files
    ./src/tactical_wildfire_dynamics/TWDCell.cpp
    ./src/tactical_wildfire_dynamics/TWDProblem.cpp
    )

set(test_files
    ./test/tactical_wildfire_dynamics/test_TWDCell.cpp
    ./test/tactical_wildfire_dynamics/test_TWDProblem.cpp
    )

add_executable(tests
    ./test/main.cpp
    ${source_files}
    ${test_files}
    )

target_link_libraries(tests Catch2::Catch2)
