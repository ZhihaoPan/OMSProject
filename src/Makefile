CC=g++
FLAGS= -Wall -std=c++11 -O3 -I./ 
SRC=TCPServer.cpp TCPClient.cpp 
OUT=-o TCPNetwork.so -fPIC -shared -lpthread
LIB_PATH = -L./include
all:
	$(CC) $(LIB_PATH) $(SRC) $(OUT) $(FLAGS) 
	$(CC) -c $(SRC) $(FLAGS) 
	ar rvs TCPNetwork.a *.o