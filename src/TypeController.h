#ifndef TYPECONTROLLER_H
#define TYPECONTROLLER_H

#include "ApiController.h"

namespace MongooseCpp {

template <typename Tid, class Tobject>
class TypeController : public ApiController
{
    public:
        TypeController() {};
        virtual ~TypeController() {};

        bool Process(Request& request, Response& response)
        {
            Tobject object;
            Tid id;

            switch(request.GetMethod())
            {
                case Request::HttpMethod::mthGET :
                    if(!request.ExistsParameter("Id"))
                    {
                        GetList(request, response);
                        return true;
                    }
                    id = ToId(request.GetParameter("Id"));
                    if(Get(id, object, response))

                    {
                        response.SetContent(ToString(object));
                    }
                    return true;
                case Request::HttpMethod::mthPUT :
                    if(!request.ExistsParameter("Id"))
                    {
                        response.SetStatut(500);
                        response.SetContent("Id parameter is required");
                        return true;
                    }
                    id = ToId(request.GetParameter("Id"));
                    ToObject(request.GetBody(), object);
                    if(Put(id, object, response))
                    {
                        response.SetStatut(200);
                        response.SetContent("OK");
                    }
                    return true;
                case Request::HttpMethod::mthPOST :
                    ToObject(request.GetBody(), object);
                    Post(object, response);
                    return true;
                case Request::HttpMethod::mthDELETE :
                    if(!request.ExistsParameter("Id"))
                    {
                        response.SetStatut(500);
                        response.SetContent("Id parameter is required");
                        return true;
                    }
                    id = ToId(request.GetParameter("Id"));
                    if(Delete(id, response))
                    {
                        response.SetStatut(200);
                        response.SetContent("OK");
                    }
                    return true;
            }

            response.SetStatut(405);
            return true;
        }

        virtual bool GetList(Request& request, Response& response) = 0;
        virtual bool Get(const Tid& id, Tobject& object, Response& response) = 0;
        virtual bool Put(const Tid& id, const Tobject& object, Response& response) = 0;
        virtual bool Post(const Tobject& object, Response& response) = 0;
        virtual bool Delete(const Tid& id, Response& response) = 0;

    protected:
        virtual std::string ToString(const Tobject& object) = 0;
        virtual Tid ToId(std::string value) = 0;
        virtual void ToObject(std::string value, Tobject& object) = 0;
};

}

#endif // TYPECONTROLLER_H
