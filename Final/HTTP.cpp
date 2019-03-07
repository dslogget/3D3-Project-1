#include "HTTP.h"
#define _t(a) std::cout<<a<<std::endl;

HTTPRequest::HTTPRequest(char* message, char* workingdir)
{
    //_t(0);
    char* mov = req;
    while(*message){
        *mov = (char)toupper((int)*message);
        mov++;
        message++;
    }
    *mov = 0;
    //_t(1);

    char delim[] = "\r\n";
    char delim2[] = " :";

    char* p = strtok(req, delim);

    while(p){
        lines.emplace_back();
        lines.at(lines.size() - 1).push_back(p);
        p = strtok(0, delim);
    }
    //_t(2);
    for(auto& ln : lines){
        char* str = ln[0];
        ln.pop_back();
        char* q = strtok(str, delim2);
        while(q){
            //_t(20);
            ln.push_back(q);
            //_t(21);
            q = strtok(0, delim2);
        }
    }
    //_t(3);
    res = "";
    if(lines.size() && lines[0].size() > 1){
        for(unsigned int i = 1; i < lines.size() - 1; i++){
            if(!strcmp(lines[i][0], "CONNECTION")){
                if(!strcasecmp(lines[i][1], "KEEP-ALIVE")){
                    keep_alive = true;
                }
                break;
            }
        }

        res += "HTTP/1.0 ";
        if(strcmp(lines.at(0).at(0), "GET") == 0){

            char filepath[100];
            strcpy(filepath, workingdir);
            int len = strlen(workingdir);
            if(filepath[len-1] != '/'){
                filepath[len++] = '/';
            }
            strcpy(&filepath[len], lines.at(0).at(1) + 1);
            if(!*(lines.at(0).at(1) + 1)){
                res += "301 Moved Permanantly\r\nLocation:/index.html\r\n";
                if(keep_alive){
                    res += "Connection: keep-alive\r\n";
                    res += "Keep-Alive: timeout=10, max=20\r\n";
                    res += "Content-length: ";
                    res += "0";
                    res += "\r\n";
                }
                res += "\r\n";

            }else{
                std::fstream file(filepath, std::ios::in | std::ios::binary);
                std::string content = "";
                if(file.is_open()){

                    while(!file.eof()){
                        std::string temp = "";
                        std::getline(file, temp);
                        content += temp;
                        if(!file.eof()){
                            content += '\n';
                        }
                    }
                    //content += "\r\n";

                    res += "200 OK\r\n";
                    if(keep_alive){
                        res+= "Connection: keep-alive\r\n";
                        res+= "Keep-Alive: timeout=10, max=20\r\n";
                        res += "Content-length: ";
                        res += std::to_string(content.size());
                        res += "\r\n";
                    }
                    res += "\r\n";
                    res += content;
                }else{
                    res += "404 Not Found\r\n";
                    content += "<!DOCTYPE html>\r\n";
                    content += "<html>\r\n";
                    content += "<body>\r\n";
                    content += "\r\n";
                    content += "<h1>ERROR: 404</h1>\r\n";
                    content += "\r\n";
                    content += "<p>FILE NOT FOUND</p>\r\n";
                    content += "\r\n";
                    content += "</body>\r\n";
                    content += "</html>\r\n";
                    if(keep_alive){
                        res += "Connection: keep-alive\r\n";
                        res += "Keep-Alive: timeout=10, max=20\r\n";
                        res += "Content-length: ";
                        res += std::to_string(content.size());
                        res += "\r\n";
                    }
                    res += "\r\n";
                    res += content;

                }
            }
        }else{
            res += "400 Bad Request";
            if(keep_alive){
                res+= "Connection: keep-alive\r\n";
                res+= "Keep-Alive: timeout=10, max=20\r\n";
                res += "Content-length: ";
                res += "0";
                res += "\r\n";
            }
            res += "\r\n";
        }


    }

}

const std::string& HTTPRequest::get_response()
{
    return res;
}



HTTPRequest::~HTTPRequest()
{
    //dtor
}
