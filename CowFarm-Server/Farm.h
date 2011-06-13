/* 
 * File:   Farm.h
 * Author: balazs
 *
 * Created on June 2, 2011, 10:47 AM
 */

#ifndef FARM_H
#define	FARM_H

#include <string>
#include <set>

using namespace std;

class Cow;

/**
 * Represents a farm.
 * A farm can contain cows and they can communicate with each other
 */
class Farm {
public:
    /**
     * Initializes a farm
     * @param name the name of the farm, must be unique
     */
    Farm(string name);
    
    /**
     * Gets the name of the farm
     * @return the name of the farm
     */
    string getName();
    /**
     * Adds cow to the farm
     * @param cow the cow
     * @return the farm
     */
    Farm& addCow(Cow *cow);
    /**
     * Remove cow from the farm
     * @param cow the cow
     * @return the farm
     */
    Farm& removeCow(Cow *cow);
    /**
     * Returns the number of cows in the farm
     * @return number of cows
     */
    unsigned numberOfCows();
    
    /**
     * Sends a list of cows to all cows in the farm
     * @return the farm
     */
    Farm &cowList();
    /**
     * Send a message to a cow
     * @param cow the cow
     * @param message the message
     * @return the farm
     */
    Farm &msg(Cow *cow, string message);
    
private:
    string name;
    set<Cow *> cows;
    
    Farm(const Farm& orig);
};

#endif	/* FARM_H */

