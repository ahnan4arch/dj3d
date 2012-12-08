//
//  source/math/3d.h
//  dj3d
//
//  Created by Xu ZhiXin on 12/31/11.
//  Copyright (c) 2011 doojava. All rights reserved.
//

#ifndef __DJ3D_MATH_3D_H__
#define __DJ3D_MATH_3D_H__ 

#include <vector>
#include "runtime/exception.h"

#define PI 3.14f
// Angle to radian
#define A2R(x) ((x)*PI*0.0056f)

#define PRECISION   0.00001f
#define ZERO        0.0f
#define ONE         1.0f

namespace dj3d
{
    typedef float f32;
    typedef unsigned char u8;
    typedef int i32;
    typedef unsigned int u32;
    
    bool EQ(const f32 a, const f32 b);
    
    struct Vertex;
    struct Vector;
  
    /*
        column major order
        m0, m4, m8, m12,
        m1, m5, m9, m13,
        m2, m6, m10,m14,
        m3, m7, m11,m15
    */
    struct Matrix 
    {
    public:
        f32 m_mat[16];
    public:
        Matrix(); // by default, a matrix is initialized as an identity 
        Matrix( const Matrix& other );
        Matrix( f32 _a, f32 _b, f32 _c, f32 _d, // column 1
               f32 _e, f32 _f, f32 _g, f32 _h,  // column 2
               f32 _i, f32 _j, f32 _k, f32 _l,  // column 3
               f32 _m, f32 _n, f32 _o, f32 _p );// column 4
        Matrix( const f32* _buf );
        // for transform
        Matrix( f32 _dx, f32 _dy, f32 _dz );  
        // for rotation, pls make sure axis_identity is a identity vector
        Matrix( const Vector& axis_identity, f32 radian ); 
        Matrix& operator = ( const Matrix& other );
        Matrix operator * ( const Matrix& other )const;
        Matrix& operator *= ( const Matrix& other );
        Matrix& operator *= ( const f32 factor );
        operator f32* (void);
        operator const f32* (void)const;
        Matrix& identity(void);
        Matrix operator!(void)const;
        Matrix& transpose(void);
    private:
           f32 cofactor(const u32 col, const u32 row)const;
    }; // class Matrix
    
    // [x, y, z]
    struct Vertex
    {
    public:
        f32 m_v[3];
    public:
        Vertex();
        Vertex( const Vertex& other );
        Vertex( f32 _x, f32 _y, f32 _z );
        Vertex& operator = ( const Vertex& other );
        Vertex operator * ( const Matrix& other )const;
        Vertex& operator *= ( const Matrix& other );
        Vertex operator + ( const Vector& v )const;
        Vertex& operator += ( const Vector& v);
        Vector operator - ( const Vertex& other )const;
        const f32& x(void)const;
        f32& x(void);
        const f32& y(void)const;
        f32& y(void);
        const f32& z(void)const;
        f32& z(void);
        
        static const Vertex zero;
    };
    
    // [x, y, z]
    struct Vector : public Vertex
    {
    private:
        // forbid to multiple a matrix
        Vector operator * ( const Matrix& other )const;
        Vector& operator *= ( const Matrix& other );
        
    public:
        Vector();
        Vector( f32 _x, f32 _y, f32 _z );
        Vector( const Vertex& other );
        Vector( const Vertex& from, const Vector& to );
        Vector& operator = ( const Vector& other );
        Vector operator - (void)const;
        Vector operator + ( const Vector& other );
        Vector& operator += ( const Vector& other );
        Vector cross( const Vector& other )const;
        f32 dot( const Vector& other )const;
        f32 distance()const;
        Vector& identity(void);
        Vector operator * ( const f32 val )const;
        
        static const Vector xIdentity; // [1, 0, 0]
        static const Vector yIdentity; // [0, 1, 0]
        static const Vector zIdentity; // [0, 0, 1]
    };
 
    class TriangleCore
    {
    private:
        Vertex m_A;
        Vertex m_B;
        Vertex m_C;

    public:    
        TriangleCore( const Vertex& A, 
                     const Vertex& B, 
                     const Vertex& C);
        
        TriangleCore( const TriangleCore & other);
        TriangleCore operator = ( const TriangleCore& other);
        Vector normalVector(void)const;

        TriangleCore& operator *= ( const Matrix& mat );
        TriangleCore operator * (const Matrix& mat) const;
        
        bool hitTest( const Vertex& origin, const Vector& direction, f32& distance )const;
    }; // class TriangleCore

    class Triangle
    {
    private:
        size_t m_base;
        
    public:
        Triangle( std::vector<u32>& indices, u32 indexA, u32 indexB, u32 indexC);
        Triangle( const Triangle& other );
        Triangle& operator = ( const Triangle& other );
        inline size_t countOfTriangles(void)const{ return 1; }
        TriangleCore getTriangleCore(const std::vector<Vertex>& vertexes, const std::vector<u32>& indices, size_t index)const;
        ~Triangle(void);
    }; // class Triangle
    
    class TriangleStrip
    {
    private:
        size_t m_begin;
        size_t m_end;
        
    public:
        TriangleStrip( std::vector<u32>& indices, u32 indexV0, u32 indexV1 );
        TriangleStrip( const TriangleStrip& other );
        TriangleStrip& operator = ( const TriangleStrip& other );
        inline size_t countOfTriangles(void)const{ return m_end - m_begin - 2; };
        TriangleCore getTriangleCore( const std::vector<Vertex>& vertexes, const std::vector<u32>& indices, size_t index )const;
        ~TriangleStrip( void );
    }; // class TriangleStrip
    
    struct Color
    {
    public:
        union
        {
            struct
            {
                f32 red;
                f32 green;
                f32 blue;
                f32 alpha;
            }color;
        
            f32 rgba[4];
        }m_color;
        
    public:
        Color( void );
        Color( f32 red, f32 green, f32 blue, f32 alpha = 1.0f );
        Color( u8 red, u8 green, u8 blue, u8 alpha = (u8)1 );
        Color( const Color& other );
        Color& operator = (const Color& other );
        
        bool operator < ( const Color & other )const;
        bool operator > ( const Color & other )const;
        bool operator == ( const Color& other) const;
        bool operator != ( const Color& other) const;
        
        static u8 toInteger( const f32 val );
        static f32 toFloat( const u8 uval );
    };
}; // namespace dj3d

std::ostream& operator<<( std::ostream& stream, const dj3d::Matrix& mat);

#endif // __DJ3D_MATH_3D_H__
