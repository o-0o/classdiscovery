# 设置最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(DiscoveryProject VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加包含目录
include_directories(include)

# 添加可执行文件 DiscoveryDeviceExecutable
add_executable(DiscoveryDeviceExecutable
    src/discoverydevice_main.cpp
    src/discoverydeviceclass.cpp
)

# 添加可执行文件 DiscoveryHostExecutable
add_executable(DiscoveryHostExecutable
    src/discoveryhost_main.cpp
    src/discoveryhostclass.cpp
)

# 链接必要的库（如果有的话）
# target_link_libraries(DiscoveryDeviceExecutable ...)
# target_link_libraries(DiscoveryHostExecutable ...)
