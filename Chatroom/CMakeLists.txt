cmake_minimum_required(VERSION 3.12)
project(chatroom)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
link_libraries(-pthread)


# 添加Server可执行文件
add_executable(server
        Server/server.cc
        Server/Option.hpp
        Server/redis.hpp
        Server/TaskQueue.cc
        Server/TaskQueue.hpp
        Server/TCPServer.cc
        Server/TCPServer.hpp
        Server/ThreadPool.cc
        Server/ThreadPool.hpp
		lib/Color.hpp
		lib/Command.hpp
		lib/Message.hpp
		lib/TCPSocket.cc
		lib/TCPSocket.hpp
)

# 添加Client可执行文件
add_executable(client
        Client/client.cc
        Client/client.hpp
        Client/Display.hpp
        Client/Input.hpp
		lib/Color.hpp
		lib/Command.hpp
		lib/Message.hpp
		lib/TCPSocket.cc
		lib/TCPSocket.hpp
)
# 链接hiredis库
target_link_libraries(server hiredis)