#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <vector>
#include <map>
#include "mongoose/mongoose.h"

namespace MongooseCpp {

class Request
{
    public:
        enum HttpMethod {mthGET, mthPOST, mthPUT, mthDELETE, mthPATCH, mthHEAD, mthUNKNOWN};
        Request();
        Request(struct mg_connection* conn, struct http_message* httpMsg);
        Request(const Request& resquest);
        Request& operator=(Request& other);
        ~Request();

        std::string GetUri();
        std::string GetBody();
        std::string GetUriPart(unsigned int part);
        HttpMethod GetMethod();
        bool ExistsParameter(const std::string& key);
        std::string GetParameter(const std::string& key);
        std::string GetQueryParameter(const std::string& key);
        std::string GetBodyParameter(const std::string& key);
        template <typename T> T GetParameter(const std::string& key)
        {
            std::string value = GetParameter(key);
            std::istringstream iss(value);
            T val;
            iss >> val;
            return val;
        }
        void SetParameter(const std::string& key, const std::string& value);
        struct mg_connection* GetMgConnection() const;
        struct http_message* GetHttpMsg() const;

    protected:

    private:
        void Initialization(struct mg_connection* conn, struct http_message* httpMsg);
        struct http_message* m_HttpMsg;
        struct mg_connection* m_Conn;
        HttpMethod m_Method;
        std::string m_Uri;
        std::vector<std::string> m_UriPart;
        std::map<std::string, std::string> m_Parameters;
};

}

#endif // REQUEST_H
