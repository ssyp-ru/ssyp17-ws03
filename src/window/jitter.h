#include <math.h>
#include <GL/glut.h>
 
#define PI_20 3.14159265358979323846

void accFrustum(GLdouble left,GLdouble right,
                GLdouble bottom,GLdouble top,
                GLdouble zNear,GLdouble zFar,
                GLdouble pixdx,GLdouble pixdy,
                GLdouble eyedx, GLdouble eyedy,
                GLdouble focus)
{
      GLdouble xwsize, ywsize;
      GLdouble dx, dy;
      GLint viewport[4];

      glGetIntegerv(GL_VIEWPORT,viewport);

      xwsize=right-left;
      ywsize=top-bottom;
      dx=-(pixdx*xwsize/(GLdouble)viewport[2] + eyedx*zNear/focus);
      dy=-(pixdy*ywsize/(GLdouble)viewport[3] + eyedy*zNear/focus);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glFrustum(left+dx,right+dx,bottom+dy,top+dy,zNear,zFar);
      glMatrixMode(GL_MODELVIEW);
      glTranslatef(-eyedx,-eyedy,0.0);
}

 

void accPerspective(GLdouble fovy,GLdouble aspect,
                    GLdouble zNear,GLdouble zFar,
                    GLdouble pixdx,GLdouble pixdy,
                    GLdouble eyedx,GLdouble eyedy,
                    GLdouble focus)
{
      GLdouble fov2,left,right,bottom,top;
      fov2=((fovy*PI_20)/180.0)/2.0;

      top=zNear/(cos(fov2)/sin(fov2));
      bottom=-top;
      right=top*aspect;
      left=-right;

      accFrustum(left,right,bottom,top,zNear,zFar,
                 pixdx,pixdy,eyedx,eyedy,focus);
}