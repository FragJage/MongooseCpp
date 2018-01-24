#include "PageController.h"

using namespace std;
namespace MongooseCpp {

PageController::PageController(string documentRoot, bool enableDirectoryListing) : m_DocumentRoot(documentRoot), m_EnableDirectoryListing(enableDirectoryListing)
{
}

PageController::~PageController()
{
}

#ifndef _MSC_VER
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"                 //This is a virtual function
#endif

bool PageController::Process(Request& request, Response& response)
{
    static struct mg_serve_http_opts s_http_server_opts;
    s_http_server_opts.document_root = m_DocumentRoot.c_str();
    if(m_EnableDirectoryListing)
        s_http_server_opts.enable_directory_listing = "yes";
    else
        s_http_server_opts.enable_directory_listing = "no";
    struct mg_connection* nc = request.GetMgConnection();
    struct http_message* msg = request.GetHttpMsg();

    mg_serve_http(nc, msg, s_http_server_opts);
    return true;
}

#ifndef _MSC_VER
#pragma GCC diagnostic pop
#endif
}
