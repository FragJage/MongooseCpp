#include <cassert>
#include "IWebController.h"
#include "WebServer.h"
#include "UnitTest/UnitTest.h"
#include "HttpClient.h"

using namespace std;

class BaseController : public MongooseCpp::IWebController
{
    public:
        BaseController();
        ~BaseController();
        bool Process(MongooseCpp::Request& request, MongooseCpp::Response& response);
};

class TestBaseController : public TestClass<TestBaseController>
{
    public:
        TestBaseController();
        ~TestBaseController();

        bool ReadHttpMethod();
        bool ReadParameters();
        bool Error404();

    private:
        MongooseCpp::WebServer server;
        HttpClient client;
        BaseController myBaseCtrl;
};
