#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include <string>
#include "IWebController.h"
#include "mongoose/mongoose.h"

namespace MongooseCpp {

class PageController : public IWebController
{
    public:
        PageController(std::string documentRoot, bool enableDirectoryListing);
        virtual ~PageController();
        bool Process(Request& request, Response& response);

    private:
        std::string m_DocumentRoot;
        bool m_EnableDirectoryListing;
};

}

#endif // PAGECONTROLLER_H
