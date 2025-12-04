# Cache-Oblivious-Image-Compressor
Cache-oblivious image compressor in C using LZ78. We aim to optimize the standard image compression tool for cache hits. 

LZ78 compression builds a dictionary of repeated patterns while scanning the input.


sample output 

$ ./image_compressor naive ../rohan.jpg output
Loaded '../rohan.jpg' (4747200 bytes of pixel data)
Naive compressed output written to 'output.naive.lz78'
=== Naive LZ78 ===
Original size     : 4747200 bytes
Compressed size   : 1148361 bytes
Compression ratio : 4.134x
Time              : 73.918988 s
---------------------------

