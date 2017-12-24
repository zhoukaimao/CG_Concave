#include <stdio.h>  
#include <stdlib.h>  
#include <GL/glut.h>  

#pragma comment(lib,"glut32.lib") 
void CALLBACK vertexCallback(GLvoid *vertex)
{
	const GLdouble *pointer = (GLdouble *)vertex;
	//glColor3dv(pointer + 3);//在此设置颜色  
	glVertex3dv(pointer);
}
void CALLBACK beginCallback(GLenum which)
{
	glBegin(which);
}
void CALLBACK endCallback()
{
	glEnd();
}
void CALLBACK errorCallback(GLenum errorCode)
{
	const GLubyte *estring;
	estring = gluErrorString(errorCode);
	fprintf(stderr, "Tessellation Error: %s\n", estring);
	exit(0);
}
void CALLBACK combineCallback(GLdouble coords[3],
	GLdouble *vertex_data[4],
	GLfloat weight[4], GLdouble **dataOut)
{
	GLdouble *vertex;
	int i;
	vertex = (GLdouble *)malloc(6 * sizeof(GLdouble));
	vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];
	for (i = 3; i < 7; i++)
	{
		vertex[i] = weight[0] * vertex_data[0][i]
			+ weight[1] * vertex_data[1][i]
			+ weight[2] * vertex_data[2][i]
			+ weight[3] * vertex_data[3][i];
	}
	*dataOut = vertex;
}
GLUtesselator * getTesser(){
	GLUtesselator *tobj = gluNewTess();
	if (!tobj) { return NULL; }

	gluTessCallback(tobj, GLU_TESS_VERTEX, (void (CALLBACK *)())vertexCallback);
	gluTessCallback(tobj, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);
	gluTessCallback(tobj, GLU_TESS_END, (void (CALLBACK *)())endCallback);
	gluTessCallback(tobj, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);
	gluTessCallback(tobj, GLU_TESS_COMBINE, (void (CALLBACK *)())combineCallback);
	//gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_POSITIVE); //GLU_TESS_WINDING_ODD  
	return tobj;
}