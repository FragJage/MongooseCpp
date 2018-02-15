#include "TestBaseController.h"
#include "HttpHelper.h"

using namespace std;

BaseController::BaseController() {}
BaseController::~BaseController() {}

bool BaseController::Process(MongooseCpp::Request& request, MongooseCpp::Response& response)
{
    string controller = request.GetParameter("controller");

    if(controller=="method")
    {
        switch(request.GetMethod())
        {
            case MongooseCpp::Request::mthGET :
                response.SetContent("mthGET");
                return true;
            case MongooseCpp::Request::mthPOST :
                response.SetContent("mthPOST");
                return true;
            case MongooseCpp::Request::mthPATCH :
                response.SetContent("mthPATCH");
                return true;
            case MongooseCpp::Request::mthHEAD :
                response.SetContent("mthHEAD");
                return true;
            default:
                response.SetStatut(404);
                return true;
        }
    }
    else
    {
        string content = controller;
        if(request.ExistsParameter("id"))
            content +=",id="+request.GetParameter("id");
        response.SetContent(content);
        return true;
    }
}

TestBaseController::TestBaseController() : TestClass("BaseController", this)
{
    addTest("ReadHttpMethod", &TestBaseController::ReadHttpMethod);
    addTest("ReadParameters", &TestBaseController::ReadParameters);
    addTest("Error 404", &TestBaseController::Error404);

    server.AddRoute("/base/{controller}/[id]", &myBaseCtrl);
    server.Start();
}

TestBaseController::~TestBaseController()
{
    server.Stop();
}

bool TestBaseController::ReadHttpMethod()
{
    string result;

    client.SendRequest("GET", "127.0.0.1", 8000, "/base/method");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetBody();
    assert("mthGET"==result);

    client.SendRequest("POST", "127.0.0.1", 8000, "/base/method", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetBody();
    assert("mthPOST"==result);

    client.SendRequest("PATCH", "127.0.0.1", 8000, "/base/method", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetBody();
    assert("mthPATCH"==result);

    client.SendRequest("HEAD", "127.0.0.1", 8000, "/base/method");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetBody();
    assert("mthHEAD"==result);

    return true;
}

bool TestBaseController::ReadParameters()
{
    string result;

    client.SendRequest("GET", "127.0.0.1", 8000, "/base/modules");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetBody();
    assert("modules"==result);

    client.SendRequest("GET", "127.0.0.1", 8000, "/base/devices/5");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetBody();
    assert("devices,id=5"==result);

    return true;
}

bool TestBaseController::Error404()
{
    string result;

    client.SendRequest("GET", "127.0.0.1", 8000, "/baseerr");
    assert(true==HttpHelper::WaitResponse(server, client));
    result = client.GetStatus();
    assert("404"==result);

    return true;
}
