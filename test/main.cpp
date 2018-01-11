#include <iostream>
#include "TestWebServer.h"
#include "TestBaseController.h"
#include "TestPageController.h"
#include "TestApiController.h"
#include "TestTypeController.h"


using namespace std;

//TO DO
//Câbler le Kill
//Example 1 : Simple web server
//Example 2 : Web server with page controller
//Example 3 : Web Api server with json controller

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

    /*
    MongooseCpp::WebServer server;
    JsonController myJsonCtrl;

    server.AddRoute("/api/v1/books/[Id]", &myJsonCtrl);
    server.Start();
    while(true)
        server.Poll();

    server.Stop();
    return 0;
    */
}
