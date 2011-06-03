/* 
 * File:   CowFarmServer.h
 * Author: balazs
 *
 * Created on June 2, 2011, 10:46 AM
 */

#ifndef COWFARMSERVER_H
#define	COWFARMSERVER_H

#include <vector>

#include "Farm.h"
#include "Cow.h"

using namespace std;

class CowFarmServer {
public:
    CowFarmServer(int port = 8765);
    virtual ~CowFarmServer();
private:
    vector<Farm *> farms;
    vector<Cow *> cows;
    
    Cow *findCow(int socket);
    Cow *findCow(string name);
    
    Farm *findFarm(string name);
    
    void parseMessageFromCow(Cow *cow, char *message);
    CowFarmServer(const CowFarmServer& orig);
};

#endif	/* COWFARMSERVER_H */

