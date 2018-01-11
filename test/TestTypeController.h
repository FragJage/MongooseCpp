#include <cassert>
#include <string>
#include <vector>
#include "TypeController.h"
#include "WebServer.h"
#include "UnitTest/UnitTest.h"
#include "HttpClient.h"


class Book
{
    public:
        Book() {};
        Book(int _ref, std::string _title, std::string _author, int _stock);
        int Ref;
        std::string Title;
        std::string Author;
        int Stock;
};

class JsonController : public MongooseCpp::TypeController<int, Book>
{
    public:
        JsonController();
        ~JsonController();

        bool GetList(MongooseCpp::Request& request, MongooseCpp::Response& response);
        bool Get(const int& Id, Book& book, MongooseCpp::Response& response);
        bool Post(const Book& book, MongooseCpp::Response& response);
        bool Put(const int& Id, const Book& book, MongooseCpp::Response& response);
        bool Delete(const int& Id, MongooseCpp::Response& response);

        std::string ToString(const Book& book);
        void ToObject(std::string value, Book& book);
    protected:
        int ToId(std::string value);
    private:
        std::vector<Book> m_Datas;
};

class TestTypeController : public TestClass<TestTypeController>
{
    public:
        TestTypeController();
        ~TestTypeController();

        bool GetList();
        bool GetObject();
        bool CreateObject();
        bool ModifyObject();
        bool DeleteObject();

    private:
        MongooseCpp::WebServer server;
        HttpClient client;
        JsonController myJsonCtrl;
};
