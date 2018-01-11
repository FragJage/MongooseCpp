#ifndef RESPONSE_H
#define RESPONSE_H

#include<string>
#include<map>
#include "mongoose/mongoose.h"

namespace MongooseCpp {

class Response
{
    public:
        Response();
        virtual ~Response();

        void SetHeader(std::string headerKey, std::string value);
        void SetHeader(std::string key, int value);
        std::string GetHeader(std::string headerKey);
        bool HasHeader(std::string headerKey);
        void SetContent(std::string content);
        std::string GetData();
        void SetStatut(int number);

    private:
        std::string m_Protocol;
        int m_Statut;
        std::string m_StatutMsg;
        std::map<std::string, std::string> m_Headers;
        std::string m_Content;
};

}

#endif // RESPONSE_H
