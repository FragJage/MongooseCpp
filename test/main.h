#include "IWebController.h"

class MyController : public MongooseCpp::IWebController
{
    public:
        MyController(std::string name);
        virtual ~MyController();
        bool Process(const MongooseCpp::Request& request, MongooseCpp::Response& response);
    private:
        std::string m_Name;
};
