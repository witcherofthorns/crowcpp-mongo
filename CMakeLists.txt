# mkdir build && cd build
# conan install .. --output-folder=conan --build=missing
# CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake -DCMAKE_BUILD_TYPE=Release .. -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./conan/conan_toolchain.cmake
# cmake --build . -j4

cmake_minimum_required(VERSION 3.22)

project(backend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CONAN_FOLDER ${PROJECT_SOURCE_DIR}/build/conan)

# conan includes
include(${CONAN_FOLDER}/asio-config.cmake)
include(${CONAN_FOLDER}/mongocxx-config.cmake)
include(${CONAN_FOLDER}/nlohmann_json-config.cmake)
find_package(asio)
find_package(mongocxx)
find_package(nlohmann_json)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/Crow)

file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    asio::asio
    Crow
    mongo::mongocxx_static
    nlohmann_json::nlohmann_json
)

message("-- Include Asio:        ->  " ${asio_INCLUDE_DIRS})
message("-- Include Crow:        ->  " ${PROJECT_SOURCE_DIR}/lib/Crow/include)
message("-- Include Mongo:       ->  " ${mongocxx_INCLUDE_DIRS})
message("-- Include Boost        ->  " ${Boost_INCLUDE_DIRS})
message("-- Include nlohmannjson ->  " ${nlohmann_json_INCLUDE_DIRS})