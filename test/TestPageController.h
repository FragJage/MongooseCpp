#include <cassert>
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
