#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <string>

#ifdef __MINGW32__
    #define _WIN32_WINNT 0x501  /* for getaddrinfo */
#endif

#ifdef WIN32
    #include <WinSock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h> /* close */
    #include <netdb.h> /* gethostbyname */
    //#define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket(s) close(s)
    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
    typedef struct in_addr IN_ADDR;
#endif

class HttpClient
{
    public:
        HttpClient();
        virtual ~HttpClient();

        bool SendRequest(std::string method, std::string host, int port, std::string path, std::string body="");
        bool WaitingResponse(unsigned int millisecond=100);
        std::string ReceiveResponse();
        std::string GetStatus();
        std::string GetBody();

    private:
        SOCKET m_Sock;
        std::string m_Page;
        void openSocket(std::string host, int port);
};

#endif // HTTPCLIENT_H
