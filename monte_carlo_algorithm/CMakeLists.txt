find_package(Boost REQUIRED)

set(source_files
    ./src/mca/twm/TranspositionEntry.cpp
    ./src/mca/twm/TranspositionTable.cpp
    ./src/mca/twm/UCT.cpp
    ./src/mca/twm/nrpa/code.cpp
    ./src/mca/twm/nrpa/playout.cpp
    ./src/mca/twm/nrpa/Policy.cpp
    ./src/mca/twm/nrpa/Sequence.cpp
    ./src/mca/twm/nrpa/NRPA.cpp
    ./src/mca/twm/nrpa/Result.cpp
    ./src/mca/twm/GRAVE.cpp
    ./src/mca/twm/snrpa/code.cpp
    ./src/mca/twm/snrpa/playout.cpp
    ./src/mca/twm/snrpa/Policy.cpp
    ./src/mca/twm/snrpa/Sequence.cpp
    ./src/mca/twm/snrpa/SNRPA.cpp
    ./src/mca/twm/snrpa/Result.cpp
    )

set(test_files
    ./test/mca/twm/test_TranspositionEntry.cpp
    ./test/mca/twm/test_TranspositionTable.cpp
    ./test/mca/twm/test_UCT.cpp
    ./test/mca/twm/nrpa/test_code.cpp
    ./test/mca/twm/nrpa/test_playout.cpp
    ./test/mca/twm/nrpa/test_Policy.cpp
    ./test/mca/twm/nrpa/test_Sequence.cpp
    ./test/mca/twm/nrpa/test_NRPA.cpp
    ./test/mca/twm/nrpa/test_Result.cpp
    ./test/mca/twm/test_GRAVE.cpp
    ./test/mca/twm/snrpa/test_code.cpp
    ./test/mca/twm/snrpa/test_playout.cpp
    ./test/mca/twm/snrpa/test_Policy.cpp
    ./test/mca/twm/snrpa/test_Sequence.cpp
    ./test/mca/twm/snrpa/test_SNRPA.cpp
    ./test/mca/twm/snrpa/test_Result.cpp
    )

add_library(monte_carlo_algorithm ${source_files})
target_link_libraries(monte_carlo_algorithm tactical_wildfire_management)

target_link_libraries(monte_carlo_algorithm fmt)

target_include_directories(monte_carlo_algorithm PUBLIC ./include)
target_include_directories(monte_carlo_algorithm PUBLIC ${Boost_INCLUDE_DIRS})

add_executable(test_mca
    ./test/main.cpp
    ${test_files}
    )

target_link_libraries(test_mca Catch2::Catch2)
target_link_libraries(test_mca monte_carlo_algorithm)

target_link_libraries(test_mca fmt)

