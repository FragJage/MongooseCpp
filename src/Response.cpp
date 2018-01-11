#include <sstream>
#include "Response.h"

using namespace std;

namespace MongooseCpp {

Response::Response()
{
    m_Protocol = "HTTP/1.1";
    SetStatut(200);
}

Response::~Response()
{
}

void Response::SetStatut(int number)
{
    m_Statut = number;
    //m_StatutMsg = mg_status_message(number);
}

void Response::SetHeader(string headerKey, string value)
{
    m_Headers[headerKey] = value;
}

void Response::SetHeader(string key, int value)
{
    ostringstream stream;

    stream << value;
    m_Headers[key] = stream.str();
}

string Response::GetHeader(string headerKey)
{
    if(m_Headers.find(headerKey) == m_Headers.end()) return "";
    return m_Headers[headerKey];
}

bool Response::HasHeader(string headerKey)
{
    return (m_Headers.find(headerKey) != m_Headers.end());
}

void Response::SetContent(string content)
{
    m_Content = content;
}

string Response::GetData()
{
    ostringstream stream;
    map<std::string, std::string>::iterator it;


    stream << m_Protocol <<" "<< m_Statut;
    if(m_StatutMsg!="") stream <<" "<< m_StatutMsg;
    stream << "\r\n";

    SetHeader("Content-Length", m_Content.size());
    for(it=m_Headers.begin(); it!=m_Headers.end(); ++it)
    {
        stream << it->first <<": "<< it->second << "\r\n";
    }
    stream << "\r\n";
    stream << m_Content;

    return stream.str();
}

}
