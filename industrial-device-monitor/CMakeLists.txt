cmake_minimum_required(VERSION 3.16)

project(IndustrialDeviceMonitor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 让 Qt 自动生成自定义信号需要的代码。
set(CMAKE_AUTOMOC ON)
# 让 Qt 根据 .ui 文件生成界面代码。
set(CMAKE_AUTOUIC ON)
if(MSVC)
    add_compile_options(/utf-8)
endif()

# Network 模块提供 QTcpServer 和 QTcpSocket。
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Sql)

add_executable(IndustrialDeviceMonitor
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/tcp_receiver.cpp
    src/tcp_receiver.h
    src/reading_store.cpp
    src/reading_store.h
)

target_link_libraries(IndustrialDeviceMonitor PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Sql
)

# 设备模拟器单次发送用 Winsock，定时发送用 Qt Network。
if(WIN32)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Concurrent)
    set(simulator_sources
        simulator/widget.cpp
        simulator/widget.h
        simulator/widget.ui
        simulator/temperature_sender.cpp
        simulator/temperature_sender.h
    )
    set(simulator_libraries
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Concurrent
        ws2_32
    )
    add_executable(DeviceSimulator simulator/main.cpp ${simulator_sources})
    target_link_libraries(DeviceSimulator PRIVATE ${simulator_libraries})
endif()

# 网络测试按需开启，不影响平时运行项目。
option(BUILD_TESTING "Build TCP receiver tests" OFF)
if(BUILD_TESTING)
    enable_testing()
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)
    add_executable(TcpReceiverTest
        tests/tcp_receiver_test.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        src/tcp_receiver.cpp
        src/tcp_receiver.h
        src/reading_store.cpp
        src/reading_store.h
    )
    target_include_directories(TcpReceiverTest PRIVATE src)
    target_link_libraries(TcpReceiverTest PRIVATE
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Sql
        Qt${QT_VERSION_MAJOR}::Test
        Qt${QT_VERSION_MAJOR}::Widgets
    )
    if(WIN32)
        add_dependencies(TcpReceiverTest DeviceSimulator)
        target_sources(TcpReceiverTest PRIVATE ${simulator_sources})
        target_include_directories(TcpReceiverTest PRIVATE simulator)
        target_link_libraries(TcpReceiverTest PRIVATE ${simulator_libraries})
    endif()
    add_test(NAME tcp_receiver COMMAND TcpReceiverTest)
    set_tests_properties(tcp_receiver PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endif()
