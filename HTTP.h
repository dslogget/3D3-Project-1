#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>


class HTTPRequest
{
    public:
        HTTPRequest(char* message, char* wrkdir);
        virtual ~HTTPRequest();

        const std::string& get_response();
        bool keep_alive = 0;


    protected:

    private:

        char req[1024] = {0};
        std::string res;
        std::vector<std::vector<char*>> lines;


};

#endif // HTTPREQUEST_H
