#include <stdio.h>  
#include <stdlib.h>  
#include <GL/glut.h>  

#pragma comment(lib,"glut32.lib")  
#include "Tess.h"
#include "Brep.h"
using namespace std;
Brep::Solid* s;
//GLdouble quad[12][3] = {
//	{ -2, 3, 0 }, { -2, 0, 0 }, { 2, 0, 0 }, { 2, 3, 0 },
//	{ -1, 2, 0 }, { -1, 1, 0 }, { 1, 1, 0 }, { 1, 2, 0 },
//	{ -0.5, 1, 0 }, { -0.5, 2, 0 }, { 0.5, 2, 0 }, { 0.5, 1, 0 } };
GLdouble quad[12][3] = {
	{ -1, 1, 0 }, { 1, 1, 0 }, { 1, -1, 0 }, { -1, -1, 0 }, { -0.5, 0.5, 0 }, { 0.5, 0.5, 0 }, { 0.5, -0.5, 0 }, { -0.5, -0.5, 0 }, { 0.8, 0.1, 0 }, { 0.8, -0.1, 0 }, { 0.7, -0.1, 0 }, { 0.7, 0.1, 0 }
};

void myIdle(void)
{
	glutPostRedisplay();
}
//------------------------------------------------------------  OnDraw()  
//  
//void CALLBACK vertexCallback(GLvoid *vertex)
//{
//	const GLdouble *pointer = (GLdouble *)vertex;
//	//glColor3dv(pointer + 3);//在此设置颜色  
//	glVertex3dv(pointer);
//}
//void CALLBACK beginCallback(GLenum which)
//{
//	glBegin(which);
//}
//void CALLBACK endCallback()
//{
//	glEnd();
//}
//void CALLBACK errorCallback(GLenum errorCode)
//{
//	const GLubyte *estring;
//	estring = gluErrorString(errorCode);
//	fprintf(stderr, "Tessellation Error: %s\n", estring);
//	exit(0);
//}
//void CALLBACK combineCallback(GLdouble coords[3],
//	GLdouble *vertex_data[4],
//	GLfloat weight[4], GLdouble **dataOut)
//{
//	GLdouble *vertex;
//	int i;
//	vertex = (GLdouble *)malloc(6 * sizeof(GLdouble));
//	vertex[0] = coords[0];
//	vertex[1] = coords[1];
//	vertex[2] = coords[2];
//	for (i = 3; i < 7; i++)
//	{
//		vertex[i] = weight[0] * vertex_data[0][i]
//			+ weight[1] * vertex_data[1][i]
//			+ weight[2] * vertex_data[2][i]
//			+ weight[3] * vertex_data[3][i];
//	}
//	*dataOut = vertex;
//}

bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX, mouseY;
float cameraDistanceX;
float cameraDistanceY;
float cameraAngleX;
float cameraAngleY;
float times = 1;

void mouseCB(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;
	times = 1;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP)
			mouseLeftDown = false;
	}

	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseRightDown = true;
		}
		else if (state == GLUT_UP)
			mouseRightDown = false;
	}

	/*
	* 鼠标滚轮控制图形缩放
	*/
	//else if (state == GLUT_UP && button == GLUT_WHEEL_UP)
	//{
	//	times = 0.008f + 1;
	//	glutPostRedisplay();
	//}

	//else if (state == GLUT_UP && button == GLUT_WHEEL_DOWN)
	//{
	//	times = -0.008f + 1;
	//	glutPostRedisplay();
	//}
}

void mouseMotionCB(int x, int y)
{
	cameraAngleX = cameraAngleY = 0;
	cameraDistanceX = cameraDistanceY = 0;

	if (mouseLeftDown)
	{
		cameraAngleY += (x - mouseX) * 1.0f;
		cameraAngleX += (y - mouseY) * 1.0f;
		mouseX = x;
		mouseY = y;
	}
	if (mouseRightDown)
	{
		cameraDistanceX = (x - mouseX) * 0.002f;
		cameraDistanceY = -(y - mouseY) * 0.002f;
		mouseY = y;
		mouseX = x;
	}

	glutPostRedisplay();
}
bool wireframe = true;


float angle = 0;
float colors[8][3] = { { 0.5,0.2,0.3 }, { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 1, 1, 0 }, { 1, 0, 1 }, { 0, 1, 1 }, { 0.8,0.7,0.3 } };
void OnDraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// clear the screen & depth buffer  
	glClear(GL_COLOR_BUFFER_BIT);
	//glScalef(1.1, 1.1, 1.1);//缩放
	//glTranslatef(cameraDistanceX, cameraDistanceY, -20);
	//glLoadIdentity();
	
	glRotatef(cameraAngleX, 1, 0, 0);
	glRotatef(cameraAngleY, 0, 1, 0); 
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	// clear the previous transform  
	//glLoadIdentity();	
	
	//glLoadIdentity();
	////angle += 5;
	//glTranslatef(0, 0, -10);
	//glRotatef(45, 0, 0, 1);
	//glRotatef(90,0,1,0);

	if(wireframe)glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glColor3d(1.0, 0.5, 0.3);
	//glRotatef(45, 1, 1, 0);
	//GLUtesselator *tobj = gluNewTess();
	//if (!tobj) { return; }

	//gluTessCallback(tobj, GLU_TESS_VERTEX, (void (CALLBACK *)())vertexCallback);
	//gluTessCallback(tobj, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);
	//gluTessCallback(tobj, GLU_TESS_END, (void (CALLBACK *)())endCallback);
	//gluTessCallback(tobj, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);
	//gluTessCallback(tobj, GLU_TESS_COMBINE, (void (CALLBACK *)())combineCallback);
	
	// glShadeModel(GL_FLAT);  

	//gluTessProperty(tobj,GLU_TESS_WINDING_RULE,GLU_TESS_WINDING_POSITIVE); //GLU_TESS_WINDING_ODD  

	GLUtesselator *tobj = getTesser();
	//Brep* brep = new Brep();
	//s = brep->cube(2,2,2);
	//s = brep->hole(2,2,1,1);
	//s = brep->demo1();
	//s = brep->demo2();
	//s = brep->demo3();
	//s = brep->demo4();
	Brep::Face* f = s->sface; //f = f->nextf->nextf;
	while (f != NULL){
		gluTessBeginPolygon(tobj, NULL);//begin polygon
		//cout << ">>Begin Face..." << endl;
		Brep::Loop* lp = f->floop;
		while (lp != NULL){
			int idx = rand()%8;
			//glColor3f(colors[idx][0], colors[idx][1], colors[idx][2]);
			//cout << ">>Begin Loop..." << endl;
			gluTessBeginContour(tobj);//begin loop
			Brep::HalfEdge* he = lp->ledge;
			Brep::Vertex* startv = he->startv;
			do
			{
				double* pos = he->startv->position.data;
				//cout <<"point"<<he->startv->ID <<" "<< "position: " << pos[0] << " " << pos[1] << " " << pos[2] << endl;
				gluTessVertex(tobj, he->startv->position.data, he->startv->position.data);
				he = he->next;
			} while (he->startv != startv);
			gluTessEndContour(tobj);
			lp = lp->nextl;
		}
		gluTessEndPolygon(tobj);
		f = f->nextf;
		//break;
	}

	//gluTessBeginPolygon(tobj, NULL);//begin polygon
	//gluTessBeginContour(tobj);
	//gluTessVertex(tobj, quad[0], quad[0]);
	//gluTessVertex(tobj, quad[1], quad[1]);
	//gluTessVertex(tobj, quad[2], quad[2]);
	//gluTessVertex(tobj, quad[3], quad[3]);
	//gluTessEndContour(tobj);

	//gluTessBeginContour(tobj);                      // inner quad (hole)  
	//gluTessVertex(tobj, quad[4], quad[4]);
	//gluTessVertex(tobj, quad[5], quad[5]);
	//gluTessVertex(tobj, quad[6], quad[6]);
	//gluTessVertex(tobj, quad[7], quad[7]);
	//gluTessEndContour(tobj);

	//gluTessBeginContour(tobj);                      // inner quad (hole)  
	//gluTessVertex(tobj, quad[8], quad[8]);
	//gluTessVertex(tobj, quad[9], quad[9]);
	//gluTessVertex(tobj, quad[10], quad[10]);
	//gluTessVertex(tobj, quad[11], quad[11]);
	//gluTessEndContour(tobj);

	//gluTessEndPolygon(tobj);//begin polygon

	gluDeleteTess(tobj);
	glutSwapBuffers();

}
Brep::Solid* examples[4];
void OnKeyPress(unsigned char key, GLint x, GLint y)
{
	switch (key){
	case 's':wireframe = !wireframe; break;
	case '1':s = examples[0]; break;
	case '2':s = examples[1]; break;
	case '3':s = examples[2]; break;
	case '4':s = examples[3]; break;
	default:break;
	}
	OnDraw();
}
void SpecialKey(unsigned char key, GLint x, GLint y)
{
	if (key == GLUT_KEY_DOWN){
		wireframe = !wireframe;
	}
	OnDraw();
}
//------------------------------------------------------------  OnInit()  
//  
void OnInit()
{
	glShadeModel(GL_SMOOTH);
	// 红色的环境光 ， 蓝色的漫反射光。效果就是 空间都是红色的光但是物体上面反射出蓝色的光

	GLfloat light_position[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	GLfloat light_diffuse[] = { 0.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); //漫反射光是在基本光基础上反射的一种光  

	GLfloat light_ambient[] = { 1.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_diffuse);//环境光,就是基本光，如白天亮了的光  

	GLfloat light_specular[] = { 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_diffuse);//高光  

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	Brep* brep = new Brep();
	examples[0] = brep->demo1();
	examples[1] = brep->demo2();
	examples[2] = brep->demo3();
	examples[3] = brep->demo4();
	s = examples[3];

	cout << "1.press s to switch render mode" << endl;
	cout << "2.press 1, 2, 3, 4 to choose examples" << endl;
	cout << "3.using mouse to rotate" << endl;
}
//------------------------------------------------------------  OnExit()  
//  
void OnExit()
{
}
//------------------------------------------------------------  OnReshape()  
//  
void OnReshape(int w, int h)
{
	// prevents division by zero when minimising window  
	if (h == 0)
	{
		h = 1;
	}

	// set the drawable region of the window  
	glViewport(0, 0, w, h);

	// set up the projection matrix  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// just use a perspective projection  
	//gluPerspective(45,(float)w/h,0.1,100);  
	if (w <= h)
	{
		glOrtho(-4.0, 4.0, -4.0 * (GLfloat)h / (GLfloat)w, 4.0 * (GLfloat)h / (GLfloat)w, 0.0, 100.0);
	}
	else
	{
		glOrtho(-4.0, 4.0, -4.0 * (GLfloat)h / (GLfloat)w, 4.0 * (GLfloat)h / (GLfloat)w, 0.0, 100.0);
	}

	// go back to model view matrix so we can move the objects about  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -50);
	glScalef(0.5, 0.5, 0.5);
}
//------------------------------------------------------------  main()  
//  
int main(int argc, char** argv)
{
	// initialize glut  
	glutInit(&argc, argv);
	// request a depth buffer, RGBA display mode, and we want double buffering  
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	// set the initial window size  
	glutInitWindowSize(480, 480);
	// create the window  
	glutCreateWindow("filling");
	// run our custom initialisation  
	OnInit(); //glLoadIdentity(); glTranslatef(0, 0, -10);
	// set the function to use to draw our scene  
	glutDisplayFunc(OnDraw);
	// set the function to handle changes in screen size  
	glutReshapeFunc(OnReshape);
	//  glutIdleFunc(&myIdle);  
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);
	//glutSpecialFunc(&SpecialKey);
	glutKeyboardFunc(&OnKeyPress);
	// set the function to be called when we exit  
	atexit(OnExit);

	// this function runs a while loop to keep the program running.  
	glutMainLoop();

	return 0;
}