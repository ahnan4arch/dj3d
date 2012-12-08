//
//  source/image/pngimg.cpp
//  dj3d
//
//  Created by Xu ZhiXin on 5/27/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//
#include "pngimg.h"
#include "platform/imageloader.h"
#include "png.h"

namespace dj3d
{
    struct PNGUserData
    {
        u8* buffer;
        size_t offset;
        size_t length;
    };

    static int 
    read_chunck_callback(png_structp png_ptr, png_unknown_chunkp chunk)
    {
        return 0;
    }

    static void 
    read_row_callback(png_structp png_ptr, png_uint_32 row, int pass)
    {
        return;
    }

    static void 
    user_error_callback(png_structp png_ptr, png_const_charp message)
    {
        throw InvalidPngFormat(message);
    }

    static void 
    user_warning_callback(png_structp png_ptr, png_const_charp message)
    {
        return;
    }

    static void
    user_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
    {
        PNGUserData* _pReadIO = (PNGUserData*)png_get_io_ptr(png_ptr);
        assert ( _pReadIO->offset + length <= _pReadIO->length );

        memcpy(data, _pReadIO->buffer + _pReadIO->offset, length);
        _pReadIO->offset += length;

        return;
    }

    std::auto_ptr<Image> PngImage::loadImage(MemoryBlock& source, ImageFormat format)
    {
        std::auto_ptr<Image> image;
        png_structp png_ptr = NULL;
        png_infop info_ptr = NULL;

        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if ( NULL == png_ptr )
        {
            goto ERR;
        }

        png_set_error_fn(png_ptr, NULL , user_error_callback, user_warning_callback);

        info_ptr = png_create_info_struct(png_ptr);
        if ( NULL == info_ptr )
        {
            goto ERR;
        }

        try
        {
            png_set_read_user_chunk_fn(png_ptr, NULL, read_chunck_callback);

            PNGUserData _read_io = { source.offsetT<u8>(0), 0, source.size() };
            png_set_read_fn(png_ptr, &_read_io, user_read_data);

            png_read_info(png_ptr, info_ptr);

            switch (  png_get_color_type(png_ptr, info_ptr) )
            {
            case PNG_COLOR_TYPE_PALETTE:
                png_set_palette_to_rgb(png_ptr);
                break;
            case PNG_COLOR_TYPE_GRAY:
                if (png_get_bit_depth(png_ptr, info_ptr) < 8)
                {
                    png_set_expand_gray_1_2_4_to_8(png_ptr);
                }
                break;
            default:
                break;
            }

            png_read_update_info(png_ptr, info_ptr);

            png_uint_32 _width = png_get_image_width(png_ptr, info_ptr);
            png_uint_32 _height = png_get_image_height(png_ptr, info_ptr);
            int _bitDepth = png_get_bit_depth(png_ptr, info_ptr);
            // int _colorType = png_get_color_type(png_ptr, info_ptr);
            int _channels = png_get_channels(png_ptr, info_ptr);

            // row pointers
            MemoryBlock _rowPointers( sizeof(png_bytep) * _height );
            image.reset( new Image(_width, _height, (4==_channels)?RGBA:RGB));
            size_t _rowStep = _width * _channels * _bitDepth / 8;
            for (png_uint_32 i = 0; i < _height; ++ i)
            {
                *(_rowPointers.offsetT<u8*>(i)) = image->offsetT<u8>(i * _rowStep);
            }

            png_read_rows(png_ptr, (png_bytepp)_rowPointers.data(), NULL, _height);

            goto QUIT;
        }
        catch( InvalidPngFormat& )
        {
            goto ERR;
        }
        catch( MemoryException& )
        {
            goto ERR;
        }

ERR:
        image.reset();
QUIT:
        if ( NULL != png_ptr )
        {
            png_destroy_read_struct( &png_ptr, &info_ptr, NULL);
        }

        return image;
    }
}; //namespace dj3d
