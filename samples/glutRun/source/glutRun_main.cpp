//
//  glutRun_main.cpp
//  glutRun
//
//  Created by Xu ZhiXin on 05/06/12.
//  Copyright (c) 2011 doojava. All rights reserved.
//

#include <iostream>
#include "entry/routine.h"
#include "core/scene.h"
#include "utility/utility.h"
#include <Windows.h>
#if 0
extern "C"{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#endif
typedef dj3d::Matrix _Mat;

class rotateModel
{
private:
    dj3d::Scene& m_scene;
    static dj3d::f32 m_step;
public:
    explicit rotateModel( dj3d::Scene& scene )
    :m_scene(scene)   
    {
        return;
    }
    
    bool operator () ( dj3d::Model& model )
    {
        model.popMatrix();
        model.pushMatrix();
        model *= dj3d::Matrix( dj3d::Vector::yIdentity, m_step );
        model *= dj3d::Matrix( dj3d::Vector::zIdentity, m_step );
        model *= dj3d::Matrix( dj3d::Vector::xIdentity, m_step );
        m_step += 0.01f;
        if ( m_step > 6.28f )
        {
            m_step = 0.0f;
        }
        
        return true;
    }    
};

dj3d::f32 rotateModel::m_step = 0.01f;
//static unsigned int s_TimeInternal = 2;

class MyTimer : public dj3d::ITimerHandler
{
public:
      virtual unsigned int timeIntervalInMilliseconds(void)const
      {
          return 20; // 20ms
      }

      virtual bool onTimer(dj3d::Scene* theScene)
      {
          if ( NULL != theScene )
          {
              rotateModel _rm(*theScene);
              theScene->forEachModel(_rm);
              theScene->refresh();
              return true;
          }

          return false;
      }

      virtual void unloaded(void)
      {
          return;
      }
}; 
#if 0
dj3d::SimpleMemory32 g_memory(1024 * 1024); // 1M

static void* __lua_alloc( void * ud, void * ptr, size_t osize, size_t nsize )
{
	if (0 == nsize)
	{
		free(ptr);
		return NULL;
	}
	else
	{
		return realloc(ptr, nsize);
	}
}
#endif
int main( int argc, char * argv[] )
{
    using namespace std;
#if 0
    cout << "init:" << endl;
    g_memory.dumpAll();
    cout << "alloc 10" << endl;
    void * p1 = g_memory.alloc(10);
	g_memory.dumpAll();
    cout << "alloc 100" << endl;
	void * p2 = g_memory.alloc(100);
	g_memory.dumpAll();
    cout << "alloc 1000" << endl;
	void * p3 = g_memory.alloc(1000);
	g_memory.dumpAll();
    cout << "free 10" << endl;
    g_memory.free( p1 ); 
	g_memory.dumpAll();
    cout << "free 100" << endl;
	g_memory.free( p2 );
	g_memory.dumpAll();
    cout << "free 1000" << endl;
	g_memory.free( p3 );
	g_memory.dumpAll();

   char buffer[256];
	int error = 0;

	lua_State *L = lua_newstate(__lua_alloc, NULL);
	luaL_openlibs(L);
#endif
    dj3d::Routine theApp( argc, argv );
    
    MyTimer _timer;
    theApp.setTimerHandler( &_timer );
    
    _Mat _m(3,4,5);
    cout << _m;
    cout << !_m;
    cout << (_m * !_m);
    theApp.run();
#if 0
    lua_close(L);
    L = NULL;
#endif
    return 0;
}



