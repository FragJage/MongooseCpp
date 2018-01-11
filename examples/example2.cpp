/***********************************************************************************************/
/**                                                                                           **/
/** EXAMPLE 2                                                                                 **/
/**                                                                                           **/
/** Web server with page controller                                                           **/
/**                                                                                           **/
/***********************************************************************************************/

#include <iostream>
#include <sstream>
#include "WebServer.h"
#include "PageController.h"

using namespace std;
using namespace MongooseCpp;


int main()
{
    WebServer myServer;
    PageController myController("./", true);

    myServer.AddRoute("/examples/*", &myController);

    if(!myServer.Start())
    {
        stError myError = myServer.GetLastError();
        cout << "Unable to start WebServer : " << myError.Message << endl;
        return -1;
    }

    cout << "WebServer is started on port 8000" << endl;
    cout << "Try http://127.0.0.1:8000/examples/web" << endl;
    cout << "Press CTRL+C to stop." << endl;

    while(true)
    {
        myServer.Poll();
    }

    myServer.Stop();

    return 0;
}
