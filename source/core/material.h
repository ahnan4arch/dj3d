//
//  source/core/material.h
//  dj3d
//
//  Created by Xu ZhiXin on 1/21/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//

#ifndef __DJ3D_CORE_MATERIAL_H__
#define __DJ3D_CORE_MATERIAL_H__

#include "math/3d.h"

namespace dj3d
{
    typedef u32 MaterialId;

    class MaterialCore
    {
    private:
        f32 m_shininess;
        Color m_specular;
        Color m_diffuse;
        Color m_ambient;
        Color m_emission;
    public:
        MaterialCore(void);
        MaterialCore(const MaterialCore& other );
        MaterialCore& operator = ( const MaterialCore& other );
        inline f32& shininess(void){ return m_shininess; }
        inline const f32& shininess(void)const{ return m_shininess; }
        inline Color& specular(void){ return m_specular; }
        inline const Color& specular(void)const{ return m_specular; }
        inline Color& diffuse(void){ return m_diffuse; }
        inline const Color& diffuse(void)const{ return m_diffuse; }
        inline Color& ambient(void){ return m_ambient; }
        inline const Color& ambient(void)const{ return m_ambient; }
        inline Color& emission(void){ return m_emission; }
        inline const Color& emission(void)const { return m_emission; }        
    }; // class MaterialCore

    class Material
    {
    private:
        std::vector<MaterialCore> m_materials;
        size_t m_current;

    public:
        Material(void);
        Material( const Material& other );
        Material& operator = ( const Material& other );

        void addMaterial( const MaterialCore& material = MaterialCore() );

        // keep for dynamic material
        size_t count(void)const;
        size_t getCurrent(void)const;
        bool setCurrent(size_t which);

        void select(void)const;
    }; // class Material

}; // namespace dj3d

#endif // __DJ3D_CORE_MATERIAL_H__
