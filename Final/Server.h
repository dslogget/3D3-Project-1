#ifndef SERVER_H
#define SERVER_H

#include <string.h>
#include <thread>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <time.h>

#include "HTTP.h"

extern char* wrkdir;


class Server
{
    public:
        Server(const char* serverIP, int port);
        virtual ~Server();

        void accept_new();
        void clean_threads();

    protected:

    private:

        std::vector<std::thread> threads;
        int socketfd = -1;

};

void handle_request(int clientsock);

#endif // SERVER_H
