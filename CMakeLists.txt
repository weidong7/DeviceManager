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
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Sql
    Network
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
    src/views/DeviceTableModel.h
    src/views/DeviceTableModel.cpp
    src/views/LoginDialog.h
    src/views/LoginDialog.cpp
    src/utils/PasswordHasher.cpp
    src/utils/PasswordHasher.h
    src/utils/SessionManager.h
    src/utils/SessionManager.cpp
    src/models/Device.h
    src/models/User.h
    src/models/ChangeLog.h
    src/tests/RepositoryTests.h
    src/tests/RepositoryTests.cpp
    src/views/mainwindow.cpp
    src/views/mainwindow.h
)

# 日志库 plog (header-only) 以及 项目根目录 (确保能找到 src 下的文件)
target_include_directories(DeviceManager PRIVATE
    ${CMAKE_SOURCE_DIR}/3rdParty/plog/include
    ${CMAKE_SOURCE_DIR}
)

# ==========================================
# 修改点 1：链接Qt库（加上了 Qt6::Network）
# ==========================================
target_link_libraries(DeviceManager PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
    Qt6::Network
)

# ==========================================
# 修改点 2：暂时注释掉 WIN32_EXECUTABLE
# 开发阶段需要看控制台日志，等打包发布时再解开
# ==========================================
# if(WIN32)
#     set_target_properties(DeviceManager PROPERTIES
#         WIN32_EXECUTABLE TRUE
#     )
# endif()
