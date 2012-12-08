//
//  source/schema/dj3d_parser.cpp
//  dj3d
//
//  Created by Xu ZhiXin on 07/10/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//
#include <assert.h>
#include <stdio.h>
#include <vector>
#include <sstream>

#include "dj3d_parser.h"
#include "core/scene.h"

#ifdef _WIN32
#include "y.tab.c"
#include "lex.yy.c"
#endif // _WIN32

enum LoadingState
{
    LS_SCENE = 1,
    LS_CAMERA = 2,
    LS_LIGHT = 3,
    LS_TEXTURE = 4,
    LS_MODEL = 5,
    LS_RENDER = 6,
    LS_TRIANGES = 7,
    LS_TRIANGLESTRIPS = 8
};

static std::stack<LoadingState> g_stateStack;

static std::vector<dj3d::f32> g_floatList;
static std::vector<dj3d::u32>  g_uintList;

static std::vector<dj3d::Vertex> g_vertexList;
static std::vector<dj3d::Vector> g_vectorList;

static std::ostringstream g_string;

static std::auto_ptr<dj3d::Scene> g_scene;
static std::auto_ptr<dj3d::Model> g_model;

struct CameraDesc
{
    dj3d::f32 fovy;
    dj3d::f32 znear;
    dj3d::f32 zfar;
    
    dj3d::Vertex where;
    dj3d::Vector front;
    dj3d::Vector up;
}; // struct CameraDesc

struct LightDesc
{
    dj3d::u32   id;
    bool        on;
    dj3d::Color ambient;
    dj3d::Color specular;
    dj3d::Color diffuse;
    dj3d::Vertex where;
    dj3d::Vector direction;
}; // struct LightDesc

static CameraDesc   g_camera;
static LightDesc    g_light;
static dj3d::u32    g_textid;
static size_t       g_vertex_base = 0;

extern FILE* yyin;

std::auto_ptr<dj3d::Scene> loadScene( const std::string& strSceneName )
{
    using namespace std;
    
    yyin = fopen( strSceneName.c_str(), "r" );
    if ( yyin )
    {
        yyparse();
        //while( yylex() );
    }
    
    return g_scene;
}

extern "C"
{
    void createScene(unsigned int id)
    {
        assert( NULL  == g_scene.get() );
        assert( g_stateStack.empty() );
        
        g_scene.reset( new dj3d::Scene() );
        g_vertex_base = 0;
        
        g_stateStack.push(LS_SCENE);
 
        printf("create scene with id = %d\n", id);
        
        return;
    }
    
    void submitMatrix(void)
    {
        assert( !g_stateStack.empty() );
        
        switch( g_stateStack.top() )
        {
            case LS_MODEL:
                assert( NULL != g_model.get() );
                assert( 16 == g_floatList.size() );
                
                *((dj3d::Matrix*)g_model.get()) = dj3d::Matrix(&g_floatList[0]);
                
                break;
            default:
                break;
        } // switch
        
        printf("submit matrix : \n");
        
        for (size_t i = 0; i < 16; i ++ )
        {
            printf("%f", g_floatList[i]);
            if ((i+1)%4)
            {
                printf(",");
            }
            else {
                printf("\n");
            }
        } // for
        
        g_floatList.clear();
        
        return;
    }
    
    void createCamera(float fovy, float znear, float zfar)
    {
        assert( !g_stateStack.empty() && LS_SCENE == g_stateStack.top() );
        
        g_stateStack.push(LS_CAMERA);
     
        g_vertexList.clear();
        g_vectorList.clear();
        
        g_camera.fovy = fovy;
        g_camera.znear = znear;
        g_camera.zfar = zfar;
        
        return;
    }
    
    void submitCamera(void)
    {
        assert ( !g_stateStack.empty() && LS_CAMERA == g_stateStack.top() );
        assert ( 1 == g_vertexList.size() && 2 == g_vectorList.size() );
        
        g_camera.where = g_vertexList[0];
        g_vertexList.clear();
        
        g_camera.front = g_vectorList[0];
        g_camera.up = g_vectorList[1];
        g_vectorList.clear();
        
        g_stateStack.pop();
        
        printf("submit camera:\n");
        printf("fovy = %f, znear = %f, zfar = %f\n", g_camera.fovy, g_camera.znear, g_camera.zfar);
        printf("[where] : %f, %f, %f\n", g_camera.where.x(), g_camera.where.y(), g_camera.where.z());
        printf("[toward] : %f, %f, %f\n", g_camera.front.x(), g_camera.front.y(), g_camera.front.z());
        printf("[up] : %f, %f, %f\n", g_camera.up.x(), g_camera.up.y(), g_camera.up.z());
        
        return;
    }
    
    void createLight(unsigned int id, int bon)
    {
        assert( !g_stateStack.empty() && LS_SCENE == g_stateStack.top() );
        
        g_stateStack.push(LS_LIGHT);
        
        g_light.id = id;
        g_light.on = ((0 == bon) ? false : true);
        
        return;
    }
    
    void submitAmbient(void)
    {
        assert( !g_stateStack.empty() && LS_LIGHT == g_stateStack.top() );
        assert( 4 == g_floatList.size() );
        
        g_light.ambient.m_color.color.red = g_floatList[0];
        g_light.ambient.m_color.color.green = g_floatList[1];
        g_light.ambient.m_color.color.blue = g_floatList[2];
        g_light.ambient.m_color.color.alpha = g_floatList[3];
        
        g_floatList.clear();
        
        return;
    }
    
    void submitSpecular(void)
    {
        assert( !g_stateStack.empty() && LS_LIGHT == g_stateStack.top() );
        assert( 4 == g_floatList.size() );
        
        g_light.specular.m_color.color.red = g_floatList[0];
        g_light.specular.m_color.color.green = g_floatList[1];
        g_light.specular.m_color.color.blue = g_floatList[2];
        g_light.specular.m_color.color.alpha = g_floatList[3];
        
        g_floatList.clear();
    }
    
    void submitDiffuse(void)
    {
        assert( !g_stateStack.empty() && LS_LIGHT == g_stateStack.top() );
        assert( 4 == g_floatList.size() );
        
        g_light.diffuse.m_color.color.red = g_floatList[0];
        g_light.diffuse.m_color.color.green = g_floatList[1];
        g_light.diffuse.m_color.color.blue = g_floatList[2];
        g_light.diffuse.m_color.color.alpha = g_floatList[3];
        
        g_floatList.clear();
        
    }
    
    void submitLightPosition(void)
    {
        assert( !g_stateStack.empty() && LS_LIGHT == g_stateStack.top() );
        
        assert( 1 == g_vertexList.size() );
        g_light.where = g_vertexList[0];
        g_vertexList.clear();
        
        return;
    }
    
    void submitLightDirection(void)
    {
        assert( !g_stateStack.empty() && LS_LIGHT == g_stateStack.top() );
        
        assert( 1 == g_vectorList.size() );
        g_light.direction = g_vectorList[0];
        g_vectorList.clear();
        
        return;
    }
    
    void submitLight(void)
    {
        assert( !g_stateStack.empty() && LS_LIGHT == g_stateStack.top() );
        assert( NULL != g_scene.get());
   
        dj3d::Light::enableLighting( true );
        
        if (g_scene->installLight( g_light.id, 
                                   g_light.ambient, 
                                   g_light.specular, 
                                   g_light.diffuse, 
                                   g_light.where, 
                                   g_light.direction ))
        {
            if (g_light.on)
            {
                g_scene->openLight(g_light.id);
            }
            else
            {
                g_scene->closeLight(g_light.id);
            }
        }

        g_stateStack.pop();
        return;
    }

    void createTexture(unsigned int id)
    {
        assert( !g_stateStack.empty() && LS_SCENE == g_stateStack.top() );
        g_string.clear();
        
        g_stateStack.push(LS_TEXTURE);
        
        g_textid = id;
        
        return;
    }
    
    void submitTexture(void)
    {
        assert( !g_stateStack.empty() && LS_TEXTURE == g_stateStack.top() );
        assert( NULL != g_scene.get() );
        
        printf("submit texture: id = %d\nfile = %s\n", g_textid, g_string.str().c_str());
        
        std::string strFile = g_string.str();
        strFile = strFile.substr(1, strFile.length() - 2);
        bool bImage = g_scene->loadTexture(g_textid, dj3d::FilePath(strFile));
        assert(bImage);
        
        g_textid = 0;
        g_string.clear();
        
        g_stateStack.pop();
        
        return;
    }
    
    void createModel(unsigned int id, unsigned int attr)
    {
        assert( NULL != g_scene.get() );
        assert( NULL == g_model.get() );
        assert( !g_stateStack.empty() && LS_SCENE == g_stateStack.top() );
        
        g_model.reset( new dj3d::Model(id) );
        g_vertex_base = g_scene->m_vertex_vec.size();
        g_stateStack.push( LS_MODEL );
        
        printf("create model with id = %d, attr = %d\n", id, attr);
        
        return;
    }
    
    void submitVertex(void)
    {
        assert( 3 == g_floatList.size() );
        
        switch ( g_stateStack.top() )
        {
            case LS_MODEL:
                g_scene->m_vertex_vec.push_back( dj3d::Vertex(g_floatList[0], g_floatList[1], g_floatList[2]) );
                break;
        
            default:
                g_vertexList.push_back( dj3d::Vertex( g_floatList[0], g_floatList[1], g_floatList[2]) );
                break;
        }
        
        printf("vertex : %f, %f, %f\n", g_floatList[0], g_floatList[1], g_floatList[2]);
        
        g_floatList.clear();
        
        return;
    }
    
    void submitVector(void)
    {
        assert( 3 == g_floatList.size() );
        g_vectorList.push_back( dj3d::Vertex( g_floatList[0], g_floatList[1], g_floatList[2]) );
        
        printf("vector : %f, %f, %f\n", g_floatList[0], g_floatList[1], g_floatList[2]);
        
        g_floatList.clear();
        
        return;
    }
    
    void createRenderUnit(unsigned int layer, unsigned int textid, unsigned int op)
    {
        assert( !g_stateStack.empty() && LS_MODEL == g_stateStack.top() );
        assert( NULL != g_model.get() );
        
        g_model->createRenderUnit((dj3d::u8)layer, (dj3d::TextureId)textid, (dj3d::u32)op, 0);
        g_stateStack.push(LS_RENDER);
        
        printf("create render unit with id = %d, textid = %d, op = %d\n", layer, textid, op);
        
        return;
    }
    
    void createTriangleList(unsigned int count)
    {
        assert( !g_stateStack.empty() && LS_RENDER == g_stateStack.top() );
        g_stateStack.push(LS_TRIANGES);
        
        g_uintList.clear();
        
        printf("triangle list start:\n");
        return;
    }
    
    void createTriangleStripList(unsigned int count)
    {
        assert( !g_stateStack.empty() && LS_RENDER == g_stateStack.top() );
        g_stateStack.push(LS_TRIANGLESTRIPS);
        
        printf("triangle strip list start:\n");
        
        return;
    }
    
    void createIndexList(void)
    {
        assert( !g_stateStack.empty() && (LS_TRIANGES == g_stateStack.top() || LS_TRIANGES == g_stateStack.top()) );
        
        g_uintList.clear();
        
        return;
    }
    
    void submitIndexList(void)
    {
        assert( !g_stateStack.empty() && (LS_TRIANGES == g_stateStack.top() || LS_TRIANGES == g_stateStack.top()) );
        
        for (size_t i = 0; i < g_uintList.size(); ++ i )
        {
            g_uintList[i] += static_cast<dj3d::u32>(g_vertex_base);
            assert( g_uintList[i] < g_scene->m_vertex_vec.size() );
        }
        
        printf("submit indexes:\n");
        for (size_t i = 0; i < g_uintList.size(); ++ i)
        {
            printf("%lu", g_uintList[i] - g_vertex_base);
            if (i + 1 < g_uintList.size())
            {
                printf(",");
            }
        }
        printf("\n");
        
        return;
    }
    
    void createTextCoordList(void)
    {
        assert( !g_stateStack.empty() && (LS_TRIANGES == g_stateStack.top() || LS_TRIANGES == g_stateStack.top()) );
        assert( !g_uintList.empty());
        
        return;
    }
    
    void submitTextCoordList(void)
    {
        assert( !g_stateStack.empty() && (LS_TRIANGES == g_stateStack.top() || LS_TRIANGES == g_stateStack.top()) );
        assert( !g_uintList.empty());
        assert( g_floatList.size() == g_uintList.size() * 2 );
        
        // only support text0 for now
        g_scene->m_textcoord_vec0.resize( g_scene->m_vertex_vec.size() );
        
        for (size_t i = 0; i < g_uintList.size(); ++ i )
        {
            g_scene->m_textcoord_vec0[g_uintList[i]] = dj3d::TextCoord(g_floatList[2 * i], g_floatList[2 * i + 1]);
        }
        
        printf("submit textcoords:\n");
        for (size_t i = 0; i < g_floatList.size(); i+=2)
        {
            printf("(%f,%f)", g_floatList[i], g_floatList[i+1]);
            if( i + 2 < g_floatList.size())
            {
                printf(",");
            }
        }
        
        printf("\n");
        
        g_floatList.clear();
        
        return;
    }
    
    void createNormalList(void)
    {
        assert( !g_stateStack.empty() && (LS_TRIANGES == g_stateStack.top() || LS_TRIANGES == g_stateStack.top()) );
        assert( !g_uintList.empty());

        g_vectorList.clear();
        
        return;
    }
    
    void submitNormalList(void)
    {
        assert( !g_stateStack.empty() && (LS_TRIANGES == g_stateStack.top() || LS_TRIANGES == g_stateStack.top()) );
        assert( !g_uintList.empty());
        assert( g_vectorList.size() == g_uintList.size() );
        
        g_scene->m_normal_vec.resize( g_scene->m_vertex_vec.size() );
        
        for (size_t i = 0; i < g_uintList.size(); ++ i)
        {
            g_scene->m_normal_vec[g_uintList[i]] = g_vectorList[i];
        }
        
        printf("submit normals:\n");
        for (size_t i = 0; i < g_vectorList.size(); ++ i)
        {
            printf("vector(%f,%f,%f)\n", g_vectorList[i].x(), g_vectorList[i].y(), g_vectorList[i].z()); 
        }
        printf("\n");
        g_vectorList.clear();
        
        return;
    }
    
    void submitTriangleList(void)
    {
        assert( !g_stateStack.empty() && LS_TRIANGES == g_stateStack.top() );
        
        g_model->addTriangles(g_uintList);
        
        g_uintList.clear();
        
        g_stateStack.pop();
        printf("triangle list end.\n");
        
        return;
    }
    
    void submitTriangleStripList(void)
    {
        assert( !g_stateStack.empty() && LS_TRIANGLESTRIPS == g_stateStack.top() );
        
        g_model->addTriangleStrips(g_uintList);
        
        g_uintList.clear();
        
        g_stateStack.pop();
        printf("triangle strip list end.\n");
        
        return;
    }
    
    void submitRenderUnit(void)
    {
        assert( !g_stateStack.empty() && LS_RENDER == g_stateStack.top() );
        
        g_model->submitRenderUnit( *g_scene );
        
        g_stateStack.pop();
        
        printf("submit render unit.\n");
        
        return;
    }
    
    void submitModel(void)
    {
        assert( !g_stateStack.empty() && LS_MODEL == g_stateStack.top() );
        
        printf("model(id = %d) is submitted.\n", g_model->modelId());
        
        if ( g_model->onLoad( *g_scene ) )
        {
            g_scene->addModel(g_model);
        }
        
        g_stateStack.pop();
        
        return;
    }
    
    void submitScene(void)
    {
        assert( NULL != g_scene.get() );
        assert( !g_stateStack.empty() && LS_SCENE == g_stateStack.top() );
        
        g_scene->moveCameraTo(g_camera.where, g_camera.front, g_camera.up);
        g_scene->setProjectParam(g_camera.fovy, g_camera.znear, g_camera.zfar);
        
        g_stateStack.pop();
        
        printf("scene submitted.\n");
        
        return;
    }
    
    void onFloat(float fVal)
    {
        g_floatList.push_back(fVal);
        return;
    }
    
    void onUint(unsigned int uVal)
    {
        g_uintList.push_back(uVal);
        return;
    }
    
    void onString(const char* str)
    {
        if (NULL != str)
        {
            g_string << str;
        }
    }
} // extern "C"
