run: build
	./client
	
build: client.cpp ../../Shared/SocketManager.hpp ../../Shared/ClientManager.hpp ../../Shared/shared.hpp
	g++ -o client client.cpp
	
clean:
	rm client