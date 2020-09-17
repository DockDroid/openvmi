include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_BINARY_DIR}/src)

set(OPENVMID_SOURCES
    ${CMAKE_SOURCE_DIR}/src/openvmi/rpc/message_processor.cpp
    ${CMAKE_SOURCE_DIR}/src/openvmi/rpc/pending_call_cache.cpp
    ${CMAKE_SOURCE_DIR}/src/openvmi/common/fd.cpp
    service/activity_manager_interface.cpp
    service/platform_service.cpp
    service/platform_service_interface.cpp
    service/platform_api_stub.cpp
    service/android_api_skeleton.cpp
    service/message_processor.cpp
    service/local_socket_connection.cpp
    service/host_connector.cpp
    service/daemon.cpp
    service/main.cpp)

add_executable(openvmid ${OPENVMID_SOURCES})
target_link_libraries(openvmid
    pthread
    process-cpp
    openvmi-protobuf)

set(HWCOMPOSER_SOURCES
    hwcomposer/hwcomposer.cpp)

add_library(hwcomposer.openvmi SHARED ${HWCOMPOSER_SOURCES})

# As we're adding Android specific bits in this project we can't
# build this safely within default build anymore. We keep this
# for easy integration into used IDEs.
set_target_properties(openvmid
    PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)

set_target_properties(hwcomposer.openvmi
    PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)

