/* 
 * File:   CowFarmServer.cpp
 * Author: balazs
 * 
 * Created on June 2, 2011, 10:46 AM
 */

#include "CowFarmServer.h"

#include <iostream>
#include <sstream>

//Libraries for socket handling
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

CowFarmServer::CowFarmServer(int port) {
    int sock_server, sock_client;
    struct sockaddr_in serv_addr;
    char buf[1024];
    int nbytes;
    
    cout << "Starting CowFarm-Server on port " << port << endl;
    
    fd_set active_fd_set, read_fd_set;
    
    
    sock_server = socket(AF_INET, SOCK_STREAM, 0);
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if(bind(sock_server, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        cout << "Binding error";
    
    if(listen(sock_server, 5) < 0)
        cout << "Unable to listen on port 8765";
    
    FD_ZERO (&active_fd_set);
    FD_SET (sock_server, &active_fd_set);
    
    while(1){
        read_fd_set = active_fd_set;
        select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL);
        
        for (int i = 0; i < FD_SETSIZE; ++i){
             if (FD_ISSET (i, &read_fd_set)){
                 
                 //New connection
                 if (i == sock_server){
                     //cout << "new connection" << endl;
                     sock_client = accept(sock_server, 0, 0);
                     FD_SET (sock_client, &active_fd_set);
                     
                     Cow *new_cow = new Cow(sock_client);
                     cows.push_back(new_cow);
                     new_cow->msg_nl("Welcome to CowFarm!");
                     new_cow->msg_nl("Please introduce yourself! (Type: 'name: Your Name')");
                 }
                 
                 //Data from existing connection
                 else{
                     nbytes = 0;
                     nbytes = read (i, buf, 1024);
                     
                     if(nbytes == -1)
                         cout << "error" << endl;
                     else if(nbytes == 0){
                         close (i);
                         FD_CLR (i, &active_fd_set);
                         cout << "closed" << endl;
                     }else{
                         for(unsigned j=0; j<strlen(buf); j++)
                             if(buf[j] == '\n') buf[j] = '\0';
                         //cout << "data received: "  << buf << endl;
                         
                         parseMessageFromCow(findCow(i), buf);
                     }
                         
                 }
             }
        }
        
    }
    
}

CowFarmServer::~CowFarmServer() {
}

Cow *CowFarmServer::findCow(int socket){
    for(unsigned i=0; i<cows.size(); i++){
        if(cows[i]->getSocket() == socket)
            return cows[i];
    }
    return 0;
}

Cow *CowFarmServer::findCow(string name){
    for(unsigned i=0; i<cows.size(); i++){
        if(cows[i]->getName() == name)
            return cows[i];
    }
    return 0;
}

Farm *CowFarmServer::findFarm(string name){
    for(unsigned i=0; i<farms.size(); i++){
        if(farms[i]->getName() == name)
            return farms[i];
    }
    return 0;
}


void CowFarmServer::parseMessageFromCow(Cow *cow, char *message){
    
    //DEBUG INFO
    cout << "Cows: " << cows.size() << endl;
    cout << "Farms: " << farms.size() << endl;
    
    
    string msg(message);
    
    if(msg.substr(0, 6) == "name: "){
        string new_name = msg.substr(6, msg.size()-7);
        if(findCow(new_name)){
            cow->msg_nl("Name already taken!");
        }else{
            cow->setName(new_name);
        }
    }
    
    else if(msg.substr(0, 6) == "join: "){
        string new_farm = msg.substr(6, msg.size()-7);
        if(cow->getName().empty()){
            cow->msg_nl("Set your name first!");
        }else{
            Farm *farm = findFarm(new_farm);
            if(farm == 0){
                farm = new Farm(new_farm);
                farms.push_back(farm);
            }
            cow->setFarm(farm);
            farm->cowList();
            
        }
        
        
    }
    
    else if(msg.substr(0, 5) == "msg: "){
        string message = msg.substr(5, msg.size()-6);
                
        if(!cow->getFarm()){
            cow->msg_nl("Join a farm first!");
        }else{
            cow->getFarm()->msg(cow, message);
        }
    }
    
    else if(msg.substr(0, 8) == "farmlist"){
        if(cow->getName().empty()){
            cow->msg_nl("Set your name first!");
        }else{
            ostringstream ss;
            ss << "farmlist: " << farms.size() << endl;

            for(unsigned i=0; i<farms.size(); i++)
                ss << farms[i]->getName() << endl;

            cow->msg(ss.str());
        }
    }
    
    else{
        cow->msg_nl("Unknown command!");
    }
}