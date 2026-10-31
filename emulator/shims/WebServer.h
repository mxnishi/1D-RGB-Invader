#pragma once
#include "Arduino.h"
#define HTTP_GET 0
#define HTTP_POST 1
#define UPLOAD_FILE_START 0
#define UPLOAD_FILE_WRITE 1
#define UPLOAD_FILE_END 2
struct HTTPUpload{int status=0;uint8_t*buf=nullptr;size_t currentSize=0,totalSize=0;};
class WebServer{public:std::map<std::string,std::function<void()>> routes;std::map<std::string,String> args;String body,type,location;int code=200;HTTPUpload up;WebServer(int){}void on(const char*p,std::function<void()>f){routes[p]=f;}void on(const char*p,int,std::function<void()>f){routes[p]=f;}void on(const char*p,int,std::function<void()>f,std::function<void()>){routes[p]=f;}void begin(){}void handleClient(){}bool hasArg(String k){return args.count(k);}String arg(String k){return hasArg(k)?args[k]:String("");}void send(int c,const char*t="text/plain",String b=""){code=c;type=t;body=b;}void sendHeader(const char*k,const char*v){if(std::string(k)=="Location")location=v;}HTTPUpload& upload(){return up;}void request(String p){body="";location="";type="text/plain";code=200;if(routes.count(p))routes[p]();else send(404,"text/plain","Not found");}};
