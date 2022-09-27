#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <vector>
#include <windows.h>
#include <glut.h>


using namespace std;

#define eps 0.00001
#define WINDOW_WIDTH 700.0
#define WINDOW_HEIGHT 700.0
#define pi (2*acos(0.0))

#define MAX_VAL 9999999
#define MIN_VAL -9999999

double rad;
double angle;

class Point
{
public:
	double x,y,z;

	Point(){
	}
	Point(double a, double b, double c){
        x = a;
        y = b;
        z = c;
	}
	~Point(){}

	double dot(Point v){
        return x*v.x + y*v.y + z*v.z;
	}

	Point operator+(Point pt) {
        return Point(x + pt.x, y + pt.y, z + pt.z);
    }

    Point operator-(Point pt) {
        return Point(x - pt.x, y - pt.y, z - pt.z);
    }

    Point operator*(double v) {

        return Point(x*v, y*v, z*v);
    }

    Point operator*(Point v){
        return Point(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
    }

    Point operator/(double pt) {

        return Point(x/pt, y/pt, z/pt);
    }


    Point normalize() {
        return *this / sqrt(x*x + y*y + z*z);
    }




};

Point pos(0, 0, -250);
Point u(0,1,0);
Point r(1, 0, 0);
Point l(0, 0, 1);
Point temp(0,0,0);
double tempVal;
int drawaxes;
int  flag;

void drawSphere(double radius, int slices, int stacks)
{
	Point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{

		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawCircle(double radius,int segments)
{
    int i;
    Point points[100];
    //glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawAxes()
{
	if(drawaxes==1)
	{
		glBegin(GL_LINES);{
		    glColor3f(0.0, 1.0, 0.0);
			glVertex3f( 200,0,0);
			glVertex3f(-200,0,0);
            glColor3f(1.0, 0.0, 0.0);
			glVertex3f(0,-200,0);
			glVertex3f(0, 200,0);
            glColor3f(0.0, 0.0, 1.0);
			glVertex3f(0,0, 200);
			glVertex3f(0,0,-200);
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}

/*
1 - rotate/look left
2 - rotate/look right
3 - look up
4 - look down
5 - tilt clockwise
6 - tilt counterclockwise
*/
void press1(){
            temp.x = l.x*cos(pi/90) - r.x*sin(pi/90);
            temp.y = l.y*cos(pi/90) - r.y*sin(pi/90);
            temp.z = l.z*cos(pi/90) - r.z*sin(pi/90);
            tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x /= tempVal;
            temp.y /= tempVal;
            temp.z /= tempVal;

            r.x = r.x*cos(pi/90) + l.x*sin(pi/90);
            r.y = r.y*cos(pi/90) + l.y*sin(pi/90);
            r.z = r.z*cos(pi/90) + l.z*sin(pi/90);
            tempVal = sqrt(r.x*r.x + r.y*r.y + r.z*r.z);
            r.x /= tempVal;
            r.y /= tempVal;
            r.z /= tempVal;
            l.x = temp.x;
            l.y = temp.y;
            l.z = temp.z;
}

void press2(){
            temp.x = l.x*cos(-pi/90) - r.x*sin(-pi/90);
            temp.y = l.y*cos(-pi/90) - r.y*sin(-pi/90);
            temp.z = l.z*cos(-pi/90) - r.z*sin(-pi/90);
            tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x /= tempVal;
            temp.y /= tempVal;
            temp.z /= tempVal;

            r.x = r.x*cos(-pi/90) + l.x*sin(-pi/90);
            r.y = r.y*cos(-pi/90) + l.y*sin(-pi/90);
            r.z = r.z*cos(-pi/90) + l.z*sin(-pi/90);
            tempVal = sqrt(r.x*r.x + r.y*r.y + r.z*r.z);
            r.x /= tempVal;
            r.y /= tempVal;
            r.z /= tempVal;
            l.x = temp.x;
            l.y = temp.y;
            l.z = temp.z;
}

void press3(){
            temp.x = l.x*cos(pi/90) + u.x*sin(pi/90);
            temp.y = l.y*cos(pi/90) + u.y*sin(pi/90);
            temp.z = l.z*cos(pi/90) + u.z*sin(pi/90);
            tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x /= tempVal;
            temp.y /= tempVal;
            temp.z /= tempVal;

            u.x = u.x*cos(pi/90) - l.x*sin(pi/90);
            u.y = u.y*cos(pi/90) - l.y*sin(pi/90);
            u.z = u.z*cos(pi/90) - l.z*sin(pi/90);
            tempVal = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
            u.x /= tempVal;
            u.y /= tempVal;
            u.z /= tempVal;
            l.x = temp.x;
            l.y = temp.y;
            l.z = temp.z;

}

void press4(){
            temp.x = l.x*cos(-pi/90) + u.x*sin(-pi/90);
            temp.y = l.y*cos(-pi/90) + u.y*sin(-pi/90);
            temp.z = l.z*cos(-pi/90) + u.z*sin(-pi/90);
            tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x /= tempVal;
            temp.y /= tempVal;
            temp.z /= tempVal;

            u.x = u.x*cos(-pi/90) - l.x*sin(-pi/90);
            u.y = u.y*cos(-pi/90) - l.y*sin(-pi/90);
            u.z = u.z*cos(-pi/90) - l.z*sin(-pi/90);
            tempVal = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
            u.x /= tempVal;
            u.y /= tempVal;
            u.z /= tempVal;
            l.x = temp.x;
            l.y = temp.y;
            l.z = temp.z;
}

void press5(){
            temp.x = r.x*cos(pi/90) - u.x*sin(pi/90);
            temp.y = r.y*cos(pi/90) - u.y*sin(pi/90);
            temp.z = r.z*cos(pi/90) - u.z*sin(pi/90);
            tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x /= tempVal;
            temp.y /= tempVal;
            temp.z /= tempVal;

            u.x = u.x*cos(pi/90) + r.x*sin(pi/90);
            u.y = u.y*cos(pi/90) + r.y*sin(pi/90);
            u.z = u.z*cos(pi/90) + r.z*sin(pi/90);
            tempVal = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
            u.x /= tempVal;
            u.y /= tempVal;
            u.z /= tempVal;
            r.x = temp.x;
            r.y = temp.y;
            r.z = temp.z;
}

void press6(){

    temp.x = r.x*cos(-pi/90) - u.x*sin(-pi/90);
    temp.y = r.y*cos(-pi/90) - u.y*sin(-pi/90);
    temp.z = r.z*cos(-pi/90) - u.z*sin(-pi/90);
    tempVal = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
    temp.x /= tempVal;
    temp.y /= tempVal;
    temp.z /= tempVal;


    u.x = u.x*cos(-pi/90) + r.x*sin(-pi/90);
    u.y = u.y*cos(-pi/90) + r.y*sin(-pi/90);
    u.z = u.z*cos(-pi/90) + r.z*sin(-pi/90);
    tempVal = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
    u.x /= tempVal;
    u.y /= tempVal;
    u.z /= tempVal;
    r.x = temp.x;
    r.y = temp.y;
    r.z = temp.z;
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case '1':
            press2();
			break;
        case '2':
            press1();
			break;
        case '3':
            press3();
			break;
        case '4':
            press4();
			break;
        case '5':
            press5();
			break;
        case '6':
            press6();
			break;
        case 'r':{
            int rotationValue = 0;
            cout << "Enter rotation value: ";
            cin >> rotationValue;
            for(int i = 0; i< rotationValue; i++){
                press6();
                }
            }
            break;
		default:
			break;
	}
}

/*
up arrow - move forward
down arrow - move backward
right arrow - move right
left arrow - move left
PgUp - move up
PgDn - move down
*/
void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			pos.x -= 2.0*l.x;
			pos.y -= 2.0*l.y;
			pos.z -= 2.0*l.z;
			break;
		case GLUT_KEY_UP:		// up arrow key
			pos.x += 2.0*l.x;
			pos.y += 2.0*l.y;
			pos.z += 2.0*l.z;
			break;
		case GLUT_KEY_RIGHT:
			pos.x += 2.0*r.x;
			pos.y += 2.0*r.y;
			pos.z += 2.0*r.z;
			break;
		case GLUT_KEY_LEFT:
			pos.x -= 2.0*r.x;
			pos.y -= 2.0*r.y;
			pos.z -= 2.0*r.z;
			break;
		case GLUT_KEY_PAGE_UP:
		    pos.x += 2.0*u.x;
			pos.y += 2.0*u.y;
			pos.z += 2.0*u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
		    pos.x -= 2.0*u.x;
			pos.y -= 2.0*u.y;
			pos.z -= 2.0*u.z;
			break;
		case GLUT_KEY_INSERT:
			break;
		case GLUT_KEY_HOME:
           /* squareLength = (squareLength/2) > 0? squareLength-2:squareLength;
            circleRadius = circleRadius < 19? circleRadius+1:circleRadius;*/
			break;
		case GLUT_KEY_END:
		    /*squareLength = (squareLength/2) < 16? squareLength+2:squareLength;
            circleRadius = circleRadius > 0? circleRadius-1:circleRadius;*/
			break;
		default:
			break;
	}
}

void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
				flag = 1-flag;
				//cout << "drawaxes value: " << drawaxes << endl;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			//........
			break;
		case GLUT_MIDDLE_BUTTON:
			//........
			break;
		default:
			break;
	}
}

//draw solid fill circle
void drawFillCircle(int radius, int segments){
    int i;
    Point points[100];
    //glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,0);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

//draw a Bangladesh National flag
void drawBdFlag(){
    //wide:height = 10:6
    //circle radius = 2/10 parts of wide
    //circle center = middle of height and 9/20 of the width of the flag
    glTranslatef(150, -20, 0);
    int zoomVal = 20;
    rad = 2;

    //circle shape of the flag
    glTranslated(-4.5*zoomVal, 3*zoomVal, 0);
    glColor3f(0.74, 0.13, 0.16);
    drawFillCircle(rad*zoomVal,80);

    //back to the centre
    glTranslated(4.5*zoomVal, -3*zoomVal, 0);

    //quads shape of flag
    glBegin(GL_QUADS);
    glColor3f(0, 0.41, 0.31);
    glVertex3f(0*zoomVal,0*zoomVal,0);
    glVertex3f(-10*zoomVal,0*zoomVal, 0);
    glVertex3f(-10*zoomVal, 6*zoomVal, 0);
    glVertex3f(0*zoomVal, 6*zoomVal, 0);
    glEnd();

    //drawSphere(rad*zoomVal, 80, 1);
}

void drawStripes(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, int nStrides){
   Point p1, p2, p3, p4, v1, v2;
   p1.x = x1;
   p1.y = y1;
   p1.z = 0;
   p2.x = x2;
   p2.y = y2;
   p2.z = 0;
   //vector 1
   v1.x = p2.x-p1.x;
   v1.y = p2.y - p1.y;
   v1.z = 0;

   p3.x = x3;
   p3.y = y3;
   p3.z = 0;
   p4.x = x4;
   p4.y = y4;
   p4.z = 0;

   //vector 2
   v2.x = p4.x - p3.x;
   v2.y = p4.y - p3.y;

   glVertex3f(p1.x, p1.y, p1.z);
    glVertex3f(p3.x, p3.y, p3.z);
   double division =  1.0/(double)nStrides;
   for(int i = 0; i < nStrides ;i++){
    //new point find
    p1.x += (division*v1.x);
    p1.y += (division*v1.y);

    //new point find
    p3.x += (division*v2.x);
    p3.y += (division*v2.y);
    glVertex3f(p1.x, p1.y, p1.z);
    glVertex3f(p3.x, p3.y, p3.z);
    //cout << p1.x << "," << p1.y << " " << p3.x << " " << p3.y  << endl;
   }
    return;
}

/*
-3,52 -97, 52
-6,54 -94, 54
-9,56 -91, 56
-12,58 -88, 58
-15,60 -85, 60

*/
void drawHome(){

    glTranslated(150, 0, 0);

    glBegin(GL_LINES);{
    glColor3f(1, 1,1);

    //door wall lines
    glVertex3f(0,0,0);
    glVertex3f(-30, 0, 0);

    glVertex3f(-30, 0, 0);
    glVertex3f(-30, 50, 0);

    glVertex3f(-30, 50, 0);
    glVertex3f(0, 50, 0);

    glVertex3f(0, 50, 0);
    glVertex3f(0, 0, 0);

    //door lines
    glVertex3f(-10, 0, 0);
    glVertex3f(-10, 25, 0);

    glVertex3f(-20, 0, 0);
    glVertex3f(-20, 25, 0);

    glVertex3f(-10, 25, 0);
    glVertex3f(-20, 25, 0);
    // side wall
    glVertex3f(-30, 0, 0);
    glVertex3f(-100, 0, 0);

    glVertex3f(-100, 0, 0);
    glVertex3f(-100, 50, 0);

    glVertex3f(-100, 50, 0);
    glVertex3f(-30, 50, 0);

    glVertex3f(-30, 50, 0);
    glVertex3f(-30, 50, 0);

    //long shed lines
    glVertex3f(-100, 50, 0);
    glVertex3f(-85, 60, 0);

    glVertex3f(-85, 60, 0);
    glVertex3f(-15, 60, 0);

    glVertex3f(-15, 60, 0);
    glVertex3f(-30, 50, 0);

    //short shed lines
    glVertex3f(-15, 60, 0);
    glVertex3f(0, 50, 0);

    //glVertex3f(-3,52, 0);
    //glVertex3f(-97, 52, 0);
    glColor3f(1, 1, 1);
    drawStripes(0, 50, -15, 60, -100, 50, -85, 60, 5);
    drawStripes(-15, 50, -85, 50,-15, 60, -85, 60, 20);
    drawStripes(0, 50, -15, 60, 0, 50, -15, 50, 5);
    drawStripes(-100, 50, -85, 50, -100, 50, -85, 60, 5);

    }
    glEnd();
   //drawStripes(0, 50, -15, 60, -100, 50, -85, 60, 5);

    //door
    glBegin(GL_QUADS);{
    glColor3f(0.04, 0.01, 0.41);
    glVertex3f(-10, 0, 0);
    glVertex3f(-20, 0, 0);
    glVertex3f(-20, 25, 0);
    glVertex3f(-10, 25, 0);
    }
    glEnd();

    //door wall
    glBegin(GL_QUADS);{

    glColor3f(0.66, 0.34, 0);
    glVertex3f(0,0,0);
    glVertex3f(-30, 0, 0);
    glVertex3f(-30, 50, 0);
    glVertex3f(0, 50, 0);
    glVertex3f(-10, 0, 0);
    }
    glEnd();

    //side wall
    glBegin(GL_QUADS);{

        glColor3f(0.66, 0.34, 0);
        glVertex3f(-30, 0, 0);
        glVertex3f(-100, 0, 0);
        glVertex3f(-100, 50, 0);
        glVertex3f(-30, 50, 0);
    }
    glEnd();

    //long shed
    glBegin(GL_QUADS);{
        //glColor3f(0.04, 0.28, 0.04);
        glColor3f(0.14, 0.44, 0.13);
        glVertex3f(-100, 50, 0);
        glVertex3f(-85, 60, 0);
        glVertex3f(-15, 60, 0);
        glVertex3f(-30, 50, 0);
    }

    glEnd();

    //short shed
    glBegin(GL_TRIANGLES);{
        glVertex3f(-30, 50, 0);
        glVertex3f(-15, 60, 0);
        glVertex3f(0, 50, 0);
    }
    glEnd();
}
void display(){
	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);
	//initialize the matrix
	glLoadIdentity();
	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?
	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);

	gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);
	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);
	/****************************
	/ Add your objects from here
	****************************/
	//add objects
    //drawAxes();
    //glTranslatef(110,0,0);

    /*
    glColor3f(0,1,0);
    drawSquare(25);
    */
    if(flag == 1){
        drawBdFlag();
    }
    else{
        drawHome();
    }

    //

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate(){
	//codes for any changes in Models, Camera
	angle+=0.5;
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	angle=0;
	drawaxes=1;
	rad = 50;
	flag = 1;
	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	//gluPerspective(fovY,	aspect_ratio,	Near, Far);
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){

	glutInit(&argc,argv);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
	//create display with title name
	glutCreateWindow("Miraz");
	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
