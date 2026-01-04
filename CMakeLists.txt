cmake_minimum_required(VERSION 3.16)
project(remnawave_node_monitor LANGUAGES CXX)

#C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#BOOST
find_package(Boost 1.75 REQUIRED COMPONENTS
    system
    thread 
    json   
)

#openssl
find_package(OpenSSL REQUIRED)

#executable
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    src/*.hpp
    src/*.h
)
add_executable(remnawave_node_monitor 
    ${SRC_FILES}
)

#Includes
target_include_directories(remnawave_node_monitor PRIVATE
    ${Boost_INCLUDE_DIRS}
)

#Linking
target_link_libraries(remnawave_node_monitor PRIVATE
    Boost::system
    Boost::thread
    Boost::json
    OpenSSL::SSL
    OpenSSL::Crypto
)