//
//  source/image/jpegimg.h
//  dj3d
//
//  Created by Xu ZhiXin on 5/27/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//
#ifndef __DJ3D_IMAGE_JPEG_H__
#define __DJ3D_IMAGE_JPEG_H__

#include "platform/imageloader.h"

namespace dj3d
{
    class Image;

    class JpegImage
    {
    public:
        static std::auto_ptr<Image> loadImage(MemoryBlock& source, ImageFormat format);
    }; // class PngImage

}; // namespace dj3d

#endif // __DJ3D_IMAGE_PNG_H__
