//
//  source/math/3d.cpp
//  dj3d
//
//  Created by Xu ZhiXin on 12/31/11.
//  Copyright (c) 2011 doojava. All rights reserved.
//

#include "3d.h"
#include <math.h>
#include <iostream>
// _A.col.row
#ifndef ROW_MAJOR_ORDER

#define _A11    0
#define _A12    1
#define _A13    2
#define _A14    3
#define _A21    4
#define _A22    5
#define _A23    6
#define _A24    7
#define _A31    8
#define _A32    9
#define _A33    10
#define _A34    11
#define _A41    12
#define _A42    13
#define _A43    14
#define _A44    15

#else

#define _A11    0
#define _A21    1
#define _A31    2
#define _A41    3
#define _A12    4
#define _A22    5
#define _A32    6
#define _A42    7
#define _A13    8
#define _A23    9
#define _A33    10
#define _A43    11
#define _A14    12
#define _A24    13
#define _A34    14
#define _A44    15

#endif

namespace dj3d
{
    bool EQ(const f32 a, const f32 b)
    { 
        return !(a < b) && !(a > b);
    }

    // Matrix 
    Matrix::Matrix()
    {
        m_mat[_A11] = ONE; m_mat[_A21] = ZERO; m_mat[_A31] = ZERO; m_mat[_A41] = ZERO;
        m_mat[_A12] = ZERO; m_mat[_A22] = ONE; m_mat[_A32] = ZERO; m_mat[_A42] = ZERO;
        m_mat[_A13] = ZERO; m_mat[_A23] = ZERO; m_mat[_A33] = ONE; m_mat[_A43] = ZERO;
        m_mat[_A14] = ZERO; m_mat[_A24] = ZERO; m_mat[_A34] = ZERO; m_mat[_A44] = ONE;

        return;
    }

    Matrix::Matrix( const Matrix& other )
    {
        memcpy(m_mat, other.m_mat, sizeof(m_mat));
        return;
    }

    Matrix::Matrix( f32 _a, f32 _b, f32 _c, f32 _d, 
        f32 _e, f32 _f, f32 _g, f32 _h, 
        f32 _i, f32 _j, f32 _k, f32 _l, 
        f32 _m, f32 _n, f32 _o, f32 _p )
    {
        m_mat[0] = _a; m_mat[4] = _e; m_mat[8] = _i; m_mat[12] = _m;
        m_mat[1] = _b; m_mat[5] = _f; m_mat[9] = _j; m_mat[13] = _n;
        m_mat[2] = _c; m_mat[6] = _g; m_mat[10] = _k; m_mat[14] = _o;
        m_mat[3] = _d; m_mat[7] = _h; m_mat[11] = _l; m_mat[15] = _p;

        return;
    }

    Matrix::Matrix( const f32* _buf )
    {
        memcpy(m_mat, _buf, sizeof(m_mat));
        return;
    }
    
    Matrix::Matrix( f32 _dx, f32 _dy, f32 _dz )
    {
        m_mat[_A11] = ONE; m_mat[_A21] = ZERO; m_mat[_A31] = ZERO; m_mat[_A41] = _dx;
        m_mat[_A12] = ZERO; m_mat[_A22] = ONE; m_mat[_A32] = ZERO; m_mat[_A42] = _dy;
        m_mat[_A13] = ZERO; m_mat[_A23] = ZERO; m_mat[_A33] = ONE; m_mat[_A43] = _dz;
        m_mat[_A14] = ZERO; m_mat[_A24] = ZERO; m_mat[_A34] = ZERO; m_mat[_A44] = ONE;

        return;
    }

    Matrix::Matrix( const Vector& axis_identity, f32 radian )
    {
        f32 _cos = cosf(radian);
        f32 _sin = sinf(radian);
        f32 _x = axis_identity.m_v[0];
        f32 _y = axis_identity.m_v[1];
        f32 _z = axis_identity.m_v[2];

        m_mat[_A11] = _x * _x * (ONE - _cos) + _cos; 
        m_mat[_A21] = _x * _y * (ONE - _cos) + _z * _sin; 
        m_mat[_A31] = _x * _z * (ONE - _cos) - _y * _sin;  
        m_mat[_A41] = ZERO;
        m_mat[_A12] = _x * _y * (ONE - _cos) - _z * _sin; 
        m_mat[_A22] = _y * _y * (ONE - _cos) + _cos; 
        m_mat[_A32] = _y * _z * (ONE - _cos) + _x * _sin;  
        m_mat[_A42] = ZERO;
        m_mat[_A13] = _x * _z * (ONE - _cos) + _y * _sin; 
        m_mat[_A23] = _y * _z * (ONE - _cos) - _x * _sin; 
        m_mat[_A33] = _z * _z * (ONE - _cos) + _cos; 
        m_mat[_A43] = ZERO;
        m_mat[_A14] = ZERO; m_mat[_A24] = ZERO; m_mat[_A34] = ZERO; m_mat[_A44] = ONE;

        return;
    }

    Matrix& Matrix::operator = ( const Matrix& other )
    {
        if ( &other != this )
        {
            memcpy(m_mat, other.m_mat, sizeof(m_mat));
        }

        return *this;
    }

    Matrix Matrix::operator * ( const Matrix& other )const
    {
        return Matrix(
            m_mat[_A11] * other.m_mat[_A11] + m_mat[_A12] * other.m_mat[_A21] + m_mat[_A13] * other.m_mat[_A31] + m_mat[_A14] * other.m_mat[_A41],
            m_mat[_A21] * other.m_mat[_A11] + m_mat[_A22] * other.m_mat[_A21] + m_mat[_A23] * other.m_mat[_A31] + m_mat[_A24] * other.m_mat[_A41],
            m_mat[_A31] * other.m_mat[_A11] + m_mat[_A32] * other.m_mat[_A21] + m_mat[_A33] * other.m_mat[_A31] + m_mat[_A34] * other.m_mat[_A41],
            m_mat[_A41] * other.m_mat[_A11] + m_mat[_A42] * other.m_mat[_A21] + m_mat[_A43] * other.m_mat[_A31] + m_mat[_A44] * other.m_mat[_A41],

            m_mat[_A11] * other.m_mat[_A12] + m_mat[_A12] * other.m_mat[_A22] + m_mat[_A13] * other.m_mat[_A32] + m_mat[_A14] * other.m_mat[_A42],
            m_mat[_A21] * other.m_mat[_A12] + m_mat[_A22] * other.m_mat[_A22] + m_mat[_A23] * other.m_mat[_A32] + m_mat[_A24] * other.m_mat[_A42],
            m_mat[_A31] * other.m_mat[_A12] + m_mat[_A32] * other.m_mat[_A22] + m_mat[_A33] * other.m_mat[_A32] + m_mat[_A34] * other.m_mat[_A42],
            m_mat[_A41] * other.m_mat[_A12] + m_mat[_A42] * other.m_mat[_A22] + m_mat[_A43] * other.m_mat[_A32] + m_mat[_A44] * other.m_mat[_A42],

            m_mat[_A11] * other.m_mat[_A13] + m_mat[_A12] * other.m_mat[_A23] + m_mat[_A13] * other.m_mat[_A33] + m_mat[_A14] * other.m_mat[_A43],
            m_mat[_A21] * other.m_mat[_A13] + m_mat[_A22] * other.m_mat[_A23] + m_mat[_A23] * other.m_mat[_A33] + m_mat[_A24] * other.m_mat[_A43],
            m_mat[_A31] * other.m_mat[_A13] + m_mat[_A32] * other.m_mat[_A23] + m_mat[_A33] * other.m_mat[_A33] + m_mat[_A34] * other.m_mat[_A43],
            m_mat[_A41] * other.m_mat[_A13] + m_mat[_A42] * other.m_mat[_A23] + m_mat[_A43] * other.m_mat[_A33] + m_mat[_A44] * other.m_mat[_A43],

            m_mat[_A11] * other.m_mat[_A14] + m_mat[_A12] * other.m_mat[_A24] + m_mat[_A13] * other.m_mat[_A34] + m_mat[_A14] * other.m_mat[_A44],
            m_mat[_A21] * other.m_mat[_A14] + m_mat[_A22] * other.m_mat[_A24] + m_mat[_A23] * other.m_mat[_A34] + m_mat[_A24] * other.m_mat[_A44],
            m_mat[_A31] * other.m_mat[_A14] + m_mat[_A32] * other.m_mat[_A24] + m_mat[_A33] * other.m_mat[_A34] + m_mat[_A34] * other.m_mat[_A44],
            m_mat[_A41] * other.m_mat[_A14] + m_mat[_A42] * other.m_mat[_A24] + m_mat[_A43] * other.m_mat[_A34] + m_mat[_A44] * other.m_mat[_A44]                      
        );
    }

    Matrix& Matrix::operator *= ( const Matrix& other )
    {
        *this = this->operator*(other);
        return *this;
    }

    Matrix& Matrix::operator *= ( const f32 factor )
    {
        m_mat[_A11] *= factor; m_mat[_A21] *= factor; m_mat[_A31] *= factor; m_mat[_A41] *= factor;
        m_mat[_A12] *= factor; m_mat[_A22] *= factor; m_mat[_A32] *= factor; m_mat[_A42] *= factor;
        m_mat[_A13] *= factor; m_mat[_A23] *= factor; m_mat[_A33] *= factor; m_mat[_A43] *= factor;
        m_mat[_A14] *= factor; m_mat[_A24] *= factor; m_mat[_A34] *= factor; m_mat[_A44] *= factor;

        return *this;
    }

    Matrix::operator f32*(void)
    {
        return m_mat;
    }

    Matrix::operator const f32* (void)const
    {
        return m_mat;
    }

    Matrix& Matrix::identity(void)
    {
        m_mat[_A11] = ONE; m_mat[_A21] = ZERO; m_mat[_A31] = ZERO;  m_mat[_A41] = ZERO;
        m_mat[_A12] = ZERO; m_mat[_A22] = ONE; m_mat[_A32] = ZERO;  m_mat[_A42] = ZERO;
        m_mat[_A13] = ZERO; m_mat[_A23] = ZERO; m_mat[_A33] = ONE; m_mat[_A43] = ZERO;
        m_mat[_A14] = ZERO; m_mat[_A24] = ZERO; m_mat[_A34] = ZERO; m_mat[_A44] = ONE;

        return *this;
    }

    Matrix Matrix::operator!(void)const
    {
        Matrix _m( cofactor(0, 0), cofactor(1, 0), cofactor(2, 0), cofactor(3, 0),
            cofactor(0, 1), cofactor(1, 1), cofactor(2, 1), cofactor(3, 1),
            cofactor(0, 2), cofactor(1, 2), cofactor(2, 2), cofactor(3, 2),
            cofactor(0, 3), cofactor(1, 3), cofactor(2, 3), cofactor(3, 3) );

        f32 _det = m_mat[_A11] * _m.m_mat[0] - m_mat[_A12]*_m.m_mat[4] + m_mat[_A13]*_m.m_mat[8] - m_mat[_A14]*_m.m_mat[12];

        if ( EQ( _det, ZERO ) )
        {
            throw InverseMatrixException("");
        }

        _det = 1.0f / _det;
        _m *= _det; 
        return _m;
    }

     f32  Matrix::cofactor(const u32 col, const u32 row)const
     {
         static const u32 _subscript[4][4] = {
            {_A11, _A12, _A13, _A14},  
            {_A21, _A22, _A23, _A24},
            {_A31, _A32, _A33, _A34}, 
            {_A41, _A42, _A43, _A44}
         };

         assert ( col <4  && row < 4);
         
         f32 _array[9];
         u32 _index = 0;
         for (u32 c = 0; c < 4; ++c)
         {
             if ( col == c ) continue;
             for(u32 r=0; r<4; ++r )
             {
                 if (row == r) continue;
                 _array[_index++] = m_mat[_subscript[c][r]];
             }
         }
         
         return ((col+row)%2 ? -1 : 1) * (_array[0]*_array[4]*_array[8] 
                                    - _array[0]*_array[5]*_array[7] 
                                    -_array[1]*_array[3]*_array[8] 
                                    + _array[3]*_array[2]*_array[7] 
                                    + _array[6]*_array[1]*_array[5] 
                                    - _array[6]*_array[2]*_array[4]); 
     }

     Matrix& Matrix::transpose(void)
     {
         f32 _tmp;

         _tmp = m_mat[_A12]; m_mat[_A12] = m_mat[_A21]; m_mat[_A21] = _tmp;
         _tmp = m_mat[_A13]; m_mat[_A13] = m_mat[_A31]; m_mat[_A31] = _tmp;
         _tmp = m_mat[_A14]; m_mat[_A14] = m_mat[_A41]; m_mat[_A41] = _tmp;
         _tmp = m_mat[_A23]; m_mat[_A23] = m_mat[_A32]; m_mat[_A32] = _tmp;
         _tmp = m_mat[_A24]; m_mat[_A24] = m_mat[_A42]; m_mat[_A42] = _tmp;
         _tmp = m_mat[_A34]; m_mat[_A34] = m_mat[_A43]; m_mat[_A43] = _tmp;

         return *this;
     }
    // Vertex

    const Vertex Vertex::zero(ZERO, ZERO, ZERO);

    Vertex::Vertex()
    {
        m_v[0] = ZERO;
        m_v[1] = ZERO;
        m_v[2] = ZERO;
        return;
    }

    Vertex::Vertex( const Vertex& other )
    {
        m_v[0] = other.m_v[0];
        m_v[1] = other.m_v[1];
        m_v[2] = other.m_v[2];
        return;
    }

    Vertex::Vertex( f32 _x, f32 _y, f32 _z )
    {
        m_v[0] = _x;
        m_v[1] = _y;
        m_v[2] = _z;
        return;
    }

    Vertex& Vertex::operator = ( const Vertex& other )
    {
        if ( &other != this )
        {
            m_v[0] = other.m_v[0];
            m_v[1] = other.m_v[1];
            m_v[2] = other.m_v[2];
        }
        return *this;
    }

    Vertex Vertex::operator * ( const Matrix& mat )const
    {
        f32 _w = m_v[0] * mat.m_mat[_A41] + m_v[1] * mat.m_mat[_A42] + m_v[2] * mat.m_mat[_A43] + mat.m_mat[_A44];
        if ( EQ(_w, ZERO) )
        {
            throw OverflowException("");
        }
        else
        {
            _w = ONE / _w;
        }

        return Vertex( ( m_v[0] * mat.m_mat[_A11] + m_v[1] * mat.m_mat[_A12] + m_v[2] * mat.m_mat[_A13]+ mat.m_mat[_A14] ) * _w,
            ( m_v[0] * mat.m_mat[_A21] + m_v[1] * mat.m_mat[_A22] + m_v[2] * mat.m_mat[_A23]+ mat.m_mat[_A24] ) * _w,
            ( m_v[0] * mat.m_mat[_A31] + m_v[1] * mat.m_mat[_A32] + m_v[2] * mat.m_mat[_A33]+ mat.m_mat[_A34] ) * _w );
    }

    Vertex& Vertex::operator *= ( const Matrix& mat )
    {
        *this = this->operator*(mat);
        return *this;
    }

    Vertex Vertex::operator + ( const Vector& v )const
    {
        return Vertex( m_v[0] + v.m_v[0], m_v[1] + v.m_v[1], m_v[2] + v.m_v[2] );
    }

    Vertex& Vertex::operator += ( const Vector& v)
    {
        m_v[0] += v.m_v[0];
        m_v[1] += v.m_v[1];
        m_v[2] += v.m_v[2];

        return *this;
    }

    Vector Vertex::operator - ( const Vertex& other )const
    {
        return Vector( other, *this );
    }

    const f32& Vertex::x(void)const
    {
        return m_v[0];
    }

    f32& Vertex::x(void)
    {
        return m_v[0];
    }

    const f32& Vertex::y(void)const
    {
        return m_v[1];
    }

    f32& Vertex::y(void)
    {
        return m_v[1];
    }

    const f32& Vertex::z(void)const
    {
        return m_v[2];
    }

    f32& Vertex::z(void)
    {
        return m_v[2];
    }

    // Vector 
    const Vector Vector::xIdentity(ONE, ZERO, ZERO);
    const Vector Vector::yIdentity(ZERO, ONE, ZERO);
    const Vector Vector::zIdentity(ZERO, ZERO, ONE);

    Vector::Vector()
    {
        return;
    }

    Vector::Vector( f32 _x, f32 _y, f32 _z ):
    Vertex( _x, _y, _z )
    {
        return;
    }

    Vector::Vector( const Vertex& other ):
    Vertex( static_cast<Vertex>(other) )
    {
        return;
    }

    Vector::Vector( const Vertex& from, const Vector& to )
    {
        m_v[0] = to.m_v[0] - from.m_v[0];
        m_v[1] = to.m_v[1] - from.m_v[1];
        m_v[2] = to.m_v[2] - from.m_v[2];

        return;
    }

    Vector& Vector::operator = ( const Vector& other )
    {
        if ( &other != this )
        {
            m_v[0] = other.m_v[0];
            m_v[1] = other.m_v[1];
            m_v[2] = other.m_v[2];
        }

        return *this;
    }

    Vector Vector::operator-(void)const
    {
        return Vector( -m_v[0], -m_v[1], -m_v[2] );
    }

    Vector Vector::operator + ( const Vector& other )
    {
        return Vector( m_v[0] + other.m_v[0] , m_v[1] + other.m_v[1], m_v[2] + other.m_v[2] );
    }

    Vector& Vector::operator += ( const Vector& other )
    {
        m_v[0] += other.m_v[0];
        m_v[1] += other.m_v[1];
        m_v[2] += other.m_v[2];

        return *this;
    }

    Vector Vector::cross( const Vector& other )const 
    {
        // a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x
        return Vector( m_v[1] * other.m_v[2] - m_v[2] * other.m_v[1], m_v[2] * other.m_v[0] - m_v[0] * other.m_v[2], m_v[0] * other.m_v[1] - m_v[1] * other.m_v[0] );
    }

    f32 Vector::dot( const Vector& other )const 
    {
        return m_v[0] * other.m_v[0] + m_v[1] * other.m_v[1] + m_v[2] * other.m_v[2];
    }

    f32 Vector::distance()const
    {
        return static_cast<f32>(sqrt( m_v[0]*m_v[0] + m_v[1]*m_v[1] + m_v[2]*m_v[2]));
    }

    Vector& Vector::identity(void)
    {
        f32 _d = distance();
        if ( !EQ( _d, ZERO ) )
        {
            _d = ONE/_d;

            m_v[0] *= _d;
            m_v[1] *= _d;
            m_v[2] *= _d;
        }
        else
        {
            throw OverflowException("");
        }

        return *this;
    }

    Vector Vector::operator * ( const f32 val )const
    {
        return Vector( m_v[0] * val, m_v[1] * val, m_v[2] * val );
    }

    TriangleCore::TriangleCore( const Vertex& A, const Vertex& B, const Vertex& C):
    m_A( A ),
        m_B( B ),
        m_C( C )
    {
        return;
    }

    TriangleCore::TriangleCore( const TriangleCore & other):
    m_A( other.m_A ),
        m_B( other.m_B ),
        m_C( other.m_C )
    {
        return;
    }

    TriangleCore TriangleCore::operator = ( const TriangleCore& other)
    {
        if (&other != this )
        {
            m_A = other.m_A;
            m_B = other.m_B;
            m_C = other.m_C;
        }

        return *this;
    }

    Vector TriangleCore::normalVector(void)const
    {
        return Vector(m_A, m_B).cross(Vector(m_B, m_C)).identity();
    }

     TriangleCore& TriangleCore::operator *= ( const Matrix& mat )
     {
         m_A *= mat;
         m_B *= mat;
         m_C *= mat;
         return *this;
     }
       
     TriangleCore TriangleCore::operator * (const Matrix& mat) const
     {
         return TriangleCore( m_A * mat, m_B * mat, m_C * mat );
     }

    bool TriangleCore::hitTest( const Vertex& origin, const Vector& direction,
        f32& distance )const
    {
        f32 u,v;
        Vector _E1( m_A, m_B );
        Vector _E2( m_A, m_C );
        Vector _P = direction.cross(_E2);

        f32 _fdet = _E1.dot(_P);
        Vector _T;
        if ( _fdet > ZERO )
        {
            _T = origin - m_A;

        }
        else 
        {
            _T = m_A - origin;
            _fdet = -_fdet;
        }

        if ( _fdet < PRECISION )
        {
            return false;
        }

        u = _T.dot(_P);
        if ( u < ZERO || u > _fdet )
        {
            return false;
        }

        Vector _Q( _T.cross(_E1) );
        v = direction.dot(_Q);
        if ( v < ZERO || u + v > _fdet )
        {
            return false;
        }

        distance = _E2.dot(_Q);

        _fdet = ONE / _fdet;

        // u *= _fdet;
        // v *= _fdet;
        distance *= _fdet;

        return true;
    }

    Triangle::Triangle(std::vector<u32>& indices, u32 indexA, u32 indexB, u32 indexC):
    m_base( indices.size() )
    {
        indices.push_back(indexA);
        indices.push_back(indexB);
        indices.push_back(indexC);

        return;
    }

    Triangle::Triangle( const Triangle& other ):
    m_base(other.m_base)
    {
        return;
    }

    Triangle& Triangle::operator = ( const Triangle& other )
    {
        if ( &other != this )
        {
            m_base = other.m_base;
        }
        return *this;
    }

    TriangleCore Triangle::getTriangleCore(const std::vector<Vertex>& vertexes, const std::vector<u32>& indices, size_t index)const
    {
        assert( m_base < indices.size() );
        assert( index == 1 );
        assert( indices[m_base] < vertexes.size() );
        assert( indices[m_base + 1] < vertexes.size() );
        assert( indices[m_base + 2] < vertexes.size() );

        return TriangleCore( vertexes[indices[m_base]],
            vertexes[indices[m_base + 1]],
            vertexes[indices[m_base + 2]]); 
    }

    Triangle::~Triangle(void)
    {
        return;
    }

    TriangleStrip::TriangleStrip( std::vector<u32>& indices, u32 indexV0, u32 indexV1 ):
    m_begin( indices.size() ),
        m_end( 0 )
    {
        assert( indexV0 + 2 <= indexV1 );

        for ( u32 i = indexV0; i <= indexV1; ++ i )
        {
            indices.push_back( i );
        }

        m_end = indices.size();

        assert( static_cast<u32>(m_end - m_begin) == indexV1 - indexV0 + 1 );

        return;
    }

    TriangleStrip::TriangleStrip( const TriangleStrip& other ):
    m_begin( other.m_begin),
        m_end( other.m_end )
    {
        return;
    }

    TriangleStrip& TriangleStrip::operator = ( const TriangleStrip& other )
    {
        if ( &other != this )
        {
            m_begin = other.m_begin;
            m_end = other.m_end;
        }
        return *this;
    }

    TriangleCore TriangleStrip::getTriangleCore( const std::vector<Vertex>& vertexes, const std::vector<u32>& indices, size_t index )const
    {
        assert ( m_begin + index + 2 < m_end );
        assert ( m_end <= indices.size() );
        assert ( indices[m_begin + index] < vertexes.size() );
        assert ( indices[m_begin + index + 1] < vertexes.size() );
        assert ( indices[m_begin + index + 2] < vertexes.size() );

        // odd: [n, n+1, n+2]
        // even: [n+1, n, n+2], (n >= 1)
        if ( index + 1 % 2 )
        {
            return TriangleCore( vertexes[indices[m_begin + index]],
                vertexes[indices[m_begin + index + 1]],
                vertexes[indices[m_begin + index + 2]] );

        }
        else
        {
            return TriangleCore( vertexes[indices[m_begin + index + 1]],
                vertexes[indices[m_begin + index]],
                vertexes[indices[m_begin + index + 2]] );
        }
    }

    TriangleStrip::~TriangleStrip( void )
    {
        return;
    }

    Color::Color(void)
    {
        m_color.color.red = ZERO;
        m_color.color.green = ZERO;
        m_color.color.blue = ZERO;
        m_color.color.alpha = ONE;

        return;
    }

    Color::Color( f32 red, f32 green, f32 blue, f32 alpha )
    {
        m_color.color.red = red<ZERO ? ZERO : (red > ONE ? ONE : red);
        m_color.color.green = green<ZERO ? ZERO : (green > ONE ? ONE : green);
        m_color.color.blue = blue<ZERO ? ZERO : (blue > ONE ? ONE : blue);;
        m_color.color.alpha = alpha<ZERO ? ZERO : (alpha > ONE ? ONE : alpha);;

        return;
    }

    Color::Color( u8 red, u8 green, u8 blue, u8 alpha )
    {
        m_color.color.red = toFloat(red);
        m_color.color.green = toFloat(green);
        m_color.color.blue = toFloat(blue);
        m_color.color.alpha = toFloat(alpha);
        return;
    }

    Color::Color( const Color& other )
    {
        m_color.color.red = other.m_color.color.red;
        m_color.color.green = other.m_color.color.green;
        m_color.color.blue = other.m_color.color.blue;
        m_color.color.alpha = other.m_color.color.alpha;

        return;
    }

    Color& Color::operator = (const Color& other )
    {
        if ( &other != this )
        {
            m_color.color.red = other.m_color.color.red;
            m_color.color.green = other.m_color.color.green;
            m_color.color.blue = other.m_color.color.blue;
            m_color.color.alpha = other.m_color.color.alpha;
        }

        return *this;
    }

    bool Color::operator < ( const Color & other )const
    {
        if ( m_color.color.red < other.m_color.color.red )
        {
            return true;
        }
        else if ( m_color.color.red > other.m_color.color.red )
        {
            return false;
        }
        else if ( m_color.color.green < other.m_color.color.green )
        {
            return true;
        }
        else if ( m_color.color.green > other.m_color.color.green )
        {
            return false;
        }
        else if ( m_color.color.blue < other.m_color.color.blue )
        {
            return true;
        }
        else if ( m_color.color.blue > other.m_color.color.blue )
        {
            return false;
        }
        else
        {
            return m_color.color.alpha < other.m_color.color.alpha;
        }
    }

    bool Color::operator > ( const Color & other )const
    {
        if ( m_color.color.red < other.m_color.color.red )
        {
            return false;
        }
        else if ( m_color.color.red > other.m_color.color.red )
        {
            return true;
        }
        else if ( m_color.color.green < other.m_color.color.green )
        {
            return false;
        }
        else if ( m_color.color.green > other.m_color.color.green )
        {
            return true;
        }
        else if ( m_color.color.blue < other.m_color.color.blue )
        {
            return false;
        }
        else if ( m_color.color.blue > other.m_color.color.blue )
        {
            return true;
        }
        else
        {
            return m_color.color.alpha > other.m_color.color.alpha;
        }
    }

    bool Color::operator == ( const Color& other) const
    {
        return ( EQ(m_color.color.red, other.m_color.color.red ) 
            && EQ( m_color.color.green, other.m_color.color.green ) 
            && EQ( m_color.color.blue, other.m_color.color.blue )
            && EQ( m_color.color.alpha, other.m_color.color.alpha ) );
    }

    bool Color::operator != ( const Color& other) const
    {
        return  ( !EQ(m_color.color.red, other.m_color.color.red ) 
            || !EQ( m_color.color.green, other.m_color.color.green ) 
            || !EQ( m_color.color.blue, other.m_color.color.blue )
            || !EQ( m_color.color.alpha, other.m_color.color.alpha ) );
    }

    u8 Color::toInteger( const f32 val )
    {
        if ( val < ZERO )
        {
            return (u8)0;
        }
        else if ( val > ONE )
        {
            return (u8)255;
        }
        else
        {
            return (u8)(255 * val); 
        }
    }

    f32 Color::toFloat( const u8 uval )
    {
        return (f32)uval * 0.00392f; // uval / 255.0f
    }

};// namespace dj3d

std::ostream& operator<<( std::ostream& stream, const dj3d::Matrix& mat)
{
    using namespace std;
    stream << "----------------------------------------------" <<endl;
    stream << mat.m_mat[_A11] << '\t' << mat.m_mat[_A21] << '\t' << mat.m_mat[_A31] << '\t' << mat.m_mat[_A41] << endl;
    stream << mat.m_mat[_A12] << '\t' << mat.m_mat[_A22] << '\t' << mat.m_mat[_A32] << '\t' << mat.m_mat[_A42] << endl;
    stream << mat.m_mat[_A13] << '\t' << mat.m_mat[_A23] << '\t' << mat.m_mat[_A33] << '\t' << mat.m_mat[_A43] << endl;
    stream << mat.m_mat[_A14] << '\t' << mat.m_mat[_A24] << '\t' << mat.m_mat[_A34] << '\t' << mat.m_mat[_A44] << endl;
    stream << "----------------------------------------------" <<endl;
    return stream;
}