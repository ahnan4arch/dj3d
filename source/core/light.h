//
//  source/core/light.h
//  dj3d
//
//  Created by Xu ZhiXin on 1/22/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//

#ifndef __DJ3D_CORE_LIGHT_H__
#define __DJ3D_CORE_LIGHT_H__

#include "math/3d.h"

namespace dj3d
{
    typedef u8 LightId;

    class Light
    {
    private:
        Color m_ambient;
        Color m_diffuse;
        Color m_specular;
        Vertex m_position;
        Vector m_direction;
        const LightId m_id; // 0 ~ 7

    private:
        static bool s_isLightingEnabled;

    public:
        explicit Light(LightId id);
        void turnOn(void);
        void turnOff(void);
        const Vertex& where(void)const;
        const Vector& direction(void)const;        
        const Color& ambientColor(void)const;        
        const Color& diffuseColor(void)const;        
        const Color& specularColor(void)const;
        Vertex& where(void);
        Vector& direction(void);
        Color& ambientColor(void);
        Color& diffuseColor(void);
        Color& specularColor(void);
        ~Light(void);

        static void enableLighting( bool bEnable = true );
        static bool isLightingEnabled(void);
    }; // class Light
}; // namespace dj3d

#endif // __DJ3D_CORE_LIGHT_H__
