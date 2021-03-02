/*
 * CLI.cpp
 *
 * Author: 207233222 Yeheli Frangi
 */

#ifndef SERVER_H_
#define SERVER_H_
#include <pthread.h>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "commands.h"
#include "CLI.h"


using namespace std;


class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};


// this class is initialize how to read or write on the socket.
class SocketIO:public DefaultIO{
	int clientID;
public:
	//constuctor.
	SocketIO(int clientID) {
		this->clientID = clientID;
	}
	//read a string from client
	virtual string read(){
		string readLine = "";
		char c=0;
		::read(clientID,&c,sizeof(char));
		while(c !='\n'){
			readLine +=c;
			::read(clientID,&c,sizeof(char));
		}
		return readLine;
	}
	//write a string from client
	virtual void write(string text){
		const char* textNew = text.c_str(); 
		send(clientID,textNew,strlen(textNew),0);
	}
	//write a float from client
	virtual void write(float f){
		string stringF = to_string(f);
		// to dellete the '0'
		stringF.erase(stringF.find_last_not_of('0')+1,std::string::npos);
		const char* stringFnew = stringF.c_str();
		send(clientID,stringFnew,strlen(stringFnew),0);
	}
	//read a float from client
	virtual void read(float* f){
		::read(clientID,f,sizeof(float));
	}
};

// this class sends the client to the handle in a socket.
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
		SocketIO socket(clientID);
		CLI cli(&socket);
		cli.start();
    }
};


// implement on Server.cpp
class Server {
	thread* t; // the thread to run the start() method in
	int fd;
	sockaddr_in server;
	sockaddr_in client;
	bool flag;

	

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
