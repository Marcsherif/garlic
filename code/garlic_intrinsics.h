#if !defined(GARLIC_INTRINSICS_H)

#ifndef GARLIC_ROOT
#pragma once
#include "root.unity.h"
#endif

#include "math.h"

//
// TODO(marc): convert all to platform-efficient versions
// and remove nath.h
//

inline i32
SignOf(i32 Value)
{
    i32 Result = (Value >= 0) ? 1 : -1;
    return(Result);
}

inline r32
SquareRoot(r32 Real32)
{
    r32 Result = sqrtf(Real32);
    return (Result);
}

inline r32
AbsolouteValue(r32 Real32)
{
    r32 Result = (r32)fabs(Real32);
    return (Result);
}

inline u32
RotateLeft(u32 Value, i32 Amount)
{
#if COMPILER_MSVC
    u32 Result = _rotl(Value, Amount);
#else
    Amount &= 31;
    u32 Result = ((Value << Amount) | (Value >> (32 - Amount)));
#endif
    return(Result);
}

inline u32
RotateRight(u32 Value, i32 Amount)
{
#if COMPILER_MSVC
    u32 Result = _rotr(Value, Amount);
#else
    Amount &= 31;
    u32 Result = ((Value >> Amount) | (Value << (32 - Amount)));
#endif
    return(Result);
}

inline i32
RoundReal32ToInt32(r32 Real32)
{
    i32 Result = (i32)roundf(Real32);
    return (Result);
}

inline i32
RoundReal32ToUInt32(r32 Real32)
{
    i32 Result = (u32)roundf(Real32);
    // TODO(marc): intinsic??
    return (Result);
}

inline i32
FloorReal32ToInt32(r32 Real32)
{
    i32 Result = (i32)floorf(Real32);
    return (Result);
}

inline i32
CeilReal32ToInt32(r32 Real32)
{
    i32 Result = (i32)ceilf(Real32);
    return (Result);
}

inline i32
TruncateReal32ToInt32(r32 Real32)
{
    i32 Result = (i32)Real32;
    return (Result);
}

inline r32
Sin(r32 Angle)
{
    r32 Result = sinf(Angle);
    return(Result);
}

inline r32
Cos(r32 Angle)
{
    r32 Result = cosf(Angle);
    return(Result);
}

inline r32
Atan2(r32 Y, r32 X)
{
    r32 Result = atan2f(Y, X);
    return(Result);
}

struct bit_scan_result
{
	b32 Found;
	u32 Index;
};
internal bit_scan_result
FindLeastSignificantSetBit(u32 Value)
{
	bit_scan_result Result = {};

	Result.Found = _BitScanForward((unsigned long *)&Result.Index, Value);

	return(Result);
}

#define GARLIC_INTRINSICS_H
#endif
