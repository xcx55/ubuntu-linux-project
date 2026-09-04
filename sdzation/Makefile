all:server client

client:netcallclient.cc
	g++ -o $@ $^ -ljsoncpp
server:netcallserver.cc
	g++ -o $@ $^ -ljsoncpp
.PHONY:clean
clean:
	rm -rf server client