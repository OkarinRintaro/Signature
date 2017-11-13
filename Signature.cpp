/* This is console app. It read file, do hashing and write result to other file. Mapped file are used for writing and reading.
App get three parameters: name of file for read, name of file for write and size of block for read.*/

#include "stdafx.h"
#include "hashsum.h"
#include "filethread.h"
#include <boost/exception/all.hpp>
#include <iostream>

using namespace std;

int main(int arg, char* argv[])
{
	shared_ptr<baseiofile> fr;
	shared_ptr<baseiofile> fw;
	shared_ptr<Hashsum> sha1;
	if (arg == 4)
	{
		try
		{
			fr = make_shared<fileread>(argv[1]);
			fw = make_shared<filewrite>(argv[2], stol(argv[3]), boost::filesystem::file_size(argv[1]));
			sha1 = make_shared<Hashsha1>(stoi(argv[3]), fr, fw);
			sha1->run();
		}
		catch (boost::exception& e)
		{
			cerr << "Caught an boost::failure. \n"
				<< "Exlanatory string: " << boost::diagnostic_information(e) << "\r\n";
		}
		catch (const std::exception& e)
		{
			cerr << "Caught an std::failure. \n"
				<< "Exlanatory string: " << e.what() << "\n";
		}
	}
	else
	{
		if (arg < 3)
			cout << "Not found path to input and output files" << "\n";
		else
			cout << "Not found size of blocks" << "\n";
	}
	cout << "\n";
	cout << "Press Any_Key" << endl;
	_getch();
	return 0;
}

