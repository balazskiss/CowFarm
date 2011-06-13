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

/**
 * Represents a cow in the farm
 */
class Cow {
public:
    /**
     * Initializes a cow with a socket descriptor
     * @param socket the socket descriptor
     */
    Cow(int socket);
    
    /**
     * Sends message to cow
     * @param message the message
     * @return the cow
     */
    Cow& msg(string message);
    /**
     * Sends message to cow and appends newline character
     * @param message the message
     * @return the cow
     */
    Cow& msg_nl(string message);
    
    /**
     * Gets the socket
     * @return the socket
     */
    int getSocket();
    /**
     * Gets the name of the cow
     * @return name
     */
    string getName();
    /**
     * Gets the farm of the cow
     * @return farm
     */
    Farm *getFarm();
    /**
     * Gets X location of cow in the farm
     * @return x position
     */
    int getXPosition();
    /**
     * Gets Y position of cow in the farm
     * @return y position
     */
    int getYPosition();
    /**
     * Gets the direction of the cow
     * Return 0 or 1 where
     * 0 means the cow faces West
     * 1 means the cow faces East
     * @return direction
     */
    int getDirection();
    
    /**
     * Sets the cow's name
     * @param name the name
     * @return the cow
     */
    Cow& setName(string name);
    /**
     * Sets the cow's farm
     * @param farm the farm
     * @return the cow
     */
    Cow& setFarm(Farm* farm);
    /**
     * Sets the cow's direction
     * @param dir 0 or 1
     * @return the cow
     */
    Cow& setDirection(int dir);
    /**
     * Moves the cow to given location
     * @param x x position
     * @param y y position
     * @return the cow
     */
    Cow &move(int x, int y);
    
private:
    string name;
    int socket;
    Farm *farm;
    int x, y, dir;
    
    Cow(const Cow& orig);
};

#endif	/* COW_H */

