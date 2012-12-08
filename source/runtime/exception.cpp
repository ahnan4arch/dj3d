//
//  source/runtime/exception.cpp
//  dj3d
//
//  Created by Xu ZhiXin on 1/1/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//

#include "exception.h"

namespace dj3d 
{
    Exception::Exception( const std::string& strMsg ):
    m_strMsg(strMsg)
    {
        return;
    }

    Exception::operator std::string&(void)
    {
        return m_strMsg;
    }

    Exception::operator const std::string&(void)const
    {
        return m_strMsg;
    }

    Exception::~Exception( void )
    {
        return;
    }

    OverflowException::OverflowException( const std::string& strMsg ):
    Exception( strMsg )
    {
        return;
    }

    InverseMatrixException::InverseMatrixException( const std::string& strMsg ):
    Exception( strMsg )
    {
        return;
    }

    InvalidIndexException::InvalidIndexException( const std::string& strMsg ):
    Exception( strMsg )
    {
        return;
    }

    UnknownBlendOpCode::UnknownBlendOpCode( const std::string& strMsg ):
    Exception( strMsg )
    {
        return;
    }

    InvalidLightIdException::InvalidLightIdException( const std::string& strMsg ):
    Exception( strMsg )
    {
        return;
    }
    
    MemoryException::MemoryException( const std::string& strMsg ):
    Exception( strMsg )
    {
        return;
    }

    InvalidJpegFormat::InvalidJpegFormat(const std::string& strMsg ):
    Exception( strMsg )
    {
        return;
    }

    InvalidPngFormat::InvalidPngFormat(const std::string& strMsg):
    Exception( strMsg )
    {
        return;
    }
}; // namespace dj3d
