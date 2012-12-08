//
//  texture2d.cpp
//  glutRun
//
//  Created by Xu ZhiXin on 1/8/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//

#include "platform/dj3d.h"
#include "texture2d.h"

namespace dj3d
{
    TextCoord::TextCoord( f32 s, f32 t )
    {
        if ( s < 0.0f )
        {
            m_s = 0.0f;
        }
        else if ( s > 1.0f )
        {
            m_s = 1.0f;
        }
        else
        {
            m_s = s;
        }

        if ( t < 0.0f )
        {
            m_t = 0.0f;
        }
        else if ( t > 1.0f )
        {
            m_t = 1.0f;
        }
        else
        {
            m_t = t;
        }

        return;
    }

    TextCoord::TextCoord():
    m_s(0.0f), 
        m_t(0.0f)
    {
        return;
    }

    TextCoord::TextCoord( const TextCoord& other ):
    m_s ( other.m_s ),
        m_t ( other.m_t )
    {
        return;
    }

    TextCoord& TextCoord::operator = ( const TextCoord& other )
    {
        if ( &other != this )
        {
            m_s = other.m_s;
            m_t = other.m_t;
        }

        return *this;
    }

    const Texture2d Texture2d::s_emptyTexture2d;

    Texture2d::Texture2d(void):
    m_internal_id(0),
    m_id(0)
    {
        return;
    }

    Texture2d::~Texture2d(void)
    {
        if ( 0 != m_internal_id )
        {
            GLuint _id = static_cast<GLuint>(m_internal_id);
            glDeleteTextures(1, &_id);
            m_internal_id = 0;
        }
        return;
    }

    bool Texture2d::select(void)const
    {
        if ( 0 == m_internal_id || NULL == m_image.get() )
        {
            glDisable(GL_TEXTURE_2D);
        }
        else
        {
            glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture( GL_TEXTURE_2D,  static_cast<GLuint>(m_internal_id));
        }

        return true;
    }

    Texture2d* Texture2d::loadTexture2d( const FilePath& textName, const size_t textId )
    {
        std::auto_ptr<Image> _image = Image::loadImage( textName );
        if ( _image.get() == NULL )
        {
            return NULL;
        }

        Texture2d * _pText = new Texture2d();
        _pText->m_id = textId;
        _pText->m_image = _image;

        GLuint _id = 0;
        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);
        _pText->m_internal_id = static_cast<u32>(_id);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        switch( _pText->m_image->format() )
        {
        case RGB:
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 
                static_cast<GLsizei>(_pText->m_image->width()),
                static_cast<GLsizei>(_pText->m_image->height()),
                0, GL_RGB, GL_UNSIGNED_BYTE, 
                _pText->m_image->offsetT<u8>(0) );
            break;

        case RGBA:
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 
                static_cast<GLsizei>(_pText->m_image->width()),
                static_cast<GLsizei>(_pText->m_image->height()),
                0, GL_RGBA, GL_UNSIGNED_BYTE, 
                _pText->m_image->offsetT<u8>(0) );
            break;

        default:
            assert( false );
        }
        return _pText;
    }

    void Texture2d::releaseTexture2d( Texture2d* pTexture )
    {
        if ( NULL != pTexture )
        {
            delete pTexture;
        }
        return;
    }

}; // namespace dj3d
