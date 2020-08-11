CC = g++ -Wall -std=c++11 # -O3 # -I./ 
FLAGS = -lpthread -lprotobuf
REPLY_PATH = reply/
OUT = server_test
SRC = server_test.cpp  $(REPLY) ../protobuf/Gateway.pb.cc ../src/TCPClient.cpp ../src/TCPServer.cpp
OBJ = server_test.o LoginReply.o CancelOrderReply.o ExecutiveReport.o HeartBeatReport.o LogoutReply.o TransactionReport.o
OTHER = ../src/TCPClient.o ../src/TCPServer.o
LIB_PATH = -I../include
REPLY = $(REPLY_PATH)LoginReply.cpp $(REPLY_PATH)CancelOrderReply.cpp $(REPLY_PATH)ExecutiveReport.cpp $(REPLY_PATH)HeartBeatReport.cpp $(REPLY_PATH)LogoutReply.cpp $(REPLY_PATH)TransactionReport.cpp

$(OUT): $(OBJ) $(OTHER)
	$(CC) $(SRC) -o $(OUT)  $(LIB_PATH) $(FLAGS)

server_test.o: server_test.cpp
	$(CC) -c server_test.cpp  $(LIB_PATH) $(FLAGS) 

LoginReply.o: $(REPLY_PATH)LoginReply.cpp $(REPLY_PATH)LoginReply.h 
	$(CC) -c $(REPLY_PATH)LoginReply.cpp $(LIB_PATH)

CancelOrderReply.o: $(REPLY_PATH)CancelOrderReply.cpp $(REPLY_PATH)CancelOrderReply.h 
	$(CC) -c $(REPLY_PATH)CancelOrderReply.cpp $(LIB_PATH)

ExecutiveReport.o: $(REPLY_PATH)ExecutiveReport.cpp $(REPLY_PATH)ExecutiveReport.h 
	$(CC) -c $(REPLY_PATH)ExecutiveReport.cpp $(LIB_PATH)

HeartBeatReport.o: $(REPLY_PATH)HeartBeatReport.cpp $(REPLY_PATH)HeartBeatReport.h 
	$(CC) -c $(REPLY_PATH)HeartBeatReport.cpp $(LIB_PATH)

LogoutReply.o: $(REPLY_PATH)LogoutReply.cpp $(REPLY_PATH)LogoutReply.h 
	$(CC) -c $(REPLY_PATH)LogoutReply.cpp $(LIB_PATH)

TransactionReport.o: $(REPLY_PATH)TransactionReport.cpp $(REPLY_PATH)TransactionReport.h 
	$(CC) -c $(REPLY_PATH)TransactionReport.cpp $(LIB_PATH)



clean:
	rm -f $(OBJ) 

cleanall:
	rm -f $(OBJ) $(OTHER)
# clean:
# 	rm -f $(OBJS) 
# all:
# 	g++ -Wall -std=c++11 -o client_test $(OBJ) client_test.cpp -L../include -I../protobuf/ ../src/TCPServer.cpp ../src/TCPClient.cpp -std=c++11 -lpthread
