run: build
	./client
	
build: client.cpp ../../SocketManagement/ClientManager.hpp ../../Tools/GetPortNumber.hpp
	g++ -o client client.cpp
	
clean:
	rm client