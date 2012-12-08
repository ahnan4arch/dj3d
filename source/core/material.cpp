//
//  material.cpp
//  glutRun
//
//  Created by Xu ZhiXin on 1/21/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//

#include "platform/dj3d.h"
#include "material.h"
#include "light.h"

namespace dj3d
{
    MaterialCore::MaterialCore(void):
m_shininess(50.0f),
    m_ambient(0.2f, 0.2f, 0.2f, 1.0f ),
    m_diffuse(0.8f, 0.8f, 0.8f, 1.0f ),
    m_specular(0.0f, 0.0f, 0.0f, 1.0f),
    m_emission(0.0f, 0.0f, 0.0f, 1.0f )
{
    return;
}

MaterialCore::MaterialCore(const MaterialCore& other ):
m_shininess( other.m_shininess ),
    m_specular( other.m_specular ),
    m_diffuse( other.m_diffuse ),
    m_ambient( other.m_ambient ),
    m_emission( other.m_emission )
{
    return;
}

MaterialCore& MaterialCore::operator = ( const MaterialCore& other )
{
    if ( &other != this )
    {
        m_shininess = other.m_shininess;
        m_specular = other.m_specular;
        m_diffuse = other.m_diffuse;
        m_ambient = other.m_ambient;
        m_emission = other.m_emission;
    }

    return *this;
}

Material::Material( void ):
m_current(0)
{
    return;
}

Material::Material( const Material& other ):
m_materials(other.m_materials),
    m_current(other.m_current)
{
    return;
}

Material& Material::operator = ( const Material& other )
{
    if ( &other != this )
    {
        m_materials = other.m_materials;
        m_current = other.m_current;
    }

    return *this;
}

void Material::addMaterial( const MaterialCore& material )
{
    m_materials.push_back( material );
    return;
}

size_t Material::count(void)const
{
    return m_materials.size();
}

size_t Material::getCurrent(void)const
{
    return m_current;
}

bool Material::setCurrent(size_t which)
{
    if ( m_materials.size() > which )
    {
        m_current = which;
        return true;
    }
    else
    {
        assert( m_materials.size() > which );
        return false;
    }
}

void Material::select(void)const
{
    assert( Light::isLightingEnabled() );
    assert( m_current < m_materials.size() );

    if ( m_current < m_materials.size() )
    {
        const MaterialCore& _material = m_materials[m_current];

        // specular
        glMaterialfv(GL_FRONT, GL_SPECULAR, _material.specular().m_color.rgba);
        assert( !(_material.shininess() > 128.0f) 
            && !(_material.shininess() < 0.0f) );
        glMaterialf(GL_FRONT, GL_SHININESS, _material.shininess());

        // diffuse
        glMaterialfv(GL_FRONT, GL_DIFFUSE, _material.diffuse().m_color.rgba);

        // ambient
        glMaterialfv(GL_FRONT, GL_AMBIENT, _material.ambient().m_color.rgba);

        // emission
        glMaterialfv(GL_FRONT, GL_EMISSION, _material.emission().m_color.rgba);
    }

    return;
}
}; // namespace dj3d