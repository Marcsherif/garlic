#if !defined(GARLIC_MODEL_H)

#ifndef GARLIC_ROOT
#pragma once
#include "root.unity.h"
#endif

struct debug_read_file_result
{
    u32 ContentsSize;
    void *Contents;
};

struct faces
{
    v3 vertex;
    v3 texture;
    v3 normal;
};

struct obj_model
{
    char *filename;
    i32 nverts;
    i32 nfaces;
    v3 *verts;
    i32 ntextures;
    v2 *textures;
    faces *faces;
};

#define GARLIC_MODEL_H
#endif
