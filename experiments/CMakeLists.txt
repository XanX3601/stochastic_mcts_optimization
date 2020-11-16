set(source_files
    ./src/experiments/twm/grid.cpp
    ./src/experiments/twm/uct.cpp
    ./src/experiments/twm/random.cpp
    ./src/experiments/twm/nrpa.cpp
    ./src/experiments/twm/snrpa.cpp
    )

set(test_files
    ./test/experiments/twm/test_grid.cpp
    ./test/experiments/twm/test_uct.cpp
    ./test/experiments/twm/test_random.cpp
    ./test/experiments/twm/test_nrpa.cpp
    ./test/experiments/twm/test_snrpa.cpp
    )

add_executable(experiments
    ./src/experiments/main.cpp
    ${source_files}
    )

target_include_directories(experiments PUBLIC ./include)

target_link_libraries(experiments tactical_wildfire_management)
target_link_libraries(experiments monte_carlo_algorithm)
target_link_libraries(experiments spdlog)
target_link_libraries(experiments CLI11::CLI11)
target_link_libraries(experiments tabulate)
target_link_libraries(experiments tbb)

set_property(TARGET experiments PROPERTY CXX_STANDARD 17)

add_executable(test_experiments
    ./test/main.cpp
    ${source_files}
    ${test_files}
    )

target_include_directories(test_experiments PUBLIC ./include)
target_include_directories(test_experiments PUBLIC ${TBB_INCLUDE_DIRS})

target_link_libraries(test_experiments Catch2::Catch2)
target_link_libraries(test_experiments tactical_wildfire_management)
target_link_libraries(test_experiments monte_carlo_algorithm)
target_link_libraries(test_experiments spdlog)
target_link_libraries(test_experiments fmt)
target_link_libraries(test_experiments tabulate)
target_link_libraries(test_experiments tbb)

set_property(TARGET test_experiments PROPERTY CXX_STANDARD 17)

