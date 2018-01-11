#include "TestWebServer.h"

using namespace std;

EmptyController::EmptyController() {}
EmptyController::~EmptyController() {}

bool EmptyController::Process(MongooseCpp::Request& request, MongooseCpp::Response& response)
{
    response.SetStatut(405);
    return true;
}

TestWebServer::TestWebServer() : TestClass("WebServer", this)
{
    addTest("Incorrect star", &TestWebServer::IncorrectStar);
    addTest("Missing ] or }", &TestWebServer::MissingChar);
    addTest("OptionalAtEnd", &TestWebServer::OptionalAtEnd);
    addTest("Start and stop", &TestWebServer::StartAndStop);
}

TestWebServer::~TestWebServer()
{
}

bool TestWebServer::IncorrectStar()
{
    MongooseCpp::WebServer server;
    MongooseCpp::stError myErr;

    assert(false==server.AddRoute("/base/*/[id]", &emptyCtrl));
    myErr = server.GetLastError();
    assert("ADDROUTE3"==myErr.Code);

    assert(false==server.AddRoute("/base/mod*", &emptyCtrl));
    myErr = server.GetLastError();
    assert("ADDROUTE3"==myErr.Code);

    return true;
}

bool TestWebServer::MissingChar()
{
    MongooseCpp::WebServer server;
    MongooseCpp::stError myErr;

    assert(false==server.AddRoute("/base/{controller/id", &emptyCtrl));
    myErr = server.GetLastError();
    assert("ADDROUTE1"==myErr.Code);

    assert(false==server.AddRoute("/base/[id", &emptyCtrl));
    myErr = server.GetLastError();
    assert("ADDROUTE2"==myErr.Code);

    return true;
}

bool TestWebServer::OptionalAtEnd()
{
    MongooseCpp::WebServer server;
    MongooseCpp::stError myErr;

    assert(false==server.AddRoute("/base/[id]/{controller}", &emptyCtrl));
    myErr = server.GetLastError();
    assert("ADDROUTE4"==myErr.Code);

    assert(false==server.AddRoute("/base/[id]/web", &emptyCtrl));
    myErr = server.GetLastError();
    assert("ADDROUTE4"==myErr.Code);

    return true;
}

bool TestWebServer::StartAndStop()
{
    MongooseCpp::WebServer server;

    server.Start();
    server.Poll(100);
    server.Stop();

    return true;
}
