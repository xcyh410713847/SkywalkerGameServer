# SkywalkerTools 路径
set(skywalker_tools ${PROJECT_SOURCE_DIR})
message(STATUS "Skywalker Tools PATH: ${skywalker_tools}")

# SkywalkerServerFramework 路径
set(skywalker_server_framework ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Skywalker server framework PATH" FORCE)
message(STATUS "Skywalker Server Framework PATH: ${skywalker_server_framework}")

include_directories(
    ${skywalker_tools}
    ${skywalker_server_framework}
)

add_subdirectory(Plugins)

add_subdirectory(Editor)
