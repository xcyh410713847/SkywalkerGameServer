# 设置 SkywalkerServerFramework 相关路径
set(skywalker_server_framework ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Skywalker server framework PATH" FORCE)
set(skywalker_server_framework_include ${skywalker_server_framework}/include CACHE STRING "Skywalker server framework include PATH" FORCE)

include_directories(${skywalker_server_framework_include})

add_subdirectory(plugins)

add_subdirectory(editor)
