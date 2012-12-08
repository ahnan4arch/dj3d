//
//  source/image/jpegimg.cpp
//  dj3d
//
//  Created by Xu ZhiXin on 5/27/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//
#include "jpegimg.h"
#include "platform/imageloader.h"
#include "jpeglib.h"

namespace dj3d
{
     // JPEG --------------------------------------------------------------------
    struct dj3d_jpeg_error_mgr
    {
        jpeg_error_mgr pub;
    }; // struct dj3d_jpeg_error_mgr
    
    METHODDEF(void)
    dj3d_jpeg_error_exit( j_common_ptr cinfo )
    {
        throw InvalidJpegFormat("");
    }
    
    std::auto_ptr<Image> JpegImage::loadImage(MemoryBlock& source, ImageFormat format)
    {
        std::auto_ptr<Image> image;
        struct jpeg_decompress_struct cinfo;
        struct jpeg_error_mgr err;
        size_t rowStride = 0;
        
        if ( source.isEmpty() )
        {
            return image;
        }
        
        assert( RGB == format );

        jpeg_create_decompress(&cinfo);
        cinfo.err = jpeg_std_error(&err);
        err.error_exit = dj3d_jpeg_error_exit;
        
        try 
        {
            jpeg_mem_src(&cinfo, source.offsetT<unsigned char>(0), static_cast<unsigned long>(source.size()) );
            jpeg_read_header(&cinfo, TRUE);
            jpeg_start_decompress( &cinfo );
        
            // calculate the size of image buffer
            rowStride = cinfo.output_width * cinfo.output_components;
        
            image.reset( new Image( cinfo.output_width, cinfo.output_height, RGB) );            
            
            // decompress the jpeg image
            while ( cinfo.output_scanline < cinfo.output_height )
            {
                u8* pbuf = image->offsetT<u8>( rowStride * cinfo.output_scanline );
                jpeg_read_scanlines( &cinfo, &pbuf, 1 );
            }
            jpeg_finish_decompress(&cinfo);
        
            goto QUIT;
        }
        catch( InvalidJpegFormat& )
        {
            goto ERR;
        }
        catch( MemoryException& )
        {
            goto ERR;
        }
    ERR:
        jpeg_abort_decompress(&cinfo);
        image.reset();
    
    QUIT:
        jpeg_destroy_decompress(&cinfo);
        return image;
    }
   
}; //namespace dj3d
