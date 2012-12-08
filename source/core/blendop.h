//
//  source/core/blendop.h
//  dj3d
//
//  Created by Xu ZhiXin on 1/31/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//

#ifndef __DJ3D_CORE_BLENDOP_H__
#define __DJ3D_CORE_BLENDOP_H__

#include "math/3d.h"
#include "runtime/exception.h"

namespace dj3d
{
    // bit 0 ~ 3 : src alpha blend factor
    // bit 4 ~ 7 : src rgb blend factor
    // bit 8 ~ 11 : dst alpha blend factor
    // bit 12 ~ 15 : dst rgb blend factor
    // bit 16 ~ 19 : alpha blend function
    // bit 20 ~ 23 : rgb blend function
    // bit 20 ~ 31 : reserved, must be zero
    enum BlendOpCode
    {
        // src alpha
        SRC_ZERO  =  0x00,
        SRC_ALPHA_ONE   =  0x01,
        SRC_ALPHA_SRC   =  0x02,
        SRC_ALPHA_ONE_MINUS_SRC = 0x03,
        SRC_ALPHA_DST   = 0x04,
        SRC_ALPHA_ONE_MINUS_DST = 0x05,
        SRC_ALPHA_SRC_ALPHA = 0x06,
        SRC_ALPHA_ONE_MINUS_SRC_ALPHA = 0x07,
        SRC_ALPHA_DST_ALPHA = 0x08,
        SRC_ALPHA_ONE_MINUS_DST_ALPHA = 0x09,
        SRC_ALPHA_CONSTANT_COLOR = 0x0A,
        SRC_ALPHA_ONE_MINUS_CONSTANT_COLOR = 0x0B,
        SRC_ALPHA_CONSTANT_ALPHA = 0x0C,
        SRC_ALPHA_ONE_MINUS_CONSTANT_ALPHA = 0x0D,
        SRC_ALPHA_SATURATE = 0x0E,
        // src rgb
        SRC_RGB_ONE   =  0x10,
        SRC_RGB_SRC   =  0x20,
        SRC_RGB_ONE_MINUS_SRC = 0x30,
        SRC_RGB_DST   = 0x40,
        SRC_RGB_ONE_MINUS_DST = 0x50,
        SRC_RGB_SRC_ALPHA = 0x60,
        SRC_RGB_ONE_MINUS_SRC_ALPHA = 0x70,
        SRC_RGB_DST_ALPHA = 0x80,
        SRC_RGB_ONE_MINUS_DST_ALPHA = 0x90,
        SRC_RGB_CONSTANT_COLOR = 0xA0,
        SRC_RGB_ONE_MINUS_CONSTANT_COLOR = 0xB0,
        SRC_RGB_CONSTANT_ALPHA = 0xC0,
        SRC_RGB_ONE_MINUS_CONSTANT_ALPHA = 0xD0,
        SRC_RGB_SATURATE = 0xE0,
        // dst alpha
        DST_ALPHA_ONE = 0x0100,
        DST_ALPHA_SRC = 0x0200,
        DST_ALPHA_ONE_MINUS_SRC = 0x0300,
        DST_ALPHA_DST = 0x0400,
        DST_ALPHA_ONE_MINUS_DST = 0x0500,
        DST_ALPHA_SRC_ALPHA = 0x0600,
        DST_ALPHA_ONE_MINUS_SRC_ALPHA = 0x0700,
        DST_ALPHA_DST_ALPHA = 0x0800,
        DST_ALPHA_ONE_MINUS_DST_ALPHA = 0x0900,
        DST_ALPHA_CONSTANT_COLOR = 0x0A00,
        DST_ALPHA_ONE_MINUS_CONSTANT_COLOR = 0x0B00,
        DST_ALPHA_CONSTANT_ALPHA = 0x0C00,
        DST_ALPHA_ONE_MINUS_CONSTANT_ALPHA = 0x0D00,
        // dst rgb
        DST_RGB_ONE = 0x1000,
        DST_RGB_SRC = 0x2000,
        DST_RGB_ONE_MINUS_SRC = 0x3000,
        DST_RGB_DST = 0x4000,
        DST_RGB_ONE_MINUS_DST = 0x5000,
        DST_RGB_SRC_ALPHA = 0x6000,
        DST_RGB_ONE_MINUS_SRC_ALPHA = 0x7000,
        DST_RGB_DST_ALPHA = 0x8000,
        DST_RGB_ONE_MINUS_DST_ALPHA = 0x9000,
        DST_RGB_CONSTANT_COLOR = 0xA000,
        DST_RGB_ONE_MINUS_CONSTANT_COLOR = 0xB000,
        DST_RGB_CONSTANT_ALPHA = 0xC000,
        DST_RGB_ONE_MINUS_CONSTANT_ALPHA = 0xD000,
        // alpha blend function
        ALPHA_FUNC_ADD  =   0x10000,
        ALPHA_FUNC_SUB  =   0x20000,
        ALPHA_FUNC_REVERSE_SUB  =   0x30000,
        ALPHA_FUNC_MIN  =   0x40000,
        ALPHA_FUNC_MAX  =   0x50000,
        ALPHA_FUNC_LOGIC_OP =   0x60000,
        // rgb blend function
        RGB_FUNC_ADD  =   0x100000,
        RGB_FUNC_SUB  =   0x200000,
        RGB_FUNC_REVERSE_SUB  =   0x300000,
        RGB_FUNC_MIN  =   0x400000,
        RGB_FUNC_MAX  =   0x500000,
        RGB_FUNC_LOGIC_OP =   0x600000,
    }; // enum BlendOp

    enum BlendOpCodeMask
    {
        SRC_ALPHA_MASK = 0x000F,
        SRC_RGB_MASK = 0x00F0,
        DST_ALPHA_MASK = 0x0F00,
        DST_RGB_MASK = 0xF000,
        ALPHA_BLEND_FUNC_MASK = 0xF0000,
        RGB_BLEND_FUNC_MASK = 0xF00000
    }; // enum BlendOpCodeMask

    class BlendOp
    {
    public:
        static void Blend( const u32 opCode );
    }; // class BlendOp
}; // namespace dj3d


#endif // __DJ3D_CORE_BLENDOP_H__
