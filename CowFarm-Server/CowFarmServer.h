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

/**
 * CowFarmServer class
 * It handles the cows, the farms, and the sockets
 * @param port
 */
class CowFarmServer {
public:
    /**
     * Initializes a server on given port
     * @param port the port number
     */
    CowFarmServer(int port = 8765);
    
private:
    set<Farm *> farms;
    set<Cow *> cows;
    
    Cow *findCow(int socket);
    Cow *findCow(string name);
    
    Farm *findFarm(string name);
    
    void parseMessageFromCow(Cow *cow, char *message);
    CowFarmServer(const CowFarmServer& orig);
};

#endif	/* COWFARMSERVER_H */

