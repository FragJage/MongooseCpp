#include <iostream>
#include "TestWebServer.h"
#include "TestBaseController.h"
#include "TestPageController.h"
#include "TestApiController.h"
#include "TestTypeController.h"


using namespace std;

int main(void)
{
    int ret = 0;
    UnitTest unitTest;


    try
    {
        unitTest.addTestClass(new TestWebServer());
        unitTest.addTestClass(new TestBaseController());
        unitTest.addTestClass(new TestPageController());
        unitTest.addTestClass(new TestApiController());
        unitTest.addTestClass(new TestTypeController());
    }
    catch(const exception &e)
    {
        unitTest.displayError(e.what());
        ret = -1;
    }

    if(ret!=-1)
        if(!unitTest.run()) ret = 1;

    return ret;
}
