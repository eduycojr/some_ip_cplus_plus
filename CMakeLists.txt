cmake_minimum_required(VERSION 3.10)
project(someip_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Suppress deprecated warnings for inet_addr on Windows
if(WIN32)
    add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

# Library
add_library(someip
    src/transport.cpp
    src/service_discovery.cpp
    src/message_router.cpp
    src/api.cpp
)

target_include_directories(someip PUBLIC include)

# Platform-specific linking
if(WIN32)
    target_link_libraries(someip PUBLIC ws2_32)
else()
    target_link_libraries(someip PUBLIC pthread)
endif()

# Add Client App
add_executable(client_app examples/client_app.cpp)
target_link_libraries(client_app PRIVATE someip)

# Add Server App
add_executable(server_app examples/server_app.cpp)
target_link_libraries(server_app PRIVATE someip)

# Examples
add_executable(server_example examples/server_example.cpp)
target_link_libraries(server_example PRIVATE someip)

add_executable(client_example examples/client_example.cpp)
target_link_libraries(client_example PRIVATE someip)

# Tests
add_executable(test_serialization tests/test_serialization.cpp)
target_link_libraries(test_serialization PRIVATE someip)

add_executable(test_endtoend tests/test_endtoend.cpp)
target_link_libraries(test_endtoend PRIVATE someip)

# Install targets
install(TARGETS someip DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)