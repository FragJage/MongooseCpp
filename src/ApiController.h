#ifndef APICONTROLLER_H
#define APICONTROLLER_H

#include "IWebController.h"

namespace MongooseCpp {

class ApiController : public IWebController
{
    public:
        ApiController();
        virtual ~ApiController();
        bool Process(Request& request, Response& response);
        virtual bool Get(Request& request, Response& response);
        virtual bool Put(Request& request, Response& response);
        virtual bool Post(Request& request, Response& response);
        virtual bool Delete(Request& request, Response& response);
};

}

#endif // APICONTROLLER_H
