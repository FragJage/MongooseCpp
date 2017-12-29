#include <sstream>
#include "Request.h"

using namespace std;
namespace MongooseCpp {

Request::Request(struct http_message* httpMsg)
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
}

Request::~Request()
{
    //dtor
}

Request::HttpMethod Request::GetMethod()
{
    return m_Method;
}

string Request::GetUri()
{
    return m_Uri;
}

string Request::GetUriPart(int part)
{
    if(part>=m_UriPart.size()) return "";
    return m_UriPart[part];
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

}
