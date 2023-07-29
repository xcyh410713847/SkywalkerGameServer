# Skywalker Tools
set(skywalker_tools ${PROJECT_SOURCE_DIR})
message(STATUS "Skywalker Tools PATH: ${skywalker_tools}")

# 设置 SkywalkerServerFramework 相关路径
set(skywalker_server_framework ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Skywalker server framework PATH" FORCE)
message(STATUS "Skywalker Server Framework PATH: ${skywalker_server_framework}")
set(skywalker_server_framework_include ${skywalker_server_framework}/include CACHE STRING "Skywalker server framework include PATH" FORCE)
message(STATUS "Skywalker Server Framework Include PATH: ${skywalker_server_framework_include}")

include_directories(
    ${skywalker_tools}
    ${skywalker_server_framework_include}
)

add_subdirectory(plugins)

add_subdirectory(editor)
