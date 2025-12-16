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

The following includes sources that I referenced throughout this work:

https://pubs.opengroup.org/onlinepubs/007904975/basedefs/stdint.h.html
https://stackoverflow.com/questions/13590812/c-freeing-structs
https://stackoverflow.com/questions/29673103/how-to-determine-array-length-of-uint8-t
https://stackoverflow.com/questions/42836185/will-a-uint8-t-array-be-terminated-by-null-similar-to-a-char-array
https://stackoverflow.com/questions/50730866/segmentation-fault-with-pointer-array
https://stackoverflow.com/questions/28585777/invalid-write-of-size-8-valgrind
https://stackoverflow.com/questions/73436485/malloc-causes-segmentation-fault-when-allocating-a-struct-for-a-linked-list'https://www.reddit.com/r/C_Programming/comments/1b2mj3a/best_practice_on_allocating_memory_for_a_struct/
https://en.cppreference.com/w/c/string/byte/strncpy
https://stackoverflow.com/questions/6205195/given-a-starting-and-ending-indices-how-can-i-copy-part-of-a-string-in-c
https://www.cs.toronto.edu/~krueger/csc209h/tut/gdb_tutorial.html
https://stackoverflow.com/questions/25360893/convert-char-to-uint8-t
https://stackoverflow.com/questions/15911714/are-int8-t-and-uint8-t-intended-to-be-char-types
C Standard Library - https://www.programiz.com/c-programming/library-function/string.h/strlen
geeks for geeks: https://www.geeksforgeeks.org/c/implementation-of-trie-prefix-tree-in-c/
https://www.reddit.com/r/C_Programming/comments/sx1xpl/since_the_size_of_datatypes_are_not_fixed_in_c/
https://pubs.opengroup.org/onlinepubs/007904975/basedefs/stdint.h.html --> need to look into for uint_t
https://github.com/nothings/stb/blob/master/stb_image.h
https://math.mit.edu/~goemans/18310S15/lempel-ziv-notes.pdf
https://forums.raspberrypi.com/viewtopic.php?t=299281

Resources for Cache Oblivious Research:
Compact Data Structures by Navarro
https://cs.au.dk/~gerth/papers/soda06.pdf
https://erikdemaine.org/papers/CacheObliviousBTrees_SICOMP/paper.pdf
https://www.youtube.com/watch?v=K1a2Bk8NrYQ
https://people.mpi-inf.mpg.de/~umeyer/dfg/swat04.pdf
https://www.youtube.com/watch?v=C6EWVBNCxsc
https://www.youtube.com/watch?v=hmReJCupbNU&t=780s
https://www.arxiv.org/pdf/cs/0211010
https://dl.acm.org/doi/pdf/10.1145/2903141