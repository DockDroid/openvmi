include_directories(
  ${Boost_INCLUDE_DIRS}
  ${MIRCLIENT_INCLUDE_DIRS}
  ${DBUS_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIRS}
  ${SDL2_IMAGE_INCLUDE_DIRS}
  ${PROPERTIES_CPP_INCLUDE_DIRS}
  ${MIRCLIENT_INCLUDE_DIRS}
  ${LIBSYSTEMD_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/external
  ${CMAKE_SOURCE_DIR}/external/process-cpp-minimal/include
  ${CMAKE_SOURCE_DIR}/external/android-emugl/shared
  ${CMAKE_SOURCE_DIR}/external/android-emugl/host/include
  ${CMAKE_BINARY_DIR}/external/android-emugl/host/include
  ${CMAKE_SOURCE_DIR}/external/android-emugl/shared/OpenglCodecCommon
  ${CMAKE_SOURCE_DIR}/external/android-emugl/host/libs
  ${CMAKE_SOURCE_DIR}/external/android-emugl/host/include/libOpenglRender
  ${CMAKE_SOURCE_DIR}/external/android-emugl/host/libs/GLESv1_dec
  ${CMAKE_BINARY_DIR}/external/android-emugl/host/libs/GLESv1_dec
  ${CMAKE_SOURCE_DIR}/external/android-emugl/host/libs/GLESv2_dec
  ${CMAKE_BINARY_DIR}/external/android-emugl/host/libs/GLESv2_dec
  ${CMAKE_SOURCE_DIR}/external/android-emugl/host/libs/renderControl_dec
  ${CMAKE_BINARY_DIR}/external/android-emugl/host/libs/renderControl_dec
  ${CMAKE_SOURCE_DIR}/external/glm
)

protobuf_generate_cpp(
    GENERATED_PROTOBUF_RPC_SRCS GENERATED_PROTOBUF_RPC_HDRS
    openvmi/protobuf/openvmi_rpc.proto)

protobuf_generate_cpp(
    GENERATED_PROTOBUF_BRIDGE_SRCS GENERATED_PROTOBUF_BRIDGE_HDRS
    openvmi/protobuf/openvmi_bridge.proto)

add_library(openvmi-protobuf
    STATIC
    ${GENERATED_PROTOBUF_BRIDGE_SRCS}
    ${GENERATED_PROTOBUF_BRIDGE_HDRS}
    ${GENERATED_PROTOBUF_RPC_SRCS}
    ${GENERATED_PROTOBUF_RPC_HDRS}
    ${GENERATED_PROTOBUF_CONTAINER_SRCS}
    ${GENERATED_PROTOBUF_CONTAINER_HDRS}
    openvmi/protobuf/openvmi_rpc.proto
    openvmi/protobuf/openvmi_bridge.proto
    openvmi/protobuf/google_protobuf_guard.cpp)
target_link_libraries(openvmi-protobuf
    ${PROTOBUF_LITE_LIBRARIES}
    ${PROTOBUF_LITE_LIBRARIES})

set(SOURCES
    openvmi/android/intent.cpp
    openvmi/android/intent.h
    openvmi/android/ip_config_builder.cpp
    openvmi/android/ip_config_builder.h

    openvmi/application/database.cpp
    openvmi/application/database.h
    openvmi/application/launcher_storage.cpp
    openvmi/application/launcher_storage.h
    openvmi/application/manager.h

    openvmi/bridge/android_api_stub.cpp
    openvmi/bridge/android_api_stub.h
    openvmi/bridge/platform_api_skeleton.cpp
    openvmi/bridge/platform_api_skeleton.h
    openvmi/bridge/platform_message_processor.cpp
    openvmi/bridge/platform_message_processor.h

    openvmi/build/config.h
    openvmi/build/config.h.in

    openvmi/cmds/session_manager.cpp
    openvmi/cmds/session_manager.h
    openvmi/cmds/system_info.cpp
    openvmi/cmds/system_info.h
    openvmi/cmds/version.cpp
    openvmi/cmds/version.h
    openvmi/cmds/wait_ready.cpp
    openvmi/cmds/wait_ready.h
    openvmi/cmds/generate_ip_config.cpp
    openvmi/cmds/generate_ip_config.h

    openvmi/common/binary_writer.cpp
    openvmi/common/binary_writer.h
    openvmi/common/dispatcher.cpp
    openvmi/common/dispatcher.h
    openvmi/common/fd.cpp
    openvmi/common/fd.h
    openvmi/common/fd_sets.h
    openvmi/common/loop_device_allocator.cpp
    openvmi/common/loop_device_allocator.h
    openvmi/common/loop_device.cpp
    openvmi/common/loop_device.h
    openvmi/common/message_channel.cpp
    openvmi/common/message_channel.h
    openvmi/common/mount_entry.cpp
    openvmi/common/mount_entry.h
    openvmi/common/scope_ptr.h
    openvmi/common/small_vector.h
    openvmi/common/type_traits.h
    openvmi/common/variable_length_array.h
    openvmi/common/wait_handle.cpp
    openvmi/common/wait_handle.h

    openvmi/dbus/bus.cpp
    openvmi/dbus/bus.h
    openvmi/dbus/codecs.h
    openvmi/dbus/interface.h
    openvmi/dbus/sd_bus_helpers.c
    openvmi/dbus/sd_bus_helpers.h
    openvmi/dbus/sd_bus_helpers.hpp
    openvmi/dbus/skeleton/application_manager.cpp
    openvmi/dbus/skeleton/application_manager.h
    openvmi/dbus/skeleton/service.cpp
    openvmi/dbus/skeleton/service.h
    openvmi/dbus/stub/application_manager.cpp
    openvmi/dbus/stub/application_manager.h

    openvmi/graphics/buffered_io_stream.cpp
    openvmi/graphics/buffered_io_stream.h
    openvmi/graphics/buffer_queue.cpp
    openvmi/graphics/buffer_queue.h
    openvmi/graphics/density.cpp
    openvmi/graphics/density.h
    openvmi/graphics/gl_extensions.h
    openvmi/graphics/gl_renderer_server.cpp
    openvmi/graphics/gl_renderer_server.h
    openvmi/graphics/layer_composer.cpp
    openvmi/graphics/layer_composer.h
    openvmi/graphics/multi_window_composer_strategy.cpp
    openvmi/graphics/multi_window_composer_strategy.h
    openvmi/graphics/opengles_message_processor.cpp
    openvmi/graphics/opengles_message_processor.h
    openvmi/graphics/primitives.h
    openvmi/graphics/program_family.cpp
    openvmi/graphics/program_family.h
    openvmi/graphics/rect.cpp
    openvmi/graphics/rect.h
    openvmi/graphics/renderer.h
    openvmi/graphics/single_window_composer_strategy.cpp
    openvmi/graphics/single_window_composer_strategy.h

    openvmi/graphics/emugl/ColorBuffer.cpp
    openvmi/graphics/emugl/ColorBuffer.h
    openvmi/graphics/emugl/DispatchTables.h
    openvmi/graphics/emugl/DisplayManager.cpp
    openvmi/graphics/emugl/DisplayManager.h
    openvmi/graphics/emugl/ReadBuffer.cpp
    openvmi/graphics/emugl/ReadBuffer.h
    openvmi/graphics/emugl/Renderable.cpp
    openvmi/graphics/emugl/Renderable.h
    openvmi/graphics/emugl/RenderApi.cpp
    openvmi/graphics/emugl/RenderApi.h
    openvmi/graphics/emugl/RenderContext.cpp
    openvmi/graphics/emugl/RenderContext.h
    openvmi/graphics/emugl/RenderControl.cpp
    openvmi/graphics/emugl/RenderControl.h
    openvmi/graphics/emugl/RendererConfig.cpp
    openvmi/graphics/emugl/RendererConfig.h
    openvmi/graphics/emugl/Renderer.cpp
    openvmi/graphics/emugl/Renderer.h
    openvmi/graphics/emugl/RenderThread.cpp
    openvmi/graphics/emugl/RenderThread.h
    openvmi/graphics/emugl/RenderThreadInfo.cpp
    openvmi/graphics/emugl/RenderThreadInfo.h
    openvmi/graphics/emugl/TextureDraw.cpp
    openvmi/graphics/emugl/TextureDraw.h
    openvmi/graphics/emugl/TextureResize.cpp
    openvmi/graphics/emugl/TextureResize.h
    openvmi/graphics/emugl/TimeUtils.cpp
    openvmi/graphics/emugl/TimeUtils.h
    openvmi/graphics/emugl/WindowSurface.cpp
    openvmi/graphics/emugl/WindowSurface.h

    openvmi/input/device.cpp
    openvmi/input/device.h
    openvmi/input/manager.cpp
    openvmi/input/manager.h

    openvmi/network/base_socket_messenger.cpp
    openvmi/network/base_socket_messenger.h
    openvmi/network/connection_context.cpp
    openvmi/network/connection_context.h
    openvmi/network/connection_creator.cpp
    openvmi/network/connection_creator.h
    openvmi/network/connections.h
    openvmi/network/connector.h
    openvmi/network/credentials.cpp
    openvmi/network/credentials.h
    openvmi/network/delegate_connection_creator.h
    openvmi/network/delegate_message_processor.cpp
    openvmi/network/delegate_message_processor.h
    openvmi/network/fd_socket_transmission.cpp
    openvmi/network/fd_socket_transmission.h
    openvmi/network/local_socket_messenger.cpp
    openvmi/network/local_socket_messenger.h
    openvmi/network/message_processor.h
    openvmi/network/message_receiver.h
    openvmi/network/message_sender.h
    openvmi/network/published_socket_connector.cpp
    openvmi/network/published_socket_connector.h
    openvmi/network/socket_connection.cpp
    openvmi/network/socket_connection.h
    openvmi/network/socket_helper.cpp
    openvmi/network/socket_helper.h
    openvmi/network/socket_messenger.cpp
    openvmi/network/socket_messenger.h
    openvmi/network/tcp_socket_connector.cpp
    openvmi/network/tcp_socket_connector.h
    openvmi/network/tcp_socket_messenger.cpp
    openvmi/network/tcp_socket_messenger.h

    openvmi/platform/base_platform.cpp
    openvmi/platform/base_platform.h
    openvmi/platform/null/platform.cpp
    openvmi/platform/null/platform.h
    openvmi/platform/sdl/keycode_converter.cpp
    openvmi/platform/sdl/keycode_converter.h
    openvmi/platform/sdl/platform.cpp
    openvmi/platform/sdl/platform.h
    openvmi/platform/sdl/sdl_wrapper.h
    openvmi/platform/sdl/window.cpp
    openvmi/platform/sdl/window.h

    openvmi/protobuf/openvmi_bridge.proto
    openvmi/protobuf/openvmi_rpc.proto
    openvmi/protobuf/google_protobuf_guard.cpp

    openvmi/qemu/adb_message_processor.cpp
    openvmi/qemu/adb_message_processor.h
    openvmi/qemu/at_parser.cpp
    openvmi/qemu/at_parser.h
    openvmi/qemu/bootanimation_message_processor.cpp
    openvmi/qemu/bootanimation_message_processor.h
    openvmi/qemu/boot_properties_message_processor.cpp
    openvmi/qemu/boot_properties_message_processor.h
    openvmi/qemu/camera_message_processor.cpp
    openvmi/qemu/camera_message_processor.h
    openvmi/qemu/fingerprint_message_processor.cpp
    openvmi/qemu/fingerprint_message_processor.h
    openvmi/qemu/gsm_message_processor.cpp
    openvmi/qemu/gsm_message_processor.h
    openvmi/qemu/hwcontrol_message_processor.cpp
    openvmi/qemu/hwcontrol_message_processor.h
    openvmi/qemu/null_message_processor.cpp
    openvmi/qemu/null_message_processor.h
    openvmi/qemu/pipe_connection_creator.cpp
    openvmi/qemu/pipe_connection_creator.h
    openvmi/qemu/qemud_message_processor.cpp
    openvmi/qemu/qemud_message_processor.h

    openvmi/rpc/channel.cpp
    openvmi/rpc/channel.h
    openvmi/rpc/connection_creator.cpp
    openvmi/rpc/connection_creator.h
    openvmi/rpc/constants.h
    openvmi/rpc/make_protobuf_object.h
    openvmi/rpc/message_processor.cpp
    openvmi/rpc/message_processor.h
    openvmi/rpc/pending_call_cache.cpp
    openvmi/rpc/pending_call_cache.h
    openvmi/rpc/template_message_processor.h

    openvmi/testing/gtest_utils.h

    openvmi/ui/splash_screen.cpp
    openvmi/ui/splash_screen.h

    openvmi/utils/environment_file.cpp
    openvmi/utils/environment_file.h

    openvmi/sensors/sensors_common.h
    openvmi/sensors/sensors_message_processor.cpp
    openvmi/sensors/sensors_message_processor.h
    openvmi/sensors/sensors_manager.cpp
    openvmi/sensors/sensors_manager.h
    openvmi/sensors/sensors_sensor.cpp
    openvmi/sensors/sensors_sensor.h
    openvmi/sensors/sensors_data.cpp
    openvmi/sensors/sensors_data.h

    openvmi/wm/display.cpp
    openvmi/wm/display.h
    openvmi/wm/manager.cpp
    openvmi/wm/manager.h
    openvmi/wm/multi_window_manager.cpp
    openvmi/wm/multi_window_manager.h
    openvmi/wm/single_window_manager.cpp
    openvmi/wm/single_window_manager.h
    openvmi/wm/stack.cpp
    openvmi/wm/stack.h
    openvmi/wm/task.cpp
    openvmi/wm/task.h
    openvmi/wm/window.cpp
    openvmi/wm/window.h
    openvmi/wm/window_state.cpp
    openvmi/wm/window_state.h

    openvmi/cli.cpp
    openvmi/cli.h
    openvmi/daemon.cpp
    openvmi/daemon.h
    openvmi/defer_action.h
    openvmi/do_not_copy_or_move.h
    openvmi/logger.cpp
    openvmi/logger.h
    openvmi/not_reachable.cpp
    openvmi/not_reachable.h
    openvmi/optional.h
    openvmi/utils.cpp
    openvmi/utils.h
    openvmi/runtime.cpp
    openvmi/runtime.h
    openvmi/system_configuration.cpp
    openvmi/system_configuration.h)

add_library(openvmi-core STATIC ${SOURCES})
target_link_libraries(openvmi-core
  ${Boost_LDFLAGS}
  ${Boost_LIBRARIES}
  ${SDL2_LDFLAGS}
  ${SDL2_LIBRARIES}
  ${SDL2_IMAGE_LDFLAGS}
  ${SDL2_IMAGE_LIBRARIES}
  ${MIRCLIENT_LDFLAGS}
  ${MIRCLIENT_LIBRARIES}
  ${LIBSYSTEMD_LDFLAGS}
  ${LIBSYSTEMD_LIBRARIES}
  pthread
  process-cpp
  emugl_common
  GLESv1_dec
  GLESv2_dec
  renderControl_dec
  OpenGLESDispatch
  OpenglCodecCommon
  openvmi-protobuf
  xdg)

add_executable(openvmi main.cpp ${BACKWARD_ENABLE})
target_link_libraries(openvmi
    openvmi-core)

install(
  TARGETS openvmi
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)
