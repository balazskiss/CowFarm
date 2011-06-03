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

class Farm {
public:
    Farm(string name);
    Farm(const Farm& orig);
    virtual ~Farm();
    
    string getName();
    Farm& addCow(Cow *cow);
    Farm& removeCow(Cow *cow);
    unsigned numberOfCows();
    
    Farm &cowList();
    Farm &msg(Cow *cow, string message);
    
private:
    string name;
    set<Cow *> cows;
};

#endif	/* FARM_H */

