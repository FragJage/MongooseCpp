#include <cassert>
#include "ApiController.h"
#include "WebServer.h"
#include "UnitTest/UnitTest.h"
#include "HttpClient.h"

using namespace std;

class ApiController : public MongooseCpp::ApiController
{
    public:
        ApiController();
        ~ApiController();
        bool Get(MongooseCpp::Request& request, MongooseCpp::Response& response);
        bool Post(MongooseCpp::Request& request, MongooseCpp::Response& response);
        bool Put(MongooseCpp::Request& request, MongooseCpp::Response& response);
        bool Delete(MongooseCpp::Request& request, MongooseCpp::Response& response);
};

class TestApiController : public TestClass<TestApiController>
{
    public:
        TestApiController();
        ~TestApiController();

        bool GETmethod();
        bool POSTmethod();
        bool PUTmethod();
        bool DELETEmethod();
        bool UnknownMethod();

    private:
        MongooseCpp::WebServer server;
        HttpClient client;
        ApiController myApiCtrl;
        MongooseCpp::ApiController myEmptyCtrl;
};
