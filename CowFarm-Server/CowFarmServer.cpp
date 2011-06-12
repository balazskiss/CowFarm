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
                     
                     
                     sock_client = accept(sock_server, 0, 0);
                     FD_SET (sock_client, &active_fd_set);
                     
                     cout << "New connection: " << sock_client << endl;
                     
                     Cow *new_cow = new Cow(sock_client);
                     cows.insert(new_cow);
                     
                     //new_cow->msg_nl("Welcome to CowFarm!");
                     //new_cow->msg_nl("Please introduce yourself! (Type: 'name: Your Name')");
                 }
                 
                 //Data from existing connection
                 else{
                     nbytes = 0;
                     nbytes = read (i, buf, 1024);
                     
                     if(nbytes == -1)
                         cout << "Socket error: " << i << endl;
                     else if(nbytes == 0){
                         close (i);
                         FD_CLR (i, &active_fd_set);
                         
                         cout << "Connection closed: " << i <<  endl;
                         
                         //remove cow and empty farms
                         Cow *cow = findCow(i);
                         Farm *farm = cow->getFarm();
                         if(farm != 0){
                             if(farm->numberOfCows() == 1){
                                 delete cow->getFarm();
                                 farms.erase(farm);
                             }else{
                                 cow->getFarm()->removeCow(cow);
                                 cow->getFarm()->cowList();
                             }
                         }
                         delete findCow(i);
                         cows.erase(cow);
                         
                     }else{
                         for(unsigned j=0; j<strlen(buf); j++){
                             if(buf[j] == '\n') buf[j] = '\0';
                             if(buf[j] == '\r') buf[j] = '\0';
                         }
                         cout << "Data from "  << i << ": " << buf << endl;
                         
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
    set<Cow *>::iterator it;
    for(it=cows.begin(); it!=cows.end(); ++it){
        Cow *cow = *it;
        if(cow->getSocket() == socket)
            return cow;
    }
    return 0;
}

Cow *CowFarmServer::findCow(string name){
    set<Cow *>::iterator it;
    for(it=cows.begin(); it!=cows.end(); ++it){
        Cow *cow = *it;
        if(cow->getName() == name)
            return cow;
    }
    return 0;
}

Farm *CowFarmServer::findFarm(string name){
    set<Farm *>::iterator it;
    for(it=farms.begin(); it!=farms.end(); ++it){
        Farm *farm = *it;
        if(farm->getName() == name)
            return farm;
    }
    return 0;
}


void CowFarmServer::parseMessageFromCow(Cow *cow, char *message){    
    
    string msg(message);
    istringstream msg_ss(msg);
    
    string cmd;
    getline(msg_ss, cmd, ':');
    if(msg_ss.fail())
        getline(msg_ss, cmd);
    else
        msg_ss.ignore(1);
       
    cout << "Parsing command: '" << cmd << "'" << endl;
    
    if(cmd == "name"){
        string new_name;
        getline(msg_ss, new_name);
        
        if(findCow(new_name)){
            cow->msg_nl("Name already taken!");
        }else{
            cow->setName(new_name);
            cow->msg_nl("OK");
        }
    }
    
    else if(cmd == "join"){
        string new_farm;
        getline(msg_ss, new_farm);
        
        if(cow->getName().empty()){
            cow->msg_nl("Set your name first!");
        }else{
            Farm *farm = findFarm(new_farm);
            if(farm == 0){
                farm = new Farm(new_farm);
                farms.insert(farm);
            }
            cow->setFarm(farm);
            farm->cowList();
            
        }
        
        
    }
    
    else if(cmd == "msg"){
        string message;
        getline(msg_ss, message);
                
        if(!cow->getFarm()){
            cow->msg_nl("Join a farm first!");
        }else{
            cow->getFarm()->msg(cow, message);
        }
    }
    
    else if(cmd == "farmlist"){
        if(cow->getName().empty()){
            cow->msg_nl("Set your name first!");
        }else{
            ostringstream ss;
            ss << "farmlist: " << farms.size() << endl;

            set<Farm *>::iterator it;
            for(it=farms.begin(); it!=farms.end(); ++it)
                ss << (*it)->getName() << endl;

            cow->msg(ss.str());
        }
    }
    
    else if(cmd == "move"){
        string dx_str, dy_str;
        getline(msg_ss, dx_str, ',');
        getline(msg_ss, dy_str);
        
        cow->move(
                cow->getXPosition() + atoi(dx_str.c_str()),
                cow->getYPosition() + atoi(dy_str.c_str()));
        cow->getFarm()->cowList();
        
    }
    
    else{
        cow->msg_nl("Unknown command!");
    }
}