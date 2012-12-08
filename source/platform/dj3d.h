//
//  source/platform/dj3d.h
//  dj3d
//
//  Created by Xu ZhiXin on 4/22/12.
//  Copyright (c) 2011 doojava. All rights reserved.
//

#ifndef __DJ3D_DJ3D_H__
#define __DJ3D_DJ3D_H__

#ifdef _WIN32

#include <GL/glew.h>
#include <GL/glut.h>

#pragma warning (disable:4290)

#else // MAC

#include "GL/glew.h"
#include <GLUT/glut.h>

#endif

#endif // __DJ3D_DJ3D_H__ 
