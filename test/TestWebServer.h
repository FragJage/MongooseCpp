#include <cassert>
#include "IWebController.h"
#include "WebServer.h"
#include "UnitTest/UnitTest.h"

using namespace std;

class EmptyController : public MongooseCpp::IWebController
{
    public:
        EmptyController();
        ~EmptyController();
        bool Process(MongooseCpp::Request& request, MongooseCpp::Response& response);
};

class TestWebServer : public TestClass<TestWebServer>
{
    public:
        TestWebServer();
        ~TestWebServer();

        bool IncorrectStar();
        bool MissingChar();
        bool OptionalAtEnd();
        bool StartAndStop();

    private:
        EmptyController emptyCtrl;
};
