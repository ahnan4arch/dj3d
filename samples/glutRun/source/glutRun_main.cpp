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
#include <tchar.h>

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

int main( int argc, char * argv[] )
{
    using namespace std;
	TCHAR szPath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH, szPath);
#ifdef _DEBUG
	_tcscat(szPath, _T("\\..\\Debug"));
#else
	_tcscat(szPath, _T("\\..\\Release"));
#endif
	::SetCurrentDirectory(szPath);

    dj3d::Routine theApp( argc, argv );
    
    MyTimer _timer;
    theApp.setTimerHandler( &_timer );
    
    _Mat _m(3,4,5);
    cout << _m;
    cout << !_m;
    cout << (_m * !_m);
    theApp.run();

    return 0;
}



