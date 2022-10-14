CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(ClienServerEcn)

FIND_PACKAGE(Boost 1.40 COMPONENTS system REQUIRED)
FIND_PACKAGE(Threads REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

SET(CMAKE_CXX_STANDARD 17)

option(WITH_GTEST "Whether to build google test" ON)

add_library(server_lib STATIC Server.h Session.h Core.cpp Core.h Common.hpp json.hpp OrderBook.cpp OrderBook.h Order.h Deal.h User.h)

ADD_EXECUTABLE(Server Server_main.cpp)
target_include_directories(Server PRIVATE ${Boost_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(Server PRIVATE Threads::Threads ${Boost_LIBRARIES} server_lib)

ADD_EXECUTABLE(Client Client_main.cpp Common.hpp json.hpp)
target_include_directories(Client PRIVATE ${Boost_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(Client PRIVATE Threads::Threads ${Boost_LIBRARIES})

if (MSVC)
    set(MY_COMLILE_OPTIONS /W4 /WX /fsanitize=address)
else ()
    set(MY_COMLILE_OPTIONS -Wall -Wextra -pedantic -Werror -Wshadow -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -fsanitize=address)
endif()

target_compile_options(server_lib PRIVATE ${MY_COMLILE_OPTIONS})
target_compile_options(Server PRIVATE ${MY_COMLILE_OPTIONS})
target_compile_options(Client PRIVATE ${MY_COMLILE_OPTIONS})

target_link_options(Server PRIVATE -fsanitize=address)
target_link_options(Client PRIVATE -fsanitize=address)

if(WITH_GTEST)
    include(FetchContent)
    FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/0affd45b62371c1de44064a5a5f8817242208442.zip
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(tests Tests.cpp "Deal.h" "Core.cpp")

    set_target_properties(tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    )

    target_link_libraries(tests gtest_main server_lib)
    target_compile_options(tests PRIVATE ${MY_COMLILE_OPTIONS})
    target_link_options(tests PRIVATE -fsanitize=address)

    include(GoogleTest)
    gtest_discover_tests(tests)
endif()