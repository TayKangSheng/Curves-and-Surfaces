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

#include "curve.h"
#include "extra.h"
#include <math.h>

using namespace std;

namespace
{
    // Approximately equal to.  We don't want to use == because of
    // precision issues with floating point.
    inline bool approx( const Vector3f& lhs, const Vector3f& rhs )
    {
        const float eps = 1e-8f;
        return ( lhs - rhs ).absSquared() < eps;
    }

    
}
    

Curve evalBezier( const vector< Vector3f >& P, unsigned steps )
{
    // Check
    if( P.size() < 4 || P.size() % 3 != 1 )
    {
        cerr << "evalBezier must be called with 3n+1 control points." << endl;
        exit( 0 );
    }

    // TODO:
    // You should implement this function so that it returns a Curve
    // (e.g., a vector< CurvePoint >).  The variable "steps" tells you
    // the number of points to generate on each piece of the spline.
    // At least, that's how the sample solution is implemented and how
    // the SWP files are written.  But you are free to interpret this
    // variable however you want, so long as you can control the
    // "resolution" of the discretized spline curve with it.

    // Make sure that this function computes all the appropriate
    // Vector3fs for each CurvePoint: V,T,N,B.
    // [NBT] should be unit and orthogonal.

    // Also note that you may assume that all Bezier curves that you
    // receive have G1 continuity.  Otherwise, the TNB will not be
    // be defined at points where this does not hold.

    cerr << "\t>>> evalBezier has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector< Vector3f >): "<< endl;
    for( unsigned i = 0; i < P.size(); ++i )
    {
        cerr << "\t>>> " << P[i] << endl;
    }

    cerr << "\t>>> Steps (type steps): " << steps << endl;
    
    Curve curve = Curve();
    vector<Vector3f> V_array;
    vector<Vector3f> T_array;
    vector<Vector3f> N_array;
    vector<Vector3f> B_array;
    
    for (int step=0 ; step<steps ; step++){
        float t = (step*1.0)/steps;
        float x = (1-t)*(1-t)*(1-t)*P[0][0]
        + (3*t)*(1-t)*(1-t)*P[1][0]
        + (3*t*t)*(1-t)*P[2][0]
        + t*t*t*P[3][0];
        float y = (1-t)*(1-t)*(1-t)*P[0][1]
        + (3*t)*(1-t)*(1-t)*P[1][1]
        + (3*t*t)*(1-t)*P[2][1]
        + t*t*t*P[3][1];
        float z = (1-t)*(1-t)*(1-t)*P[0][2]
        + (3*t)*(1-t)*(1-t)*P[1][2]
        + (3*t*t)*(1-t)*P[2][2]
        + t*t*t*P[3][2];
        V_array.push_back(Vector3f( x, y, z));
        
        float x_prime = ( -3.0*(1-t)*(1-t) )*P[0][0]
        + ( 3-(8*t)+(7*t*t) )*P[1][0]
        + ( (6*t) - (9*t*t) )*(1-t)*P[2][0]
        + ( 4*t*t*t )*P[3][0];
        float y_prime = ( -3.0*(1-t)*(1-t) )*P[0][1]
        + ( 3-(8*t)+(7*t*t) )*P[1][1]
        + ( (6*t) - (9*t*t) )*(1-t)*P[2][1]
        + ( 4*t*t*t )*P[3][1];
        float z_prime = ( -3.0*(1-t)*(1-t) )*P[0][2]
        + ( 3-(8*t)+(7*t*t) )*P[1][2]
        + ( (6*t) - (9*t*t) )*(1-t)*P[2][2]
        + ( 4*t*t*t )*P[3][2];
        T_array.push_back(Vector3f( x_prime, y_prime, z_prime ).normalized());
        
    }
    
    Vector3f B_init = Vector3f(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                               static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                               static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
    
    N_array.push_back(B_init.cross(B_init, T_array[0]));
    B_array.push_back(T_array[0].cross(T_array[0], N_array[0]));
    
    for (int i=1 ; i<V_array.size() ; i++){
        N_array.push_back(B_array[i-1].cross(B_array[i-1], T_array[i]));
        B_array.push_back(T_array[i].cross(T_array[i],N_array[i]));
    }
    
    for (int i=0 ; i<V_array.size() ; i++){
        CurvePoint temp = CurvePoint();
        temp.V = V_array[i];
//        cerr << "\t>>> " << V_array[i][0] << ","<< V_array[i][1] << ","<< V_array[i][2] << "," << endl;
        temp.T = T_array[i];
        temp.N = N_array[i];
        temp.B = B_array[i];
        curve.push_back(temp);
    }
    
    cerr << "\t>>> Returning curve." << endl;
    
    return curve;
}

Vector3f B_init = Vector3f(0,0,1);

Curve evalBspline( const vector< Vector3f >& P, unsigned steps )
{
    // Check
    if( P.size() < 4 )
    {
        cerr << "evalBspline must be called with 4 or more control points." << endl;
        exit( 0 );
    }

    // TODO:
    // It is suggested that you implement this function by changing
    // basis from B-spline to Bezier.  That way, you can just call
    // your evalBezier function.

    cerr << "\t>>> evalBSpline has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector< Vector3f >): "<< endl;
    for( unsigned i = 0; i < P.size(); ++i )
    {
        cerr << "\t>>> " << P[i] << endl;
    }

    cerr << "\t>>> Steps (type steps): " << steps << endl;
    
    Curve curve = Curve();

    for (int piece=0 ; piece<(P.size()-3) ; piece++){
//        cerr << "piece: " << piece << endl;
        vector<Vector3f> temp_V_array;
        vector<Vector3f> temp_T_array;
        vector<Vector3f> temp_N_array;
        vector<Vector3f> temp_B_array;
        
        for (int step=0 ; step<=steps ; step++){
            
            float t = (step*1.0)/steps;
            float x = ( (1.0/6.0)*(1-t)*(1-t)*(1-t) )*P[piece+0][0]
            + ( (1.0/6.0)*(3*t*t*t - 6*t*t + 4) )*P[piece+1][0]
            + ( (1.0/6.0)*(-3*t*t*t + 3*t*t + 3*t + 1) )*P[piece+2][0]
            + ( (1.0/6.0)*t*t*t )*P[piece+3][0];
            float y = ( (1.0/6.0)*(1-t)*(1-t)*(1-t) )*P[piece+0][1]
            + ( (1.0/6.0)*(3*t*t*t - 6*t*t + 4) )*P[piece+1][1]
            + ( (1.0/6.0)*(-3*t*t*t + 3*t*t + 3*t + 1) )*P[piece+2][1]
            + ( (1.0/6.0)*t*t*t )*P[piece+3][1];
            float z = ( (1.0/6.0)*(1-t)*(1-t)*(1-t) )*P[piece+0][2]
            + ( (1.0/6.0)*(3*t*t*t - 6*t*t + 4) )*P[piece+1][2]
            + ( (1.0/6.0)*(-3*t*t*t + 3*t*t + 3*t + 1) )*P[piece+2][2]
            + ( (1.0/6.0)*(t*t*t) )*P[piece+3][2];
            temp_V_array.push_back(Vector3f( x, y, z));
//            cerr << "\t>>> x, y, z:\t" << x << ", \t\t" << y << ", \t\t" << z << endl;
            
            float x_prime = ( (-1.0/2.0)*(1-t)*(1-t) )*P[piece+0][0]
            + ( (1.0/6.0)*(9*t*t - 12*t) )*P[piece+1][0]
            + ( (1.0/6.0)*(-9*t*t+6*t+3) )*P[piece+2][0]
            + ( (1.0/2.0)*t*t )*P[piece+3][0];
            float y_prime = ( (-1.0/2.0)*(1-t)*(1-t) )*P[piece+0][1]
            + ( (1.0/6.0)*(9*t*t - 12*t) )*P[piece+1][1]
            + ( (1.0/6.0)*(-9*t*t+6*t+3) )*P[piece+2][1]
            + ( (1.0/2.0)*t*t )*P[piece+3][1];
            float z_prime = ( (-1.0/2.0)*(1-t)*(1-t) )*P[piece+0][2]
            + ( (1.0/6.0)*(9*t*t - 12*t) )*P[piece+1][2]
            + ( (1.0/6.0)*(-9*t*t+6*t+3) )*P[piece+2][2]
            + ( (1.0/2.0)*t*t )*P[piece+3][2];
            temp_T_array.push_back(Vector3f( x_prime, y_prime, z_prime ).normalized());
        }
        
//        Vector3f B_init = Vector3f(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
//                                   static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
//                                   static_cast <float> (rand()) / static_cast <float> (RAND_MAX));

        temp_N_array.push_back(B_init.cross(B_init,temp_T_array[0]).normalized());
        temp_B_array.push_back(temp_T_array[0].cross(temp_T_array[0],temp_N_array[0]).normalized());
        
        for (int i=1 ; i<temp_V_array.size() ; i++){
            temp_N_array.push_back(temp_B_array[i-1].cross(temp_B_array[i-1],temp_T_array[i]).normalized());
            temp_B_array.push_back(temp_T_array[i].cross(temp_T_array[i],temp_N_array[i]).normalized());
        }
        for (int i=0 ; i<temp_V_array.size() ; i++){
            CurvePoint temp = CurvePoint();
            temp.V = temp_V_array[i];
            temp.T = temp_T_array[i];
            temp.N = temp_N_array[i];
            temp.B = temp_B_array[i];
            curve.push_back(temp);
            B_init = temp.B;
//            cerr << "\t>>> vz, tz, nz:\t" << temp.V[2] << ", \t\t" << temp.T[2] << ", \t\t" << temp.N[2] << endl;
        }
        
    }
    
    cerr << "\t>>> Returning curve." << endl;
    
    return curve;
}

Curve evalCircle( float radius, unsigned steps )
{
    // This is a sample function on how to properly initialize a Curve
    // (which is a vector< CurvePoint >).
    
    // Preallocate a curve with steps+1 CurvePoints
    Curve R( steps+1 );

    // Fill it in counterclockwise
    for( unsigned i = 0; i <= steps; ++i )
    {
        // step from 0 to 2pi
        float t = 2.0f * M_PI * float( i ) / steps;

        // Initialize position
        // We're pivoting counterclockwise around the y-axis
        R[i].V = radius * Vector3f( cos(t), sin(t), 0 );
        
        // Tangent vector is first derivative
        R[i].T = Vector3f( -sin(t), cos(t), 0 );
        
        // Normal vector is second derivative
        R[i].N = Vector3f( -cos(t), -sin(t), 0 );

        // Finally, binormal is facing up.
        R[i].B = Vector3f( 0, 0, 1 );
    }

    return R;
}

void drawCurve( const Curve& curve, float framesize )
{
    // Save current state of OpenGL
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    // Setup for line drawing
    glDisable( GL_LIGHTING ); 
    glColor4f( 1, 1, 1, 1 );
    glLineWidth( 1 );
    
    // Draw curve
    glBegin( GL_LINE_STRIP );
    for( unsigned i = 0; i < curve.size(); ++i )
    {
        glVertex( curve[ i ].V );
    }
    glEnd();

    glLineWidth( 1 );

    // Draw coordinate frames if framesize nonzero
    if( framesize != 0.0f )
    {
        Matrix4f M;

        for( unsigned i = 0; i < curve.size(); ++i )
        {
            M.setCol( 0, Vector4f( curve[i].N, 0 ) );
            M.setCol( 1, Vector4f( curve[i].B, 0 ) );
            M.setCol( 2, Vector4f( curve[i].T, 0 ) );
            M.setCol( 3, Vector4f( curve[i].V, 1 ) );

            glPushMatrix();
            glMultMatrixf( M );
            glScaled( framesize, framesize, framesize );
            glBegin( GL_LINES );
            glColor3f( 1, 0, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 1, 0, 0 );
            glColor3f( 0, 1, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 1, 0 );
            glColor3f( 0, 0, 1 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 0, 1 );
            glEnd();
            glPopMatrix();
        }
    }
    
    // Pop state
    glPopAttrib();
}

