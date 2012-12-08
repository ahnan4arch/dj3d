//
//  source/platform/imageloader.h
//  dj3d
//
//  Created by Xu ZhiXin on 1/15/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//

#include "platform/dj3d.h"
#include "core/scene.h"
#include "core/blendop.h"
#include "routine.h"
#include <list>

extern std::auto_ptr<dj3d::Scene> loadScene( const std::string& strSceneName );

namespace dj3d
{
    static void __onTimer(int);
    //---------------------------------------------------------------------
    // RoutineImpl  
    //---------------------------------------------------------------------
    class RoutineImpl
    {
        friend void __onTimer( int );
    
    private:
        RoutineImpl ( const RoutineImpl& );
        RoutineImpl& operator = ( const RoutineImpl& );
    private:
        std::auto_ptr<dj3d::Scene> m_pscene;

        struct _TTimeProc
        {
            u32 m_timeTick;
            ITimerHandler* m_timerHandler;

            explicit _TTimeProc(  ITimerHandler* timeHandler ):
            m_timeTick(0),
                m_timerHandler( timeHandler )
            {
                assert ( NULL != m_timerHandler );
            }

            _TTimeProc( const _TTimeProc & other ):
            m_timeTick ( other.m_timeTick ),
                m_timerHandler( other.m_timerHandler )
            {}

            _TTimeProc& operator = ( const _TTimeProc& other )
            {
                if ( &other != this )
                {
                    m_timeTick = other.m_timeTick;
                    m_timerHandler = other.m_timerHandler;
                }

                return *this;
            }

            bool operator < ( const _TTimeProc& other )const
            {
                assert ( NULL != m_timerHandler && NULL != other.m_timerHandler );
                return ( m_timerHandler->timeIntervalInMilliseconds() < other.m_timerHandler->timeIntervalInMilliseconds() );
            }

            bool operator == ( const _TTimeProc& other ) const
            {
                assert ( NULL != m_timerHandler && NULL != other.m_timerHandler );
                return ( m_timerHandler->timeIntervalInMilliseconds() == other.m_timerHandler->timeIntervalInMilliseconds() );
            }
        }; 

        class __CheckExpiredTimer
        {
        public:
            bool operator () ( const RoutineImpl::_TTimeProc& target ) const
            {
                return (NULL == target.m_timerHandler);
            }
        }; // class __CheckExpiredTimer

        static RoutineImpl* s_Routine;
        static std::list<_TTimeProc> s_timeProcs;
    
        // for test only
        void __loadFakeScene(void);
    public:
        RoutineImpl(void);
        bool loadScene( const std::string& strSceneUri );
        void setTimerHandler( ITimerHandler* timerHandler );
        virtual ~RoutineImpl(void);
    };

    RoutineImpl * RoutineImpl::s_Routine = NULL;
    static const u32 s_minimal_interval = 20; // 20 ms
    std::list<RoutineImpl::_TTimeProc> RoutineImpl::s_timeProcs;

    static void __onTimer(int)
    {
        using namespace std;

        bool _bExpired = false;

        for ( list<RoutineImpl::_TTimeProc>::iterator _it = RoutineImpl::s_timeProcs.begin(); \
            RoutineImpl::s_timeProcs.end() != _it; ++_it )
        {
            assert ( NULL != _it->m_timerHandler );
            _it->m_timeTick += s_minimal_interval;
            if ( _it->m_timeTick >= _it->m_timerHandler->timeIntervalInMilliseconds() )
            {
                if ( !_it->m_timerHandler->onTimer( RoutineImpl::s_Routine->m_pscene.get() ) )
                {
                    _it->m_timerHandler->unloaded();
                    _it->m_timerHandler = NULL;
                    _bExpired = true;
                }
                else
                {
                    _it->m_timeTick = 0;
                }
            }
        }// for

        if ( _bExpired )
        {
            // remove expired time handlers
            RoutineImpl::__CheckExpiredTimer _expiredTimer;
            RoutineImpl::s_timeProcs.remove_if( _expiredTimer );
        }

        if ( NULL !=  RoutineImpl::s_Routine )
        {
            glutTimerFunc( s_minimal_interval, __onTimer, 0 );
        }

        return;
    }

    RoutineImpl::RoutineImpl(void)
    {
        assert (NULL == s_Routine);
        s_Routine = this;

        return;
    }
/*
    <scene id=123>
        <matrix>1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1</matrix>
        <camera>
            <vertex>1,2,3</vertex>
            <vector>3,4,5</vector>
            <vector>7,8,9</vector>
        </camera>
        <texture id=1001>...a.jpg</texture>
        <texture id=1002>...b.png</texture>
        <texture id=1003>...c.jpg</texture>
        <model id=132 attr=xxx>
            <matrix>...</matrix>
            <vertex>...</vertex>
            <vertex>...</vertex>
            <render layer=0 textid=1001 op=1114129>
                <triangles>
                    <index>1,2,3,4,5</index>
                    <textcoord>0.0,1.0, 0.0,1.0, 0.1,0.2, 0.3,04, 0.5,1.0</textcoord>
                    <normal>
                        <vector>1,3,4</vector>
                        <vector>4,5,6</vector>
                        ...
                    </normal>
                </triangles>
                <trianglestrips>
                    <index>1,3,4,5,6,7,8</index>
                    <textcoord>0.0,1.0, 0.0,1.0, 0.1,0.2, 0.3,04, 0.5,1.0</textcoord>
                    <normal>
                        <vector>1,3,2</vector>
                        <vector>4,5,6</vector>
                        ...
                    </normal>    
                </trianglestrips>
            </render>
            <render layer=0 textureid=1002 op=555>
            </render>
        </model>
    </scene>
 */
    // for test only
    void RoutineImpl::__loadFakeScene(void)
    {
        using namespace std;

        //parseScene(NULL);
        // test code begin 
        m_pscene.reset(new Scene());
        Scene* pScene = m_pscene.get();
        assert ( NULL != pScene );

        bool _bImage = false;
#ifdef _WIN32
        _bImage = pScene->loadTexture(1002, FilePath("..\\..\\..\\..\\resource\\images\\freeBSD.png"));
        _bImage = pScene->loadTexture(1001, FilePath("..\\..\\..\\..\\resource\\images\\TangWei.jpg"));
        
#else
        _bImage = pScene->loadTexture(1002, FilePath("/Users/labuser/workspace/dj3d/samples/glutRun/resource/images/freeBSD.png"));
        _bImage = pScene->loadTexture(1001, FilePath("/Users/labuser/workspace/dj3d/samples/glutRun/resource/images/TangWei.jpg"));
#endif
        assert( _bImage );
        // load data
        // for vertexes
        pScene->m_vertex_vec.push_back( Vertex(-1.0f, 1.0f, 1.0f) ); // A - 0
        pScene->m_vertex_vec.push_back( Vertex(1.0f, 1.0f, 1.0f) );  // B - 1
        pScene->m_vertex_vec.push_back( Vertex(1.0f, 1.0f, -1.0f) ); // C - 2
        pScene->m_vertex_vec.push_back( Vertex(-1.0f, 1.0f,  -1.0f) ); // D - 3
        pScene->m_vertex_vec.push_back( Vertex(-1.0f, -1.0f, 1.0f) ); // E - 4
        pScene->m_vertex_vec.push_back( Vertex(1.0f, -1.0f, 1.0f) ); // F - 5
        pScene->m_vertex_vec.push_back( Vertex(1.0f, -1.0f, -1.0f) ); // G - 6
        pScene->m_vertex_vec.push_back( Vertex(-1.0f, -1.0f, -1.0f) ); // H - 7
/*
        // for colors
       pScene->m_color_vec.push_back( Color(1.0f, 0.0f, 0.0f) );
        pScene->m_color_vec.push_back( Color(1.0f, 0.5f, 1.0f) );
        pScene->m_color_vec.push_back( Color(1.0f, 0.3f, 1.0f) );
        pScene->m_color_vec.push_back( Color(1.0f, 0.3f, 1.0f) );
        pScene->m_color_vec.push_back( Color(1.0f, 1.0f, 0.0f) );
        pScene->m_color_vec.push_back( Color(0.5f, 1.0f, 1.0f) );
        pScene->m_color_vec.push_back( Color(0.2f, 0.5f, 0.7f) );
        pScene->m_color_vec.push_back( Color(0.4f, 0.1f, 0.6f) );
*/
        // for normal vectors
        pScene->m_normal_vec.push_back( Vector(Vertex::zero,Vertex(-1.0f, 1.0f, 1.0f)).identity() ); 
        pScene->m_normal_vec.push_back( Vector(Vertex::zero,Vertex(1.0f, 1.0f, 1.0f)).identity() ); 
        pScene->m_normal_vec.push_back( Vector(Vertex::zero,Vertex(1.0f, 1.0f, -1.0f)).identity() );
        pScene->m_normal_vec.push_back( Vector(Vertex::zero,Vertex(-1.0f, 1.0f,  -1.0f)).identity() );
        pScene->m_normal_vec.push_back( Vector(Vertex::zero,Vertex(-1.0f, -1.0f, 1.0f)).identity() );
        pScene->m_normal_vec.push_back( Vector(Vertex::zero,Vertex(1.0f, -1.0f, 1.0f)).identity() );
        pScene->m_normal_vec.push_back( Vector(Vertex::zero,Vertex(1.0f, -1.0f, -1.0f)).identity() );
        pScene->m_normal_vec.push_back( Vector(Vertex::zero,Vertex(-1.0f, -1.0f, -1.0f)).identity() );

        // for 2D texture, layer 0
        pScene->m_textcoord_vec0.push_back( TextCoord(0.0f, 0.0f) ); // A 0
        pScene->m_textcoord_vec0.push_back( TextCoord(1.0f, 0.0f) ); // B 1
        pScene->m_textcoord_vec0.push_back( TextCoord(0.0f, 0.0f) ); // C 2
        pScene->m_textcoord_vec0.push_back( TextCoord(1.0f, 0.0f) ); // D 3
        pScene->m_textcoord_vec0.push_back( TextCoord(0.0f, 1.0f) ); // E 4
        pScene->m_textcoord_vec0.push_back( TextCoord(1.0f, 1.0f) ); // F 5
        pScene->m_textcoord_vec0.push_back( TextCoord(0.0f, 1.0f) ); // G 6
        pScene->m_textcoord_vec0.push_back( TextCoord(1.0f, 1.0f) ); // H 7


        Model* pModel = new Model(1234);

        pModel->createRenderUnit(0, 1001, 
            SRC_RGB_ONE|SRC_ALPHA_ONE|RGB_FUNC_ADD|ALPHA_FUNC_ADD, 
            0);

        vector<u32> _vi;

        _vi.push_back(0);_vi.push_back(4);_vi.push_back(1);
        _vi.push_back(1);_vi.push_back(4);_vi.push_back(5);
        _vi.push_back(1);_vi.push_back(5);_vi.push_back(2);
        _vi.push_back(2);_vi.push_back(5);_vi.push_back(6);
        _vi.push_back(3);_vi.push_back(0);_vi.push_back(1);
        _vi.push_back(3);_vi.push_back(1);_vi.push_back(2);
        _vi.push_back(0);_vi.push_back(3);_vi.push_back(7);
        _vi.push_back(0);_vi.push_back(7);_vi.push_back(4);
        _vi.push_back(5);_vi.push_back(7);_vi.push_back(6);
        _vi.push_back(4);_vi.push_back(7);_vi.push_back(5);
        _vi.push_back(2);_vi.push_back(6);_vi.push_back(7);
        _vi.push_back(2);_vi.push_back(7);_vi.push_back(3);

        pModel->addTriangles(_vi);
        pModel->submitRenderUnit(*pScene);

        assert( pScene->m_models.end() == pScene->m_models.find(pModel->modelId()) );

        if ( pModel->onLoad(*pScene) )
        {
            pScene->m_models.insert( make_pair<ModelId,Model*>(pModel->modelId(), pModel));
        }
        else
        {
            delete pModel;
        }

        pModel = NULL;

        Vertex where(0.0f, 3.0f, 12.0f );
        pScene->moveCameraTo( where, 
                     Vector(where, Vertex::zero),
                     dj3d::Vector::yIdentity );
        
   
        pScene->setProjectParam(30, 10, 250);
        // test code end
    }

    bool RoutineImpl::loadScene( const std::string& strSceneUri )
    {
        using namespace std;

        assert ( NULL == m_pscene.get() );

        m_pscene = ::loadScene(strSceneUri);
        if ( NULL == m_pscene.get() )
        {
            return false;
        }

        if ( NULL != m_pscene.get() )
        {
            m_pscene->onLoad();
            dj3d::Scene::setCurrentScene( m_pscene.get() );
            return true;
        }
        else
        {
            return false;
        }
    }

    void RoutineImpl::setTimerHandler( ITimerHandler* timerHandler )
    {
        if ( NULL != timerHandler )
        {
            s_timeProcs.push_back( RoutineImpl::_TTimeProc(timerHandler) );
            s_timeProcs.sort();
        }
        return;
    }

    RoutineImpl::~RoutineImpl(void)
    {
        dj3d::Scene::setCurrentScene( NULL );
        s_Routine = NULL;
        return;
    }

    Routine::Routine( int argc, char* argv[] ):
    m_routineImpl(NULL)
    {
        glutInit(&argc, (char **)argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
        glutInitWindowPosition(300, 50);
        glutInitWindowSize(800, 600);
        glutCreateWindow("my glut window");
        glClearColor(0.0f, 0, 0, 0);
        glClearDepth(1.0f);

        if ( GLEW_OK == glewInit() )
        {
            m_routineImpl = new RoutineImpl();        
            ((RoutineImpl*)m_routineImpl)->loadScene("..\\Debug\\example.xml");

            glutTimerFunc( s_minimal_interval, __onTimer, 0 );
        }
    
        return;
    }

    void Routine::run(void)
    {
        if ( NULL != m_routineImpl )
        {
            glutMainLoop();
        }
        return;
    }

    void Routine::setTimerHandler( ITimerHandler* timerHandler )
    {
        assert ( NULL != m_routineImpl );
        ((RoutineImpl*)m_routineImpl)->setTimerHandler( timerHandler );
        return;
    }

    Routine::~Routine(void)
    {
        if ( NULL != m_routineImpl)
        {
            delete (RoutineImpl*)m_routineImpl;
            m_routineImpl = NULL;
        }
        return;
    }

}; // namespace dj3d
