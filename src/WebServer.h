#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <string>
#include <vector>
#include <sstream>
#include "mongoose/mongoose.h"
#include "IWebController.h"


#ifdef __MINGW32__
static std::string to_string(int data)
{
    std::stringstream s;
    s<<data;
    return s.str();
}
#endif // MINGW

namespace MongooseCpp {

enum ParamType { Fixed, Required, Optional};

struct stParam
{
    std::string name;
    ParamType paramType;
};

struct stController
{
    IWebController* controller;
    std::vector<struct stParam> paramPartQuery;
};

struct stError
{
    std::string Code;
    std::string Message;
};

class WebServer
{
    public:
        WebServer();
        WebServer(int TCPPort);
        virtual ~WebServer();

        bool Start();
        void Stop();
        void Poll(unsigned int milliTimeout=100);
        bool AddRoute(const std::string& route, IWebController* controller);
        stError GetLastError();
        static void StaticEventHandler(struct mg_connection* nc, int ev, void* ev_data, void* user_data);
        void EventHandler(struct mg_connection* nc, int ev, struct http_message* msg);

    protected:

    private:
        bool matchUrl(Request& request, const struct stController& controller);

        enum ParseStep { FixedUri, Parameter, Next};
        void setLastError(std::string code, std::string msg);

        bool m_isStarted;
        stError m_LastError;
        int m_TCPPort;
        std::vector<struct stController> m_Controllers;

        struct mg_mgr m_MgManager;
        struct mg_connection *m_MgConnection;
};

}
#endif // WEBSERVER_H
