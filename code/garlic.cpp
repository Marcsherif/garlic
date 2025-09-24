#ifndef GARLIC_ROOT
#pragma once
#include "root.unity.h"
#endif

#include <windows.h>
#include "garlic.h"
#include "garlic_model.cpp"
#include "garlic_tga.cpp"
#include "stdio.h"

global_variable tga_color white = Tga(255, 255, 255, 255);
global_variable tga_color red = Tga(255, 0, 0, 255);
global_variable tga_color green = Tga(0, 255, 0, 255);
global_variable tga_color blue = Tga(0, 0, 255, 255);

global_variable v3 lightDir = V3(0,0,-1);
global_variable v3 camera = V3(0, 0, 5);
global_variable i32 width  = 1080;
global_variable i32 height = 1080;
global_variable i32 depth = 255;

#define FLOAT_MAX 3.402823466e+38F

m4x4 viewport(i32 x, i32 y, i32 w, i32 h)
{
    m4x4 m = Identity();
    m.E[0][3] = x+w/2.0f;
    m.E[1][3] = y+h/2.0f;
    m.E[2][3] = depth/2.0f;

    m.E[0][0] = w/2.0f;
    m.E[1][1] = h/2.0f;
    m.E[2][2] = depth/2.0f;
    return m;
}

void
Line(r32 x0, r32 y0, r32 x1, r32 y1, loaded_tga &image, tga_color color)
{
    b32 steep = false;

    if(AbsolouteValue(x0-x1) < AbsolouteValue(y0-y1))
    { // if the line is steep, we transpose the image
        Swap(x0, y0, r32);
        Swap(x1, y1, r32);
        steep = true;
    }

    if(x0 > x1)
    { // make it left−to−right
        Swap(x0, x1, r32);
        Swap(y0, y1, r32);
    }

    i32 dx = (i32)(x1-x0);
    i32 dy = (i32)(y1-y0);
    i32 derror2 = abs(dy)*2;
    i32 error2 = 0;
    i32 y = (i32)y0;
    i32 yincr = (y1 > y0 ? 1 : -1);
    if(steep)
    {
        for(i32 x = (i32)x0;
            x <= x1;
            ++x)
        {
            Set(&image, y, x, color);
            error2 += derror2;
            if(error2 > dx)
            {
                y += (y1 > y0 ? 1 : -1);
                error2 -= dx*2;
            }
        }
    }
    else
    {
        for(i32 x = (i32)x0;
            x <= x1;
            ++x)
        {
            Set(&image, x, y, color);
            error2 += derror2;
            if(error2 > dx)
            {
                y += yincr;
                error2 -= dx*2;
            }
        }
    }
}

void
Triangle(v2 t0, v2 t1, v2 t2, loaded_tga &image, tga_color color)
{
    Line(t0.X, t0.Y, t1.X, t1.Y, image, color);
    Line(t1.X, t1.Y, t2.X, t2.Y, image, color);
    Line(t2.X, t2.Y, t0.X, t0.Y, image, color);
}

void
SweepFilledTriangle(v2 t0, v2 t1, v2 t2, loaded_tga &image, tga_color color)
{ // Old School Sweeping method
    if(t0.Y == t1.Y && t0.Y == t2.Y) return;
    if(Maximum(t0.Y, t1.Y) == t0.Y) Swap(t0, t1, v2);
    if(Maximum(t1.Y, t2.Y) == t1.Y) Swap(t1, t2, v2);
    if(Maximum(t0.Y, t1.Y) == t0.Y) Swap(t0, t1, v2);

    i32 total_height = (i32)(t2.Y-t0.Y);
    for(i32 i = 0;
        i < total_height;
        i++)
    {
        b32 second_half = i > t1.Y - t0.Y || t1.Y == t0.Y;
        i32 segment_height = second_half ? (i32)(t2.Y-t1.Y) : (i32)(t1.Y-t0.Y);

        r32 alpha = (r32)i / total_height;
        r32 beta  = (r32)(i-(second_half ? t1.Y-t0.Y : 0))/segment_height;

        v2 A = t0 + (t2-t0)*alpha;
        v2 B = second_half ? t1 + (t2-t1)*beta : t0 + (t1-t0)*beta;

        if (A.X > B.X) Swap(A, B, v2);

        for (i32 j = (i32)A.X;
             j <= (i32)B.X;
             j++)
        {
            Set(&image, j, (i32)(t0.Y+i), color); // attention, due to int casts t0.Y+i != A.Y
        }
    }

}

v3
barycentric(v3 A, v3 B, v3 C, v3 P)
{
    v3 s[2];
    for(i32 i = 0; i < 2; ++i)
    {
        s[i].X = C.E[i]-A.E[i];
        s[i].Y = B.E[i]-A.E[i];
        s[i].Z = A.E[i]-P.E[i];
    }

    v3 u = s[0] ^ s[1];
    if(AbsolouteValue(u.Z) > 1e-2)
    {
        return V3(1.0f-(u.X+u.Y)/u.Z, u.Y/u.Z, u.X/u.Z);
    }
    return V3(-1,1,1);
}

void
FilledTriangle(v3 pts[3], r32 *zbuffer, loaded_tga &image, tga_color color)
{ // Barycentric coodrinates based fill.
    rectangle2 bbox = rectangle2{V2(FLOAT_MAX, FLOAT_MAX),
                                 V2(-FLOAT_MAX, -FLOAT_MAX)};

    v2 clamp = V2((r32)image.width-1, (r32)image.height-1);

    for(i32 i = 0;
        i < 3;
        ++i)
    {
        for(i32 j = 0;
            j < 2;
            ++j)
        {
            bbox.Min.E[j] = Maximum(0.0f, Minimum(bbox.Min.E[j], pts[i].E[j]));
            bbox.Max.E[j] = Minimum(clamp.E[j], Maximum(bbox.Max.E[j], pts[i].E[j]));
        }
    }

    v3 P;
    for(P.Y = bbox.Min.Y;
        P.Y <= bbox.Max.Y;
        ++P.Y)
    {
        for(P.X = bbox.Min.X;
            P.X <= bbox.Max.X;
            ++P.X)
        {
            v3 bc_screen = barycentric(pts[0], pts[1], pts[2], P);
            if(bc_screen.X < 0 || bc_screen.Y < 0 || bc_screen.Z < 0) continue;
            P.Z = 0;
            for(i32 i = 0;
                i < 3;
                ++i)
            {
                P.Z += pts[i].Z * bc_screen.E[i];
            }
            if(zbuffer[i32(P.X+P.Y*width)] < P.Z)
            {
                zbuffer[i32(P.X+P.Y*width)] = P.Z;
                Set(&image, (i32)P.X, (i32)P.Y, color);
            }
        }
    }
}

v3
WorldToScreen(v3 v)
{
    return V3((r32)RoundReal32ToInt32((v.X + 1.0f) * width / 2.0f + 0.5f),
              (r32)RoundReal32ToInt32((v.Y + 1.0f) * width / 2.0f + 0.5f),
                  v.Z);
}

void
DrawWireMesh(obj_model model, loaded_tga image)
{
    for(i32 i = 0;
        i < model.nfaces;
        ++i)
    {
        v3 face = model.faces[i].vertex;
        for(i32 j = 0;
            j < 3;
            ++j)
        {
            v3 v0 = GetVertex(&model, (i32)face.E[j]);
            v3 v1 = GetVertex(&model, (i32)face.E[(j+1)%3]);

            v3 start = WorldToScreen(v0);
            v3 end = WorldToScreen(v1);

            Line(start.X, start.Y, end.X, end.Y, image, white);
        }
    }
}

void
DrawShaders(obj_model model, loaded_tga image)
{
    r32 *zbuffer = (r32 *)VirtualAlloc(0, width*height*4, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

    r32 *zptr = zbuffer;
    *zptr = -FLOAT_MAX;
    int inc = 0;
    int size = width*height*4;
    while(inc < size)
    {
        *zptr = -FLOAT_MAX;
        zptr++; inc+=4;
    }

    m4x4 proj = Identity();
    m4x4 viewPort = viewport(width/8, height/8, width*3/4, height*3/4);
    proj.E[3][2] = -1.0f/camera.Z;

    for(i32 i = 0;
        i < model.nfaces;
        ++i)
    {
        v3 face = model.faces[i].vertex;
        v3 screenCoords[3];
        v3 worldCoords[3];
        for(i32 j = 0;
            j < 3;
            ++j)
        {
            v3 v = GetVertex(&model, (i32)face.E[j]);

            //screenCoords[j] = DeHomogonize(proj*Homogonize(v));

            v4 newV4 = Homogonize(v);
            newV4.W = 1 - (newV4.Z / camera.Z);
            v3 newV = WorldToScreen(DeHomogonize(newV4));
            screenCoords[j] = newV;

            worldCoords[j] = v;
        }
        v3 normal = (worldCoords[2] - worldCoords[0]) ^ (worldCoords[1] - worldCoords[0]);
        normal = Normalize(normal);
        r32 intensity = Inner(normal, lightDir);
        if(intensity > 0)
        {
            FilledTriangle(screenCoords, zbuffer, image, Tga(u8(intensity * 255),
                                                              u8(intensity * 255),
                                                              u8(intensity * 255),
                                                              255));
        }
    }
}

v2
TextureToScreen(v2 v, tga_file textureFile)
{
    return V2((r32)RoundReal32ToInt32(v.X * textureFile.header.imageWidth),
              (r32)RoundReal32ToInt32(v.Y * textureFile.header.imageHeight));
}

void
FilledTriangle(v3 vertCoords[3], v2 texCoords[3], tga_file textureFile, r32 *zbuffer, loaded_tga &image, tga_color color)
{
    rectangle2 bbox = rectangle2{V2(FLOAT_MAX, FLOAT_MAX),
                                 V2(-FLOAT_MAX, -FLOAT_MAX)};

    v2 clamp = V2((r32)image.width-1, (r32)image.height-1);

    for(i32 i = 0;
        i < 3;
        ++i)
    {
        for(i32 j = 0;
            j < 2;
            ++j)
        {
            bbox.Min.E[j] = Maximum(0.0f, Minimum(bbox.Min.E[j], vertCoords[i].E[j]));
            bbox.Max.E[j] = Minimum(clamp.E[j], Maximum(bbox.Max.E[j], vertCoords[i].E[j]));
        }
    }

#if 0
    v3 P;
    for(P.Y = bbox.Min.Y;
        P.Y <= bbox.Max.Y;
        ++P.Y)
    {
        for(P.X = bbox.Min.X;
            P.X <= bbox.Max.X;
            ++P.X)
        {
            v3 bary = barycentric(vertCoords[0], vertCoords[1], vertCoords[2], P);

            if(bary.X < 0 || bary.Y < 0 || bary.Z < 0) continue;
            P.Z = 0;
            for(i32 i = 0;
                i < 3;
                ++i)
            {
                P.Z += vertCoords[i].Z * bary.E[i];
            }
            if(zbuffer[i32(P.X+P.Y*width)] < P.Z)
            {
#if 0
                char TextBuffer[256] = {};
                _snprintf_s(TextBuffer, sizeof(TextBuffer),
                        "texcoord0: %f %f\t texcoord1: %f %f\t texcoord2: %f %f\t\n",
                        texCoords[0].X, texCoords[0].Y, texCoords[1].X, texCoords[1].Y, texCoords[2].X, texCoords[2].Y);
                OutputDebugStringA(TextBuffer);
#endif

                zbuffer[i32(P.X+P.Y*width)] = P.Z;
                v2 coords = bary.X * texCoords[0] + bary.Y * texCoords[1] + bary.Z * texCoords[2];

                //coords = TextureToScreen(coords, textureFile);

                //int tex_x = (int)(Maximum(coords.X * textureFile.header.imageWidth, 0.0f));
                //int tex_y = (int)(Maximum(textureFile.header.imageHeight - (coords.Y * textureFile.header.imageHeight), 0.0f));
                //u8 *ptr = textureFile.data + u8(tex_y * textureFile.header.imageWidth + tex_x)*3;
                //u8 *ptr = textureFile.data + (u8)((coords.Y*textureFile.header.imageWidth + coords.X*3));

                //tga_color bcol = Tga(*(ptr+2), *(ptr+1), *(ptr),*(ptr+3));
                //tga_color bcol = Get(&textureFile, (i32)coords.X, (i32)coords.Y);
                //color.r *= bcol.r;
                //color.b *= bcol.b;
                //color.g *= bcol.g;

                //Set(&image, (i32)P.X, (i32)P.Y, color);
            }
        }
    }
#endif
}

void
DrawTextures(obj_model model, tga_file textureFile, loaded_tga image)
{
    r32 *zbuffer = (r32 *)VirtualAlloc(0, width*height*4, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

    r32 *zptr = zbuffer;
    *zptr = -FLOAT_MAX;
    int inc = 0;
    int size = width*height*4;
    while(inc < size)
    {
        *zptr = -FLOAT_MAX;
        zptr++; inc+=4;
    }

    for(i32 i = 0;
        i < model.nfaces;
        ++i)
    {
        v3 vertexIndex = model.faces[i].vertex;
        v3 textureIndex = model.faces[i].texture;
        v3 screenCoords[3];
        v3 worldCoords[3];
        v2 textureCoords[3];
        for(i32 j = 0;
            j < 3;
            ++j)
        {
            v3 v = GetVertex(&model, (i32)vertexIndex.E[j]);
            v2 t = GetTexture(&model, (i32)textureIndex.E[j]);
            screenCoords[j] = WorldToScreen(v);
            //textureCoords[j] = TextureToScreen(t, textureFile);
            textureCoords[j] = t;
            worldCoords[j] = v;
        }
        v3 normal = (worldCoords[2] - worldCoords[0]) ^ (worldCoords[1] - worldCoords[0]);
        normal = Normalize(normal);
        r32 intensity = Inner(normal, lightDir);
        if(intensity > 0)
        {
#if 0
            FilledTriangle(screenCoords, textureCoords, textureFile, zbuffer, image,
                           Tga(u8(intensity * 255),
                           u8(intensity * 255),
                           u8(intensity * 255),
                           255));

#endif
        }
    }

    for(int y = 0;
        y < 910;
        ++y)
    {
        for(int x = 0;
            x<textureFile.header.imageWidth;
            ++x)
        {
//            tga_color bcol = Get(&textureFile, x, y);

            u8 *ptr = textureFile.data + u32((x + y * textureFile.header.imageWidth)*(textureFile.header.bitsPerPixel/8));
            tga_color bcol = Tga(*(ptr+2), *(ptr+1), *(ptr), 0);
            Set(&image, x, y, bcol);
        }
    }
    if(zbuffer)
    {
        VirtualFree(zbuffer, 0, MEM_RELEASE);
    }
}

int
main()
{
    obj_model *model = InitializeModel("obj/african_head.obj");

    loaded_tga image = InitializeTga((i16)width, (i16)height, RGB);
    //tga_file textureFile = ReadTgaFile("obj/african_head_texture.obj");

    //DrawWireMesh(*model, image);
    DrawShaders(*model, image);
    //DrawTextures(*model, textureFile, image);

    //v2 t0[3] = {V2(10, 70),   V2(50, 160),  V2(30, 80)};
    //v2 t1[3] = {V2(50, 10),  V2(20, 50),   V2(70, 50)};
    //v2 t2[3] = {V2(50, 80),  V2(20, 50),   V2(70, 50)};
    ////FilledTriangle(t0, image, red);
    //FilledTriangle(t1, image, white);
    //FilledTriangle(t2, image, green);


    FlipVertically(&image);
    WriteTgaFile(&image, "output.tga");

    return 0;
}
