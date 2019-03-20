#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <fstream>

int main(int argc, char** arg)
{
    for(int i = 1; i < argc; i++){
        std::cout << "url " << i << std::endl;
        char* iter = arg[i];
        while(*iter){
            *iter = tolower(*iter);
            iter++;
        }
        char* domain;
        char* port;
        char* toget;
        //http://adsadas:12323423/file
        iter = strtok(arg[i], "://");
        /*while(*iter){
            iter++;
        }
        iter++;
        while(*iter == '/'){
            iter++;
        }*/

        domain = strtok(0, "://");
        port = strtok(0, "/\0");
        char tmp[] = "127.0.0.1";
        if(!strcmp(domain, "localhost")){
            domain = tmp;
        }
        //std::cout << *domain << std::endl;


        toget = strtok(0, "\0");


        std::cout << domain << std::endl << (port?port:"80") << std::endl << (toget?toget:"") << std::endl;


        if(!strcmp(arg[i], "http")){

            bool done = false;
            char buf[1024] = {0};

            while(!done){
                int socketfd = socket(AF_INET, SOCK_STREAM, 0);
                if(socketfd == -1){
                    std::cout << "Err Sock" << std::cout;
                    throw(1);
                }

                struct sockaddr_in adr = {0};

                adr.sin_family = AF_INET;
                adr.sin_addr.s_addr = inet_addr(domain);
                adr.sin_port = htons(port?atoi(port):80);

                if(connect(socketfd, (struct sockaddr*) &adr, sizeof(adr)) == -1){
                    std::cout << "Err connect" << std::endl;
                    throw(2);
                }


                std::cout << "connected" << std::endl;
                std::string req = "GET /";
                if(toget){
                    req += toget;
                }
                req += " http/1.0\r\n";
                char* iter;
                std::fstream savefile(toget, std::fstream::out);
                send(socketfd, req.c_str(), req.length(), 0);
                recv(socketfd, buf, sizeof(buf), 0);



                char* body = strstr(buf, "\r\n\r\n");
                iter = buf;
                while(iter != body&&*iter){
                    *iter = tolower(*iter);
                    iter++;
                }
                strtok(buf, " ");
                iter = strtok(0, " ");
                int code = atoi(iter);
                body = body + 4;
                while(*iter){
                    iter++;
                }
                iter++;

                std::cout << code << std::endl;
                if(code == 301){
                    char* ptr = strstr(iter, "location");
                    ptr = strtok(ptr, ": \r\n");
                    toget = strtok(0, ": \r\n");
                    std::cout << ++toget << std::endl;
                    //sleep(1);

                }else{
                    done = true;
                    std::cout << "Opening file " << toget << std::endl;
                    std::cout << body << std::endl;
                    savefile << body;
                    savefile.close();
                }
                shutdown(socketfd, 2);
            }
        }
    }

    //while(1);

    return 0;
}
