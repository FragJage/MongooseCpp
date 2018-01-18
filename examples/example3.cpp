/***********************************************************************************************/
/**                                                                                           **/
/** EXAMPLE 3                                                                                 **/
/**                                                                                           **/
/** Api server with json controller                                                           **/
/** Be careful, the json decoding is very light for the example,                              **/
/**  it's strongly recommended to use a json library to decode json string.                   **/
/***********************************************************************************************/

#include <iostream>
#include <sstream>
#include <algorithm>
#include "WebServer.h"
#include "TypeController.h"

using namespace std;
using namespace MongooseCpp;

class Book
{
    public:
        Book() {};
        Book(int _ref, std::string _title, std::string _author, int _stock) : Ref(_ref), Title(_title), Author(_author), Stock(_stock) {};
        int Ref;
        std::string Title;
        std::string Author;
        int Stock;
};

class JsonController : public MongooseCpp::TypeController<unsigned int, Book>
{
    public:
        JsonController()
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
        };

        ~JsonController()
        {
        };

        bool GetList(MongooseCpp::Request& request, MongooseCpp::Response& response)
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
        };

        bool Get(const unsigned int& Id, Book& book, MongooseCpp::Response& response)
        {
            if(Id>=m_Datas.size())
            {
                response.SetStatut(404);
                response.SetContent("Book not found");
                return false;
            }
            book = m_Datas[Id];
            return true;
        };

        bool Post(const Book& book, MongooseCpp::Response& response)
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
        };

        bool Put(const unsigned int& Id, const Book& book, MongooseCpp::Response& response)
        {
            if(Id>=m_Datas.size())
            {
                response.SetStatut(404);
                response.SetContent("Book not found");
                return false;
            }

            m_Datas[Id] = book;

            return true;
        };

        bool Delete(const unsigned int& Id, MongooseCpp::Response& response)
        {
            if(Id>=m_Datas.size())
            {
                response.SetStatut(404);
                response.SetContent("Book not found");
                return false;
            }

            m_Datas.erase(m_Datas.begin()+Id);

            return true;
        };

        std::string ToString(const Book& book)
        {
            ostringstream oss;
            oss << "Book {" << endl;
            oss << "Ref:" << book.Ref << "," << endl;
            oss << "Title:\"" << book.Title << "\"," << endl;
            oss << "Author:\"" << book.Author << "\"," << endl;
            oss << "Stock:" << book.Stock << endl;
            oss << "}" << endl;
            return oss.str();
        };

		unsigned int ToId(std::string value)
        {
            istringstream iss(value);
			unsigned int id;
            iss >> id;
            return id;
        };

        void ToObject(std::string value, Book& book)
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
        };

    private:
        std::vector<Book> m_Datas;
};

int main()
{
    WebServer myServer;
    JsonController myController;

    myServer.AddRoute("/api/v1/books/[Id]", &myController);

    if(!myServer.Start())
    {
        stError myError = myServer.GetLastError();
        cout << "Unable to start WebServer : " << myError.Message << endl;
        return -1;
    }

    cout << "WebApiServer is started on port 8000" << endl;
    cout << "Try http://127.0.0.1:8000/api/v1/books or http://127.0.0.1:8000/api/v1/books/1" << endl;
    cout << "Press CTRL+C to stop." << endl;

    while(true)
    {
        myServer.Poll();
    }

    myServer.Stop();

    return 0;
}
