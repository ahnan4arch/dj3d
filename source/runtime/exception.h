//
//  source/runtime/exception.h
//  dj3d
//
//  Created by Xu ZhiXin on 1/1/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//

#ifndef __DJ3D_RUNTIME_EXCEPTION_H__
#define __DJ3D_RUNTIME_EXCEPTION_H__

#include <string>
#include <assert.h>

namespace dj3d 
{
    class Exception
    {
    protected:
        std::string m_strMsg;
    public:
        explicit Exception( const std::string& strMsg );
        operator std::string&(void);
        operator const std::string&(void)const;
        virtual ~Exception( void );
    }; // class Exception

    class OverflowException : public Exception
    {
    public:
        explicit OverflowException( const std::string& strMsg );
    }; // class OverflowException

    class InverseMatrixException : public Exception
    {
    public:
        explicit InverseMatrixException( const std::string& strMsg );
    }; // class InverseMatrixException

    class InvalidIndexException : public Exception
    {
    public:
        explicit InvalidIndexException( const std::string& strMsg );
    }; // class InvalidIndexException

    class UnknownBlendOpCode : public Exception
    {
    public:
        explicit UnknownBlendOpCode( const std::string& strMsg );
    }; //class UnknownBlendOpCode

    class InvalidLightIdException: public Exception
    {
    public:
        explicit InvalidLightIdException( const std::string& strMsg );
    }; // class InvalidLightIdException

    class DuplicatedTextureId : public Exception
    {
    public:
        explicit DuplicatedTextureId( const std::string& strMsg );
    }; // class DuplicatedTextureId 

    class DuplicatedMaterialId : public Exception
    {
    public:
        explicit DuplicatedMaterialId( const std::string& strMsg );
    }; // class DuplicatedMaterialId
         
    class MemoryException : public Exception
    {
    public:
        explicit MemoryException( const std::string& strMsg );
    }; // class OverflowException

    class InvalidJpegFormat : Exception
    {
    public:
        InvalidJpegFormat( const std::string& strMsg );
    }; // class InvalidJpegFormat

    class InvalidPngFormat : Exception
    {
    public:
        InvalidPngFormat( const std::string& strMsg );
    }; // InvalidPngFormat

}; // namespace dj3d

#endif // __DJ3D_RUNTIME_EXCEPTION_H__
