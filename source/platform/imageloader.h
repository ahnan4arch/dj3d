//
//  source/platform/imageloader.h
//  dj3d
//
//  Created by Xu ZhiXin on 1/15/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//

#ifndef __DJ3D_PLATFORM_IMAGE_LOADER_H__
#define __DJ3D_PLATFORM_IMAGE_LOADER_H__

#include "math/3d.h"
#include <memory>
#include "utility/utility.h"

namespace dj3d
{
    enum ImageFormat
    {
        UNKNOWN = 0,
        RGB = 1,
        RGBA = 2
    }; // enum ImageFormat
    
    class Image : public MemoryBlock
    {
    private:
        const size_t m_width;
        const size_t m_height;
        const ImageFormat m_format;

    private:
        Image( const Image& );
        Image& operator = ( const Image& );
 
   public:
        Image( size_t width, size_t height, ImageFormat format );
        inline size_t width(void)const{ return m_width; }
        inline size_t height(void)const{ return m_height; }
        inline ImageFormat format(void)const{ return m_format; }
        static std::auto_ptr<Image> loadImage( const FilePath& fileName );
        static std::auto_ptr<Image> loadImage( MemoryBlock& blk );
        virtual ~Image(void);
    }; // class Image
}; // naemspace dj3d

#endif  // __DJ3D_PLATFORM_IMAGE_LOADER_H__
