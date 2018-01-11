#include "UnitTest.h"


using namespace std;

UnitTest::UnitTest()
{
}

UnitTest::~UnitTest()
{
	vector<ITestClass *>::iterator iTestClass = m_TestClassList.begin();
	vector<ITestClass *>::iterator iEndTestClass = m_TestClassList.end();

	while (iTestClass != iEndTestClass)
	{
		delete *iTestClass;
		++iTestClass;
	}
	m_TestClassList.clear();
}

bool UnitTest::run()
{
	vector<ITestClass*>::iterator iTestClass = m_TestClassList.begin();
	vector<ITestClass*>::iterator iEndTestClass = m_TestClassList.end();
	bool ret = true;
	int runCount = 0;
	int errorCount = 0;

	while (iTestClass != iEndTestClass)
	{
		if(!(*iTestClass)->runTests(&runCount, &errorCount)) ret = false;
		++iTestClass;
	}

    std::cout << endl;
    std::cout << termcolor::lightYellow << "TOTAL Runs : " << runCount;
    if(errorCount>0)
        std::cout << termcolor::lightRed << "    Errors : " << errorCount;
    else
        std::cout << termcolor::lightGreen << "    Errors : 0";
    std::cout << endl << endl;

    std::cout << termcolor::white;
    std::cout.copyfmt(std::ios(nullptr));

	return ret;
}

void UnitTest::addTestClass(ITestClass* testClass)
{
	m_TestClassList.push_back(testClass);
}


void UnitTest::displayError(const string& message)
{
    std::cout << termcolor::lightRed << "ERROR : " << message <<endl;
    std::cout << termcolor::white;
    std::cout.copyfmt(std::ios(nullptr));
}
