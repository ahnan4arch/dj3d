//
//  source/core/rendermgr.cpp
//  dj3d
//
//  Created by Xu ZhiXin on 1/19/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//

#include "platform/dj3d.h"
#include "rendermgr.h"

namespace dj3d 
{
    DuplicatedTextureId::DuplicatedTextureId( const std::string& strMsg ):
Exception( strMsg )
{
    return;
}

DuplicatedMaterialId::DuplicatedMaterialId( const std::string& strMsg ):
Exception( strMsg )
{
    return;
}

LightModel::LightModel(void):
m_light0((u8)0),
    m_light1((u8)1),
    m_light2((u8)2),
    m_light3((u8)3),
    m_light4((u8)4),
    m_light5((u8)5),
    m_light6((u8)6),
    m_light7((u8)7)
{
    return;
}

void LightModel::setAmbient( const Color& color )
{
    assert( Light::isLightingEnabled() );

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, color.m_color.rgba);

    return;
}

LightModel::~LightModel(void)
{
    return;
}

TextureMap::TextureMap(void)
{
    return;
}

TextureMap::~TextureMap(void)
{
    using namespace std;

    for( map<TextureId, TextureData*>::const_iterator _cp = m_texture_map.begin();
        _cp != m_texture_map.end(); ++ _cp )
    {
        if ( NULL != _cp->second )
        {
            delete _cp->second;
        }
    }

    m_texture_map.clear();

    return;
}

MaterialMap::MaterialMap(void)
{
    return;
}

MaterialMap::~MaterialMap(void)
{
    return;
}

RenderMgr::TextureData::TextureData(const FilePath& filePath, const TextureId textId):
m_filePath(filePath),
    m_texture2d(NULL)
{
    m_texture2d = Texture2d::loadTexture2d(filePath, textId);
    return;
}

RenderMgr::TextureData::~TextureData(void)
{
    if ( NULL != m_texture2d )
    {
        Texture2d::releaseTexture2d(m_texture2d);
        m_texture2d = NULL;
    }

    return;
}

RenderMgr::RenderIndex::RenderIndex( u8 layer, const Texture2d* texture, MaterialId material ):
m_layer(layer),
    m_texture(texture),
    m_material(material)
{
    return;
}

RenderMgr::RenderIndex::RenderIndex( const RenderIndex& other ):
m_layer(other.m_layer),
    m_texture(other.m_texture),
    m_material(other.m_material)
{
    return;
}

RenderMgr::RenderIndex& RenderMgr::RenderIndex::operator = ( const RenderIndex& other)
{
    if ( &other != this )
    {
        m_layer = other.m_layer;
        m_texture = other.m_texture;
        m_material = other.m_material;
    }
    return *this;
}

bool RenderMgr::RenderIndex::operator < ( const RenderIndex& other )const
{
    if ( m_layer < other.m_layer )
    {
        return true;
    }
    else if ( m_layer > other.m_layer )
    {
        return false;
    }
    else if ( m_texture == other.m_texture )
    {
        return m_material < other.m_material;
    }
    else if ( NULL == m_texture )
    {
        return true;
    }
    else if ( NULL == other.m_texture )
    {
        return false;
    }
    else 
    {
        return m_texture->id() < other.m_texture->id();
    }
}

bool RenderMgr::RenderIndex::operator == ( const RenderIndex& other )const
{
    return (m_layer == other.m_layer \
        && m_texture == other.m_texture \
        && m_material == other.m_material );
}

RenderMgr::RenderMgr(void)
{
    // add space holder for non-texture
    m_texture_map.insert( std::make_pair(0, (TextureData*)NULL) );

    // add space holder for default material
    Material _material;
    _material.addMaterial( MaterialCore() );
    _material.setCurrent(0);
    m_material_map.insert( std::make_pair(0, _material));

    return;
}

void RenderMgr::_initRenderingEnv(void)
{
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    /*
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth(1.35f);
    */   
    return;
}

bool RenderMgr::loadTexture( TextureId textId, const FilePath& fileName )throw(DuplicatedTextureId)
{
    using namespace std;

    if ( 0 == textId )
    {
        assert( 0 != textId );
        return false;
    }

    map<TextureId, TextureData*>::const_iterator _cp = m_texture_map.find( textId );
    if ( m_texture_map.end() != _cp && fileName != _cp->second->m_filePath )
    {
        throw DuplicatedTextureId("");
    }

    auto_ptr<TextureData> _pData(new TextureData( fileName, textId ));
    if ( NULL == _pData->m_texture2d )
    {
        return false;
    }
    else
    {
        m_texture_map.insert( make_pair(textId, _pData.release()) );
        return true;
    }
}

bool RenderMgr::loadMaterial( MaterialId materialId, const Material& material )throw(DuplicatedMaterialId)
{
    using namespace std;

    map<MaterialId, Material>::const_iterator _cp = m_material_map.find( materialId );
    if ( m_material_map.end() != _cp )
    {
        throw DuplicatedMaterialId("");
    }
    else
    {
        m_material_map.insert( make_pair(materialId, material) );   
        return true;
    }
}

bool RenderMgr::registerModel( u8 layer, TextureId textId, MaterialId material, const IModelRendering* pModel )
{
    using namespace std;

    if ( NULL == pModel )
    {
        return false;
    }

    map<TextureId, TextureData*>::const_iterator _cpText = m_texture_map.find( textId );
    if ( m_texture_map.end() == _cpText )
    {
        return false;
    }

    map<MaterialId, Material>::const_iterator _cpMaterial = m_material_map.find( material );
    if ( m_material_map.end() == _cpMaterial )
    {
        return false;
    }

    const Texture2d* _ptext = (0 == textId) ? &Texture2d::s_emptyTexture2d : _cpText->second->m_texture2d;

    RenderIndex _rIndex( layer, _ptext, material );

    _TMap::iterator _it = m_rendering_map.find( _rIndex );
    if ( m_rendering_map.end() == _it )
    {
        m_rendering_map.insert( make_pair( _rIndex, std::vector<const IModelRendering*>() ) );
        _it = m_rendering_map.find( _rIndex );
    }

    _it->second.push_back( pModel );

    return true;
}

void RenderMgr::renderAll( Scene& scene )
{
    using namespace std;

    u8 _currentLayer = ~0;
    const Texture2d* _pCurrentTexture = NULL;
    MaterialId _currentMaterial = ~0;

    for (_TMap::const_iterator _cp = m_rendering_map.begin(); \
        m_rendering_map.end() != _cp; ++ _cp )
    {
        // apply texture
        if ( _cp->first.m_texture != _pCurrentTexture )
        {
            if ( _cp->first.m_layer != _currentLayer )
            {
                _currentLayer = _cp->first.m_layer;
                setTextureLayer(_currentLayer);  
            } // if layer needs to be changed

            _cp->first.m_texture->select();
            _pCurrentTexture = _cp->first.m_texture;
        }

        // apply material, do nothing if no light is enabled
        if ( Light::isLightingEnabled() && _cp->first.m_material != _currentMaterial )
        {
            map<MaterialId, Material>::const_iterator _cpMaterial = m_material_map.find( _cp->first.m_material );

            if ( m_material_map.end() != _cpMaterial )
            {
                _cpMaterial->second.select();
            }
            else
            {
                assert( m_material_map.end() != _cpMaterial );
            }

            _currentMaterial = _cp->first.m_material;
        }

        // render
        for ( size_t i = 0; i < _cp->second.size(); ++ i )
        {
            if ( _cp->second[i]->isVisible() )
            {
                _cp->second[i]->renderModel( scene, 
                    _cp->first.m_layer, 
                    _cp->first.m_texture->id(), 
                    _cp->first.m_material );
            }
        }

    } // for

    return;
}

RenderMgr::~RenderMgr(void)
{
    using namespace std;
    for ( map<TextureId, TextureData*>::iterator _it = m_texture_map.begin(); \
        m_texture_map.end() != _it; ++_it )
    {
        if ( NULL != _it->second )
        {
            delete _it->second;
        }
    }

    m_texture_map.clear();

    return;
}

}; // namespace dj3d
