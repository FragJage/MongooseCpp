#include "ApiController.h"

namespace MongooseCpp {

ApiController::ApiController()
{
}

ApiController::~ApiController()
{
}

bool ApiController::Process(Request& request, Response& response)
{
    switch(request.GetMethod())
    {
        case Request::HttpMethod::mthGET :
            return Get(request, response);
        case Request::HttpMethod::mthPUT :
            return Put(request, response);
        case Request::HttpMethod::mthPOST :
            return Post(request, response);
        case Request::HttpMethod::mthDELETE :
            return Delete(request, response);
        default :
            response.SetStatut(405);
            return true;
    }
}

bool ApiController::Get(Request& request, Response& response)
{
    response.SetStatut(405);
    return true;
}

bool ApiController::Put(Request& request, Response& response)
{
    response.SetStatut(405);
    return true;
}

bool ApiController::Post(Request& request, Response& response)
{
    response.SetStatut(405);
    return true;
}

bool ApiController::Delete(Request& request, Response& response)
{
    response.SetStatut(405);
    return true;
}


}
