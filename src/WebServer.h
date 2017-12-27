#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <string>
#include "mongoose/mongoose.h"

namespace MongooseCpp {

class WebServer
{
    public:
        WebServer();
        virtual ~WebServer();

        bool Start();
        void Stop();
        void Poll();
        static void EventHandler(struct mg_connection *nc, int ev, void *p);

    protected:

    private:
        void setLastError(std::string code, std::string msg);

        std::string m_LastErrCode;
        std::string m_LastErrMessage;
        struct mg_mgr m_MgManager;
        struct mg_connection *m_MgConnection;
};

}
#endif // WEBSERVER_H
