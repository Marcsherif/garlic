#include "garlic_platform.h"
#if !defined(GARLIC_MATH_H)

#ifndef GARLIC_ROOT
#pragma once
#include "root.unity.h"
#endif

union v2
{
    struct
    {
        r32 X, Y;
    };
    r32 E[2];
};

union v3
{
    struct
    {
        r32 X, Y, Z;
    };
    struct
    {
        r32 R, G, B;
    };
    struct
    {
        v2 XY;
        r32 Ignored0_;
    };
    struct
    {
        r32 Ignored1_;
        v2 YZ;
    };
    r32 E[3];
};

union v4
{
    struct
    {
        union
        {
            v3 XYZ;
            struct
            {
                r32 X, Y, Z;
            };
        };

        r32 W;
    };
    struct
    {
        union
        {
            v3 RGB;
            struct
            {
                r32 R, G, B;
            };
        };

        r32 A;
    };
    r32 E[4];
};

struct rectangle2
{
    v2 Min;
    v2 Max;
};

struct rectangle3
{
    v3 Min;
    v3 Max;
};

inline v2
V2(r32 X, r32 Y)
{
    v2 Result;

    Result.X = X;
    Result.Y = Y;

    return(Result);
}

inline v3
V3(r32 X, r32 Y, r32 Z)
{
    v3 Result;

    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;

    return(Result);
}

inline v3
V3(v2 XY, r32 Z)
{
    v3 Result;

    Result.X = XY.X;
    Result.Y = XY.Y;
    Result.Z = Z;

    return(Result);
}

inline v4
V4(v3 P, r32 W)
{
    v4 Result;

    Result.X = P.X;
    Result.Y = P.Y;
    Result.Z = P.Z;
    Result.W = W;

    return(Result);
}

inline v4
V4(r32 X, r32 Y, r32 Z, r32 W)
{
    v4 Result;

    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = W;

    return(Result);
}

struct m4x4
{
    r32 E[4][4];
};

//
// NOTE: Scalar operations
//

inline r32
Square(r32 A)
{
    r32 Result = A*A;

    return(Result);
}

//
// NOTE: v2 operations
//

inline v2
operator*(r32 A, v2 B)
{
    v2 Result;

    Result.X = A*B.X;
    Result.Y = A*B.Y;

    return(Result);
}

inline v2
operator*(v2 B, r32 A)
{
    v2 Result = A*B;

    return(Result);
}

inline v2 &
operator*=(v2 &A, r32 B)
{
    A = B * A;

    return(A);
}

inline v2
operator/(r32 A, v2 B)
{
    v2 Result;

    Result.X = A/B.X;
    Result.Y = A/B.Y;

    return(Result);
}

inline v2
operator/(v2 B, r32 A)
{
    v2 Result = A/B;

    return(Result);
}

inline v2
operator+(v2 A, v2 B)
{
    v2 Result;

    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;

    return(Result);
}

inline v2 &
operator+=(v2 &A, v2 B)
{
    A = A + B;

    return(A);
}

inline v2
operator-(v2 A)
{
    v2 Result;

    Result.X = -A.X;
    Result.Y = -A.Y;

    return(Result);
}

inline v2
operator-(v2 A, v2 B)
{
    v2 Result;

    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;

    return(Result);
}

inline v2
operator-(v2 A, r32 B)
{
    A.X = A.X - B;
    A.Y = A.Y - B;

    return(A);
}

inline v2 &
operator-=(v2 &A, r32 B)
{
    A = A - B;

    return(A);
}

inline v2 &
operator-=(v2 &A, v2 B)
{
    A = A - B;

    return(A);
}

inline v2
Hadamard(v2 A, v2 B)
{
    v2 Result = {A.X*B.X, A.Y*B.Y};

    return(Result);
}

inline r32
Inner(v2 A, v2 B)
{
    r32 Result = A.X*B.X + A.Y*B.Y;

    return(Result);
}

inline r32
LengthSq(v2 A)
{
    r32 Result = Inner(A, A);

    return(Result);
}

inline r32
Length(v2 A)
{
    r32 Result = SquareRoot(LengthSq(A));

    return(Result);
}

//
// NOTE: v3 operations
//

inline v3
operator*(r32 A, v3 B)
{
    v3 Result;

    Result.X = A*B.X;
    Result.Y = A*B.Y;
    Result.Z = A*B.Z;

    return(Result);
}

inline v3
operator*(v3 B, r32 A)
{
    v3 Result = A*B;

    return(Result);
}

inline v3
operator*(v3 A, v3 B)
{
    v3 Result;

    Result.X = A.X*B.X;
    Result.Y = A.Y*B.Y;
    Result.Z = A.Z*B.Z;

    return(Result);
}

inline v3 &
operator*=(v3 &A, r32 B)
{
    A = B * A;

    return(A);
}

inline v3
operator/(r32 A, v3 B)
{
    v3 Result;

    Result.X = A/B.X;
    Result.Y = A/B.Y;
    Result.Z = A/B.Z;

    return(Result);
}

inline v3
operator/(v3 B, r32 A)
{
    v3 Result = A/B;

    return(Result);
}

inline v3
operator+(v3 A, v3 B)
{
    v3 Result;

    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    Result.Z = A.Z + B.Z;

    return(Result);
}

inline v3 &
operator+=(v3 &A, v3 B)
{
    A = A + B;

    return(A);
}

inline v3
operator-(v3 A)
{
    v3 Result;

    Result.X = -A.X;
    Result.Y = -A.Y;
    Result.Z = -A.Z;

    return(Result);
}

inline v3
operator-(v3 A, v3 B)
{
    v3 Result;

    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    Result.Z = A.Z - B.Z;

    return(Result);
}

inline v3
operator-(v3 A, r32 B)
{
    A.X = A.X - B;
    A.Y = A.Y - B;
    A.Z = A.Z - B;

    return(A);
}

inline v3 &
operator-=(v3 &A, r32 B)
{
    A = A - B;

    return(A);
}

inline v3 &
operator-=(v3 &A, v3 B)
{
    A = A - B;

    return(A);
}

inline v3
operator^(v3 A, v3 B)
{
    v3 Result;

    Result.X = (A.Y * B.Z) - (A.Z * B.Y);
    Result.Y = (A.Z * B.X) - (A.X * B.Z);
    Result.Z = (A.X * B.Y) - (A.Y * B.X);

    return(Result);
}
inline v3
Hadamard(v3 A, v3 B)
{
    v3 Result = {A.X*B.X, A.Y*B.Y, A.Z*B.Z};

    return(Result);
}

inline r32
Inner(v3 A, v3 B)
{
    r32 Result = A.X*B.X + A.Y*B.Y + A.Z*B.Z;

    return(Result);
}

inline r32
LengthSq(v3 A)
{
    r32 Result = Inner(A, A);

    return(Result);
}

inline r32
Length(v3 A)
{
    r32 Result = SquareRoot(LengthSq(A));

    return(Result);
}

inline v3
Normalize(v3 A)
{
    A = A*(1/Length(A));

    return(A);
}

inline v3
DeHomogonize(v4 A)
{
    return V3(A.X/A.W, A.Y/A.W, A.Z/A.W);
}

//
// NOTE: v4 operations
//

inline v4
Homogonize(v3 A)
{
    return V4(A.X, A.Y, A.Z, 1);
}

//
// NOTE: Rectangle2
//

inline v2
GetMinCorner(rectangle2 Rect)
{
    v2 Result = Rect.Min;
    return(Result);
}

inline v2
GetMaxCorner(rectangle2 Rect)
{
    v2 Result = Rect.Max;
    return(Result);
}

inline v2
GetCenter(rectangle2 Rect)
{
    v2 Result = 0.5f*(Rect.Min + Rect.Max);
    return(Result);
}

inline rectangle2
RectMinMax(v2 Min, v2 Max)
{
    rectangle2 Result;

    Result.Min = Min;
    Result.Max = Max;

    return(Result);
}

inline rectangle2
RectMinDim(v2 Min, v2 Dim)
{
    rectangle2 Result;

    Result.Min = Min;
    Result.Max = Min + Dim;

    return(Result);
}

inline rectangle2
RectCenterHalfDim(v2 Center, v2 HalfDim)
{
    rectangle2 Result;

    Result.Min = Center - HalfDim;
    Result.Max = Center + HalfDim;

    return(Result);
}

inline rectangle2
RectCenterDim(v2 Center, v2 Dim)
{
    rectangle2 Result = RectCenterHalfDim(Center, 0.5f*Dim);

    return(Result);
}

inline rectangle2
AddRadiusTo(rectangle2 A, v2 Radius)
{
    rectangle2 Result;

    Result.Min = A.Min - Radius;
    Result.Max = A.Max + Radius;

    return(Result);
}

inline b32
IsInRectangle(rectangle2 Rectangle, v2 Test)
{
    b32 Result = ((Test.X >= Rectangle.Min.X) &&
                     (Test.Y >= Rectangle.Min.Y) &&
                     (Test.X < Rectangle.Max.X) &&
                     (Test.Y < Rectangle.Max.Y));

    return(Result);
}

//
// NOTE: Rectangle3
//

inline v3
GetMinCorner(rectangle3 Rect)
{
    v3 Result = Rect.Min;
    return(Result);
}

inline v3
GetMaxCorner(rectangle3 Rect)
{
    v3 Result = Rect.Max;
    return(Result);
}

inline v3
GetCenter(rectangle3 Rect)
{
    v3 Result = 0.5f*(Rect.Min + Rect.Max);
    return(Result);
}

inline rectangle3
RectMinMax(v3 Min, v3 Max)
{
    rectangle3 Result;

    Result.Min = Min;
    Result.Max = Max;

    return(Result);
}

inline rectangle3
RectMinDim(v3 Min, v3 Dim)
{
    rectangle3 Result;

    Result.Min = Min;
    Result.Max = Min + Dim;

    return(Result);
}

inline rectangle3
RectCenterHalfDim(v3 Center, v3 HalfDim)
{
    rectangle3 Result;

    Result.Min = Center - HalfDim;
    Result.Max = Center + HalfDim;

    return(Result);
}

inline rectangle3
RectCenterDim(v3 Center, v3 Dim)
{
    rectangle3 Result = RectCenterHalfDim(Center, 0.5f*Dim);

    return(Result);
}

inline rectangle3
AddRadiusTo(rectangle3 A, v3 Radius)
{
    rectangle3 Result;

    Result.Min = A.Min - Radius;
    Result.Max = A.Max + Radius;

    return(Result);
}

inline b32
IsInRectangle(rectangle3 Rectangle, v3 Test)
{
    b32 Result = ((Test.X >= Rectangle.Min.X) &&
                     (Test.Y >= Rectangle.Min.Y) &&
                     (Test.Z >= Rectangle.Min.Z) &&
                     (Test.X < Rectangle.Max.X) &&
                     (Test.Y < Rectangle.Max.Y) &&
                     (Test.Z < Rectangle.Max.Z));

    return(Result);
}

inline b32
RectanglesIntersect(rectangle3 A, rectangle3 B)
{
    b32 Result = !((B.Max.X <= A.Min.X) ||
                      (B.Min.X >= A.Max.X) ||
                      (B.Max.Y <= A.Min.Y) ||
                      (B.Min.Y >= A.Max.Y) ||
                      (B.Max.Z <= A.Min.Z) ||
                      (B.Min.Z >= A.Max.Z));

    return(Result);
}

//
// 4x4 Matrices
//

internal m4x4
operator*(m4x4 A, m4x4 B)
{
    m4x4 R = {};

    for(int r = 0; r <= 3; ++r)
    {
        for(int c = 0; c <= 3; ++c)
        {
            for(int i = 0; i <= 3; ++i)
            {
                R.E[r][c] += A.E[r][i]*B.E[i][c];
            }
        }
    }

    return(R);
}

internal v4
Transform(m4x4 A, v4 P)
{
    v4 R;

    R.X = P.X*A.E[0][0] + P.Y*A.E[0][1] + P.Z*A.E[0][2] + P.W*A.E[0][3];
    R.Y = P.X*A.E[1][0] + P.Y*A.E[1][1] + P.Z*A.E[1][2] + P.W*A.E[1][3];
    R.Z = P.X*A.E[2][0] + P.Y*A.E[2][1] + P.Z*A.E[2][2] + P.W*A.E[2][3];
    R.W = P.X*A.E[3][0] + P.Y*A.E[3][1] + P.Z*A.E[3][2] + P.W*A.E[3][3];

    return R;
}

inline v3
operator*(m4x4 A, v3 P)
{
    v3 R = Transform(A, V4(P, 1.0f)).XYZ;
    return(R);
}

inline v4
operator*(m4x4 A, v4 P)
{
    v4 R = Transform(A, P);
    return(R);
}

inline m4x4
Identity(void)
{
    m4x4 r =
    {
        {{1, 0, 0, 0},
         {0, 1, 0, 0},
         {0, 0, 1, 0},
         {0, 0, 0, 1}},
    };

    return r;
}

#define GARLIC_MATH_H
#endif
