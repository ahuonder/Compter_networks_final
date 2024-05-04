run: build
	./server

build: server.cpp ./EchoServer.hpp ../../SocketManagement/ServerManager.hpp ../../Tools/GetPortNumber.hpp
	g++ -o server server.cpp
	
clean:
	rm server