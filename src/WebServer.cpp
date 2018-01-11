#include <sstream>
#include "WebServer.h"

using namespace std;
namespace MongooseCpp {

WebServer::WebServer() : WebServer(8000)
{
}

WebServer::WebServer(int TCPPort)
{
    m_MgConnection = nullptr;
    m_TCPPort = TCPPort;
    m_isStarted = false;
}

WebServer::~WebServer()
{
    if(m_isStarted) Stop();
}

void WebServer::StaticEventHandler(struct mg_connection* nc, int ev, void* ev_data, void* user_data)
{
    if(ev!=MG_EV_HTTP_REQUEST) return;

    WebServer* server = (WebServer*) user_data;
    if(server==nullptr) return;

    struct http_message* msg = (struct http_message*) ev_data;
    if(msg==nullptr) return;

    server->EventHandler(nc, ev, msg);
}

void WebServer::EventHandler(struct mg_connection* nc, int ev, struct http_message* msg)
{
    vector<struct stController>::const_iterator it;
    bool match = false;
    Request request(nc, msg);
    Response response;

    response.SetHeader("Server", "webServer/1.0.0");
    //stream << "timeout=" << m_timeOut << ", max=" << m_maxConn;
    //response->SetHeader("Keep-Alive", stream.str());

    for(it=m_Controllers.begin(); it!=m_Controllers.end(); ++it)
    {
        if(matchUrl(request, *it))
        {
            if(it->controller->Process(request, response))
            {
                string data = response.GetData();
                if(data!="") mg_send(nc, data.c_str(), data.size());
                match = true;
                break;
            }
        }
    }

    if(!match)
        mg_http_send_error(nc, 404, "");
}

bool WebServer::Start()
{
    string TCPPort = to_string(m_TCPPort);

    if(m_isStarted) Stop();
    mg_mgr_init(&m_MgManager, nullptr);
    m_MgConnection = mg_bind(&m_MgManager, TCPPort.c_str(), WebServer::StaticEventHandler, this);
    if (m_MgConnection == nullptr)
    {
        setLastError("START1", "Failed to create listener");
        return false;
    }
    mg_set_protocol_http_websocket(m_MgConnection);

    m_isStarted = true;
    return true;
}

void WebServer::Stop()
{
    mg_mgr_free(&m_MgManager);
    m_isStarted = false;
}

void WebServer::Poll(unsigned int milliTimeout)
{
    mg_mgr_poll(&m_MgManager, milliTimeout);
}

bool WebServer::AddRoute(const string& route, IWebController* controller)
{   // /web/*
    // /api/v1/modules/[id]
    // /api/v1/devices/[id]/*
    // /api/v1/{controller}/[id]

    bool atTheEnd = false;
    bool hasOptional = false;
    istringstream iss(route);
    string token;
    stParam myParam;
    stController myCtrl;

    while (getline(iss, token, '/'))
    {
        if(atTheEnd)
        {
            setLastError("ADDROUTE3", "Character '*' must be at the end of the route.");
            return false;
        }

        if((token.find('*')!=string::npos)&&(token!="*"))
        {
            setLastError("ADDROUTE3", "Character '*' must be alone at the end of the route.");
            return false;
        }

        switch(token.front())
        {
            case '{' :
                if(token.back()!='}')
                {
                    setLastError("ADDROUTE1", "Missing character '}'.");
                    return false;
                }
                if(hasOptional)
                {
                    setLastError("ADDROUTE4", "Optional parameter must be all at the end of the route.");
                    return false;
                }
                myParam.name = token.substr(1, token.size()-2);
                myParam.paramType = ParamType::Required;
                break;

            case '[' :
                if(token.back()!=']')
                {
                    setLastError("ADDROUTE2", "Missing character ']'.");
                    return false;
                }
                hasOptional = true;
                myParam.name = token.substr(1, token.size()-2);
                myParam.paramType = ParamType::Optional;
                break;

            case '*' :
                atTheEnd = true;
                myParam.name = "*";
                myParam.paramType = ParamType::Optional;
                break;

            default :
                if(hasOptional)
                {
                    setLastError("ADDROUTE4", "Optional parameter must be all at the end of the route.");
                    return false;
                }
                myParam.name = token;
                myParam.paramType = ParamType::Fixed;
        }
        myCtrl.paramPartQuery.push_back(myParam);
    }

    myCtrl.controller = controller;
    m_Controllers.push_back(myCtrl);
    return true;
}

bool WebServer::matchUrl(Request& request, const struct stController& controller)
{   // /web/*
    // /api/v1/modules/[id]
    // /api/v1/devices/[id]/*

    int i=0;
    string uriPart;
    vector<struct stParam>::const_iterator it = controller.paramPartQuery.begin();
    vector<struct stParam>::const_iterator itEnd = controller.paramPartQuery.end();

    while(it != itEnd)
    {
        uriPart = request.GetUriPart(i);
        switch(it->paramType)
        {
            case ParamType::Fixed :
                if(uriPart!=it->name) return false;
                break;
            case ParamType::Required :
                if(uriPart=="") return false;
                request.SetParameter(it->name, uriPart);
                break;
            case ParamType::Optional :
                if(it->name=="*") return true;
                if(uriPart!="") request.SetParameter(it->name, uriPart);
                break;
        }
        ++it;
        ++i;
    }

    if(request.GetUriPart(i)!="") return false;
    return true;
}

stError WebServer::GetLastError()
{
    return m_LastError;
}

void WebServer::setLastError(string code, string msg)
{
    m_LastError.Code = code;
    m_LastError.Message = msg;
}

}
