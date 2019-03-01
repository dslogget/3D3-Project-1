#include "Server.h"

char* wrkdir;

Server::Server(const char* serverIP, int port)
{
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd == -1){
        std::cout << "Err Sock" << std::cout;
        throw(1);
    }

    struct sockaddr_in adr = {0};

    adr.sin_family = AF_INET;

    adr.sin_addr.s_addr = inet_addr(serverIP);
    adr.sin_port = htons(port);

    if(bind(socketfd, (struct sockaddr*) &adr, sizeof(adr)) == -1){
        std::cout << "Err bind" << std::endl;
        throw(2);
    }
    std::cout << "Bound" << std::endl;

    if(listen(socketfd, 5) == -1){
        std::cout << "Error listen" << std::endl;
        throw(3);
    }
    std::cout << "listening" << std::endl;
}

Server::~Server()
{
    shutdown(socketfd, 2);

    for(auto& th : threads){
        th.join();
    }
}


void Server::accept_new()
{
    struct sockaddr_storage clientaddr;
    int clientsock;
    socklen_t clientsize = sizeof(clientaddr);


    clientsock = accept(socketfd, (struct sockaddr*)&clientaddr, &clientsize);
    //std::cout << clientsock << std::endl;

    //std::thread thread(handle_request, clientsock);
    threads.emplace_back(handle_request, clientsock);

}


void Server::clean_threads()
{
    for(int i = threads.size() - 1; i >= 0; i--){
        if(threads[i].joinable()){
            threads[i].join();
            threads.erase(threads.begin() + i);
        }
    }
}


void handle_request(int clientsock)
{
    char buf[1024] = {0};
    int sizeRecv = recv(clientsock, buf, 1024, 0);
    std::cout << buf << std::endl;
    bool persist = 0;
    do{
        HTTPRequest req(buf, wrkdir);
        persist = req.keep_alive;


        const std::string res = req.get_response();
        std::cout << res << std::endl;
        int len = res.length();

        int oldlen = 0;
        while(len > 0){
            send(clientsock, &res.c_str()[oldlen], len, 0);
            oldlen = len;
            len = res.length() - len;
        }
        sizeRecv = recv(clientsock, buf, 1024, 0);

    }while(sizeRecv && persist);



    std::cout << "shutting down" << std::endl;

    shutdown(clientsock, 2);
}



