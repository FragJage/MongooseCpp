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
        Request(struct http_message* httpMsg);
        virtual ~Request();

        std::string GetUri();
        std::string GetUriPart(int part);
        HttpMethod GetMethod();
        bool ExistsParameter(std::string key);
        std::string GetParameter(std::string key);
        template <typename T> T GetParameter(std::string key)
        {
            std::string value = GetParameter(key);
            std::istringstream iss(value);
            T val;
            iss >> val;
            return val;
        }
        void SetParameter(const std::string& key, const std::string& value);

    protected:

    private:
        struct http_message* m_HttpMsg;
        HttpMethod m_Method;
        std::string m_Uri;
        std::vector<std::string> m_UriPart;
        std::map<std::string, std::string> m_Parameters;
};

}

#endif // REQUEST_H
