#include "stdafx.h"
#include "hashsum.h"


Hashsum::Hashsum()
{
	
}

Hashsum::~Hashsum()
{
	
}

Hashsha1::Hashsha1(unsigned int sz_blk,shared_ptr<baseiofile> _rd,shared_ptr<baseiofile> _wrt):size_block(sz_blk),rd(_rd),wrt(_wrt),count_bytes(0)
{
	cores = boost::thread::hardware_concurrency(); 
}

Hashsha1::~Hashsha1()
{
	
}

void Hashsha1::run()
{
	int aligment_size_for_read = 35 * 100000 / rd->alignment();
	uintmax_t size_block_virt_memory_for_read = aligment_size_for_read * rd->alignment();
	uintmax_t count_blocks = ceil(float(size_block_virt_memory_for_read) / float(size_block));
	uintmax_t size_block_virt_memory_for_write = count_blocks * 5 * sizeof(unsigned int);
	int aligment_size_for_write = ceil(float(size_block_virt_memory_for_write) / float(wrt->alignment()));  
	size_block_virt_memory_for_write = aligment_size_for_write*wrt->alignment();
	uintmax_t size_file_for_read = boost::filesystem::file_size(rd->getNameFile());
	unsigned int count = 0;
	if(size_file_for_read < size_block_virt_memory_for_read)
		count = 1;
	else
		count = ceil(float(size_file_for_read) / float(size_block_virt_memory_for_read));
	for (int i = 0; i != count; ++i)
	{
		if (rd->mapped_file(size_block_virt_memory_for_read) && wrt->mapped_file(size_block_virt_memory_for_write))
		{
			for (unsigned int j = 0; j != cores; ++j)
				group.add_thread(new boost::thread(&Hashsha1::hash_calculate, this, j, size_block_virt_memory_for_write/cores));
			group.join_all();
			rd->close();
			wrt->close();
		}
		else
		{
			cout << "Could not open files" << "\n";
		}
	}
	cout << 100 << "%" << "\r";
}

void Hashsha1::hash_calculate(int index_thread, uintmax_t count_bt)
{
	const char *in = rd->data();
	const char *in_begin = in;
	char *out = wrt->data();
	uintmax_t offset = size_block*index_thread;
	in += offset;
	int sz = 5 * sizeof(unsigned int);
	out += sz*index_thread;
	uintmax_t size_file_rd = rd->sizefile();
	uintmax_t size_file_wrt = wrt->sizefile();
	unsigned int bufout[5];
	while (offset < size_file_rd)
	{
		boost::uuids::detail::sha1 sha1;
		if (offset + size_block > size_file_rd)
			sha1.process_bytes(in, size_file_rd - offset);
		else
			sha1.process_bytes(in, size_block);
		sha1.get_digest(bufout);
		memcpy(out, bufout, sz);
		offset += cores*size_block;
		in = in_begin + offset;
		out += sz*cores;
	}
	mtx.lock();
	count_bytes += count_bt;
	cout << (count_bytes * 100) / size_file_wrt << "%" << "\r";
	mtx.unlock();
}