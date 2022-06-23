cmake_minimum_required(VERSION 3.17)
project(server)

include_directories("/opt/boost/include")

set(CMAKE_CXX_STANDARD 17)

add_executable(server src/main.cpp src/datasource.cpp src/datasource.h src/tcpconnection.cpp src/tcpconnection.h src/tcpserver.cpp src/tcpserver.h src/marketdata.cpp src/marketdata.h src/typedefs.h src/marketdatarecord.cpp src/marketdatarecord.h src/subscriptionsmanager.cpp src/subscriptionsmanager.h src/subscription.cpp src/subscription.h src/util.h src/util.cpp)