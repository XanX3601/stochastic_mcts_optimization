set(source_files
    ./src/tactical_wildfire_dynamics/Cell.cpp
    ./src/tactical_wildfire_dynamics/Problem.cpp
    ./src/tactical_wildfire_dynamics/Board.cpp
    ./src/tactical_wildfire_dynamics/Action.cpp
    )

set(test_files
    ./test/tactical_wildfire_dynamics/test_Cell.cpp
    ./test/tactical_wildfire_dynamics/test_Problem.cpp
    ./test/tactical_wildfire_dynamics/test_Board.cpp
    ./test/tactical_wildfire_dynamics/test_Action.cpp
    )

add_library(tactical_wildfire_dynamics ${source_files})

target_link_libraries(tactical_wildfire_dynamics fmt::fmt)

target_include_directories(tactical_wildfire_dynamics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(tests
    ./test/main.cpp
    ${test_files}
    )

target_link_libraries(tests Catch2::Catch2)
target_link_libraries(tests tactical_wildfire_dynamics)

