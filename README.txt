This is a C program that compresses and/or decompresses a text file using Huffman compression. 

To compress (encode) a file, issue the command:


hcompress -e filename

Note: the filename should be in the same repository. You can test it with decind.txt

This should create a new file called "filename.huf" (same name with a .huf extension) which is the compressed version of the file.

To uncompress (decode) a compressed file, issue the command:

hcompress -d filename.huf

This should create a new file called "filename.huf.dec" which is the uncompressed/decoded version of the file.
