#include <cassert>
#include "PageController.h"
#include "WebServer.h"
#include "HttpClient.h"
#include "HttpHelper.h"
#include "UnitTest/UnitTest.h"

using namespace std;

class TestPageController : public TestClass<TestPageController>
{
    public:
        TestPageController();
        ~TestPageController();

        bool WithDirListing();
        bool WithoutDirListing();
        bool SimplePage();
        bool Error404();
};
