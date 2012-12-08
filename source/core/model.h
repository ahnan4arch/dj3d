//
//  source/core/model.h, static models
//  dj3d
//
//  Created by Xu ZhiXin on 12/31/11.
//  Copyright (c) 2011 doojava. All rights reserved.
//

#ifndef __DJ3D_CORE_MODEL_H__
#define __DJ3D_CORE_MODEL_H__

#include "math/3d.h"
#include "texture2d.h"
#include "material.h"
#include "entry/routine.h"
#include <stack>
#include <bitset>
#include <map>

namespace dj3d
{
    typedef int ModelId;
    
    class Scene;
    class Model;

    class Range
    {
    private:
        size_t m_start;
        size_t m_end;
    public:
        Range( const size_t start, const size_t end );
        Range( const Range& other );
        Range& operator = (const Range& other );
        inline size_t start(void)const{ return m_start; }
        inline size_t end(void)const{ return m_end; }
    }; // class Range

    class RenderUnitKey
    {
    private:
        const TextureId m_texture_id;
        const u8 m_layer; // default to zero
        const MaterialId m_material;
    public:
        RenderUnitKey( u8 layer, TextureId textId, const MaterialId& material );

        bool operator < ( const RenderUnitKey& other )const;
        bool operator == ( const RenderUnitKey& other) const;

        inline TextureId id(void)const { return m_texture_id; }
        inline u8 layer(void)const { return m_layer; }  
        inline MaterialId material(void)const{ return m_material; }

        virtual ~RenderUnitKey(void);
    };// class RenderUnitKey

    class IModelRendering
    {
    public:
        enum IndexType
        {
            IT_TRIANGLE = 0,
            IT_TRIANGLE_STRIP = 1
        };

        virtual const u32* getIndexValue( IndexType indexType, size_t offset )const = 0;
        virtual bool isVisible(void)const = 0;

        // for rendering
        virtual void renderModel(Scene& scene, u8 textLayer, TextureId textId, const MaterialId& material )const = 0;
    }; // interface IElementIndices

    class RenderUnit : public RenderUnitKey
    {
    private:
        const IModelRendering* m_base;
        const u32 m_op;
        std::vector<Range> m_triangles_indices_range;
        std::vector<Range> m_triStrips_indices_range;

    private:
        RenderUnit( const RenderUnit&  );
        RenderUnit& operator = ( const RenderUnit& );

    public:
        RenderUnit( u8 layer, TextureId id, 
            u32 op, const MaterialId& material, 
            const IModelRendering*  base);
        void addTriangles( const Range& range );
        void addTriStrips( const Range& range );
        void render(Scene& scene)const;
        virtual ~RenderUnit(void);

    }; // class RenderUnitKey

    class ModelRendering : public IModelRendering
    {
    private:
        ModelRendering( const ModelRendering& );
        ModelRendering& operator = ( const ModelRendering& );

    protected:
        std::map<RenderUnitKey, RenderUnit*> m_rendering_map;
        // for loading
        RenderUnit* m_loading_render_unit;

    protected:
        virtual void _beforeRendering(void)const = 0;
        virtual void _afterRendering(void)const = 0;

    public:
        ModelRendering(void);

        bool createRenderUnit(u8 layer, TextureId id, u32 op, const MaterialId& material);

        bool submitRenderUnit(Scene& scene);

        virtual void renderModel(Scene& scene, u8 textLayer, TextureId textId, const MaterialId& material )const;

        virtual ~ModelRendering(void);
    }; // class ModelRendering

    class Triangles : public virtual ModelRendering
    {
    private:
        Triangles( const Triangles& );
        Triangles& operator = ( const Triangles& );

    private:
        std::vector<Triangle> m_triangles;
        // indices of vertexes of the triangles
        std::vector<u32> m_triangle_indices;

    public:
        Triangles(void);

        const u32& getIndexValue( size_t offset )const
        {
            assert( offset < m_triangle_indices.size() );
            return m_triangle_indices[offset];
        }

        void addTriangles(const std::vector<u32>& vertex_indices);

        template<typename TFunc>
        void forEachTriangle( const std::vector<Vertex>& vertexes, TFunc& func )
        {
            for ( size_t i = 0; i < m_triangles.size(); ++ i )
            {
                if ( !func( m_triangles[i].getTriangleCore(vertexes, m_triangle_indices, i) ) )
                {
                    break;
                }
            }
            return;
        }
    }; // class Triangles

    class TriangleStrips : public virtual ModelRendering
    {
    private:
        TriangleStrips( const TriangleStrips& );
        TriangleStrips& operator = ( const TriangleStrips& );

    private:
        std::vector<TriangleStrip> m_triStrips;
        // indices of vertexes of the triangle strips
        std::vector<u32> m_triStrip_indices;

    public:
        TriangleStrips(void);

        const u32& getIndexValue( size_t offset )const
        {
            assert( offset < m_triStrip_indices.size() );
            return m_triStrip_indices[offset];
        }

        void addTriangleStrips( const std::vector<u32>& vertex_indices);

        template<typename TFunc>
        void forEachTriangle( const std::vector<Vertex>& vertexes, TFunc& func )
        {
            for ( size_t i = 0; i < m_triStrips.size(); ++ i )
            {
                if ( !func( m_triStrips[i].getTriangleCore(vertexes, m_triStrip_indices, i) ) )
                {
                    break;
                }
            }
            return;
        }
    }; // class TriagnleStrips

   class Model : public Matrix,  public Triangles,  public TriangleStrips
    {
        friend class RenderUnit;
 
    private:
        static const ModelId s_undefId;
        ModelId m_id;
        std::stack< Matrix > m_matrixStack;

        enum _ModelAttr
        {
            RENDERABLE = 0,
            VISIBLE,
            WIREFRAME,
            CCW,

            ATTR_COUNT
        };

        std::bitset<ATTR_COUNT> m_attributes;

    private:
        class _HitTest
        {
        private:
            const Vertex& m_origin;
            const Vector& m_direction;
            f32 m_distance;
            const Matrix& m_mat;
        public:
            _HitTest(const Matrix& mat, const Vertex& origin, const Vector& direction):
              m_mat( mat ),
              m_origin( origin ),
               m_direction( direction ),
               m_distance(-1.0f)
              {
                  return;
              }

              bool operator()(const TriangleCore& tri)
              {
                  return !(tri * m_mat).hitTest( m_origin, m_direction, m_distance );
              }

              inline operator f32(void)const{ return m_distance; }
        }; // class HitTest

    protected:
        // ModelRendering
        virtual void _beforeRendering(void)const;
        virtual void _afterRendering(void)const;

    public:
        Model(void);
        explicit Model( ModelId id );

        // IModelRendering
        virtual bool isVisible(void)const;
        virtual const u32* getIndexValue( IndexType indexType, size_t offset )const;
  
        void show(void);
        void hide(void);
        ModelId modelId(void)const;
        void pushMatrix(void);
        void popMatrix(void);
        const Matrix& getMatrix(void)const;
        Matrix& getMatrix(void);

        // for picking up
        bool hitTest( const std::vector<Vertex>& vertexes,
            const Vertex& origin, const Vector& dirIdentity, 
            f32& distance );

        virtual bool onLoad(Scene& scene);

        virtual ~Model(void);
    }; // class Model
}; // naemspace dj3d

#endif // __DJ3D_CORE_MODEL_H__
