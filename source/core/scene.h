//
//  source/core/scene.h
//  dj3d
//
//  Created by Xu ZhiXin on 12/31/11.
//  Copyright (c) 2011 doojava. All rights reserved.
//

#ifndef __DJ3D_CORE_SCENE_H__
#define __DJ3D_CORE_SCENE_H__

#include <stack>
#include <map>
#include "rendermgr.h"

namespace dj3d
{
    class CoreBuffer
    {
    public:
        std::vector<Vertex> m_vertex_vec;
        std::vector<Vector> m_normal_vec;
        std::vector<Color> m_color_vec;
        std::vector<TextCoord> m_textcoord_vec0;
        std::vector<TextCoord> m_textcoord_vec1;
        std::vector<TextCoord> m_textcoord_vec2;
        std::vector<TextCoord> m_textcoord_vec3;
        std::vector<TextCoord> m_textcoord_vec4;
        std::vector<TextCoord> m_textcoord_vec5;
        std::vector<TextCoord> m_textcoord_vec6;
        std::vector<TextCoord> m_textcoord_vec7;
    protected:
        bool activateArrays(void);
    }; // class CoreBuffer

   class Scene : public CoreBuffer, public RenderMgr
    {
        friend class RoutineImpl;
    private:
        Scene( const Scene& );
        Scene& operator = ( const Scene& );

    private:
        bool m_bDoubleBuffers; // true if double buffers is used.        
        f32 m_defaultDepth; // 1.0f
        std::map<ModelId, Model*> m_models;
        Matrix m_viewMatrix;
        Matrix m_projMatrix;
        
        f32 m_fovy;
        f32 m_znear;
        f32 m_zfar;
        f32 m_width;
        f32 m_height;

    protected:
        bool m_needVisibleDetection;

    public:
        template<typename TFunc>
        void forEachModel( TFunc& func, bool bVisibleOnly = true )
        {
            using namespace std;
            for ( map<ModelId,Model*>::iterator it = m_models.begin();
                m_models.end() != it; ++ it )
            {
                assert ( NULL != it->second );
                if ( bVisibleOnly && !it->second->isVisible() )
                {
                    continue;
                }
                if ( !func( *(it->second) ) )
                {
                    break;
                }
            }
            return;
        }
  
    public:
        Scene();
        virtual ~Scene();

    public:
        void moveCameraTo( const dj3d::Vertex& where, 
                           const dj3d::Vector& toward, 
                           const dj3d::Vector& up );
        
        void setProjectParam( const f32 fovy, 
                              const f32 znear, 
                              const f32 zfar );
        
        void pickUp( const Vertex& origin, 
                     const Vector& dir, 
                     std::map<f32, ModelId>& targets )const;
        
        void refresh(void);
        
        bool copyModel( const ModelId source, 
                        const ModelId dest, 
                        const Matrix& mat );

        bool addModel(std::auto_ptr<Model> model);
        
        virtual void onDraw(void);
        virtual void onSize(int width, int height);
        
        bool installLight(int which, 
                          const dj3d::Color& ambient, 
                          const dj3d::Color& specular, 
                          const dj3d::Color& diffuse, 
                          const dj3d::Vertex& position, 
                          const dj3d::Vector& direction);
        
        bool openLight(int which);
        bool closeLight(int which);
        
    protected:
        void setLens( f32 left, f32 top, f32 right, f32 bottom, f32 zNear, f32 zFar, bool bOrtho = false );
        // aspect = width / height
        void setLens( f32 fovy, f32 aspect, f32 zNear, f32 zFar );
        
        virtual void _visibleDection(void);
        virtual void setTextureLayer( const u8 layer );

        virtual void onLoad(void);
        
    private:
        static void _initialize();

        Light* getLightByIndex(int index);
        
    public:
        static Scene* setCurrentScene( Scene* pScene );
    }; // class Scene
}; // namespace dj3d

#endif // __DJ3D_CORE_SCENE_H__
