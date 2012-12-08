//
//  source/core/texture2d.h
//  dj3d
//
//  Created by Xu ZhiXin on 1/8/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//

#ifndef __DJ3D_CORE_TEXTURE_2D_H__
#define __DJ3D_CORE_TEXTURE_2D_H__

#include "math/3d.h"
#include "platform/imageloader.h"

namespace dj3d 
{
    typedef size_t TextureId;

    struct TextCoord
    {
        f32 m_s;
        f32 m_t;
        TextCoord( f32 s, f32 t );
        TextCoord();
        TextCoord( const TextCoord& other );
        TextCoord& operator = ( const TextCoord& other );
    }; // struct TextCoord

    class Texture2d
    {
    private:
        Texture2d( const Texture2d& );
        Texture2d& operator = ( const Texture2d& );
    private:
        u32 m_internal_id;
        TextureId m_id;
        std::auto_ptr<Image> m_image;        
    protected:
        Texture2d(void);
        virtual ~Texture2d(void);
    public:
        inline TextureId id(void)const{ return m_id;}
        bool isTransparent(void)const{ return false; }
        bool select(void)const;
    public:
        static Texture2d* loadTexture2d( const FilePath& textName, const size_t textId );
        static void releaseTexture2d( Texture2d* pTexture );
        static const Texture2d s_emptyTexture2d;
    }; // class Texture2d
}; // namespace dj3d

#endif // __DJ3D_CORE_TEXTURE_2D_H__
