#include "CowFarmServer.h"


int main(int argc, char *argv[]){
    int port = 8765;
    if(argc == 2)
        port = atoi(argv[1]);
    
    CowFarmServer server(port);
    
    return 0;
}