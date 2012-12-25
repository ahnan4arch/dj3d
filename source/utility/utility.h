//
//  source/utility/utility.h
//  dj3d
//
//  Created by Xu ZhiXin on 1/19/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//

#ifndef __DJ3D_UTILITY_H__
#define __DJ3D_UTILITY_H__

#include <string>
#include "runtime/exception.h"

namespace dj3d
{
    class FilePath
    {
    private:
        std::string m_utf8FileName;
    public:
        explicit FilePath( const std::string& strPlatformPathName );
        FilePath ( const FilePath& other );
        FilePath & operator = ( const FilePath& other );
        inline operator std::string&(void){ return m_utf8FileName; }
        inline operator const std::string&(void)const{ return m_utf8FileName;}
        bool operator == ( const FilePath& other )const;
        bool operator != ( const FilePath& other )const;
        ~FilePath(void);
    }; // class FilePath

    class MemoryBlock
    {
    private:
        typedef unsigned char ElementType;
        ElementType* m_pBuffer;
        size_t m_size;
    private:

    public:
        MemoryBlock(void);
        MemoryBlock(const MemoryBlock& other);
        explicit MemoryBlock(size_t size);
        MemoryBlock(void* pData, size_t size);
        bool alloc( size_t size );
        void clear(void);
        void swap( MemoryBlock& other );
        MemoryBlock& operator = ( const MemoryBlock& other );

        template<typename T>
        T* offsetT( size_t offset )
        {
            if ( sizeof(T) * offset < m_size )
            {
                return (T*)m_pBuffer + offset;
            }
            else
            {
                return NULL;
            }
        }

        template<typename T>
        const T* offsetT( size_t offset ) const
        {
            if ( sizeof(T) * offset < m_size )
            {
                return (T*)m_pBuffer + offset;
            }
            else
            {
                return NULL;
            }
        }

        template<typename T>
        size_t countOfT(void)const
        {
            return m_size / sizeof(T);
        }

        inline void* data(void) { return m_pBuffer; }
        inline const void* data(void)const{ return m_pBuffer; }
        inline size_t size(void)const{ return m_size; } 
        inline bool isEmpty(void)const{ return 0 == m_size; }
        virtual ~MemoryBlock(void);
    }; // class MemoryBlock
    
}; // namespace dj3d

#endif // __DJ3D_UTILITY_H__
