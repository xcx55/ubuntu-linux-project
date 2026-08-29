.PHONY:all
all:server client
server:echo_server.cc
	g++ $^ -o $@ -std=c++14
client:echo_client.cc
	g++ $^ -o $@ -std=c++14

PHONY:clean
clean:
	rm -rf client server