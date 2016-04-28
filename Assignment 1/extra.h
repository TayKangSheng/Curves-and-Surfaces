#ifndef EXTRA_H
#define EXTRA_H

// Code to include the opengl and glut depending on your platform
#ifdef __APPLE__
#include <GLUT/glut.h>
#elif _WIN32
#include <windows.h>
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif
// ----------------------

#include "vecmath.h"

#ifndef M_PI
#define M_PI  3.14159265358979
#endif

// Inline functions to help with drawing
inline void glVertex( const Vector3f& a )
{
    glVertex3fv(a);
}

inline void glNormal( const Vector3f& a ) 
{
    glNormal3fv(a);
}

inline void glLoadMatrix( const Matrix4f& m )
{
    glLoadMatrixf( (Matrix4f) m );
}

inline void glMultMatrix( const Matrix4f& m )
{
    glMultMatrixf( (Matrix4f) m );
}

#endif
