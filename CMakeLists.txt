cmake_minimum_required(VERSION 3.10)
project(someip_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Library
add_library(someip
    src/transport.cpp
    src/service_discovery.cpp
    src/message_router.cpp
)

target_include_directories(someip PUBLIC include)

# Examples
add_executable(server_example examples/server_example.cpp)
target_link_libraries(server_example PRIVATE someip pthread)

add_executable(client_example examples/client_example.cpp)
target_link_libraries(client_example PRIVATE someip pthread)

# Tests (simple tests using assertions)
add_executable(test_serialization tests/test_serialization.cpp)
target_link_libraries(test_serialization PRIVATE someip pthread)

add_executable(test_endtoend tests/test_endtoend.cpp)
target_link_libraries(test_endtoend PRIVATE someip pthread)

# Install targets (optional)
install(TARGETS someip DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)