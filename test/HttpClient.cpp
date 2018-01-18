#include <iostream>
#include <sstream>
#include <cstring>
#include "HttpClient.h"

#define BUFLEN 256

using namespace std;

HttpClient::HttpClient()
{
    #if defined(WIN32)
        WSADATA WSAData;
        if(WSAStartup(MAKEWORD(2,2), &WSAData))
        {
            cout <<"Error, WSAStartup failed." << endl;
            exit(-1);
        }
    #endif
}

HttpClient::~HttpClient()
{
    #if defined(WIN32)
        WSACleanup();
    #endif
}

void HttpClient::openSocket(string host, int port)
{
    SOCKADDR_IN sin;
	int status;
	struct addrinfo hints, *res;

	memset(&hints, 0, sizeof(hints));
	if ((status = getaddrinfo(host.c_str(), nullptr, &hints, &res)) != 0)
	{
		cout << "Error " << status << " : " << gai_strerror(status) << endl;
		exit(-1);
	}

    m_Sock = socket(AF_INET,SOCK_STREAM,0);
	auto sockaddr_ipv4 = reinterpret_cast<sockaddr_in*>(res->ai_addr);
	sin.sin_addr = sockaddr_ipv4->sin_addr;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
	freeaddrinfo(res);

    if(connect(m_Sock, (SOCKADDR *)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        cout << "Error, socket connection failed" << endl;
        exit(-1);
    }
}

bool HttpClient::SendRequest(string method, string host, int port, string path, string body)
{
    openSocket(host, port);

    ostringstream request;
    request << method << " " << path << " HTTP/1.1\r\n";
    request << "Host: " << host << ":" << port << "\r\n";

    if(body=="")
    {
        request << "\r\n";
    }
    else
    {
        request << "Content-Type: application/x-www-form-urlencoded\r\n";
        request << "Content-Length: " << body.size() << "\r\n";
        request << "\r\n";
        request << body;
    }

    string stringBuffer = request.str();
    char* charBuffer = new char [stringBuffer.size()+1];
    unsigned int sendMax = stringBuffer.size();
    unsigned int sendSum = 0;
    int sendByte = -1;

	#ifdef __MINGW32__
		strcpy(charBuffer, stringBuffer.c_str());
	#else
		strcpy_s(charBuffer, stringBuffer.size()+1, stringBuffer.c_str());
	#endif

    while(sendSum < sendMax)
    {
        sendByte = send(m_Sock, charBuffer+sendSum, sendMax-sendSum, 0);
        sendSum+=sendByte;
        if(sendByte<1) break;
    }

    delete[] charBuffer;
    return (sendByte>0);
}

bool HttpClient::WaitingResponse(unsigned int milliTimeout)
{
     struct timeval timeout;
     timeout.tv_sec = 0;
     timeout.tv_usec = milliTimeout;

     fd_set readfs;
     FD_ZERO(&readfs);
     FD_SET(m_Sock,&readfs);
     int nb = select(m_Sock+1,&readfs,NULL,NULL,&timeout);

     return (nb>0);
}

string HttpClient::ReceiveResponse()
{
    char recvBuffer[BUFLEN];
    int recvByte;
    string stringBuffer = "";

    while(WaitingResponse(100))
    {
        recvByte = recv(m_Sock, recvBuffer, BUFLEN-1, 0);
        if(recvByte>0)
        {
            recvBuffer[recvByte] = '\0';
            stringBuffer.append(recvBuffer);
        }
        if(recvByte==0) break;
     }

    closesocket(m_Sock);
    m_Page = stringBuffer;
    return stringBuffer;
}

string HttpClient::GetStatus()
{
    if(m_Page.substr(0, 4)!="HTTP") return "";

    return m_Page.substr(m_Page.find(" ")+1, 3);
}

string HttpClient::GetBody()
{
    size_t bodyPos = m_Page.find("\r\n\r\n");
    if(bodyPos == string::npos) return "";

    size_t lenPos = m_Page.substr(0, bodyPos).find("Content-Length:");
    if(lenPos == string::npos) return "";

    int len;
    istringstream iss(m_Page.substr(lenPos+15, m_Page.find("\r\n", lenPos, bodyPos)));
    iss >> len;

    return m_Page.substr(m_Page.find("\r\n\r\n")+4, len);
}
