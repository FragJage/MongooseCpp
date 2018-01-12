#include <sstream>
#include "Request.h"

using namespace std;
namespace MongooseCpp {

Request::Request(struct mg_connection* conn, struct http_message* httpMsg)
{
    string method(httpMsg->method.p, httpMsg->method.len);

    if(method=="GET")
        m_Method = Request::mthGET;
    else if(method=="POST")
        m_Method = Request::mthPOST;
    else if(method=="PUT")
        m_Method = Request::mthPUT;
    else if(method=="DELETE")
        m_Method = Request::mthDELETE;
    else if(method=="PATCH")
        m_Method = Request::mthPATCH;
    else if(method=="HEAD")
        m_Method = Request::mthHEAD;
    else m_Method = Request::mthUNKNOWN;

    m_Uri = string(httpMsg->uri.p, httpMsg->uri.len);
    istringstream iss(m_Uri);
    string token;

    while (getline(iss, token, '/'))
    {
        m_UriPart.push_back(token);
    }

    m_HttpMsg = httpMsg;
    m_Conn = conn;
}

Request::~Request()
{
}

Request::HttpMethod Request::GetMethod()
{
    return m_Method;
}

string Request::GetUri()
{
    return m_Uri;
}

string Request::GetBody()
{
    return string(m_HttpMsg->body.p, m_HttpMsg->body.len);
}

string Request::GetUriPart(unsigned int part)
{
    if(part>=m_UriPart.size()) return "";
    return m_UriPart[part];
}

string Request::GetBodyParameter(string key)
{
    char param[128];
    int len;

    len = mg_get_http_var(&m_HttpMsg->body, key.c_str(), param, sizeof(param));
    if(len<0) return "";
    return string(param, len);
}

string Request::GetQueryParameter(string key)
{
    char param[128];
    int len;

    len = mg_get_http_var(&m_HttpMsg->query_string, key.c_str(), param, sizeof(param));
    if(len<0) return "";
    return string(param, len);
}

bool Request::ExistsParameter(string key)
{
    return (m_Parameters.find(key) != m_Parameters.end());
}

string Request::GetParameter(string key)
{
    if(m_Parameters.find(key) == m_Parameters.end()) return "";
    return m_Parameters[key];
}

void Request::SetParameter(const string& key, const string& value)
{
    m_Parameters[key] = value;
}

struct mg_connection* Request::GetMgConnection()
{
    return m_Conn;
}

struct http_message* Request::GetHttpMsg()
{
    return m_HttpMsg;
}

}
