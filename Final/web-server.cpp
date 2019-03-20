#include <string.h>
#include <thread>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Server.h"

int main(int argc, char** arg)
{
    char* addr;
    char tmp[] = "127.0.0.1";
    addr = tmp;
    int portnum = 40000;
    char tmp2[] = "./";
    wrkdir = tmp2;

    switch(argc){
case(4):
    wrkdir = arg[3];
case(3):
    portnum = atoi(arg[2]);
case(2):
    addr = arg[1];
    while(*arg[1]){
        *arg[1] = tolower(*arg[1]);
        arg[1]++;
    }
    if(!strcmp(addr, "localhost")){
        addr = tmp;
    }

default:
case(1):
    break;
    }


    Server myserv(addr, portnum);
    //myserv.accept_and_respond();
    while(1){
        myserv.accept_new();
        myserv.clean_threads();
    }

}
