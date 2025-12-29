# Basic C Arena Allocator
I have created this small project after learning about the usage of Arena Allocators to allocate memory via the heap, instead of just utilising malloc to dynamically allocate memory.

An arena is a large memory pool, which acts as a stack, where newly allocated elements are pushed to the stack. All elements are stored congruently, meaning that each section of memory are next to each other.
This can be useful when allocating multiple of the same object, or with dynamic arrays. At the end of it's lifetime, the large memory pool is freed as a whole.
Arenas are used to combat fragmented memory/objects when using Malloc, which slows down access time. Additionally, memory leaks can occur when forgetting to free every single element. 

My version of the arena is not fully optimised, as it does not account for padding/offsets, as memory is best accessed as multiples of 2 (usually 4 or 8).

Inspired by https://github.com/Magicalbat
