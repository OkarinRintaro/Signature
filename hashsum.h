#pragma once
#include <vector>
#include <cstdlib>
#include "filethread.h"
#include <boost/thread.hpp>
#include <boost\uuid\sha1.hpp>
#include <chrono>

using namespace std;

class Hashsum
{
public:
	Hashsum();
	virtual ~Hashsum();
	virtual void run() = 0;
	virtual void hash_calculate(int,uintmax_t) = 0;
};

class Hashsha1 : public Hashsum
{
	unsigned int size_block;
	boost::mutex mtx;
	boost::thread_group group;
	shared_ptr<baseiofile> rd;
	shared_ptr<baseiofile> wrt;
	uintmax_t count_bytes;
	unsigned int cores;
	Hashsha1(){}
public:
	~Hashsha1();
	void run();
	explicit Hashsha1(unsigned int sz_blk,shared_ptr<baseiofile>,shared_ptr<baseiofile>);
	void hash_calculate(int,uintmax_t);
};