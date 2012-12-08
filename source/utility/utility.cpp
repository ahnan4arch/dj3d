//
//  source/utility/utility.cpp
//  dj3d
//
//  Created by Xu ZhiXin on 1/19/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//

#include "utility.h"

namespace dj3d
{
    FilePath::FilePath( const std::string& strPlatformPathName ):
    m_utf8FileName( strPlatformPathName )
    {
        return;
    }
    
    FilePath::FilePath ( const FilePath& other ):
    m_utf8FileName( other.m_utf8FileName )
    {
        return;
    }
   
    FilePath & FilePath::operator = ( const FilePath& other )
    {
        if ( &other != this )
        {
            m_utf8FileName = other.m_utf8FileName;
        }
        return *this;
    }
    
    bool FilePath::operator == ( const FilePath& other )const
    {
        return m_utf8FileName == other.m_utf8FileName;
    }
        
    bool FilePath::operator != ( const FilePath& other )const
    {
        return m_utf8FileName != other.m_utf8FileName;
    }
    
    FilePath::~FilePath(void)
    {
        return;
    }
        
    MemoryBlock::MemoryBlock(void):
    m_pBuffer(NULL),
    m_size(0)
    {
        return;
    }
    
    MemoryBlock::MemoryBlock(const MemoryBlock& other ):
    m_pBuffer( NULL ),
    m_size(0)
    {
        *this = other;
        return;
    }
    
    MemoryBlock::MemoryBlock(size_t size):
    m_pBuffer(NULL),
    m_size(0)
    {
        alloc( size );
        return;
    }
    
    MemoryBlock::MemoryBlock(void* pData, size_t size):
    m_pBuffer(NULL),
    m_size(0)
    {
        alloc( size );
        if ( NULL != pData && size > 0 )
        {
            memcpy( m_pBuffer, pData, size );
        }
        return;
    }
    
    bool MemoryBlock::alloc( size_t size )
    {
        if ( size != m_size )
        {
            clear();
            
            if ( size > 0 )
            {
                m_pBuffer = (ElementType*)malloc(size);
                if ( NULL != m_pBuffer )
                {
                    m_size = size;
                }
                else
                {
                    throw MemoryException("");
                }
            }
        }
        
        return NULL != m_pBuffer;
    }
    
    void MemoryBlock::clear(void)
    {
        if ( NULL != m_pBuffer )
        {
            free(m_pBuffer);
            m_pBuffer = NULL;
            m_size = 0;
        }
        
        return;
    }
    
    void MemoryBlock::swap( MemoryBlock& other )
    {
        if ( &other != this )
        {
            ElementType* _pbuf = other.m_pBuffer;
            size_t _tmp = other.m_size;
            
            other.m_pBuffer = m_pBuffer;
            other.m_size = m_size;
            
            m_pBuffer = _pbuf;
            m_size = _tmp;
        }
        
        return;
    }
    
    MemoryBlock& MemoryBlock::operator = ( const MemoryBlock& other )
    {
        if ( &other != this )
        {
            if ( 0 == other.m_size )
            {
                clear();
            }
            else
            {
                alloc( other.m_size );
                memcpy( m_pBuffer, other.m_pBuffer, m_size );
            }            
        }
        
        return *this;
    }
    
    MemoryBlock::~MemoryBlock(void)
    {
        clear();
        return;
    }
    
}; // namespace dj3d
