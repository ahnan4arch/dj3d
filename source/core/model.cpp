//
//  source/core/model.cpp
//  dj3d
//
//  Created by Xu ZhiXin on 12/31/11.
//  Copyright (c) 2011 doojava. All rights reserved.
//

#include "platform/dj3d.h"
#include "model.h"
#include "scene.h"
#include "blendop.h"

namespace dj3d 
{
    const ModelId Model::s_undefId = -1;
    f32 s_step = 0.0f;

    Range::Range( const size_t start, const size_t end ):
    m_start( start ),
        m_end( end )
    {
        return;
    }

    Range::Range( const Range& other ):
    m_start(other.m_start),
        m_end(other.m_end)
    {
        return;
    }

    Range& Range::operator = (const Range& other )
    {
        if (&other != this )
        {
            m_start = other.m_start;
            m_end = other.m_end;
        }

        return *this;
    }

    RenderUnitKey::RenderUnitKey( u8 layer, TextureId textId, const MaterialId& material ):
    m_layer(layer),
        m_texture_id(textId),
        m_material(material)
    {
        return;
    }

    bool RenderUnitKey::operator < ( const RenderUnitKey& other )const
    {
        if (m_layer < other.m_layer)
        {
            return true;
        }
        else if ( m_layer > other.m_layer )
        {
            return false;
        }
        else if ( m_texture_id < other.m_texture_id )
        {
            return true;
        }
        else if ( m_texture_id > other.m_texture_id )
        {
            return false;
        }
        else
        {
            return m_material < other.m_material;
        }
    }

    bool RenderUnitKey::operator == ( const RenderUnitKey& other) const
    {
        return ( m_layer == other.m_layer \
            && m_texture_id == other.m_texture_id \
            && m_material == other.m_material );
    }

    RenderUnitKey::~RenderUnitKey(void)
    {
        return;
    }

    RenderUnit::RenderUnit( u8 layer, TextureId id,  
        u32 op, const MaterialId& material, 
        const IModelRendering* base):
    RenderUnitKey(layer, id, material ),
        m_base(base),
        m_op(op)
    {
        return;
    }

    void RenderUnit::addTriangles( const Range& range )
    {
        if ( range.end() > range.start() )
        {
            m_triangles_indices_range.push_back( range );
        }
        return;
    }

    void RenderUnit::addTriStrips( const Range& range )
    {
        if ( range.end() > range.start() )
        {
            m_triStrips_indices_range.push_back( range );
        }
        return;
    }

    void RenderUnit::render(Scene& scene)const
    {
        // texture & material are ready, handle op here:
        BlendOp::Blend( m_op );

        // draw triangles
        for (size_t i = 0; i < m_triangles_indices_range.size(); ++ i )
        {
            const Range& _range = m_triangles_indices_range[i];
            glDrawElements(GL_TRIANGLES, 
                static_cast<GLsizei>(_range.end() - _range.start()), 
                GL_UNSIGNED_INT, 
                m_base->getIndexValue(IModelRendering::IT_TRIANGLE, 
                _range.start()));
        }

        // draw triangle strips
        for (size_t i = 0; i < m_triStrips_indices_range.size(); ++ i )
        {
            const Range& _range = m_triStrips_indices_range[i];
            glDrawElements(GL_TRIANGLE_STRIP, 
                static_cast<GLsizei>(_range.end() - _range.start()), 
                GL_UNSIGNED_INT, 
                m_base->getIndexValue(IModelRendering::IT_TRIANGLE_STRIP, 
                _range.start()));
        }

        return;
    }

    RenderUnit::~RenderUnit(void)
    {
        return;
    }

    ModelRendering::ModelRendering(void):
    m_loading_render_unit( NULL )
    {
        return;
    }

    bool ModelRendering::createRenderUnit(u8 layer, TextureId id, u32 op, const MaterialId& material)
    {
        if ( NULL != m_loading_render_unit )
        {
            assert( NULL == m_loading_render_unit);
            return false;
        }

        m_loading_render_unit = new RenderUnit( layer, id, op, material, this );

        if ( m_rendering_map.end() != m_rendering_map.find( * dynamic_cast<RenderUnitKey* >(m_loading_render_unit) ) )
        {
            RenderUnit* _gc = m_loading_render_unit;
            m_loading_render_unit = m_rendering_map.find( * dynamic_cast<RenderUnitKey* >(m_loading_render_unit) )->second;
            delete _gc;
            _gc = NULL;
        }

        return true;
    }

    bool ModelRendering::submitRenderUnit(Scene& scene)
    {
        using namespace std;

        if ( NULL == m_loading_render_unit )
        {
            assert( NULL != m_loading_render_unit);
            return false;
        }

        if ( m_rendering_map.end() == m_rendering_map.find( * dynamic_cast<RenderUnitKey* >(m_loading_render_unit) ) )
        {
            m_rendering_map.insert(make_pair(* dynamic_cast<RenderUnitKey* >(m_loading_render_unit), m_loading_render_unit));
            scene.registerModel(m_loading_render_unit->layer(), m_loading_render_unit->id(), m_loading_render_unit->material(), this);
        }

        m_loading_render_unit = NULL;

        return true;
    }

    void ModelRendering::renderModel(Scene& scene, u8 textLayer, TextureId textId, const MaterialId& material)const
    {
        using namespace std;

        if ( !isVisible() )
        {
            return;
        }

        map<RenderUnitKey,RenderUnit*>::const_iterator _cp = 
            m_rendering_map.find(RenderUnitKey(textLayer, textId, material));

        if ( m_rendering_map.end() != _cp )
        {
            _beforeRendering();            
            _cp->second->render(scene);
            _afterRendering();
        }

        return;
    }

    ModelRendering::~ModelRendering(void)
    {
        using namespace std;

        assert( NULL == m_loading_render_unit );

        for ( map<RenderUnitKey, RenderUnit*>::const_iterator _cp = m_rendering_map.begin();
            _cp != m_rendering_map.end(); _cp++ )
        {
            if ( _cp->second != NULL )
            {
                delete _cp->second;
            }
        }

        m_rendering_map.clear();
    }

    Triangles::Triangles(void)
    {
        return;
    }

    void Triangles::addTriangles(const std::vector<u32>& vertex_indices)
    {
        assert( NULL != m_loading_render_unit);
        size_t _start = m_triangle_indices.size();

        size_t _count = vertex_indices.size();
        assert ( _count > 0 && (_count % 3 == 0) );

        for ( size_t i = 0; i < _count; i +=3 )
        {
            m_triangles.push_back(Triangle(m_triangle_indices, 
                vertex_indices[i], 
                vertex_indices[i+1], 
                vertex_indices[i+2]));
        } // for each triangle

        m_loading_render_unit->addTriangles(
            Range(_start, m_triangle_indices.size()));

        return;
    }

    TriangleStrips::TriangleStrips(void)
    {
        return;
    }

    void TriangleStrips::addTriangleStrips( const std::vector<u32>& vertex_indices)
    {
        assert( NULL != m_loading_render_unit);

        return;
    }

    Model::Model(void):
    m_id( s_undefId )
    {
        m_attributes[RENDERABLE] = true;
        m_attributes[VISIBLE] = true;
        m_attributes[WIREFRAME] = false;
        m_attributes[CCW] = true;

        return;
    }

    Model::Model(ModelId id):
    m_id( id )
    {
        m_attributes[RENDERABLE] = true;
        m_attributes[VISIBLE] = true;
        m_attributes[WIREFRAME] = false;
        m_attributes[CCW] = true;        

        return;
    }

    bool Model::isVisible(void)const
    {
        // for now, return m_isVisible
        // TODO: add space detection
        return m_attributes[RENDERABLE] && m_attributes[VISIBLE];
    }

    const u32* Model::getIndexValue( IndexType indexType, size_t offset )const
    {
        switch ( indexType )
        {
        case IModelRendering::IT_TRIANGLE:
            return &Triangles::getIndexValue(offset);

        case IModelRendering::IT_TRIANGLE_STRIP:
            return &TriangleStrips::getIndexValue(offset);

        default:
            assert( false );
            return NULL;
        }
    }

    void Model::show(void)
    {
        if ( m_attributes[RENDERABLE] )
        {
            m_attributes[VISIBLE] = true;
        }
        return;
    }

    void Model::hide(void)
    {
        if ( m_attributes[RENDERABLE] )
        {
            m_attributes[VISIBLE] = false;
        }
        return;
    }

    ModelId Model::modelId(void)const
    {
        return m_id;
    }

    void Model::pushMatrix(void)
    {
        m_matrixStack.push(*this);
        return;
    }

    void Model::popMatrix(void)
    {
        //assert( m_matrixStack.size() > 0 );

        if ( m_matrixStack.size() > 0 )
        {
            *(Matrix*)this = m_matrixStack.top();
            m_matrixStack.pop();
        }

        return;
    }

    const Matrix& Model::getMatrix(void)const
    {
        return *this;
    }

   Matrix& Model::getMatrix(void)
   {
       return *this;
   }

    void Model::_beforeRendering()const
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultMatrixf(*(Matrix*)this);
        glPolygonMode(GL_FRONT_AND_BACK, m_attributes[WIREFRAME] ? GL_LINE : GL_FILL);
        glFrontFace( m_attributes[CCW] ? GL_CCW : GL_CW );

        return;
    }

    void Model::_afterRendering()const
    {
        glPopMatrix();

        return;
    }

    bool Model::hitTest( const std::vector<Vertex>& vertexes,
        const Vertex& origin, const Vector& dirIdentity, 
        f32& distance )
    {
        _HitTest _func( *this, origin, dirIdentity );

        // for triangles
        Triangles::forEachTriangle(vertexes, _func);
        if ( _func > ZERO )
        {
            distance = _func;
            return true;
        }

        // for triangle strips
        TriangleStrips::forEachTriangle(vertexes, _func);
        if ( _func > ZERO )
        {
            distance = _func;
            return true;
        }

        return false; 
    }

    bool Model::onLoad(Scene& scene)
    {
        return true;
    }

    Model::~Model(void)
    {
        return;
    }

};