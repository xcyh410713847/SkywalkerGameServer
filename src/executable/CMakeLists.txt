set(skywalker_exe_name "skywalker_launcher")

add_executable(${skywalker_exe_name}
    main.cpp
)

# 关闭警告
target_compile_options(${skywalker_exe_name} PRIVATE "/wd4819")
