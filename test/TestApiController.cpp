#include "TestApiController.h"
#include "WebServer.h"
#include "HttpClient.h"
#include "HttpHelper.h"

using namespace std;

ApiController::ApiController() {}
ApiController::~ApiController() {}

#ifndef _MSC_VER
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"                 //This is virtual functions
#endif

bool ApiController::Get(MongooseCpp::Request& request, MongooseCpp::Response& response)
{
    response.SetContent("mthGET");
    return true;
}

bool ApiController::Post(MongooseCpp::Request& request, MongooseCpp::Response& response)
{
    response.SetContent("mthPOST");
    return true;
}

bool ApiController::Put(MongooseCpp::Request& request, MongooseCpp::Response& response)
{
    response.SetContent("mthPUT");
    return true;
}

bool ApiController::Delete(MongooseCpp::Request& request, MongooseCpp::Response& response)
{
    response.SetContent("mthDELETE");
    return true;
}

#ifndef _MSC_VER
#pragma GCC diagnostic pop
#endif

TestApiController::TestApiController() : TestClass("TestApiController", this), server(8002)
{
    addTest("GETmethod", &TestApiController::GETmethod);
    addTest("POSTmethod", &TestApiController::POSTmethod);
    addTest("PUTmethod", &TestApiController::PUTmethod);
    addTest("DELETEmethod", &TestApiController::DELETEmethod);
    addTest("UnknownMethod", &TestApiController::UnknownMethod);

    server.AddRoute("/api/v1", &myApiCtrl);
    server.AddRoute("/api/empty", &myEmptyCtrl);
    server.Start();
}

TestApiController::~TestApiController()
{
    server.Stop();
}

bool TestApiController::GETmethod()
{
    string result;

    client.SendRequest("GET", "127.0.0.1", 8002, "/api/v1");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetBody();
    assert("mthGET"==result);

    client.SendRequest("GET", "127.0.0.1", 8002, "/api/empty");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetStatus();
    assert("405"==result);

    return true;
}

bool TestApiController::POSTmethod()
{
    string result;

    client.SendRequest("POST", "127.0.0.1", 8002, "/api/v1", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetBody();
    assert("mthPOST"==result);

    client.SendRequest("POST", "127.0.0.1", 8002, "/api/empty", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetStatus();
    assert("405"==result);

    return true;
}

bool TestApiController::PUTmethod()
{
    string result;

    client.SendRequest("PUT", "127.0.0.1", 8002, "/api/v1", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetBody();
    assert("mthPUT"==result);

    client.SendRequest("PUT", "127.0.0.1", 8002, "/api/empty", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetStatus();
    assert("405"==result);

    return true;
}

bool TestApiController::DELETEmethod()
{
    string result;

    client.SendRequest("DELETE", "127.0.0.1", 8002, "/api/v1", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetBody();
    assert("mthDELETE"==result);

    client.SendRequest("DELETE", "127.0.0.1", 8002, "/api/empty", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetStatus();
    assert("405"==result);

    return true;
}

bool TestApiController::UnknownMethod()
{
    string result;

    client.SendRequest("DELZZZ", "127.0.0.1", 8002, "/api/v1", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetStatus();
    assert("405"==result);

    client.SendRequest("DELZZZ", "127.0.0.1", 8002, "/api/empty", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetStatus();
    assert("405"==result);

    return true;
}
