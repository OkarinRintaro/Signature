#include "stdafx.h"
#include "filethread.h"
#include <iostream>


baseiofile::baseiofile(char* argv):name_file(argv)
{

}

baseiofile::~baseiofile()
{
	
}

char* baseiofile::getNameFile() const
{
	return name_file;
}


filewrite::filewrite(char* argv,int sz_block, uintmax_t size_file_for_read):baseiofile(argv), size_block(0),offset(0)
{
	int count = 0;
	int aligment_size_for_read = 35 * 100000 / alignment();
	uintmax_t size_block_virt_memory_for_read = aligment_size_for_read * alignment();
	if (size_file_for_read < size_block_virt_memory_for_read)
		count = 1;
	else
		count = ceil(float(size_file_for_read) / float(size_block_virt_memory_for_read));
	size_file = count;
}

bool filewrite::mapped_file(uintmax_t size_aligment)
{
	if (offset == 0)
		size_file *= size_aligment;
	if (size_file < offset + size_aligment)
		size_block = size_file - offset;
	else
		size_block = size_aligment;
	boost::iostreams::mapped_file_params param;
	param.path = name_file;
	param.new_file_size = size_file;
	param.length = size_block;
	param.offset = offset;
	param.flags = boost::iostreams::mapped_file::mapmode::readwrite;
	map_file.open(param);
	offset += size_aligment;
	if (map_file.is_open())
		return true;
	else
		return false;
}

void filewrite::close()
{
	map_file.close();
}

char* filewrite::data() const
{
	if (map_file.is_open())
		return map_file.data();
	else
		return nullptr;
}


uintmax_t filewrite::sizefile() const
{
	return size_file;
}

int filewrite::alignment() const
{
	return boost::iostreams::mapped_file_sink::alignment();
}

//=======================================================================================


fileread::fileread(char* argv):baseiofile(argv),offset(0)
{
}

bool fileread::mapped_file(uintmax_t size_aligment)
{
	uintmax_t size_file = boost::filesystem::file_size(name_file);
	if (size_file < offset + size_aligment)
		size_block = size_file - offset;
	else
		size_block = size_aligment;
	int sdfsd = map_file.alignment();
	map_file.open(name_file, size_block, offset);
	offset += size_aligment;
	if (map_file.is_open())
		return true;
	else
		return false;
}

void fileread::close()
{
	map_file.close();
}


char* fileread::data() const
{
	if (map_file.is_open())
		return const_cast<char*>(map_file.data());
	else
		return nullptr;
}

uintmax_t fileread::sizefile() const
{
	return size_block;
}

int fileread::alignment() const
{
	return boost::iostreams::mapped_file_source::alignment();
}

