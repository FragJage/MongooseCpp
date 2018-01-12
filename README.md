# MongooseCpp
Mongoose c++ library
[![Build Status](https://travis-ci.org/FragJage/MongooseCpp.svg?branch=master)](https://travis-ci.org/FragJage/MongooseCpp)
[![Coverage Status](https://coveralls.io/repos/github/FragJage/MongooseCpp/badge.svg?branch=master&bust=0)](https://coveralls.io/github/FragJage/MongooseCpp?branch=master)

MongooseCpp
===========
Mongoose c++ library embeded web server

Introduction
============
Embeded web server write in c++ and based on Mongoose.

Features
========
 - Web server with controller and configurable route.
 - Page controller to display statics pages.
 - Api controller to manage GET, POST, PUT and DELETE methods.
 - Type controler to manage a web api with a object.
 - Interface to create any controllers.

How to use
==========
 Add src and dependency/mongoose folder into your project and see examples folder.
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

Portability
===========
Unit tests passed successfully on :
 - Windows 10 (CPU Intel Core i7)
 - (TO DO) Linux Ubuntu (CPU Intel Celeron)
 - (TO DO) Linux Ubuntu (CPU Intel Atom)
 - (TO DO) Linux Raspian on Raspberry Pi (CPU ARM)
 - (TO DO) Linux FunPlug on NAS DNS-320 (CPU ARM)

For more informations
=====================
(TO DO) See documentation in doc/html/index.html

Licence
=======
MongooseCpp is free software : you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

MongooseCpp is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with MongooseCpp. If not, see http://www.gnu.org/licenses/.