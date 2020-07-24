set(source_files
    ./src/twm/Cell.cpp
    ./src/twm/Problem.cpp
    ./src/twm/Board.cpp
    ./src/twm/Action.cpp
    )

set(test_files
    ./test/twm/test_Cell.cpp
    ./test/twm/test_Problem.cpp
    ./test/twm/test_Board.cpp
    ./test/twm/test_Action.cpp
    )

add_library(tactical_wildfire_management ${source_files})

target_link_libraries(tactical_wildfire_management fmt::fmt)

target_include_directories(tactical_wildfire_management PUBLIC ./include)

add_executable(test_tactical_wildfire_management
    ./test/main.cpp
    ${test_files}
    )

target_link_libraries(test_tactical_wildfire_management Catch2::Catch2)
target_link_libraries(test_tactical_wildfire_management
    tactical_wildfire_management
    )

