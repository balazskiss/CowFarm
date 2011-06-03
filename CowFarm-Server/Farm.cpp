/* 
 * File:   Farm.cpp
 * Author: balazs
 * 
 * Created on June 2, 2011, 10:47 AM
 */

#include "Farm.h"
#include "Cow.h"

#include <sstream>

Farm::Farm(string name) {
    this->name = name;
}

Farm::Farm(const Farm& orig) {
}

Farm::~Farm() {
}

string Farm::getName(){
    return name;
}

Farm& Farm::addCow(Cow *cow){
    cows.insert(cow);
    
    return *this;
}

Farm& Farm::removeCow(Cow *cow){
    cows.erase(cow);
    //notify others
    
    return *this;
}

Farm &Farm::cowList(){
    ostringstream ss;
    ss << "cowlist: " << numberOfCows() << endl;
    
    set<Cow *>::iterator it;
    for(it=cows.begin(); it!=cows.end(); ++it){
        ss << (*it)->getSocket() << ": ";
        ss << (*it)->getName() << ", ";
        ss << (*it)->getXPosition() << ", ";
        ss << (*it)->getYPosition() << endl;
    }
    
    string msg = ss.str();  
    for(it=cows.begin(); it!=cows.end(); ++it){
        (*it)->msg(msg);
    }
    
    return *this;
}

Farm &Farm::msg(Cow *cow, string message){
    ostringstream ss;
    ss << "msg from " << cow->getSocket() << ": " << message;
    string msg = ss.str();
    
    set<Cow *>::iterator it;
    for(it=cows.begin(); it!=cows.end(); ++it){
        (*it)->msg_nl(msg);
    }
    
    return *this;
}

unsigned Farm::numberOfCows(){
    return cows.size();
}