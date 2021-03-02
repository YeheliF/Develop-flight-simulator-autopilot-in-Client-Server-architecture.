/*
 * CLI.cpp
 *
 * Author: 207233222 Yeheli Frangi
 */
#include "Server.h"
//constructor. creat a socket, bind and listen tat this socket.
Server::Server(int port)throw (const char*) {
	fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd<0){
		throw "socket failed";
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
	//if the bind didn't success
	if(bind(fd,(struct sockaddr*)&server, sizeof(server))<0){
		throw "bind failed";
	}
	//if the listen didn't success
	if(listen(fd,3)<0){
		throw "listen failed";
	}
	flag = true;
}
//this function get a client, and sent it to the functions.
void Server::start(ClientHandler& ch)throw(const char*){	
	t=new thread([&ch,this](){
		// the srever is ready to answer a lot of clients one by one.
		while(flag){
			socklen_t clientSize = sizeof(client);
			alarm(3);
			int aClient = accept(fd,(struct sockaddr*)&client,&clientSize);
			//if the accept didn't success
			if(aClient <0){
				throw "accept failed";
			}
			ch.handle(aClient);
			close(aClient);
		}
		close(fd);
	});
}
//this function stops the server from listen to clients.
void Server::stop(){
	flag = false;
	t->join(); // do not delete this!

}
//discructor.
Server::~Server() {
}

