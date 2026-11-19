cmake_minimum_required(VERSION 3.16)

project(DeviceManager VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt自动处理
set(CMAKE_AUTOMOC ON)  # 自动运行moc
set(CMAKE_AUTOUIC ON)  # 自动处理.ui文件
set(CMAKE_AUTORCC ON)  # 自动处理资源文件

# 查找Qt库
# 添加 plog 头文件路径
include_directories(3rdParty/plog/include)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Sql
)

# 添加可执行文件
add_executable(DeviceManager
    src/main.cpp
    src/repositories/databasemanager.h
    src/repositories/databasemanager.cpp
    src/repositories/DeviceRepository.h
    src/repositories/DeviceRepository.cpp
    src/repositories/UserRepository.cpp
    src/repositories/UserRepository.h
    src/repositories/LogRepository.cpp
    src/repositories/LogRepository.h
    src/models/Device.h
    src/models/User.h
    src/models/ChangeLog.h
    src/tests/RepositoryTests.h
    src/tests/RepositoryTests.cpp
)

# 日志库 plog (header-only)
target_include_directories(DeviceManager PRIVATE
    ${CMAKE_SOURCE_DIR}/3rdParty/plog/include
)


# 链接Qt库
target_link_libraries(DeviceManager PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
)


# Windows下设置GUI应用（去掉控制台窗口）
if(WIN32)
    set_target_properties(DeviceManager PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
