set(source_files
    ./src/experiments/grid.cpp
    )

set(test_files
    ./test/experiments/test_grid.cpp
    )

add_executable(experiments
    ./src/experiments/main.cpp
    ${source_files}
    )

target_include_directories(experiments PUBLIC ./include)

target_link_libraries(experiments tactical_wildfire_management)
target_link_libraries(experiments monte_carlo_algorithm)
target_link_libraries(experiments spdlog)
target_link_libraries(experiments cxxopts)

add_executable(test_experiments
    ./test/main.cpp
    ${source_files}
    ${test_files}
    )

target_include_directories(test_experiments PUBLIC ./include)

target_link_libraries(test_experiments Catch2::Catch2)
target_link_libraries(test_experiments tactical_wildfire_management)
target_link_libraries(test_experiments monte_carlo_algorithm)

