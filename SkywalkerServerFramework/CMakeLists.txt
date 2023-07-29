function(SkywalkerServerFramework_MessageDebug info)
    message(STATUS "[Skywalker Server Framework] ${info}")
endfunction()

SkywalkerServerFramework_MessageDebug("Start Build Skywalker Server Framework")

# SkywalkerTools 路径
set(skywalker_tools ${PROJECT_SOURCE_DIR})
SkywalkerServerFramework_MessageDebug("Skywalker Tools PATH: ${skywalker_tools}")

# SkywalkerServerFramework 路径
set(skywalker_server_framework ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Skywalker server framework PATH" FORCE)
SkywalkerServerFramework_MessageDebug("Skywalker Server Framework PATH: ${skywalker_server_framework}")

include_directories(
    ${skywalker_tools}
    ${skywalker_server_framework}
)

add_subdirectory(Core)

add_subdirectory(Plugin)

add_subdirectory(Editor)

SkywalkerServerFramework_MessageDebug("End Build Skywalker Server Framework")
