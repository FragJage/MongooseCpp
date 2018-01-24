#include "HttpHelper.h"

#include <iostream>

bool HttpHelper::WaitResponse(MongooseCpp::WebServer& server, HttpClient& client)
{
std::cout << "HWAIT..." << std::endl;
    int i, imax=25;
    for(i=0; i<imax; i++)
    {
std::cout << i << "...";
        server.Poll(100);
        if(client.WaitingResponse(100)) break;
    }

    if(i==imax) return false;

std::cout << "BEFORE RECV..." << std::endl;
	client.ReceiveResponse();
    return true;
}
