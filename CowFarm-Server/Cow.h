/* 
 * File:   Cow.h
 * Author: balazs
 *
 * Created on June 2, 2011, 10:47 AM
 */

#ifndef COW_H
#define	COW_H

#include <string>

#include "Farm.h"

using namespace std;

class Cow {
public:
    Cow(int socket);
    Cow(const Cow& orig);
    virtual ~Cow();
    
    Cow& msg(string message);
    Cow& msg_nl(string message);
    
    int getSocket();
    
    string getName();
    Cow& setName(string name);
    
    Farm *getFarm();
    Cow& setFarm(Farm* farm);
    
    int getXPosition();
    int getYPosition();
    Cow &move(int x, int y);
    
private:
    string name;
    int socket;
    Farm *farm;
    int x, y;
};

#endif	/* COW_H */

