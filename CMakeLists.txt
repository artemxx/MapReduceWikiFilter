cmake_minimum_required(VERSION 3.16)
project(MapReduceWikiFilter)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)
find_package(Boost COMPONENTS fiber system filesystem program_options REQUIRED)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fsanitize=address")

add_executable(MapReduceWikiFilter mapreduce.cpp commons.cpp runner.cpp)
target_link_libraries(MapReduceWikiFilter PRIVATE Boost::headers Boost::fiber Boost::system Boost::filesystem Boost::program_options)