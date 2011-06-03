/* 
 * File:   Cow.cpp
 * Author: balazs
 * 
 * Created on June 2, 2011, 10:47 AM
 */

#include <iostream>
#include <vector>

#include "Cow.h"

using namespace std;

Cow::Cow(int socket) {
    this->socket = socket;
    this->name = "";
    this->farm = 0;
    this->x = 0;
    this->y = 0;
}

Cow::Cow(const Cow& orig) {
}

Cow::~Cow() {
}


Cow& Cow::msg(string message){
    const char *msg = message.c_str();
    write(socket, msg, strlen(msg));
    return *this;
}

Cow& Cow::msg_nl(string message){
    message.push_back('\n');
    return msg(message);
}

int Cow::getSocket(){
    return this->socket;
}

Cow& Cow::setName(string name){
    if(name == this->name) return *this;
    
    this->name = name;
    cout << socket << "'s new name: " << this->name << endl;
    return *this;
}
string Cow::getName(){
    return this->name;
}

Farm *Cow::getFarm(){
    return this->farm;
}
Cow& Cow::setFarm(Farm* farm){
    if(farm == this->farm) return *this;
    
    if(this->farm != 0){
        //quit
    }
    
    farm->addCow(this);
    this->farm = farm;
    
    return *this;
}

int Cow::getXPosition(){
    return x;
}
int Cow::getYPosition(){
    return y;
}
Cow &Cow::move(int x, int y){
    this->x = x;
    this->y = y;
    return *this;
}