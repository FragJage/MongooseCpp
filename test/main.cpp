#include <iostream>
#include "main.h"
#include "WebServer.h"

using namespace std;


MyController::MyController(string name)
{
    m_Name = name;
}

MyController::~MyController()
{
}

bool MyController::Process(const MongooseCpp::Request& request, MongooseCpp::Response& response)
{
    response.SetContent(m_Name);
    return true;
}

int main(void)
{
    MongooseCpp::WebServer server;

    server.AddRoute("/web/*", new MyController("Web"));
    server.AddRoute("/api/v1/modules/[id]", new MyController("modules"));
    server.AddRoute("/api/v1/devices/[id]/*", new MyController("devices"));
    server.AddRoute("/api/v1/{controller}/[id]", new MyController("multiController"));

    if(!server.Start())
    {
        return -1;
    }

    for(;;)
    {
        server.Poll();
    }
    server.Stop();

    return 0;
}

/*
server.setOption("document_root", "html");
server.setOption("listening_ports", "8080");
server.setOption("num_threads", "5");

server.start();

cout << "Test server started, press enter to quit..." << endl;
cin.ignore();

server.stop();
*/
/*
    m_PagesController.SetRootDir(documentRoot);
    m_PagesController.AddDefaultPage("index.html");
    m_WebMgr.AddRoute("/web/*", &m_PagesController);

    m_ModulesController.SetCacheManager(&m_CacheMgr);
    m_ModulesController.SetxPLSender(&m_xPLMgr);
    m_WebMgr.AddRoute("/api/v1/modules/[id]", &m_ModulesController);

    m_AdvanceConfigController.SetCacheManager(&m_CacheMgr);
    m_AdvanceConfigController.SetxPLSender(&m_xPLMgr);
    m_WebMgr.AddRoute("/api/v1/advanceconfig/[id]", &m_AdvanceConfigController);

    m_DevicesController.SetCacheManager(&m_CacheMgr);
    m_DevicesController.SetxPLSender(&m_xPLMgr);
    m_WebMgr.AddRoute("/api/v1/devices/[id]", &m_DevicesController);

    m_DataloggersController.SetCacheManager(&m_CacheMgr);
    m_DataloggersController.SetxPLSender(&m_xPLMgr);
    m_WebMgr.AddRoute("/api/v1/datalogger/{search}/[device]", &m_DataloggersController);

    m_MessagesController.SetxPLSender(&m_xPLMgr);
    m_WebMgr.AddRoute("/api/v1/messages/", &m_MessagesController);
*/
