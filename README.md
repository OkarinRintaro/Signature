# Signature
Library: Boost 1.64

This is console app. It get three parameters: name of file for read, name of file for write and size of block for read.

Work algorithm:  
1. Getting size of sheet of virtual memory.  
2. Mapping all or part of file.  
3. Hashing each piece equaling size of block(sha1).  
4. Writing result in output file.  
5. Offset in other part of file for read.

Algorithm execute in multiple threads.
