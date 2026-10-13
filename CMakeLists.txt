# cmake_minimum_required(VERSION 3.10)
cmake_minimum_required(VERSION 3.10)
project(someip_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Coverage / sanitizer options ---
option(ENABLE_COVERAGE "Enable GCC/Clang code coverage flags" OFF)
option(ENABLE_SANITIZERS "Enable ASan/UBSan/TSan flags (Clang recommended)" OFF)

if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Enabling coverage flags")
        add_compile_options(-O0 -g -fno-omit-frame-pointer --coverage)
        add_link_options(--coverage)
    else()
        message(WARNING "Coverage requested but compiler is not GCC/Clang: ${CMAKE_CXX_COMPILER_ID}")
    endif()
endif()

if(ENABLE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Enabling sanitizer flags")
        add_compile_options(-O1 -g -fno-omit-frame-pointer -fsanitize=address,undefined,thread)
        add_link_options(-fsanitize=address,undefined,thread)
    else()
        message(WARNING "Sanitizers requested but compiler is not GCC/Clang: ${CMAKE_CXX_COMPILER_ID}")
    endif()
endif()

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

# Taillight standalone examples (WBS-2.6)
add_executable(taillight_server_app examples/taillight_server_app.cpp)
target_link_libraries(taillight_server_app PRIVATE someip)

add_executable(taillight_client_app examples/taillight_client_app.cpp)
target_link_libraries(taillight_client_app PRIVATE someip)

# Tests
add_executable(test_serialization tests/test_serialization.cpp)
target_link_libraries(test_serialization PRIVATE someip)

add_executable(test_endtoend tests/test_endtoend.cpp)
target_link_libraries(test_endtoend PRIVATE someip)

# WBS 2.7 additional tests
add_executable(test_message_router_error tests/test_message_router_error.cpp)
target_link_libraries(test_message_router_error PRIVATE someip)

add_executable(test_transport_retry_backoff tests/test_transport_retry_backoff.cpp)
target_link_libraries(test_transport_retry_backoff PRIVATE someip)

add_executable(test_transport_stress tests/test_transport_stress.cpp)
target_link_libraries(test_transport_stress PRIVATE someip)

# Register tests with CTest so `ctest` discovers them
enable_testing()

# Unit / integration tests
if(TARGET test_serialization)
  add_test(NAME test_serialization COMMAND $<TARGET_FILE:test_serialization>)
endif()
if(TARGET test_endtoend)
  add_test(NAME test_endtoend COMMAND $<TARGET_FILE:test_endtoend>)
endif()
if(TARGET test_message_router_error)
  add_test(NAME test_message_router_error COMMAND $<TARGET_FILE:test_message_router_error>)
endif()
if(TARGET test_transport_retry_backoff)
  add_test(NAME test_transport_retry_backoff COMMAND $<TARGET_FILE:test_transport_retry_backoff>)
endif()
if(TARGET test_transport_stress)
  add_test(NAME test_transport_stress COMMAND $<TARGET_FILE:test_transport_stress>)
endif()

# Install targets
install(TARGETS someip DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
