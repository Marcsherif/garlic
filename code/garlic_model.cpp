#ifndef GARLIC_ROOT
#pragma once
#include "root.unity.h"
#endif

internal v3
GetVertex(obj_model *model, i32 vertexCoord)
{
    Assert(vertexCoord <= model->nverts);
    v3 Result = *(model->verts + vertexCoord);
    return (Result);
}

internal v2
GetTexture(obj_model *model, i32 textureCoord)
{
    Assert(textureCoord <= model->ntextures);
    v2 Result = *(model->textures + textureCoord);
    return (Result);
}

internal void
DEBUG_PLATFORM_FREE_ENTIRE_FILE(void *Memory)
{
    if(Memory)
    {
        VirtualFree(Memory, 0, MEM_RELEASE);
    }
}

internal debug_read_file_result
DEBUG_PLATFORM_READ_ENTIRE_FILE(char *Filename)
{
    debug_read_file_result Result = {};

    HANDLE FileHandle = CreateFileA(Filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if(FileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER FileSize;
        if(GetFileSizeEx(FileHandle, &FileSize))
        {
            u32 FileSize32 = SafeTruncateUInt64(FileSize.QuadPart);
            Result.Contents = VirtualAlloc(0, FileSize32, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
            if(Result.Contents)
            {
                DWORD BytesRead;
                if(ReadFile(FileHandle, Result.Contents, FileSize32, &BytesRead, 0) &&
                   (FileSize32 == BytesRead))
                {
                    Result.ContentsSize = FileSize32;
                }
                else
                {
                    // TODO(marc): Logging
                    DEBUG_PLATFORM_FREE_ENTIRE_FILE(Result.Contents);
                    Result.Contents = 0;
                }
            }
            else
            {
                // TODO(marc): Logging
            }
        }
        else
        {
        // TODO(marc): Logging
        }

        CloseHandle(FileHandle);
    }
    else
    {
        // TODO(marc): Logging
    }

    return(Result);
}

internal void
CatStrings(size_t SourceACount, char *SourceA,
           size_t SourceBCount, char *SourceB,
           size_t DestCount, char *Dest)
{
    for(int Index = 0;
        Index < SourceACount;
        ++Index)
    {
        *Dest++ = *SourceA++;
    }

    for(int Index = 0;
        Index < SourceBCount;
        ++Index)
    {
        *Dest++ = *SourceB++;
    }

    *Dest++ = 0;
}

internal void
CopyStrings(size_t SourceACount, char *SourceA,
           size_t DestCount, char *Dest)
{
    for(int Index = 0;
        Index < SourceACount;
        ++Index)
    {
        *Dest++ = *SourceA++;
    }

    *Dest++ = 0;
}

internal int
StringLength(char *String)
{
    int Count = 0;
    if(String != 0)
    {
        while(*String++)
        {
            ++Count;
        }
    }
    return (Count);
}

internal void
ChopDelim(char **string, char delimiter)
{
    if(string)
    {
        while(**string && **string != delimiter)
        {
            *string += 1;
        }
        *string += 1;
    }
}

internal void
ClearString(char *string, char value, i32 StringLength)
{
    for(int Index = 0;
        Index < StringLength;
        ++Index)
    {
        *string++ = value;
    }

    *string++ = 0;
}

internal obj_model *
InitializeModel(char *filename)
{
    obj_model *model = {};
#if 1 // TODO: string and file parsing later
    debug_read_file_result fp = DEBUG_PLATFORM_READ_ENTIRE_FILE(filename);
    if(fp.ContentsSize != 0)
    {
        model = (obj_model *)VirtualAlloc(0, fp.ContentsSize*4, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
        model->filename = filename;
        char *ch = (char *)fp.Contents;
        u32 nVerts = 0;
        u32 ntextures = 0;
        u32 nFaces = 0;
        while(*ch)
        {
            if(*ch == 'v')
            {
                ch++;
                if(*ch == ' ')
                {
                    nVerts++;
                }
                if(*ch == 't')
                {
                    ntextures++;
                }
            }
            else if(*ch == 'f')
            {
                nFaces++;
            }

            ChopDelim(&ch, '\n');
        }

        ch = (char *)fp.Contents;
        model->verts = (v3 *)(model + sizeof(obj_model));
        model->textures = (v2 *)(model->verts + sizeof(*model->verts)*nVerts);
        model->faces = (faces *)(model->textures + sizeof(*model->textures)*ntextures);
        while(*ch)
        {
            //int len = LengthUntilDelim(ch, '\n');
            //CopyStrings(len, ch, len, line + 0);

            if(*ch == 'v')
            {
                ch++;
                if(*ch == ' ')
                {
                    v3 vertex = {};

                    ChopDelim(&ch, ' ');
                    vertex.X = (r32)atof(ch);

                    ChopDelim(&ch, ' ');
                    vertex.Y = (r32)atof(ch);

                    ChopDelim(&ch, ' ');
                    vertex.Z = (r32)atof(ch);

                    model->verts[model->nverts++] = vertex;
                }
                if(*ch == 't')
                {
                    v2 texture = {};

                    ChopDelim(&ch, ' ');
                    ChopDelim(&ch, ' ');
                    texture.X = (r32)atof(ch);

                    ChopDelim(&ch, ' ');
                    texture.Y = (r32)atof(ch);

                    ChopDelim(&ch, ' ');

                    model->textures[model->ntextures++] = texture;
                }
            }
            else if(*ch == 'f')
            {
                v3 vertex = {};
                v3 texture = {};

                ChopDelim(&ch, ' ');
                vertex.X = (r32)atof(ch);

                ChopDelim(&ch, '/');
                texture.X = (r32)atof(ch);

                ChopDelim(&ch, ' ');
                vertex.Y = (r32)atof(ch);

                ChopDelim(&ch, '/');
                texture.Y = (r32)atof(ch);

                ChopDelim(&ch, ' ');
                vertex.Z = (r32)atof(ch);

                ChopDelim(&ch, '/');
                texture.Z = (r32)atof(ch);

                vertex = vertex - 1;
                model->faces[model->nfaces].vertex = vertex;

                texture = texture - 1;
                model->faces[model->nfaces++].texture = texture;
            }

            //ClearString(line, 0, sizeof(line));
            //ch += len + 1;
            ChopDelim(&ch, '\n');
        }
    }
#endif

    return model;
}
