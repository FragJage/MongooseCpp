#include "WebServer.h"

using namespace std;

namespace MongooseCpp {

WebServer::WebServer()
{
    m_MgConnection = nullptr;
}

WebServer::~WebServer()
{
    //dtor
}

void WebServer::EventHandler(struct mg_connection *nc, int ev, void *p)
{
    struct mg_serve_http_opts s_http_server_opts;
    s_http_server_opts.document_root = ".";  // Serve current directory
    s_http_server_opts.enable_directory_listing = "yes";
    if (ev == MG_EV_HTTP_REQUEST)
        mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
}

bool WebServer::Start()
{
    mg_mgr_init(&m_MgManager, nullptr);
    m_MgConnection = mg_bind(&m_MgManager, "8000", WebServer::EventHandler);
    if (m_MgConnection == nullptr)
    {
        setLastError("START1", "Failed to create listener");
        return false;
    }
    mg_set_protocol_http_websocket(m_MgConnection);
}

void WebServer::Stop()
{
    mg_mgr_free(&m_MgManager);
}

void WebServer::Poll()
{
    mg_mgr_poll(&m_MgManager, 1000);
}

void WebServer::setLastError(string code, string msg)
{
    m_LastErrCode = code;
    m_LastErrMessage = msg;
}

}
