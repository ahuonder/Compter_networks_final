run: build
	./client
	
build: client.cpp ../../SocketManagement/ClientManager.hpp ../../Tools/GetPortNumber.hpp
	g++ -std=c++20 -stdlib=libc++ -o client client.cpp
	
clean:
	rm client