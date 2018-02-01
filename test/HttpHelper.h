#ifndef HTTPHELPER_H
#define HTTPHELPER_H

#include "WebServer.h"
#include "HttpClient.h"

class HttpHelper
{
    public:
        static bool WaitResponse(MongooseCpp::WebServer& server, HttpClient& client);
};

#endif // HTTPHELPER_H
