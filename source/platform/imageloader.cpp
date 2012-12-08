//
//  source/platform/imageloader.cpp
//  dj3d
//
//  Created by Xu ZhiXin on 1/15/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//

#include "imageloader.h"
#include "image/jpegimg.h"
#include "image/pngimg.h"

namespace dj3d
{
    Image::Image( size_t width, size_t height, ImageFormat format ):
    m_width( width ),
    m_height( height ),
    m_format( format )
    {
        assert( width > 0 );
        assert( height > 0 );
        assert( UNKNOWN != m_format );
        
        size_t components;
        switch ( m_format )
        {
            case RGB:
                components = 3;
                break;
            case RGBA:
                components = 4;
                break;
            default:
                assert( false );
        } // switch
        
        alloc( m_width * components * m_height );

        return;
    }
    
    std::auto_ptr<Image> Image::loadImage( const FilePath& fileName )
    {
        FILE* _fp = fopen( ((std::string)fileName).c_str(), "rb" );
        if ( NULL == _fp )
        {
            return  std::auto_ptr<Image>();
        }
        
        // get length of the file
        assert( 0 == ftell(_fp) );
        size_t lenOfFile = 0;
        fseek(_fp, 0, SEEK_END);
        lenOfFile = static_cast<size_t>(ftell(_fp));
        fseek(_fp, 0, SEEK_SET );
        
        MemoryBlock _blk( lenOfFile );
        fread( _blk.data(), 1, _blk.size(), _fp );
        fclose( _fp );
        _fp = NULL;

        return loadImage(_blk);
    }
    
    std::auto_ptr<Image> Image::loadImage( MemoryBlock& blk )
    {
        std::auto_ptr<Image> _image;

        if (blk.isEmpty())
        {
            return _image;
        }

        // read image as jpeg
        _image = JpegImage::loadImage(blk, RGB);

        if ( NULL == _image.get() )
        {
            // read image as png
            _image = PngImage::loadImage(blk, RGBA);
        }

        return _image;
    }
    
    Image::~Image(void)
    {
        return;
    }
}; // naemspace dj3d

