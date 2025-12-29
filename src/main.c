#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define GB(x) (x << 30)
#define MB(x) (x << 20)
#define KB(x) (x << 10)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef signed int s32;             // 4 bytes
typedef unsigned int u32;           // 4 bytes
typedef float f32;                  // 4 bytes
typedef unsigned char u8;           // 1 bytes
typedef signed char s8;             // 1 bytes
typedef signed short s16;           // 2 bytes
typedef unsigned short u16;         // 2 bytes
typedef unsigned long long u64;     // 8 bytes

typedef struct Arena{       // Declaration of an Arena struct. 
    u8* buffer;             // Contains the total memory pool to be allocated.
    u64 arenaPtr;           // The pointer which keeps track of allocated elements.
    u64 arenaSize;          // The maximum size of the arena.
}Arena;

void arenaAllocate(u64 size, Arena* arena){
    if (size == 0){                                    // Base case, where the size passed is equal to zero.
        return;
    }

    arena->buffer = (u8*)malloc(size);                 // Memory is allocated via malloc.
    if (arena->buffer == NULL){                        // If malloc fails, then memory is attempted to be freed and error message is returned.
        free(arena->buffer);
        printf("\nMemory allocation failed.\n");
        exit(1);
        return;
    }

    arena->arenaPtr = 0;                                // The arena pointer is set equal to zero.
    arena->arenaSize = size;                            // The arena size is set to the size parameter.
}

void arenaFree(Arena* arena){                           // Frees arena after usage.
    free(arena->buffer);
    arena->buffer = NULL;                               // Set to NULL to combat dangling pointer.
}

void arenaClear(Arena* arena){                          // Clears all values within the arena.
    arena->arenaPtr = 0;                                // Arena pointer is set equal to zero. 
    memset(arena->buffer, 0, arena->arenaSize);         // Sets all memory within buffer to zero.
}

void* arenaPush(Arena* arena, u64 allocSize){                                           // Pushes memory onto the arena.
    if(allocSize == 0){                                                                 // Edge case to deal with if allocated memory size is less than zero.
        return NULL;
    }
    
    if (MAX(arena->arenaSize, (arena->arenaPtr + allocSize)) != arena->arenaSize){      // Checks if the the newly allocated memory is not greater than the arena size.
        printf("\n\nUnable to allocate memory for element. Arena is full.\n\n");        // Prints error message informing the user what has occured.
        return NULL;                                                                    // Returns NULL to indicate the operation failed.
    }

    u8* memoryAllocated = arena->buffer + arena->arenaPtr;                              // Pointer which points at the beginning of the allocated memory.
    arena->arenaPtr+=allocSize;                                                         // Increments the arena pointer by the allocated size.    

    return memoryAllocated;                                                             // The memory allocated is now returned.
}

typedef struct String{              // Quick string struct implementation.
    u8* string;        
    u64 size;
}String;

int main(){
    Arena arena;                        // Arena is declared.
    arenaAllocate(KB(1), &arena);       // A kilobyte of memory for the arena is allocated.

    s16* x = (s16*)arenaPush(&arena, sizeof(s16));  // Allocate memory for a 2 byte integer (short).
    *x = 5; 

    String message;                                                                                         // String struct object created.
    const char* stringMes = "This message was created via an arena!\0";
    message.size = sizeof("This message was created via an arena!\0");                                      // String's size is set.
    message.string = (u8*)arenaPush(&arena, sizeof("This message was created via an arena!\0"));            // Memory is pushed. 
    strcpy((char*)message.string, stringMes);                                                               // String is set.

    printf("X's value: %d\n", *x);                                                                      // Integer value is printed.
    printf("Message: %s\n\nAfter clearing: (values should be zeroed...)\n\n", message.string);          // Message is printed. 

    arenaClear(&arena);                                                                                 // The arena's memory is cleared.
    printf("X's value: %d\n", *x);                                                                      // Integer value is printed.
    printf("Message: %s\n\nAfter freeing: (values should be random...)\n\n", message.string);           // Message is printed.

    arenaFree(&arena);                                                                                  // Arena is freed.
    printf("X's value: %d\n", *x);                                                                      // Integer value is printed.
    printf("Message: %s\n", message.string);                                                            // Message is printed.

    arenaAllocate(MB(4), &arena);                           // 4 megabytes are allocated to the arena.
    s32* dynamicArray = (s32*)arenaPush(&arena, MB(1));     // A dynamic array of 4MB 32 bit integers is allocated.
    if (dynamicArray != NULL){
        for (size_t i = 0; i < MB(1); i++){                 // Elements are added.
            dynamicArray[i] = i;
        }
    }
    arenaFree(&arena);                                      // Arena is freed.

    return 0;
}