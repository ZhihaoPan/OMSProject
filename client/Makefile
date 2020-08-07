CC = g++ -std=c++11
FLAGS = -lpthread -lprotobuf
OUT = client_test
SRC = client_test.cpp Login.cpp NewOrder.cpp CancelOrder.cpp ../protobuf/Gateway.pb.cc ../src/TCPClient.cpp ../src/TCPServer.cpp
OBJ = client_test.o Login.o NewOrder.o CancelOrder.o 
OTHEROBJ = ../src/TCPClient.o ../src/TCPServer.o
LIB_PATH = -I../include 

$(OUT): $(OBJ) $(OTHEROBJ)
	$(CC) $(SRC) -o $(OUT)  $(LIB_PATH) $(FLAGS)

client_test.o: client_test.cpp
	$(CC) -c client_test.cpp $(LIB_PATH)

Login.o: Login.cpp Login.h
	$(CC)  -c Login.cpp $(LIB_PATH)

NewOrder.o: NewOrder.cpp NewOrder.h
	$(CC)  -c NewOrder.cpp $(LIB_PATH)

CancelOrder.o: CancelOrder.cpp CancelOrder.h
	$(CC)  -c CancelOrder.cpp $(LIB_PATH)

clean:
	rm -f $(OBJ) 
# all:
# 	g++ -Wall -std=c++11 -o client_test $(OBJ) client_test.cpp -L../include -I../protobuf/ ../src/TCPServer.cpp ../src/TCPClient.cpp -std=c++11 -lpthread
