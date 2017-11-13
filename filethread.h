#pragma once

#include <memory>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/filesystem.hpp>



using namespace std;

class baseiofile
{	
protected:
	char* name_file;
	baseiofile() {}
public:
	baseiofile(char*);
	virtual ~baseiofile();
	char* getNameFile()const;
	virtual uintmax_t sizefile() const= 0;
	virtual bool mapped_file(uintmax_t) = 0;
	virtual void close() = 0;
	virtual char* data() const = 0;
	virtual int alignment() const = 0;
};


class filewrite:public baseiofile
{
private:
	uintmax_t size_file;
	uintmax_t size_block;
	uintmax_t offset;
	boost::iostreams::mapped_file_sink map_file;
	filewrite(){}
public:
	explicit filewrite(char*, int, uintmax_t);
	bool mapped_file(uintmax_t);
	void close();
	char* data() const;
	uintmax_t sizefile() const;
	virtual int alignment() const;
};

class fileread :public baseiofile
{
private:
	uintmax_t size_block;
	uintmax_t offset;
	boost::iostreams::mapped_file_source map_file;
	fileread(){}
public:
	explicit fileread(char*);
	bool mapped_file(uintmax_t);
	void close();
	char* data() const;
	uintmax_t sizefile() const;
	virtual int alignment() const;
};