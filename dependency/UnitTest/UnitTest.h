#ifndef _FRAGUNITTEST_H
#define _FRAGUNITTEST_H

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include"UnitTest/termcolor.h"

class ITestClass
{
	public:
	    virtual ~ITestClass() {};
		virtual bool runTests(int* prunCount, int* perrorCount) = 0;
		virtual std::string getClassName() = 0;
};

template<typename T>
class TestClass : public ITestClass
{
	public:
		typedef bool (T::*TestMethod)();
		TestClass(std::string className, T* testClass)
		{
			m_ClassName = className;
			m_TestClass = testClass;
		};

		~TestClass()
		{
		};

		void addTest(std::string title, TestMethod testMethod)
		{
			m_NameList.push_back(title);
			m_MethodList.push_back(testMethod);
		};

		bool runTests(int* prunCount, int* perrorCount)
		{
			TestMethod testMethod;
			int i, len , padLenght;
			int imax = m_NameList.size();
			int runCount = 0;
			int errorCount = 0;
			bool result = true;
			std::ios::fmtflags oldFmt(std::cout.flags());


            padLenght = m_ClassName.length();
			for(i=0; i<imax; i++)
			{
                len = m_NameList[i].length();
                if (len > padLenght) padLenght = len;
            }
            padLenght += 5;

            std::cout << termcolor::lightYellow << "- " << m_ClassName << " " << std::setfill('-') << std::setw(padLenght+10 - m_ClassName.length()) << "-" << std::endl;
			for(i=0; i<imax; i++)
			{
			    runCount++;
                std::cout << "    " << termcolor::lightWhite << m_NameList[i] << std::setfill(' ') << std::setw(padLenght - m_NameList[i].length()) << " ";
				testMethod = m_MethodList[i];
                try
                {
                    if ((m_TestClass->*testMethod)())
                        std::cout << termcolor::lightGreen << "OK";
                    else
                    {
                      result = false;
                      errorCount++;
                      std::cout << termcolor::lightRed << "KO";
                    }
                }
                catch(const std::exception & e)
                {
                    result = false;
                    errorCount++;
                    std::cout << termcolor::lightRed << "FAILED" << std::endl;
                    std::cout << termcolor::white << "EXCEPTION" << std::endl << e.what();
                }
				std::cout << std::endl;
			}

            std::cout << std::endl;
			std::cout << termcolor::lightYellow << "    Runs : " << runCount;
			if(errorCount>0)
                std::cout << termcolor::lightRed << "    Errors : " << errorCount;
            else
                std::cout << termcolor::lightGreen << "    Errors : 0";
			std::cout << std::endl << std::endl;

            std::cout.flags(oldFmt);

            *prunCount += runCount;
            *perrorCount += errorCount;

            return result;
		};

		std::string getClassName()
		{
			return m_ClassName;
		}

	private:
		std::string m_ClassName;
		std::vector<std::string> m_NameList;
		std::vector<TestMethod> m_MethodList;
		T* m_TestClass;
};

class UnitTest
{
	public:
		UnitTest();
		~UnitTest();
		bool run();
		void addTestClass(ITestClass* testClass);
		void displayError(const std::string& message);

	private:
		std::vector<ITestClass *> m_TestClassList;
};

#endif
