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
    client.SendRequest("GET", "127.0.0.1", 8002, "/api/v1");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("mthGET"==client.GetBody());

    client.SendRequest("GET", "127.0.0.1", 8002, "/api/empty");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("405"==client.GetStatus());

    return true;
}

bool TestApiController::POSTmethod()
{
    client.SendRequest("POST", "127.0.0.1", 8002, "/api/v1", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("mthPOST"==client.GetBody());

    client.SendRequest("POST", "127.0.0.1", 8002, "/api/empty", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("405"==client.GetStatus());

    return true;
}

bool TestApiController::PUTmethod()
{
    client.SendRequest("PUT", "127.0.0.1", 8002, "/api/v1", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("mthPUT"==client.GetBody());

    client.SendRequest("PUT", "127.0.0.1", 8002, "/api/empty", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("405"==client.GetStatus());

    return true;
}

bool TestApiController::DELETEmethod()
{
    client.SendRequest("DELETE", "127.0.0.1", 8002, "/api/v1", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("mthDELETE"==client.GetBody());

    client.SendRequest("DELETE", "127.0.0.1", 8002, "/api/empty", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("405"==client.GetStatus());

    return true;
}

bool TestApiController::UnknownMethod()
{
    client.SendRequest("DELZZZ", "127.0.0.1", 8002, "/api/v1", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("405"==client.GetStatus());

    client.SendRequest("DELZZZ", "127.0.0.1", 8002, "/api/empty", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("405"==client.GetStatus());

    return true;
}
