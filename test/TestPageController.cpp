#include "TestPageController.h"
#include "PageController.h"
#include "WebServer.h"
#include "HttpClient.h"
#include "HttpHelper.h"

using namespace std;

TestPageController::TestPageController() : TestClass("PageController", this)
{
    addTest("WithDirListing", &TestPageController::WithDirListing);
    addTest("WithoutDirListing", &TestPageController::WithoutDirListing);
    addTest("SimplePage", &TestPageController::SimplePage);
    addTest("Error 404", &TestPageController::Error404);
}

TestPageController::~TestPageController()
{
}

bool TestPageController::WithDirListing()
{
    MongooseCpp::PageController pageCtrl("./test", true);
    MongooseCpp::WebServer server(8001);
    HttpClient client;


    server.AddRoute("/pages/*", &pageCtrl);
    server.Start();

    client.SendRequest("GET", "127.0.0.1", 8001, "/pages/");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("200"==client.GetStatus());

    server.Stop();
    return true;
}

bool TestPageController::WithoutDirListing()
{
    MongooseCpp::PageController pageCtrl("./test", false);
    MongooseCpp::WebServer server(8001);
    HttpClient client;


    server.AddRoute("/pages/*", &pageCtrl);
    server.Start();

    client.SendRequest("GET", "127.0.0.1", 8001, "/pages/");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("403"==client.GetStatus());

    server.Stop();
    return true;
}

bool TestPageController::SimplePage()
{
    MongooseCpp::PageController pageCtrl("./test", false);
    MongooseCpp::WebServer server(8001);
    HttpClient client;


    server.AddRoute("/pages/*", &pageCtrl);
    server.Start();

    client.SendRequest("GET", "127.0.0.1", 8001, "/pages/test.html");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("200"==client.GetStatus());
    assert("<H1>Test</H1>"==client.GetBody());

    server.Stop();
    return true;
}

bool TestPageController::Error404()
{
    MongooseCpp::PageController pageCtrl("./test", false);
    MongooseCpp::WebServer server(8001);
    HttpClient client;


    server.AddRoute("/pages/*", &pageCtrl);
    server.Start();

    client.SendRequest("GET", "127.0.0.1", 8001, "/pages/test404.html");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("404"==client.GetStatus());

    server.Stop();
    return true;
}
