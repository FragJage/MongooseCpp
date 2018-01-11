#include <sstream>
#include <algorithm>
#include "TestTypeController.h"
#include "WebServer.h"
#include "HttpClient.h"
#include "HttpHelper.h"

#include "TypeController.h"

using namespace std;


Book::Book(int _ref, string _title, string _author, int _stock)
{
    Ref = _ref;
    Title = _title;
    Author = _author;
    Stock = _stock;
}

JsonController::JsonController()
{
    Book book1(123112, "Les Fleurs du mal", "Charles Baudelaire", 75);
    Book book2(123342, "Les misérables", "Victor Hugo", 68);
    Book book3(123654, "L'étranger", "Albert Camus", 75);
    Book book4(123759, "Les Liaisons dangereuses", "Choderlos de Laclos", 75);
    Book book5(123987, "Le Petit Prince", "Antoine de Saint-Exupéry", 75);

    m_Datas.push_back(book1);
    m_Datas.push_back(book2);
    m_Datas.push_back(book3);
    m_Datas.push_back(book4);
    m_Datas.push_back(book5);
}

JsonController::~JsonController()
{
}

bool JsonController::GetList(MongooseCpp::Request& request, MongooseCpp::Response& response)
{
    vector<Book>::const_iterator it;
    ostringstream oss;


    oss << "{ Books : [" << endl;
    for(it=m_Datas.begin(); it!=m_Datas.end(); ++it)
    {
        if(it!=m_Datas.begin()) oss << "," << endl;
        oss << "{" << endl;
        oss << "Title:\"" << it->Title << "\"," << endl;
        oss << "Author:\"" << it->Author << "\"," << endl;
        oss << "}";
    }
    oss << "]}" << endl;

    response.SetContent(oss.str());
    return true;
}

bool JsonController::Get(const int& Id, Book& book, MongooseCpp::Response& response)
{
    if(Id==999)
    {
        response.SetStatut(403);
        response.SetContent("Id 999 unauthorized");
        return false;
    }
    if(Id>=m_Datas.size())
    {
        response.SetStatut(404);
        response.SetContent("Book not found");
        return false;
    }
    book = m_Datas[Id];
    return true;
}

bool JsonController::Post(const Book& book, MongooseCpp::Response& response)
{
    ostringstream oss;
    vector<Book>::iterator it = find_if(m_Datas.begin(), m_Datas.end(), [&book] (const Book& bookData) { return bookData.Ref == book.Ref; } );

    if(it!=m_Datas.end())
    {
        oss << "Book ref " << book.Ref << " already exist";
        response.SetContent(oss.str());
        return true;
    }

    m_Datas.push_back(book);
    oss << "New Id:" << m_Datas.size()-1;
    response.SetContent(oss.str());
    return true;
}

bool JsonController::Put(const int& Id, const Book& book, MongooseCpp::Response& response)
{
    if(Id>=m_Datas.size())
    {
        response.SetStatut(404);
        response.SetContent("Book not found");
        return false;
    }

    m_Datas[Id] = book;

    return true;
}

bool JsonController::Delete(const int& Id, MongooseCpp::Response& response)
{
    if(Id>=m_Datas.size())
    {
        response.SetStatut(404);
        response.SetContent("Book not found");
        return false;
    }

    m_Datas.erase(m_Datas.begin()+Id);

    return true;
}

string JsonController::ToString(const Book& book)
{
    ostringstream oss;
    oss << "Book {" << endl;
    oss << "Ref:" << book.Ref << "," << endl;
    oss << "Title:\"" << book.Title << "\"," << endl;
    oss << "Author:\"" << book.Author << "\"," << endl;
    oss << "Stock:" << book.Stock << endl;
    oss << "}" << endl;
    return oss.str();
}

int JsonController::ToId(string value)
{
    istringstream iss(value);
    int id;
    iss >> id;
    return id;
}

void JsonController::ToObject(string value, Book& book)
{
    size_t posBegin, posEnd;

    posBegin = value.find("Ref:")+4;
    posEnd = value.find(",", posBegin);
    istringstream issId(value.substr(posBegin, posEnd-posBegin));
    issId >> book.Ref;

    posBegin = value.find("Title:")+7;
    posEnd = value.find(",", posBegin)-1;
    book.Title = value.substr(posBegin, posEnd-posBegin);

    posBegin = value.find("Author:")+8;
    posEnd = value.find(",", posBegin)-1;
    book.Author = value.substr(posBegin, posEnd-posBegin);

    posBegin = value.find("Stock:")+6;
    posEnd = value.find_first_of("\r\n", posBegin);
    istringstream issQte(value.substr(posBegin, posEnd-posBegin));
    issQte >> book.Stock;
}

TestTypeController::TestTypeController() : TestClass("TestTypeController", this), server(8003)
{
    addTest("GetList", &TestTypeController::GetList);
    addTest("GetObject", &TestTypeController::GetObject);
    addTest("CreateObject", &TestTypeController::CreateObject);
    addTest("ModifyObject", &TestTypeController::ModifyObject);
    addTest("DeleteObject", &TestTypeController::DeleteObject);

    server.AddRoute("/api/v1/books/[Id]", &myJsonCtrl);
    server.Start();
}

TestTypeController::~TestTypeController()
{
    server.Stop();
}

bool TestTypeController::GetList()
{
    client.SendRequest("GET", "127.0.0.1", 8003, "/api/v1/books");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("200"==client.GetStatus());
    assert("{ Books : ["==client.GetBody().substr(0,11));
    return true;
}

bool TestTypeController::GetObject()
{
    client.SendRequest("GET", "127.0.0.1", 8003, "/api/v1/books/1");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("200"==client.GetStatus());
    string body = client.GetBody();
    assert("Ref:123342"==body.substr(body.find("Ref:"), 10));

    client.SendRequest("GET", "127.0.0.1", 8003, "/api/v1/books/8");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("404"==client.GetStatus());
    assert("Book not found"==client.GetBody());

    client.SendRequest("GET", "127.0.0.1", 8003, "/api/v1/books/999");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("403"==client.GetStatus());
    assert("Id 999 unauthorized"==client.GetBody());

    return true;
}

bool TestTypeController::CreateObject()
{
    Book myBook;
    Book readedBook;
    string body;

    myBook.Ref = 123654;
    myBook.Title = "Madame Bovary";
    myBook.Author = "Gustave Flaubert";
    myBook.Stock = 12;
    body = myJsonCtrl.ToString(myBook);
    client.SendRequest("POST", "127.0.0.1", 8003, "/api/v1/books", body);
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("Book ref 123654 already exist"==client.GetBody());

    myBook.Ref = 123655;
    body = myJsonCtrl.ToString(myBook);
    client.SendRequest("POST", "127.0.0.1", 8003, "/api/v1/books", body);
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("New Id:5"==client.GetBody());

    client.SendRequest("GET", "127.0.0.1", 8003, "/api/v1/books/5");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("200"==client.GetStatus());
    myJsonCtrl.ToObject(client.GetBody(), readedBook);
    assert(myBook.Ref==readedBook.Ref);
    assert(myBook.Title==readedBook.Title);
    assert(myBook.Author==readedBook.Author);
    assert(myBook.Stock==readedBook.Stock);

    return true;
}

bool TestTypeController::ModifyObject()
{
    Book readedBook;
    string body;

    client.SendRequest("GET", "127.0.0.1", 8003, "/api/v1/books/2");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("200"==client.GetStatus());
    myJsonCtrl.ToObject(client.GetBody(), readedBook);

    readedBook.Stock = 71;
    body = myJsonCtrl.ToString(readedBook);

    client.SendRequest("PUT", "127.0.0.1", 8003, "/api/v1/books", body);
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("500"==client.GetStatus());
    assert("Id parameter is required"==client.GetBody());

    client.SendRequest("PUT", "127.0.0.1", 8003, "/api/v1/books/9", body);
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("404"==client.GetStatus());
    assert("Book not found"==client.GetBody());

    client.SendRequest("PUT", "127.0.0.1", 8003, "/api/v1/books/2", body);
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("200"==client.GetStatus());
    assert("OK"==client.GetBody());

    return true;
}

bool TestTypeController::DeleteObject()
{
    client.SendRequest("DELETE", "127.0.0.1", 8003, "/api/v1/books/7");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("404"==client.GetStatus());
    assert("Book not found"==client.GetBody());

    client.SendRequest("DELETE", "127.0.0.1", 8003, "/api/v1/books/5");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("200"==client.GetStatus());
    assert("OK"==client.GetBody());

    client.SendRequest("GET", "127.0.0.1", 8003, "/api/v1/books/5");
    assert(true==HttpHelper::WaitResponse(server, client));
    assert("404"==client.GetStatus());
    assert("Book not found"==client.GetBody());

    return true;
}
