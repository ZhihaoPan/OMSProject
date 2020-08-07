CC = g++ -Wall -std=c++11 -O3 -I./ 
FLAGS = -lpthread -lprotobuf
REPLY_PATH = reply/
OUT = server_test
SRC = server_test.cpp $(REPLY_PATH)LoginReply.cpp ../protobuf/Gateway.pb.cc ../src/TCPClient.cpp ../src/TCPServer.cpp
OBJ = server_test.o LoginReply.o 
OTHER = ../src/TCPClient.o ../src/TCPServer.o
LIB_PATH = -I../include

$(OUT): $(OBJ) $(OTHER)
	$(CC) $(SRC) -o $(OUT)  $(LIB_PATH) $(FLAGS)

server_test.o: server_test.cpp
	$(CC) -c server_test.cpp  $(LIB_PATH) $(FLAGS) 

LoginReply.o: $(REPLY_PATH)LoginReply.cpp $(REPLY_PATH)LoginReply.h 
	$(CC) -c $(REPLY_PATH)LoginReply.cpp $(LIB_PATH)

clean:
	rm -f $(OBJ) 

cleanall:
	rm -f $(OBJ) $(OTHER)
# clean:
# 	rm -f $(OBJS) 
# all:
# 	g++ -Wall -std=c++11 -o client_test $(OBJ) client_test.cpp -L../include -I../protobuf/ ../src/TCPServer.cpp ../src/TCPClient.cpp -std=c++11 -lpthread
