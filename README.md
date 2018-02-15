[![Build Status](https://travis-ci.org/FragJage/MongooseCpp.svg?branch=master)](https://travis-ci.org/FragJage/MongooseCpp)
[![Build status](https://ci.appveyor.com/api/projects/status/yr84g6ic47jix751?svg=true)](https://ci.appveyor.com/project/FragJage/MongooseCpp)
[![Coverity Scan Build](https://scan.coverity.com/projects/14945/badge.svg)](https://scan.coverity.com/projects/fragjage-mongoosecpp)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/cb3f187d93bb4687a14e9ef2b6250838)](https://www.codacy.com/app/FragJage/MongooseCpp?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=FragJage/MongooseCpp&amp;utm_campaign=Badge_Grade)

MongooseCpp
===========
Mongoose c++ embeded web server cross-platform.

Introduction
============
Embedded web server write in c++ and based on Mongoose. Build on Travis and Appveyor factories. A good solution to add an user interface for your programs.

Features
========
 - Web server with controller and configurable route.
 - Page controller to display statics pages.
 - Api controller to manage GET, POST, PUT and DELETE methods.
 - Type controler to manage a web api with an object.
 - Interface to create any controllers.

How to use
==========
 Add src and dependency/mongoose folders into your project and see examples folder.
 
 To build, add define MG_ENABLE_CALLBACK_USERDATA. You can see CMakeLists.txt, Code::Blocks or MSVC projects in builds folder.
 
 For example :
    
		int main()
		{
			WebServer myServer;
			HelloController helloController;
			PageController pageController("./", true);
			JsonController jsonController;
			MultiController multiController;

			myServer.AddRoute("/hello", &helloController);
			myServer.AddRoute("/examples/*", &pageController);
			myServer.AddRoute("/api/v1/books/[Id]", &jsonController);
			myServer.AddRoute("/multi/{controller}/[Id]", &multiController);

			myServer.Start();
			while(true)
			{
				myServer.Poll();
			}
			myServer.Stop();
			return 0;
		}


Licence
=======
MongooseCpp is free software : you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

MongooseCpp is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with MongooseCpp. If not, see http://www.gnu.org/licenses/.
