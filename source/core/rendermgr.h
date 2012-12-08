//
//  source/core/rendermgr.h
//  dj3d
//
//  Created by Xu ZhiXin on 1/19/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//

#ifndef __DJ3D_CORE_RENDER_MGR_H__
#define __DJ3D_CORE_RENDER_MGR_H__

#include "model.h"
#include "light.h"

namespace dj3d 
{
    class LightModel
    {
    protected:
        Light m_light0;
        Light m_light1;
        Light m_light2;
        Light m_light3;
        Light m_light4;
        Light m_light5;
        Light m_light6;
        Light m_light7;

    public:
        LightModel(void);
        void setAmbient( const Color& color = Color( 0.2f, 0.2f, 0.2f, 1.0f ) );
        virtual ~LightModel(void);
    }; // class LightModel

    class TextureMap
    {
    protected:
        struct TextureData
        {
            FilePath m_filePath;
            Texture2d* m_texture2d;

            TextureData(const FilePath& filePath, const TextureId textId );
            ~TextureData(void);
        };

        std::map<TextureId, TextureData*> m_texture_map;

    protected:
        TextureMap(void);
        virtual ~TextureMap(void);
    };

    // separate from RenderMgr to support dymanic material
    class MaterialMap
    {
    protected:
        std::map<MaterialId, Material> m_material_map;

    protected:
        MaterialMap(void);
        virtual ~MaterialMap(void);
    };

    class RenderMgr : public LightModel, public TextureMap, public MaterialMap
    {
    private:
        struct RenderIndex
        {
            u8 m_layer;
            const Texture2d* m_texture;
            MaterialId m_material;

            RenderIndex( u8 layer, const Texture2d* texture, MaterialId material );
            RenderIndex( const RenderIndex& other );
            RenderIndex& operator = ( const RenderIndex& other);
            bool operator < ( const RenderIndex& other )const;
            bool operator == ( const RenderIndex& other )const;
        }; // struct RenderIndex

        typedef std::map<RenderIndex, std::vector<const IModelRendering*> > _TMap;
        _TMap m_rendering_map;

    protected:
        void _initRenderingEnv(void);
        virtual void setTextureLayer( const u8 layer ) = 0;

    public:
        RenderMgr(void);

        bool loadTexture( TextureId textId, const FilePath& fileName );

        bool loadMaterial( MaterialId materialId, const Material& material );

        bool registerModel( u8 layer, TextureId textId, MaterialId material, const IModelRendering* pModel );

        void renderAll( Scene& scene );

        virtual ~RenderMgr(void);
    }; // class RenderMgr    
}; // namespace dj3d

#endif // __DJ3D_CORE_RENDER_MGR_H__
