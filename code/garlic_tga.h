#if !defined(TGA_H)

#ifndef GARLIC_ROOT
#pragma once
#include "root.unity.h"
#endif

union tga_color
{
    struct
    {
        u8 b, g, r, a;
    };
    u8 e[4];
    u32 val;

};

internal tga_color
Tga(u8 r, u8 g, u8 b, u8 a)
{
    tga_color bgra = {};

    bgra.r = r;
    bgra.g = g;
    bgra.b = b;
    bgra.a = a;

    return bgra;
}

internal tga_color
Tga(u8 *ptr, u8 bpp)
{
    tga_color bgra = {};

    if(bpp!=1)
    {
        bgra.b = *ptr++;
        bgra.r = *ptr++;
        bgra.g = *ptr++;
    }
    if(bpp==4 || bpp==1) bgra.a = *ptr++;

    return bgra;
}

#pragma pack(push, 1)
struct tga_header
{
    u8 idLength;
    u8 colorMapType;
    u8 imageType;
    u16 firstEntryIndex;
    u16 colorMapLength;
    u8 colorMapEntrySize;
    u16 xOrigin;
    u16 yOrigin;
    u16 imageWidth;
    u16 imageHeight;
    u8 bitsPerPixel;
    u8 imageDescriptor;
};
#pragma pack(pop)

enum format
{
    GRAYSCALE = 1,
    RGB = 3,
    RGBA = 4,
};

struct tga_file
{
    tga_header header;
    u8 *data;
};

#define TGA_H
#endif
