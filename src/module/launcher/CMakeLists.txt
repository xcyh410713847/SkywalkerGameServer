set(skywalker_module_launcher_name "launcher")

# 查找当前目录的所有源文件
aux_source_directory(. DIR_LIB_SRCS)

add_library(${skywalker_module_launcher_name} SHARED ${DIR_LIB_SRCS})

# 关闭警告
target_compile_options(${skywalker_module_launcher_name} PRIVATE "/wd4819")
