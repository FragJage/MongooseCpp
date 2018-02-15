#include "TestPageController.h"

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
    string result;
    MongooseCpp::PageController pageCtrl("./test", true);
    MongooseCpp::WebServer server(8001);
    HttpClient client;


    server.AddRoute("/pages/*", &pageCtrl);
    server.Start();

    client.SendRequest("GET", "127.0.0.1", 8001, "/pages/");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetStatus();
    assert("200"==result);

    server.Stop();
    return true;
}

bool TestPageController::WithoutDirListing()
{
    string result;
    MongooseCpp::PageController pageCtrl("./test", false);
    MongooseCpp::WebServer server(8001);
    HttpClient client;


    server.AddRoute("/pages/*", &pageCtrl);
    server.Start();

    client.SendRequest("GET", "127.0.0.1", 8001, "/pages/");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetStatus();
    assert("403"==result);

    server.Stop();
    return true;
}

bool TestPageController::SimplePage()
{
    string result;
    MongooseCpp::PageController pageCtrl("./test", false);
    MongooseCpp::WebServer server(8001);
    HttpClient client;


    server.AddRoute("/pages/*", &pageCtrl);
    server.Start();

    client.SendRequest("GET", "127.0.0.1", 8001, "/pages/test.html");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetStatus();
    assert("200"==result);
    result = client.GetBody();
    assert("<H1>Test</H1>"==result);

    server.Stop();
    return true;
}

bool TestPageController::Error404()
{
    string result;
    MongooseCpp::PageController pageCtrl("./test", false);
    MongooseCpp::WebServer server(8001);
    HttpClient client;


    server.AddRoute("/pages/*", &pageCtrl);
    server.Start();

    client.SendRequest("GET", "127.0.0.1", 8001, "/pages/test404.html");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetStatus();
    assert("404"==result);

    server.Stop();
    return true;
}
