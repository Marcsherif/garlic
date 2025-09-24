#if !defined(GARLIC_H)

#include "garlic_platform.h"

#define Minimum(A, B) ((A < B) ? (A) : (B))
#define Maximum(A, B) ((A > B) ? (A) : (B))

struct memory_arena
{
    memory_index Size;
    u8 *Base;
    memory_index Used;
};

inline void
InitializeArena(memory_arena *Arena, memory_index Size, void *Base)
{
    Arena->Size = Size;
    Arena->Base = (u8 *)Base;
    Arena->Used = 0;
}

#define PushStruct(Arena, type) (type *)PushSize_(Arena, sizeof(type))
#define PushArray(Arena, Count, type) (type *)PushSize_(Arena, (Count)*sizeof(type))
inline void *
PushSize_(memory_arena *Arena, memory_index Size)
{
    Assert((Arena->Used + Size) <= Arena->Size);
    void *Result = Arena->Base + Arena->Used;
    Arena->Used += Size;
    return(Result);
}

///
///
///

struct loaded_tga
{
    u16 width;
    u16 height;
    u8 bytesPerPixel;
    u16 pitch;
    u8 *data;
};

struct render
{
    memory_arena arena;
    loaded_tga image;
};

#include "garlic_intrinsics.h"
#include "garlic_math.h"
#include "garlic_tga.h"
#include "garlic_model.h"

#define GARLIC_H
#endif
