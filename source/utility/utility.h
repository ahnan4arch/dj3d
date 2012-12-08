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
#if 0
    template<size_t sizeOfBlock, typename TLengthType>
    class SimpleMemory
    {
    private:
        struct _TCoreUnit;

        typedef struct 
        { 
            struct _TCoreUnit* next; 
            TLengthType count; 
        } _TCoreInfo;

        typedef union
        {
            _TCoreInfo info;
            unsigned char	reserved[sizeOfBlock];
        }_TCoreHeader;

        typedef struct _TCoreUnit 
        {
            _TCoreHeader _U;
            unsigned char	   buffer[1];
        }TCoreUnit;

        TCoreUnit * freeList;
        TCoreUnit * usedList;

        unsigned char*	base;
        size_t			baseSize;
        static const size_t __sizeByType[];

    private:
        _TCoreUnit* _getCoreUnit(const void* pDataPtr)const
        {
            if( NULL == pDataPtr )
            {
                return NULL;
            }
            else
            {
                return (_TCoreUnit*)((unsigned char*)pDataPtr - sizeof(_TCoreHeader));
            }
        }

        unsigned char * _getData(const _TCoreUnit* pCoreUnit)const
        {
            return pCoreUnit ? (unsigned char*)&pCoreUnit->buffer[0] : NULL;
        }

        _TCoreUnit* _getNext(const _TCoreUnit* pCoreUnit)const
        {
            return pCoreUnit ? pCoreUnit->_U.info.next : NULL;
        }

        void _setNext(_TCoreUnit* pCurrent, _TCoreUnit* pNext)
        {
            assert ( NULL != pCurrent);
            pCurrent->_U.info.next = pNext;
            return;
        }

        TLengthType	_getLength(const _TCoreUnit* pCoreUnit)const
        {
            return pCoreUnit ? pCoreUnit->_U.info.count : 0;
        }

        void _setLength(_TCoreUnit* pCoreUnit, const TLengthType& newLength)
        {
            assert (NULL != pCoreUnit);
            pCoreUnit->_U.info.count = newLength;
            return;
        }

        _TCoreUnit* _createBlock(unsigned char* pStart, unsigned char* pEnd)
        {
            assert( pEnd - pStart >= 2 * sizeOfBlock );
            assert( 0 == (pEnd - pStart) % sizeOfBlock );

            _TCoreUnit* _pResult = (_TCoreUnit*)pStart;
            assert( 0 == (pEnd - _getData(_pResult)) % sizeOfBlock );
            _setLength( _pResult, (pEnd - _getData(_pResult)) / sizeOfBlock );
            _setNext( _pResult, NULL );

            return _pResult;
        }

        _TCoreUnit* _mergeBlocks(_TCoreUnit* pFirst, _TCoreUnit* pSecond)
        {
            assert( pFirst && pSecond && pFirst->_U.info.next == pSecond );

            unsigned char * _pEnd = _getData(pSecond) + _getLength(pSecond) * sizeOfBlock;

            assert ( 0 == (_pEnd - (unsigned char*)pFirst) % sizeOfBlock );
            assert ( 0 == (_pEnd - _getData(pFirst)) % sizeOfBlock );

            _setNext(pFirst, _getNext(pSecond));
            _setLength(pFirst, static_cast<TLengthType>((_pEnd - _getData(pFirst)) / sizeOfBlock));

            return pFirst;
        }

        void _used2free(_TCoreUnit* pPrev, _TCoreUnit* pBlock)
        {
            assert( _getNext(pPrev) == pBlock || ( NULL == pPrev && pBlock == usedList ) );

            if ( pPrev )
            {
                _setNext(pPrev, _getNext(pBlock));
            }
            else
            {
                usedList = _getNext(pBlock);
            }

            _setNext(pBlock, NULL);

            _TCoreUnit*	_pPrev = freeList;
            _TCoreUnit* _pNext = NULL;

            if ( freeList > pBlock )
            {
                _pPrev = NULL;
                _pNext = freeList;
            }
            else
            {
                while ( _getNext(_pPrev) && _getNext(_pPrev) < pBlock )
                {
                    _pPrev = _getNext(_pPrev);
                }
                _pNext = _getNext(_pPrev);
            }

            _setNext(pBlock, _pNext);

            if ( NULL != _pPrev )
            {
                _setNext(_pPrev, pBlock);
            }
            else
            {
                freeList = pBlock;
            }

            // search in used list
            const _TCoreUnit* _pUsed = usedList;
            if ( _pPrev )
            {
                while ( _pUsed && _pUsed < _pPrev )
                {
                    _pUsed = _getNext(_pUsed);
                }

                if ( _pUsed > _pPrev && _pUsed > pBlock )	// no blocks between prev and current nodes
                {
                    pBlock = _mergeBlocks( _pPrev, pBlock );
                }
            }

            if ( _pNext )
            {
               while ( _pUsed && _pUsed < pBlock )
                {
                    _pUsed = _getNext(_pUsed);
                }

                if ( NULL == _pUsed || (_pUsed > pBlock && _pUsed < _pNext ))	// no blocks between current and next nodes
                {
                    pBlock = _mergeBlocks( pBlock, _pNext );
                }
            }

            return;
        }

    public:
        explicit SimpleMemory(size_t total):
        freeList(NULL),
            usedList(NULL),
            base(NULL),
            baseSize(0)
        {
            assert( sizeOfBlock >= sizeof(_TCoreInfo) );
            assert( total >= sizeof(_TCoreUnit*) + sizeof(TLengthType) + sizeOfBlock );

            // round size
            if (total % sizeOfBlock)
            {
                total = ( (total / sizeOfBlock) + 1 ) * sizeOfBlock;
            }

            base = (unsigned char*)malloc(total);
            baseSize = total;
            freeList = _createBlock(base, base + baseSize);

            return;
        }

        void* alloc(size_t size)
        {
            TLengthType _count = size / sizeOfBlock;
            if ( size % sizeOfBlock )
            {
                ++_count;
            }

            _TCoreUnit* _pBlk = freeList;
            _TCoreUnit* _pPrev = NULL;

            while ( _pBlk && _getLength(_pBlk) < _count )
            {
                _pPrev = _pBlk;
                _pBlk = _getNext(_pBlk);
            }

            if (!_pBlk)
            {
                return NULL;
            }
            else
            {
                _TCoreUnit* _pResult = _pBlk;

                if ( _count == _getLength(_pBlk) || _count + 1 == _getLength(_pBlk) )
                {
                    if (_pPrev)
                    {
                        _setNext(_pPrev, _getNext(_pBlk));
                    }
                    else
                    {
                        freeList = _getNext(_pBlk);
                    }
                }
                else // split the block
                {
                    _pBlk = _createBlock( _getData(_pResult) + _count * sizeOfBlock, 
                        _getData(_pResult) + _getLength(_pResult) * sizeOfBlock );

                    _setNext(_pBlk, _getNext(_pResult));

                    _setLength(_pResult, _count);

                    if (_pPrev)
                    {
                        _setNext(_pPrev, _pBlk);
                    }
                    else
                    {
                        freeList = _pBlk;
                    }
                }

                _setNext(_pResult, NULL);

                // add _pResult into used list
                if (usedList)
                {
                    _pBlk = usedList;
                    _pPrev = NULL;

                    while(_pBlk && _pBlk < _pResult)
                    {
                        _pPrev = _pBlk;
                        _pBlk = _getNext(_pBlk);
                    }

                    if (_pPrev)
                    {
                        _setNext(_pResult, _getNext(_pPrev));
                        _setNext(_pPrev, _pResult);
                    }
                    else
                    {
                        _setNext(_pResult, _getNext(usedList));
                        usedList = _pResult;
                    }
                }
                else
                {
                    usedList = _pResult;
                }

                return _getData(_pResult);
            }
        }

        void free(void* ptr)
        {
            _TCoreUnit* _pBlk = usedList;
            _TCoreUnit* _pPrev = NULL;

            while ( _pBlk && _getData(_pBlk) < ptr )
            {
                _pPrev = _pBlk;
                _pBlk = _getNext(_pBlk);
            }

            if ( _pBlk && _getData(_pBlk) == ptr )
            {
                _used2free( _pPrev, _pBlk );;
            }

            return;
        }

        void* realloc(size_t newSize)
        {
            assert ( false );
            return NULL;
        }

        virtual ~SimpleMemory(void)
        {
            if ( base )
            {
                free(base);
            }

            return;
        }

        // for debug
        void dumpList(const _TCoreUnit* lst)const
        {
            using namespace std;

            clog << "[block address][count][buffer address][next block]" << endl;

            clog << hex;	
            while ( lst )
            {
                clog << "[" << (unsigned int)lst << "][" << _getLength(lst) << "][" << (unsigned int)_getData(lst) << "][";
                if ( _getNext(lst) )
                {
                    clog << (unsigned int)_getNext(lst);
                }
                else
                {
                    clog << "NULL";
                }
                clog << "]" << endl;

                lst = _getNext(lst);
            } // while 

            clog << endl;
        }

        void dumpFreeList(void)const
        {
            using namespace std;
            clog << "-------------- dump free list --------------" << endl;
            dumpList( freeList );
            clog << "============================================" << endl;
        }

        void dumpUsedList(void)const
        {
            using namespace std;
            clog << "-------------- dump used list --------------" << endl;
            dumpList( usedList );
            clog << "============================================" << endl;
        }

        void dumpAll(void)const
        {
            dumpFreeList();
            dumpUsedList();
            return;
        }

    }; // template SimpleMemory

    typedef SimpleMemory<16, unsigned short> SimpleMemory16;
    typedef SimpleMemory<32, unsigned short> SimpleMemory32;
#endif
    
}; // namespace dj3d

#endif // __DJ3D_UTILITY_H__
