//
//  source/core/blendop.cpp
//  dj3d
//
//  Created by Xu ZhiXin on 1/31/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//

#include "platform/dj3d.h"

#include "blendop.h"

namespace dj3d
{

static GLenum _BlendOpCode2GlEnum( u32 code ) throw (UnknownBlendOpCode)
{
    switch (code)
    {
    case SRC_ZERO:
        return GL_ZERO;

    case SRC_ALPHA_ONE:
    case SRC_RGB_ONE:
    case DST_ALPHA_ONE:
    case DST_RGB_ONE:
        return GL_ONE;

    case SRC_ALPHA_SRC:
    case SRC_RGB_SRC:
    case DST_ALPHA_SRC:
    case DST_RGB_SRC:
        return GL_SRC_COLOR;

    case SRC_ALPHA_ONE_MINUS_SRC:
    case SRC_RGB_ONE_MINUS_SRC:
    case DST_ALPHA_ONE_MINUS_SRC:
    case DST_RGB_ONE_MINUS_SRC:
        return GL_ONE_MINUS_SRC_COLOR;

    case SRC_ALPHA_DST:
    case SRC_RGB_DST:
    case DST_ALPHA_DST:
    case DST_RGB_DST:
        return GL_DST_COLOR;

    case SRC_ALPHA_ONE_MINUS_DST:
    case SRC_RGB_ONE_MINUS_DST:
    case DST_ALPHA_ONE_MINUS_DST:
    case DST_RGB_ONE_MINUS_DST:
        return GL_ONE_MINUS_DST_COLOR;

    case SRC_ALPHA_SRC_ALPHA:
    case SRC_RGB_SRC_ALPHA:
    case DST_ALPHA_SRC_ALPHA:
    case DST_RGB_SRC_ALPHA:
        return GL_SRC_ALPHA;

    case SRC_ALPHA_ONE_MINUS_SRC_ALPHA:
    case SRC_RGB_ONE_MINUS_SRC_ALPHA:
    case DST_ALPHA_ONE_MINUS_SRC_ALPHA:
    case DST_RGB_ONE_MINUS_SRC_ALPHA:
        return GL_ONE_MINUS_SRC_ALPHA;

    case SRC_ALPHA_DST_ALPHA:
    case SRC_RGB_DST_ALPHA:
    case DST_ALPHA_DST_ALPHA:
    case DST_RGB_DST_ALPHA:
        return GL_DST_ALPHA;

    case SRC_ALPHA_ONE_MINUS_DST_ALPHA:
    case SRC_RGB_ONE_MINUS_DST_ALPHA:
    case DST_ALPHA_ONE_MINUS_DST_ALPHA:
    case DST_RGB_ONE_MINUS_DST_ALPHA:
        return GL_ONE_MINUS_DST_ALPHA;

    case SRC_ALPHA_CONSTANT_COLOR:
    case SRC_RGB_CONSTANT_COLOR:
    case DST_ALPHA_CONSTANT_COLOR:
    case DST_RGB_CONSTANT_COLOR:
        return GL_CONSTANT_COLOR;

    case SRC_ALPHA_ONE_MINUS_CONSTANT_COLOR:
    case SRC_RGB_ONE_MINUS_CONSTANT_COLOR:
    case DST_ALPHA_ONE_MINUS_CONSTANT_COLOR:
    case DST_RGB_ONE_MINUS_CONSTANT_COLOR:
        return GL_ONE_MINUS_CONSTANT_COLOR;

    case SRC_ALPHA_CONSTANT_ALPHA:
    case SRC_RGB_CONSTANT_ALPHA:
    case DST_ALPHA_CONSTANT_ALPHA:
    case DST_RGB_CONSTANT_ALPHA:
        return GL_CONSTANT_ALPHA;

    case SRC_ALPHA_ONE_MINUS_CONSTANT_ALPHA:
    case SRC_RGB_ONE_MINUS_CONSTANT_ALPHA:
    case DST_ALPHA_ONE_MINUS_CONSTANT_ALPHA:
    case DST_RGB_ONE_MINUS_CONSTANT_ALPHA:
        return GL_ONE_MINUS_CONSTANT_ALPHA;

    case SRC_ALPHA_SATURATE:
    case SRC_RGB_SATURATE:
        return GL_SRC_ALPHA_SATURATE;

    case ALPHA_FUNC_ADD:
    case RGB_FUNC_ADD:
        return GL_FUNC_ADD;

    case ALPHA_FUNC_SUB:
    case RGB_FUNC_SUB:
        return GL_FUNC_SUBTRACT;

    case ALPHA_FUNC_REVERSE_SUB:
    case RGB_FUNC_REVERSE_SUB:
        return GL_FUNC_REVERSE_SUBTRACT;

    case ALPHA_FUNC_MIN:
    case RGB_FUNC_MIN:
        return GL_MIN;

    case ALPHA_FUNC_MAX:
    case RGB_FUNC_MAX:
        return GL_MAX;

    case ALPHA_FUNC_LOGIC_OP:
    case RGB_FUNC_LOGIC_OP:
        return GL_LOGIC_OP;

    default:
        break;
    }

    throw UnknownBlendOpCode("");
}

void BlendOp::Blend( const u32 opCode )
{
    GLenum _srcAlpha = _BlendOpCode2GlEnum( opCode & SRC_ALPHA_MASK );
    GLenum _srcRGB = _BlendOpCode2GlEnum( opCode & SRC_RGB_MASK );
    GLenum _dstAlpha = _BlendOpCode2GlEnum( opCode & DST_ALPHA_MASK );
    GLenum _dstRGB = _BlendOpCode2GlEnum( opCode & DST_RGB_MASK );
    GLenum _alphaBlend = _BlendOpCode2GlEnum( opCode & ALPHA_BLEND_FUNC_MASK );
    GLenum _rgbBlend = _BlendOpCode2GlEnum( opCode & RGB_BLEND_FUNC_MASK );

    glBlendFuncSeparate( _srcRGB, _dstRGB, _srcAlpha, _dstAlpha );
    glBlendEquationSeparate(_rgbBlend, _alphaBlend);

    return;
}
}; // namespace dj3d