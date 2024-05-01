run: build
	./server

build: server.cpp ../../Shared/SocketManager.hpp  ../../Shared/ServerManager.hpp ../../Shared/shared.hpp
	g++ -o server server.cpp
	
clean:
	rm server