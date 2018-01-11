/***********************************************************************************************/
/**                                                                                           **/
/** EXAMPLE 1                                                                                 **/
/**                                                                                           **/
/** Simple web server                                                                         **/
/**                                                                                           **/
/***********************************************************************************************/

#include <iostream>
#include <sstream>
#include "WebServer.h"
#include "IWebController.h"

using namespace std;
using namespace MongooseCpp;

class HelloController : public IWebController
{
    bool Process(MongooseCpp::Request& request, MongooseCpp::Response& response)
    {
        ostringstream oss;
        string name;

        oss << "<html><body>" << endl;
        name = request.GetQueryParameter("name");
        if(name!="")
            oss << "Hello " << request.GetQueryParameter("name") << endl;
        else
            oss << "Hello, what's your name ?" << endl;
        oss << "</body></html>" << endl;

        response.SetContent(oss.str());
        return true;
    };
};

int main()
{
    WebServer myServer;
    HelloController myController;

    myServer.AddRoute("/hello", &myController);

    if(!myServer.Start())
    {
        stError myError = myServer.GetLastError();
        cout << "Unable to start WebServer : " << myError.Message << endl;
        return -1;
    }

    cout << "WebServer is started on port 8000" << endl;
    cout << "Try http://127.0.0.1:8000/hello or http://127.0.0.1:8000/hello?name=John" << endl;
    cout << "Press CTRL+C to stop." << endl;

    while(true)
    {
        myServer.Poll();
    }

    myServer.Stop();

    return 0;
}
