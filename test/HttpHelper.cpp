#include "HttpHelper.h"

bool HttpHelper::WaitResponse(MongooseCpp::WebServer& server, HttpClient& client)
{
    int i, imax=25;
    for(i=0; i<imax; i++)
    {
        server.Poll(100);
        if(client.WaitingResponse(100)) break;
    }

    if(i==imax) return false;

    client.ReceiveResponse();
    return true;
}
