add_library(cache_naive_helpers cache_naive_helpers.c cache_naive_helpers.h)
add_library(trie trie.c trie.h)
add_executable(main main.c)
add_library(stb_image INTERFACE stb_image.h)

# add_executable(main main.c)
target_link_libraries(main PUBLIC cache_naive_helpers PRIVATE stb_image PRIVATE trie PRIVATE m)

# https://stackoverflow.com/questions/10005982/how-do-you-set-gdb-debug-flag-with-cmake
# https://stackoverflow.com/questions/60604249/how-to-make-a-header-only-library-with-cmake