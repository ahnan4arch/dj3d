//
//  source/core/light.cpp
//  dj3d
//
//  Created by Xu ZhiXin on 1/22/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//

#include "platform/dj3d.h"
#include "light.h"

namespace dj3d
{
    bool Light::s_isLightingEnabled = false;

    void Light::enableLighting( bool bEnable )
    {
        if ( bEnable == s_isLightingEnabled )
        {
            return;
        }

        if ( bEnable )
        {
            glEnable( GL_NORMALIZE );
            glEnable( GL_LIGHTING );
        }
        else
        {
            glDisable( GL_NORMALIZE );
            glDisable( GL_LIGHTING );
        }

        s_isLightingEnabled = bEnable;
        return;
    }

    bool Light::isLightingEnabled(void)
    {
        return s_isLightingEnabled;
    }

    Light::Light(LightId id) throw (InvalidLightIdException):
    m_id( id )
    {
        if( m_id >= 8 )
        {
            throw InvalidLightIdException("");
        }
        return;
    }

    void Light::turnOn(void)
    {
        assert( s_isLightingEnabled );

        f32 _position[4] =  { 
            m_position.m_v[0], m_position.m_v[1], m_position.m_v[2], 0.0f 
        };

        switch ( m_id )
        {
        case 0:
            glLightfv(GL_LIGHT0, GL_POSITION, _position);
            glLightfv(GL_LIGHT0, GL_AMBIENT, m_ambient.m_color.rgba);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, m_diffuse.m_color.rgba);
            glLightfv(GL_LIGHT0, GL_SPECULAR, m_specular.m_color.rgba);
            glEnable(GL_LIGHT0);
            break;
        case 1:
            glLightfv(GL_LIGHT1, GL_POSITION, _position);
            glLightfv(GL_LIGHT1, GL_AMBIENT, m_ambient.m_color.rgba);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, m_diffuse.m_color.rgba);
            glLightfv(GL_LIGHT1, GL_SPECULAR, m_specular.m_color.rgba);
            glEnable(GL_LIGHT1);
            break;
        case 2:
            glLightfv(GL_LIGHT2, GL_POSITION, _position);
            glLightfv(GL_LIGHT2, GL_AMBIENT, m_ambient.m_color.rgba);
            glLightfv(GL_LIGHT2, GL_DIFFUSE, m_diffuse.m_color.rgba);
            glLightfv(GL_LIGHT2, GL_SPECULAR, m_specular.m_color.rgba);
            glEnable(GL_LIGHT2);
            break;
        case 3:
            glLightfv(GL_LIGHT3, GL_POSITION, _position);
            glLightfv(GL_LIGHT3, GL_AMBIENT, m_ambient.m_color.rgba);
            glLightfv(GL_LIGHT3, GL_DIFFUSE, m_diffuse.m_color.rgba);
            glLightfv(GL_LIGHT3, GL_SPECULAR, m_specular.m_color.rgba);
            glEnable(GL_LIGHT3);
            break;
        case 4:
            glLightfv(GL_LIGHT4, GL_POSITION, _position);
            glLightfv(GL_LIGHT4, GL_AMBIENT, m_ambient.m_color.rgba);
            glLightfv(GL_LIGHT4, GL_DIFFUSE, m_diffuse.m_color.rgba);
            glLightfv(GL_LIGHT4, GL_SPECULAR, m_specular.m_color.rgba);
            glEnable(GL_LIGHT4);
            break;
        case 5:
            glLightfv(GL_LIGHT5, GL_POSITION, _position);
            glLightfv(GL_LIGHT5, GL_AMBIENT, m_ambient.m_color.rgba);
            glLightfv(GL_LIGHT5, GL_DIFFUSE, m_diffuse.m_color.rgba);
            glLightfv(GL_LIGHT5, GL_SPECULAR, m_specular.m_color.rgba);
            glEnable(GL_LIGHT5);
            break;
        case 6:
            glLightfv(GL_LIGHT6, GL_POSITION, _position);
            glLightfv(GL_LIGHT6, GL_AMBIENT, m_ambient.m_color.rgba);
            glLightfv(GL_LIGHT6, GL_DIFFUSE, m_diffuse.m_color.rgba);
            glLightfv(GL_LIGHT6, GL_SPECULAR, m_specular.m_color.rgba);
            glEnable(GL_LIGHT6);
            break;
        case 7:
            glLightfv(GL_LIGHT7, GL_POSITION, _position);
            glLightfv(GL_LIGHT7, GL_AMBIENT, m_ambient.m_color.rgba);
            glLightfv(GL_LIGHT7, GL_DIFFUSE, m_diffuse.m_color.rgba);
            glLightfv(GL_LIGHT7, GL_SPECULAR, m_specular.m_color.rgba);
            glEnable(GL_LIGHT7);
            break;
        default:
            assert( false );
            break;
        } // switch

        return;
    }

    void Light::turnOff(void)
    {
        assert( s_isLightingEnabled );

        switch ( m_id )
        {
        case 0:
            glDisable(GL_LIGHT0);
            break;
        case 1:
            glDisable(GL_LIGHT1);
            break;
        case 2:
            glDisable(GL_LIGHT2);
            break;
        case 3:
            glDisable(GL_LIGHT3);
            break;
        case 4:
            glDisable(GL_LIGHT4);
            break;
        case 5:
            glDisable(GL_LIGHT5);
            break;
        case 6:
            glDisable(GL_LIGHT6);
            break;
        case 7:
            glDisable(GL_LIGHT7);
            break;
        default:
            assert( false );
            break;
        } // switch

        return;
    }

    const Vertex& Light::where(void)const
    {
        return m_position;
    }

    const Vector& Light::direction(void)const
    {
        return m_direction;
    }

    const Color& Light::ambientColor(void)const
    {
        return m_ambient;
    }

    const Color& Light::diffuseColor(void)const
    {
        return m_diffuse;
    }

    const Color& Light::specularColor(void)const
    {
        return m_specular;
    }

    Vertex& Light::where(void)
    {
        return m_position;
    }

    Vector& Light::direction(void)
    {
        return m_direction;
    }

    Color& Light::ambientColor(void)
    {
        return m_ambient;
    }

    Color& Light::diffuseColor(void)
    {
        return m_diffuse;
    }

    Color& Light::specularColor(void)
    {
        return m_specular;
    }

    Light::~Light(void)
    {
        return;
    }

}; // namespace dj3d
