add_library(image_compressor
  # cache_naive_helpers.c
  # cache_naive_helpers.h      
  # controller.c
  # cache_naive_lz.c           
  # main.c
  # cache_oblivious_helpers.c  
  stb_image.h
  # cache_oblivious_helpers.h  
  # cache_oblivious_lz.c
  trie.c
)



add_executable(trie trie.c)
target_link_libraries(trie image_compressor)
target_link_libraries(image_compressor PRIVATE m)