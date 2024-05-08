run: build
	./server

build: server.cpp ./EchoServer.hpp ../../SocketManagement/ServerManager.hpp ../../Tools/GetPortNumber.hpp
	g++ -std=c++20 -stdlib=libc++ -o server server.cpp
	
clean:
	rm server