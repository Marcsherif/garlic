#ifndef GARLIC_ROOT
#pragma once
#include "root.unity.h"
#endif

internal loaded_tga
InitializeTga(u16 width, u16 height, u8 bytesPerPixel)
{
    loaded_tga result = {};

    if(result.data)
    {
        Assert(!"Already Initialized");
    }

    result.width = width;
    result.height = height;
    result.bytesPerPixel = bytesPerPixel;
    result.pitch = width*bytesPerPixel;

    int tgaSize = width*height*bytesPerPixel+result.pitch;
    result.data = (u8 *)VirtualAlloc(0, tgaSize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

    return result;
}

internal tga_color
Get(tga_file *tga, i32 x, i32 y)
{
    if (!tga->data || x < 0 || y < 0 || x >= tga->header.imageWidth || y >= tga->header.imageHeight) return {};
    return Tga(tga->data + u8(x*(tga->header.bitsPerPixel/8) + y * tga->header.imageWidth) , (tga->header.bitsPerPixel/8));
}

internal b32
Set(loaded_tga *tga, i32 x, i32 y, tga_color c)
{
    b32 result = true;

    if(!tga->data || x < 0 || y < 0 || x >= tga->width || y >= tga->height)
    {
        result = false;
    }

    u32 offset = (y * tga->width + x)*tga->bytesPerPixel;
    u8 *Dest = tga->data + offset;
    if(tga->bytesPerPixel != 1)
    {
        *Dest++ = c.b;
        *Dest++ = c.g;
        *Dest++ = c.r;
    }

    if(tga->bytesPerPixel == 4 || tga->bytesPerPixel == 1)
    {
        *Dest++ = c.a;
    }

    return result;
}

internal void
Fill(loaded_tga *tga, tga_color c)
{
    u8 *Dest = tga->data;
    for(i32 row = 0;
        row < tga->height;
        ++row)
    {
        for(i32 col = 0;
            col < tga->width;
            ++col)
        {
            if(tga->bytesPerPixel != 1)
            {
                *Dest++ = c.b;
                *Dest++ = c.g;
                *Dest++ = c.r;
            }

            if(tga->bytesPerPixel == 4 || tga->bytesPerPixel == 1)
            {
                *Dest++ = c.a;
            }
        }
    }
}

internal b32
FlipVertically(loaded_tga *tga)
{
    b32 result;
    if(tga->data)
    {
        i32 half = tga->height / 2;

        u8 *startRow = tga->data;
        u8 *endRow = tga->data + tga->width * tga->height * tga->bytesPerPixel - tga->pitch;
        u8 temp;
        for(i32 y = 0;
            y < half;
            ++y)
        {
            u8 *start = startRow;
            u8 *end = endRow;
            for(i32 x = 0;
                x < tga->width;
                ++x)
            {
                for(i32 byte = 0;
                    byte < tga->bytesPerPixel;
                    ++byte, ++start, ++end)
                {
                    temp = *start;
                    *start = *end;
                    *end = temp;
                }
            }

            startRow += tga->pitch;
            endRow -= tga->pitch;
        }

        result = true;
    }
    else
    {
        result = false;
    }

    return result;
}

internal b32
WriteTgaFile(loaded_tga *tga, char *filename)
{
    b32 result = true;

    if(!filename)
    {
        result = false;
    }

    tga_header header = {};

    header.bitsPerPixel = (u8)(tga->bytesPerPixel * 8);
    header.imageWidth = tga->width;
    header.imageHeight = tga->height;
    header.imageType = (tga->bytesPerPixel==GRAYSCALE?(3):(2));
    header.imageDescriptor = 0x20;

    u32 memorySize = header.imageWidth * header.imageHeight * tga->bytesPerPixel;
    HANDLE fileHandle = CreateFileA(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(fileHandle != INVALID_HANDLE_VALUE)
    {
        DWORD BytesWritten;
        if(WriteFile(fileHandle, (void *)&header, sizeof(header), &BytesWritten, 0))
        {
            b32 test = true;
        }
        if(WriteFile(fileHandle, tga->data, memorySize, &BytesWritten, 0))
        {
            b32 test2 = true;
        }
    }
    CloseHandle(fileHandle);

    return result;
}

internal tga_file
ReadTgaFile(char *Filename)
{
    debug_read_file_result textureMap = DEBUG_PLATFORM_READ_ENTIRE_FILE("obj/african_head_diffuse.tga");

    tga_file tga = {};

    tga_header *header = {};
    header = (tga_header *)textureMap.Contents;

    tga.header = *header;
    tga.data = ((u8 *)textureMap.Contents + sizeof(tga_header));

    return tga;
}
