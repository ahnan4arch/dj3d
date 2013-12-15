//
//  source/core/scene.cpp
//  dj3d
//
//  Created by Xu ZhiXin on 12/31/11.
//  Copyright (c) 2011 doojava. All rights reserved.
//

#include "platform/dj3d.h"
#include "scene.h"
#include "blendop.h"

namespace dj3d
{
    bool CoreBuffer::activateArrays(void)
    {
        // for vertexes
        if ( !m_vertex_vec.empty() )
        {
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, &m_vertex_vec[0]);
        }

        // for normal vectors, it doesn't make sense without light
        if ( Light::isLightingEnabled() && !m_normal_vec.empty() )
        {
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT, 0, &m_normal_vec[0]);
        }

        // for colors
        if ( !m_color_vec.empty() )
        {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, GL_FLOAT, 0, &m_color_vec[0]);
        }

        // for texture coordinates
        if ( !m_textcoord_vec0.empty() \
            || !m_textcoord_vec1.empty() \
            || !m_textcoord_vec2.empty() \
            || !m_textcoord_vec3.empty() \
            || !m_textcoord_vec4.empty() \
            || !m_textcoord_vec5.empty() \
            || !m_textcoord_vec6.empty() \
            || !m_textcoord_vec7.empty() )
        {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        }

        return true;
    }

    static Scene* s_currentScene = NULL;
    static bool s_initialized = false;

    static void __displayRoutine(void)
    {
        if ( NULL != s_currentScene )
        {
            s_currentScene->onDraw();
        }

        return;
    }

    static void __reshapeRoutine(int width, int height)
    {
        if ( NULL != s_currentScene )
        {
            s_currentScene->onSize(width, height);
        }

        return;
    }

    void Scene::_initialize(void)
    {
        if ( !s_initialized )
        {
            glutDisplayFunc(__displayRoutine);
            glutReshapeFunc(__reshapeRoutine);

            s_initialized = true;
        }
        return;
    }
    
    Scene* Scene::setCurrentScene( Scene* pScene )
    {
        if ( pScene != s_currentScene )
        {
            Scene * _tmp = s_currentScene;
            s_currentScene = pScene;
            return _tmp;
        }
        else
        {
            return pScene;
        }
    }
  
    Scene::Scene():
    m_bDoubleBuffers( true ),
    m_defaultDepth( ONE ),
    m_fovy(30.0f),
    m_znear(10.0f),
    m_zfar(250.0f),
    m_width(0.0f),
    m_height(0.0f)
    {
        _initialize();
        return;
    }

    Scene::~Scene()
    {
        using namespace std;

        if ( s_currentScene == this )
        {
            s_currentScene = NULL;
        }

        for ( map<ModelId,Model*>::iterator it = m_models.begin();
            m_models.end() != it; ++ it )
        {
            assert ( NULL != it->second );
            delete it->second;
        }

        m_models.clear();

        return;
    }

    void Scene::moveCameraTo( const dj3d::Vertex& where, const dj3d::Vector& toward, const dj3d::Vector& up )
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        gluLookAt(where.x(), where.y(), where.z(), 
        where.x() + toward.x(), where.y() + toward.y(), where.z() + toward.z(), 
            up.x(), up.y(), up.z());
        glGetFloatv(GL_MODELVIEW_MATRIX, m_viewMatrix);
        glPopMatrix();

        m_needVisibleDetection = true;

        return;
    }

    void Scene::setProjectParam( const f32 fovy, const f32 znear, const f32 zfar )
    {
        m_fovy = fovy;
        m_znear = znear;
        m_zfar = zfar;
        
        if ( m_width > 0.0f && m_height > 0.0f )
        {
           setLens(m_fovy, m_width / m_height, m_znear, m_zfar); 
        }
        return;
    }
    
    void Scene::pickUp( const Vertex& origin, const Vector& dir, std::map<f32, ModelId>& targets )const
    {
        using namespace std;

        Vector dirIdentity = dir;
        dirIdentity.identity();

        f32 distance;

        for ( map<ModelId,Model*>::const_iterator cp = m_models.begin();
            m_models.end() != cp; ++ cp )
        {
            if ( cp->second->isVisible() )
            {
                if (cp->second->hitTest(m_vertex_vec, 
                    origin, dirIdentity, distance))
                {
                    targets.insert(make_pair(distance, cp->first));
                }
            }
        } // for each model

        return;
    }

    void Scene::refresh(void)
    {
          glutPostRedisplay();
          return;
    }

    bool copyModel( const ModelId source, const ModelId dest, const Matrix& mat )
    {
        return false;
    }

    bool Scene::addModel(std::auto_ptr<Model> model)
    {
        using namespace std;
        
        assert( m_models.end() == m_models.find(model->modelId()) );
        
        if ( model->onLoad(*this) )
        {
            ModelId _id = model->modelId();
			m_models[_id] = model.release();
            // m_models.insert( make_pair<ModelId, Model*>( _id, model.release() ) );
            return true;
        }
        else
        {
            return false;
        }
    }
    
    void Scene::setLens( f32 left, f32 top, f32 right, f32 bottom, f32 zNear, f32 zFar, bool bOrtho )
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if ( bOrtho )
        {
            glOrtho(left, right, bottom, top, zNear, zFar);
        }
        else
        {
            glFrustum(left, right, bottom, top, zNear, zFar);
        }

        glGetFloatv(GL_PROJECTION_MATRIX, m_projMatrix);
        m_needVisibleDetection = true;

        return;
    }

    void Scene::setLens( f32 fovy, f32 aspect, f32 zNear, f32 zFar )
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fovy, aspect, zNear, zFar);

        glGetFloatv(GL_PROJECTION_MATRIX, m_projMatrix);
        m_needVisibleDetection = true;

        return;
    }

    void Scene::onLoad(void)
    {
        glClearColor(0.5f, 0.75f, 1.0f, 1.0f );
        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        _initRenderingEnv();

        activateArrays(); 

        return;
    }

    Light* Scene::getLightByIndex(int index)
    {
        Light* _pLight = NULL;
        
        switch(index)
        {
            case 0:
                _pLight = &m_light0;
                break;
            case 1:
                _pLight = &m_light1;
                break;
            case 2:
                _pLight = &m_light2;
                break;
            case 3:
                _pLight = &m_light3;
                break;
            case 4:
                _pLight = &m_light4;
                break;
            case 5:
                _pLight = &m_light5;
                break;
            case 6:
                _pLight = &m_light6;
                break;
            case 7:
                _pLight = &m_light7;
                break;
            default:
                break;
        }

        return _pLight;
    }
    
    bool Scene::installLight(int which, 
                      const dj3d::Color& ambient, 
                      const dj3d::Color& specular, 
                      const dj3d::Color& diffuse, 
                      const dj3d::Vertex& position, 
                      const dj3d::Vector& direction)
    {
        Light* _pLight = getLightByIndex(which);
        if (NULL != _pLight)
        {
            _pLight->ambientColor() = ambient;
            _pLight->diffuseColor() = specular;
            _pLight->specularColor() = diffuse;
            _pLight->where() = position;
            _pLight->direction() = direction;
            
            return true;
        }
        else 
        {
            return false;
        }
    }
    
    bool Scene::openLight(int which)
    {
        Light* _pLight = getLightByIndex(which);
        if ( NULL != _pLight )
        {
            _pLight->turnOn();
            
            return true;
        }
        else 
        {
            return false;
        }
    }
    
    bool Scene::closeLight(int which)
    {
        Light* _pLight = getLightByIndex(which);
        if ( NULL != _pLight )
        {
            _pLight->turnOff();
            
            return true;
        }
        else 
        {
            return false;
        }
    }
    
    void Scene::_visibleDection(void)
    {
        if ( m_needVisibleDetection )
        {
            //...

            m_needVisibleDetection = false;
        }

        return;
    }

    void Scene::setTextureLayer( const u8 layer )
    {
        switch ( layer )
        {
        case 0:
            if ( !m_textcoord_vec0.empty() )
            {
                glClientActiveTexture(GL_TEXTURE0);
                glTexCoordPointer(2, GL_FLOAT, 0, &m_textcoord_vec0[0]);
            }
            break;
        case 1:
            if ( !m_textcoord_vec1.empty() )
            {
                glClientActiveTexture(GL_TEXTURE1);
                glTexCoordPointer(2, GL_FLOAT, 0, &m_textcoord_vec1[0]);
            }
            break;

        case 2:
            if ( !m_textcoord_vec2.empty() )
            {
                glClientActiveTexture(GL_TEXTURE2);
                glTexCoordPointer(2, GL_FLOAT, 0, &m_textcoord_vec2[0]);
            }
            break;

        case 3:
            if ( !m_textcoord_vec3.empty() )
            {
                glClientActiveTexture(GL_TEXTURE3);
                glTexCoordPointer(2, GL_FLOAT, 0, &m_textcoord_vec3[0]);
            }
            break;

        case 4:
            if ( !m_textcoord_vec4.empty() )
            {
                glClientActiveTexture(GL_TEXTURE4);
                glTexCoordPointer(2, GL_FLOAT, 0, &m_textcoord_vec4[0]);
            }
            break;

        case 5:
            if ( !m_textcoord_vec5.empty() )
            {
                glClientActiveTexture(GL_TEXTURE5);
                glTexCoordPointer(2, GL_FLOAT, 0, &m_textcoord_vec5[0]);
            }
            break;

        case 6:
            if ( !m_textcoord_vec6.empty() )
            {
                glClientActiveTexture(GL_TEXTURE6);
                glTexCoordPointer(2, GL_FLOAT, 0, &m_textcoord_vec6[0]);
            }
            break;

        case 7:
            if ( !m_textcoord_vec7.empty() )
            {
                glClientActiveTexture(GL_TEXTURE7);
                glTexCoordPointer(2, GL_FLOAT, 0, &m_textcoord_vec7[0]);
            }
            break;

        default:
            break;
        }
    }

    void Scene::onDraw(void)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // set camera
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(m_viewMatrix);

        _visibleDection();
        renderAll(*this);

        glutSwapBuffers();
        return;
    }

    void Scene::onSize(int width, int height)
    {
        m_width = (f32)width;
        m_height = (f32)height;
        
        glViewport(0, 0, width, height);
        setLens(m_fovy, m_width / m_height, m_znear, m_zfar);

        return;
    }
}; // namespace dj3d
