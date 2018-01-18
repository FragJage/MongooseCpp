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

    response.SetStatut(404);
    return true;
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
    client.SendRequest("GET", "127.0.0.1", 8000, "/base/method");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("mthGET"==client.GetBody());

    client.SendRequest("POST", "127.0.0.1", 8000, "/base/method", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("mthPOST"==client.GetBody());

    client.SendRequest("PATCH", "127.0.0.1", 8000, "/base/method", "BODY");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("mthPATCH"==client.GetBody());

    client.SendRequest("HEAD", "127.0.0.1", 8000, "/base/method");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("mthHEAD"==client.GetBody());

    return true;
}

bool TestBaseController::ReadParameters()
{
    client.SendRequest("GET", "127.0.0.1", 8000, "/base/modules");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("modules"==client.GetBody());

    client.SendRequest("GET", "127.0.0.1", 8000, "/base/devices/5");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("devices,id=5"==client.GetBody());

    return true;
}

bool TestBaseController::Error404()
{
    client.SendRequest("GET", "127.0.0.1", 8000, "/baseerr");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("404"==client.GetStatus());

    return true;
}
