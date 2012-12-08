//
//  source/platform/routine.h
//  dj3d
//
//  Created by Xu ZhiXin on 4/20/12.
//  Copyright (c) 2012 doojava. All rights reserved.
//
#ifndef __DJ3D_ROUTINE_H__
#define __DJ3D_ROUTINE_H__

#include "math/3d.h"

namespace dj3d
{
    class Scene;

    class ITimerHandler
    {
    public:
        virtual unsigned int timeIntervalInMilliseconds(void)const = 0;
        virtual bool onTimer(Scene* theScene) = 0;
        virtual void unloaded(void) = 0;
    }; // interface ITimerHandler

    class Routine
    {
    private:
        Routine( const Routine& );
        Routine& operator = ( const Routine& );
    
    private:

       void* m_routineImpl;
  
    public:
        Routine( int argc, char* argv[] );
        
        void run(void);

        void setTimerHandler( ITimerHandler* timerHandler );

        virtual ~Routine(void);
    }; // class Routine
}; // namespace dj3d

#endif // __DJ3D_ROUTINE_H__
