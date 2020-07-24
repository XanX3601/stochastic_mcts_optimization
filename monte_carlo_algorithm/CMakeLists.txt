find_package(Boost REQUIRED)

set(source_files
    ./src/mca/twm/TranspositionEntry.cpp
    ./src/mca/twm/TranspositionTable.cpp
    ./src/mca/twm/UCT.cpp
    )

set(test_files
    ./test/mca/twm/test_TranspositionEntry.cpp
    ./test/mca/twm/test_TranspositionTable.cpp
    ./test/mca/twm/test_UCT.cpp
    )

add_library(monte_carlo_algorithm ${source_files})
target_link_libraries(monte_carlo_algorithm tactical_wildfire_management)

target_include_directories(monte_carlo_algorithm PUBLIC ./include)
target_include_directories(monte_carlo_algorithm PUBLIC ${Boost_INCLUDE_DIRS})

add_executable(test_mca
    ./test/main.cpp
    ${test_files}
    )

target_link_libraries(test_mca Catch2::Catch2)
target_link_libraries(test_mca monte_carlo_algorithm)

