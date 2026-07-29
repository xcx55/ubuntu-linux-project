all: server client

server:server.cc
	g++ $^ -o $@ -std=c++11
client:client.cc
	g++ $^ -o $@ -std=c++11
.PHOMY:clean
clean:
	rm -rf server client