#include<Gl/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define outcode int
#define w 135
#define h 52
#define maxx0 20
#define maxy0 25
#define dx0 15
#define dy0 10
// 1.Sierpinski
GLfloat v[4][3]={{0.0,0.0,1.0},{0.0,0.9,-0.3},{-0.8,-0.4,-0.3},{0.8,-0.4,-0.3}};
GLfloat colors1[4][3]={{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},{1.0,1.0,1.0}};
int n=3;

// 2.Liang-Barsky
double xmini=50,ymini=50,xmaxi=100,ymaxi=100;
double xvmini=200,yvmini=200,xvmaxi=300,yvmaxi=300;
double X0=60,Y0=40,X1=100,Y1=120;

// 3.Color Cube
GLfloat vertices[][3] = {{-1.0,-1.0,1.0},{-1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,-1.0,1.0},{-1.0,-1.0,-1.0},{-1.0,1.0,-1.0},{1.0,1.0,-1.0}, {1.0,-1.0,-1.0}};
/*colors are assigned to the vertices */
 GLfloat colors[][3] = {{-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},{1.0,1.0,-1.0},{-1.0,1.0,-1.0}, {-1.0,-1.0,1.0}, {1.0,-1.0,1.0},{1.0,1.0,1.0},{-1.0,1.0,1.0 }};
static GLfloat theta[] = {0.0,0.0,0.0};
static GLint axis = 2;

// 4.House
GLfloat house[3][9] = {{150.0,150.0,225.0,300.0,300.0,200.0,200.0,250.0,250.0},{150.0,250.0,300.0,250.0,150.0,150.0,200.0,200.0,150.0},{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0}};
GLfloat rot_mat[3][3]={{0},{0},{0}};
GLfloat result[3][9]={{0},{0},{0}};
GLfloat h1=150.0;
GLfloat k=150.0;
GLfloat theta4=45.0;

// 5.Cohen-Sutherland
double xmin=50,ymin=50,xmax=100,ymax=100;
double xvmin=200,yvmin=200,xvmax=300,yvmax=300;
double xz=60,yz=20,xo=80,yo=120;

const int TOP=1;
const int BOTTAM=2;
const int RIGHT=4;
const int LEFT=8;

// 6.Cylinder and parallelepiped
// 7.Teapot
// 8.Color Cube
// 9.Scanline
float x91,x92,x93,x94,y91,y92,y93,y94;

//10.Mesh
GLint x10[maxx0],y10[maxy0];
GLint x00=50,y00=50;
GLint i10,j10;

// 1.Sierpinski
void triangle(GLfloat *va,GLfloat *vb,GLfloat *vc)
{
    glBegin(GL_TRIANGLES);
		glVertex3fv(va);
		glVertex3fv(vb);
		glVertex3fv(vc);
    glEnd();
	glFlush();
}
void tetra(GLfloat *a,GLfloat *b,GLfloat *c,GLfloat *d)
{
	glColor3fv(colors1[0]);
	triangle(a,b,c);
	glColor3fv(colors1[1]);
	triangle(a,c,d);
	glColor3fv(colors1[2]);
	triangle(a,d,b);
	glColor3fv(colors1[3]);
	triangle(b,d,c);
}
void divide_tetra(GLfloat *a,GLfloat *b,GLfloat *c,GLfloat *d,int m)
{
	GLfloat mid[6][3];
	int j;
	if(m>0)
	{
		for(j=0;j<3;j++) mid[0][j]=(a[j]+b[j])/2;
		for(j=0;j<3;j++) mid[1][j]=(a[j]+c[j])/2;
		for(j=0;j<3;j++) mid[2][j]=(a[j]+d[j])/2;
		for(j=0;j<3;j++) mid[3][j]=(b[j]+d[j])/2;
		for(j=0;j<3;j++) mid[4][j]=(d[j]+c[j])/2;
		for(j=0;j<3;j++) mid[5][j]=(b[j]+c[j])/2;

		divide_tetra(a,mid[0],mid[1],mid[2],m-1);
		divide_tetra(mid[0],b,mid[3],mid[5],m-1);
		divide_tetra(mid[1],mid[4],c,mid[5],m-1);
		divide_tetra(mid[2],mid[3],mid[4],d,m-1);
	}
	else
		tetra(a,b,c,d);
}

void display1(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	divide_tetra(v[0],v[1],v[2],v[3],n);
	glFlush();
}
void init1()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-2.0,2.0,-2.0,2.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(1.0,1.0,1.0,1.0);
}

// 2.Liang-Barsky
int cliptest(double p,double q,double *t1,double *t2)
{
	double t=q/p;
	if(p<0.0)
	{
		if(t> *t1) *t1=t;
		if(t> *t2) return(false);
	}
	else if(p>0.0)
	{
			if(t< *t2) *t2=t;
			if(t< *t1) return(false);
	}
	else if(p==0.0)
	{
				if(q<0.0) return(false);
	}
	return(true);
}

void LiangBarsky(double x0,double y0,double x1,double y1)
{



	double ddx;
	ddx = x1 - x0;
	double ddy;
	ddy = y1 - y0;
	double te=0.0;
	double tl=1.0;
	if(cliptest(-ddx,x0-xmini,&te,&tl))
	if(cliptest(ddx,xmaxi-x0,&te,&tl))
	if(cliptest(-ddy,y0-ymini,&te,&tl))
	if(cliptest(ddy,ymaxi-y0,&te,&tl))
	{
		if(tl<1.0)
		{
			x1=x0+tl*ddx;
			y1=y0+tl*ddy;
		}
		if(te>0.0)
		{
			x0=x0+te*ddx;
			y0=y0+te*ddy;
		}

		double sx=(xvmaxi-xvmini)/(xmaxi-xmini);
		double sy=(yvmaxi-yvmini)/(ymaxi-ymini);
		double vx0=xvmini+(x0-xmini)*sx;
		double vy0=yvmini+(y0-ymini)*sy;
		double vx1=xvmini+(x1-xmini)*sx;
		double vy1=yvmini+(y1-ymini)*sy;
		glColor3f(1.0,0.0,0.0);
		glBegin(GL_LINE_LOOP);
			glVertex2f(xvmini,yvmini);
			glVertex2f(xvmaxi,yvmini);
			glVertex2f(xvmaxi,yvmaxi);
			glVertex2f(xvmini,yvmaxi);
		glEnd();
		glColor3f(0.0,0.0,1.0);
		glBegin(GL_LINES);
			glVertex2d(vx0,vy0);
			glVertex2d(vx1,vy1);
		glEnd();
	}
}

void display2()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINES);
		glVertex2d(X0,Y0);
		glVertex2d(X1,Y1);
	glEnd();
	glColor3f(0.0,0.0,1.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(xmini,ymini);
		glVertex2f(xmaxi,ymini);
		glVertex2f(xmaxi,ymaxi);
		glVertex2f(xmini,ymaxi);
	glEnd();
	LiangBarsky(X0,Y0,X1,Y1);
	glFlush();
}

void init2()
{
	glClearColor(1.0,1.0,1.0,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,499.0,0.0,499.0);
}

// 3.Color Cube
void polygon(int a, int b, int c, int d)
{	/* draw a polygon via list of vertices */
	glBegin(GL_POLYGON);
		glColor3fv(colors[a]);
		glVertex3fv(vertices[a]);
		glColor3fv(colors[b]);
		glVertex3fv(vertices[b]);
		glColor3fv(colors[c]);
		glVertex3fv(vertices[c]);
		glColor3fv(colors[d]);
		glVertex3fv(vertices[d]);
	glEnd();
}
void colorcube()
{	/* map vertices to faces */
	polygon(0,3,2,1);
	polygon(2,3,7,6);
	polygon(0,4,7,3);
	polygon(1,2,6,5);
	polygon(4,5,6,7);
	polygon(0,1,5,4);
}
void display3()
{ /* display callback, clear frame buffer and z buffer,
   rotate cube and draw, swap buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);
	colorcube();
	glFlush();
	glutSwapBuffers();
}

void spinCube()
{	/* idle callback, spin cube 2 degrees about selected axis */
	theta[axis] += 0.05;
	if( theta[axis] > 360.0 ) theta[axis] -= 360.0;
	glutPostRedisplay();
}
void mouse3(int btn, int state, int x, int y)
{	/* mouse callback, selects an axis about which to rotate */
	if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	if(btn==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
	if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
}
void myReshape(int w1, int h1)
{
    glViewport(0, 0, w1, h1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w1 <= h1)
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h1 / (GLfloat) w1,2.0 * (GLfloat) h1 / (GLfloat) w1, -10.0, 10.0);
    else
        glOrtho(-2.0 * (GLfloat) w1 / (GLfloat) h1,2.0 * (GLfloat) w1 / (GLfloat) h1, -2.0, 2.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

// 4.House
void multiply()
{
     int i,j,l;
     for(i=0;i<3;i++)
     for(j=0;j<9;j++)
     {
                     result[i][j]=0;
                     for(l=0;l<3;l++)
                     result[i][j]=result[i][j]+rot_mat[i][l]*house[l][j];
     }
}
int rotate()
{
	GLfloat m,n;
	m=-h1*(cos(theta4)-1)+k*(sin(theta4));
	n=-k*(cos(theta4)-1)-h1*(sin(theta4));
    rot_mat[0][0]=cos(theta4);
    rot_mat[0][1]=-sin(theta4);
    rot_mat[0][2]=m;
    rot_mat[1][0]=sin(theta4);
    rot_mat[1][1]=cos(theta4);
    rot_mat[1][2]=n;
    rot_mat[2][0]=0;
    rot_mat[2][1]=0;
    rot_mat[2][2]=1;
    multiply();
 return 0;
 }


void rhouse()
{
     glColor3f(0.0,0.0,1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(result[0][0],result[1][0]);
	glVertex2i(result[0][1],result[1][1]);
	glVertex2i(result[0][3],result[1][3]);
	glVertex2i(result[0][4],result[1][4]);
	glEnd();
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(result[0][5],result[1][5]);
	glVertex2i(result[0][6],result[1][6]);
	glVertex2i(result[0][7],result[1][7]);
	glVertex2i(result[0][8],result[1][8]);
	glEnd();
	glColor3f(0.0,0.0,1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(result[0][1],result[1][1]);
	glVertex2i(result[0][2],result[1][2]);
	glVertex2i(result[0][3],result[1][3]);
	glEnd();
}
void dhouse()
{
     glColor3f(0.0,0.0,1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(house[0][0],house[1][0]);
	glVertex2i(house[0][1],house[1][1]);
	glVertex2i(house[0][3],house[1][3]);
	glVertex2i(house[0][4],house[1][4]);
	glEnd();
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(house[0][5],house[1][5]);
	glVertex2i(house[0][6],house[1][6]);
	glVertex2i(house[0][7],house[1][7]);
	glVertex2i(house[0][8],house[1][8]);
	glEnd();

	 glColor3f(0.0,0.0,1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(house[0][1],house[1][1]);
	glVertex2i(house[0][2],house[1][2]);
	glVertex2i(house[0][3],house[1][3]);
	glEnd();
}
void display4()
{
	glClear(GL_COLOR_BUFFER_BIT);
	dhouse();
	rotate();
	rhouse();
	glFlush();
}
void init4()
{
	glClearColor(1.0,1.0,1.0,1.0);
	glColor3f(1.0,0.0,0.0);
	glPointSize(1.0);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,499.0,0.0,499.0);

}

// 5.Cohen-Sutherland
outcode computecode(double x,double y)
{
	outcode code=0;
	if(y>ymax)
	code |=TOP;
	if(y<ymin)
	code |=BOTTAM;
	if(x>xmax)
	code |=LEFT;
	return code;
}
void cohenclipanddraw(double xz,double yz,double xo,double yo)
{
	outcode outcode0,outcode1,outcodeout;
	double sx,sy,vx0,vy0,vx1,vy1;
	int accept=0,done=0;
	outcode0=computecode(xz,yz);
	outcode1=computecode(xo,yo);

	do
	{
		if(!(outcode0|outcode1))
		{
			accept=1;
			done=1;
		}
		else if(outcode0&outcode1)
			done=1;
		else
		{
			double x,y;
			outcodeout=outcode0?outcode0:outcode1;
			if(outcodeout&TOP)
			{
				x=xz+(xo-xz)*(ymax-yz)/(yo-yz);
				y=ymax;
			}
			else if(outcodeout&BOTTAM)
			{
				x=xz+(xo-xz)*(ymin-yz)/(yo-yz);
				y=ymin;
			}
			else if(outcodeout&RIGHT)
			{
				y=yz+(yo-yz)*(xmax-xz)/(xo-xz);
				x=xmax;
			}
			else
			{
				y=yz+(yo-yz)*(xmin-xz)/(xo-xz);
				x=xmin;
			}
			if(outcodeout==outcode0)
			{
				xz=x;yz=y;
				outcode0=computecode(xz,yz);
			}
			else
			{
				xo=x;yo=y;
				outcode1=computecode(xo,yo);
			}
		}
	}
	while(!done);

	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(xvmin,yvmin);
		glVertex2f(xvmax,yvmin);
		glVertex2f(xvmax,yvmax);
		glVertex2f(xvmin,yvmax);
	glEnd();

	if(accept)
	{
		sx=(xvmax-xvmin)/(xmax-xmin);
	sy=(yvmax-yvmin)/(ymax-ymin);

	vx0=xvmin+(xz-xmin)*sx;
	vy0=yvmin+(yz-ymin)*sy;
	vx1=xvmin+(xo-xmin)*sx;
	vy1=yvmin+(yo-ymin)*sy;

	glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINES);
	glVertex2f(vx0,vy0);
	glVertex2f(vx1,vy1);
	glEnd();
	}
}
void init5()
{
	glClearColor(1.0,1.0,1.0,1.0);
	glColor3f(1.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,500.0,0.0,500.0);
	glMatrixMode(GL_MODELVIEW);
}

void display5()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINES);
		glVertex2f(xz,yz);
		glVertex2f(xo,yo);
	glEnd();
	glColor3f(0.0,0.0,1.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(xmin,ymin);
		glVertex2f(xmax,ymin);
		glVertex2f(xmax,ymax);
		glVertex2f(xmin,ymax);
	glEnd();
    cohenclipanddraw(xz,yz,xo,yo);
	glFlush();
}

// 6.Cylinder and parallelepiped
void draw_pixel(GLint cx, GLint cy)
{	
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
		glVertex2i(cx, cy);
	glEnd();
}

void plotpixels(GLint h6, GLint k, GLint x, GLint y)
{
	draw_pixel(x+h6, y+k);
	draw_pixel(-x+h6, y+k);
	draw_pixel(x+h6, -y+k);
	draw_pixel(-x+h6, -y+k);
	draw_pixel(y+h6, x+k);
	draw_pixel(-y+h6, x+k);
	draw_pixel(y+h6, -x+k);
	draw_pixel(-y+h6, -x+k);
}

void circle_draw(GLint h6, GLint k, GLint r)
{
	GLint d=1-r, x=0,y=r;
	while(y>x)
	{
		plotpixels(h6, k, x, y);
		if(d<0) d+=2*x+3;
		else
		{
			d+=2*(x-y)+5;
			--y;
		}
		++x;
	}
	plotpixels(h6, k, x, y);
}

void cylinderdraw()
{
	GLint xc=100,yc=100,r=50; 
	GLint i, n=70;
	for(i=0;i<n;i+=3)
	{
		circle_draw(xc, yc+i, r);
	}
}

void parallelpiped(int x1, int x2, int y1, int y2)
{
	glColor3f(0.0,0.0,1.0);
	glPointSize(2.0);
	glBegin(GL_LINE_LOOP);
		glVertex2i(x1,y1);
		glVertex2i(x2,y1);
		glVertex2i(x2,y2);
		glVertex2i(x1,y2);
	glEnd();
}

void parallelpiped_draw()
{
	int x1=200,x2=300,y1=100,y2=175;
	GLint i, n=40;
	for(i=0;i<n;i+=3)
	{
		parallelpiped(x1+i,x2+i,y1+i,y2+i);
	}
}

void init6(void)
{
	glClearColor(1.0,1.0,1.0,0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0,400.0,0.0,300.0);
}

void display6(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,0.0,0.0);
	cylinderdraw();
	parallelpiped_draw();
	glFlush();
}

// 7.Teapot
void wall(double thickness)
{
	glPushMatrix();
	glTranslated(0.5,0.5*thickness, 0.5);
	glScaled(1.0,thickness, 1.0);
	glutSolidCube(1.0);
	glPopMatrix();
}

void tableleg(double thick, double len)
{
	glPushMatrix();
	glTranslated(0,len/2,0);
	glScaled(thick, len, thick);
	glutSolidCube(1.0);
	glPopMatrix();
}

void table(double topwid, double topthick, double legthick, double leglen)
{
	glPushMatrix();
	glTranslated(0,leglen,0);
	glScaled(topwid, topthick, topwid);
	glutSolidCube(1.0);
	glPopMatrix();
	double dist=0.95*topwid/2.0-legthick/2.0;
	glPushMatrix();
	glTranslated(dist, 0, dist);
	tableleg(legthick, leglen);
	glTranslated(0.0,0.0,-2*dist);
	tableleg(legthick, leglen);
	glTranslated(-2*dist, 0, 2*dist);
	tableleg(legthick, leglen);
	glTranslated(0,0,-2*dist);
	tableleg(legthick,leglen);
	glPopMatrix();
}

void displaySolid(void)
{
	GLfloat mat_ambient[]={0.7f,0.7f,0.7f,1.0f};
	GLfloat mat_diffuse[]={0.5f,0.5f,0.5f,1.0f};
	GLfloat mat_specular[]={1.0f,1.0f,1.0f,1.0f};
	GLfloat mat_shininess[]={50.0f};
	glMaterialfv(GL_FRONT,GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS, mat_shininess);

	GLfloat lightintensity[]={0.7f,0.7f,0.7f,1.0f};
	GLfloat lightposition[]={2.0f,6.0f,3.0f,0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightintensity);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double winht=1.0;
	glOrtho(-winht*64/48, winht*64/48, -winht, winht,  
                                          0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(2.3,1.3,2.0,0.0,0.25,0.0,0.0,1.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	  glTranslated(0.5,0.4,0.5);
	  glRotated(30,0,1,0);
	  glutSolidTeapot(0.08);
	glPopMatrix();
	glPushMatrix();
	  glTranslated(0.4,0,0.4);
	  table(0.6,0.02,0.02,0.3);
	glPopMatrix();
	wall(0.02);
	glPushMatrix();
	  glRotated(90.0,0.0,0.0,1.0);
	  wall(0.02);
	glPopMatrix();
	glPushMatrix();
	  glRotated(-90.0,1.0,0.0,0.0);
	  wall(0.02);
	glPopMatrix();
	glFlush();
}

// 8.Perspective Color Cube
void polygon8(int a, int b, int c, int d)
{
	glBegin(GL_POLYGON);
		glColor3fv(colors[a]);
		glVertex3fv(vertices[a]);
		glColor3fv(colors[b]);
		glVertex3fv(vertices[b]);
		glColor3fv(colors[c]);
		glVertex3fv(vertices[c]);
		glColor3fv(colors[d]);
		glVertex3fv(vertices[d]);
	glEnd();
}

void colorcube8()
{
	polygon8(0,3,2,1);
	polygon8(2,3,7,6);
	polygon8(0,4,7,3);
	polygon8(1,2,6,5);
	polygon8(4,5,6,7);
	polygon8(0,1,5,4);
}

static GLfloat theta8[]={0.0,0.0,0.0};
static GLint axis8=2;
static GLdouble viewer8[]={0.0,0.0,5.0};

void display8(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(viewer8[0], viewer8[1],viewer8[2],0.0,0.0,0.0, 
0.0,1.0,0.0);
	glRotatef(theta8[0],1.0,0.0,0.0);
	glRotatef(theta8[1],0.0,1.0,0.0);
	glRotatef(theta8[2],0.0,0.0,1.0);
	colorcube8();
	glFlush();
	glutSwapBuffers();
}

void mymouse8(int btn,int state,int x,int y)
{
	if(btn==GLUT_LEFT_BUTTON&&state==GLUT_DOWN) axis8=0;
	if(btn==GLUT_MIDDLE_BUTTON&&state==GLUT_DOWN) axis8=1;
	if(btn==GLUT_RIGHT_BUTTON&&state==GLUT_DOWN) axis8=2;
	theta8[axis8]+=2.0;
	if(theta8[axis8]>360.0) theta8[axis8]-=360.0;
	display8();
}

void keys8(unsigned char key, int x, int y)
{
	if(key == 'x') viewer8[0]-=1.0;
	if(key == 'X') viewer8[0]+=1.0;
	if(key == 'y') viewer8[1]-=1.0;
	if(key == 'Y') viewer8[1]+=1.0;
	if(key == 'z') viewer8[2]-=1.0;
	if(key == 'Z') viewer8[2]+=1.0;
	display8();
}

void myReshape8(int w8, int h8)
{
	glViewport(0,0,w8,h8);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(w8<=h8)
        glFrustum(-2.0,2.0,-2.0*(GLfloat)h8/(GLfloat)w8,2.0*(GLfloat)h8/(GLfloat)w8, 2.0,20.0);
	else 
        glFrustum(-2.0,2.0,-2.0*(GLfloat)w8/(GLfloat)h8,2.0*(GLfloat)w8/(GLfloat)h8, 2.0,20.0);
	glMatrixMode(GL_MODELVIEW);
}

//9.Scanline
void edgedetect(GLfloat x91, GLfloat y91, GLfloat x92, GLfloat y92,GLint *le,int *re)
{
	float mx,x,temp;
	int i;
	if((y92-y91)<0)
	{
		temp=y91;y91=y92;y92=temp;
		temp=x91;x91=x92;x92=temp;
	}
	if((y92-y91)!=0)
		mx=(x92-x91)/(y92-y91);
	else
		mx=x92-x91;
	x=x91;
	for(i=y91;i<=y92;i++)
	{
		if(x<(float)le[i])
			le[i]=(int)x;
		if(x>(float)re[i])
			re[i]=(int)x;
		x+=mx;
	}
}
void draw_pixel(GLint x, GLint y, GLfloat v)
{
	glColor3f(0.7,0.5,0.8);
	glBegin(GL_POINTS);
		glVertex2i(x,y);
	glEnd();
}
void scanfill(float x91,float y91, float x92, float y92,float x93, float y93,float x94, float y94)
{
	int le[500],re[500];
	int i,scanline;
	for(i=0;i<500;i++)
	{
		le[i]=500;
		re[i]=0;
	}
    edgedetect(x91,y91,x92,y92,le,re);
    edgedetect(x92,y92,x93,y93,le,re);
    edgedetect(x93,y93,x94,y94,le,re);
    edgedetect(x94,y94,x91,y91,le,re);
	for(scanline=0; scanline<500; scanline+=5)
	{
		if(le[scanline]<=re[scanline])
		    for(i=(int)le[scanline];i<(int)re[scanline];i++)
			draw_pixel(i,scanline,0.5);
	}

}
void display9()
{
     x91=200.0;y91=200.0;x92=100.0;y92=300.0;x93=200.0;y93=400.0;x94=300.0;y94=300.0;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0,0.0,1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x91,y91);
	glVertex2f(x92,y92);
	glVertex2f(x93,y93);
	glVertex2f(x94,y94);
	glEnd();
	scanfill(x91,y91,x92,y92,x93,y93,x94,y94);
	glFlush();
}
void init9()
{
	glClearColor(1.0,1.0,1.0,1.0);
	glColor3f(1.0,0.0,0.0);
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,500.0,0.0,500.0);
}

//10.Mesh
void init0()
{
	glClearColor(1.0,1.0,1.0,1.0); //TO SET BACKGROUND COLOR
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0,500.0,0.0,400.0);
}

void display0()
{
	glClear(GL_COLOR_BUFFER_BIT); //TO AVOID TRANSPARENCY
	for(i10=0;i10<maxx0;i10++)
	{
		x10[i10]=x00+i10*dx0;
	}
	for(j10=0;j10<maxy0;j10++)
	{
		y10[j10]=y00+j10*dy0;
	}
	for(i10=0;i10<maxx0-1;i10++)
	{
		for(j10=0;j10<maxy0-1;j10++)
		{
			glColor3f(1.0,0.0,1.0);
			glBegin(GL_LINE_LOOP);
			glVertex2i(x10[i10],y10[j10]);
			glVertex2i(x10[i10],y10[j10+1]);
			glVertex2i(x10[i10+1],y10[j10+1]);
			glVertex2i(x10[i10+1],y10[j10]);
			glEnd();
			glFlush();
		}
	}
}

void Write(char string[])
{
	int i=-1;
    //glColor3f(0.5,0.8,1.0);
    while(string[++i]!='\0')
    {
                  //glColor3f(0.5,0.8,1.0);
                  // glutStrokeCharacter(GLUT_STROKE_ROMAN, *string++);
                   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
}
void WriteS(char string[])
{
	int i=-1;
    //glColor3f(0.5,0.8,1.0);
    while(string[++i]!='\0')
    {
                  //glColor3f(0.5,0.8,1.0);
                  // glutStrokeCharacter(GLUT_STROKE_ROMAN, *string++);
                   glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}
void display(void)
{
	//Background
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor3f (0.10,0.10,0.7);
	glBegin(GL_POLYGON);
		glVertex2f(-2,2);
		glVertex2f(-1,2);
		glVertex2f(-1,-2);
		glVertex2f(-2,-2);
	glEnd();
	
	
	//Output Screen
	glColor3f (0.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glVertex2f(-1,2);
		glVertex2f(2,2);
		glVertex2f(2,-2);
		glVertex2f(-1,-2);
	glEnd();
	
	//Button 1
	glColor3f (0.74,0.74,0.74);
	glBegin(GL_QUADS);
		glVertex2f(-1.95,1.5);
		glVertex2f(-1.55,1.5);
		glVertex2f(-1.55,1.2);
		glVertex2f(-1.95,1.2);
	glEnd();
	
	glColor3f (0.10,0.10,0.7);
	glRasterPos2f(-1.922,1.36);
	Write("3D Sierpinski");
	glRasterPos2f(-1.835,1.24);
	Write("Gasket");
	
	//Button 2
	glColor3f (0.74,0.74,0.74);
	glBegin(GL_QUADS);
		glVertex2f(-1.45,1.5);
		glVertex2f(-1.05,1.5);
		glVertex2f(-1.05,1.2);
		glVertex2f(-1.45,1.2);
	glEnd();
	
	glColor3f (0.10,0.10,0.7);
	glRasterPos2f(-1.42,1.32);
	Write("Liang-Barsky");
	
	//Button 3
	glColor3f (0.74,0.74,0.74);
	glBegin(GL_QUADS);
		glVertex2f(-1.95,1.1);
		glVertex2f(-1.55,1.1);
		glVertex2f(-1.55,0.8);
		glVertex2f(-1.95,0.8);
	glEnd();
	
	glColor3f (0.10,0.10,0.7);
	glRasterPos2f(-1.86,0.97);
	Write("Spin the");
	glRasterPos2f(-1.90,0.85);
	Write("Color Cube");
	
	//Button 4
	glColor3f (0.74,0.74,0.74);
	glBegin(GL_QUADS);
		glVertex2f(-1.45,1.1);
		glVertex2f(-1.05,1.1);
		glVertex2f(-1.05,0.8);
		glVertex2f(-1.45,0.8);
	glEnd();
	
	glColor3f (0.10,0.10,0.7);
	glRasterPos2f(-1.435,0.9);
	Write("Rotating House");	
	
	//Button 5
	glColor3f (0.74,0.74,0.74);
	glBegin(GL_QUADS);
		glVertex2f(-1.95,0.7);
		glVertex2f(-1.55,0.7);
		glVertex2f(-1.55,0.4);
		glVertex2f(-1.95,0.4);
	glEnd();
	
	glColor3f (0.10,0.10,0.7);
	glRasterPos2f(-1.835,0.575);
	Write("Cohen");
	glRasterPos2f(-1.895,0.455);
	Write("Sutherland");	

	//Button 6
	glColor3f (0.74,0.74,0.74);
	glBegin(GL_QUADS);
		glVertex2f(-1.45,0.7);
		glVertex2f(-1.05,0.7);
		glVertex2f(-1.05,0.4);
		glVertex2f(-1.45,0.4);
	glEnd();
	
	glColor3f (0.10,0.10,0.7);
	glRasterPos2f(-1.37,0.575);
	Write("Cylinder &");
	glRasterPos2f(-1.425,0.455);
	Write("Parallelepiped");
	
	//Button 7
	glColor3f (0.74,0.74,0.74);
	glBegin(GL_QUADS);
		glVertex2f(-1.95,0.3);
		glVertex2f(-1.55,0.3);
		glVertex2f(-1.55,0.0);
		glVertex2f(-1.95,0.0);
	glEnd();
	
	glColor3f (0.10,0.10,0.7);
	glRasterPos2f(-1.85,0.125);
	Write("Teapot");
	
	//Button 8
	glColor3f (0.74,0.74,0.74);
	glBegin(GL_QUADS);
		glVertex2f(-1.45,0.3);
		glVertex2f(-1.05,0.3);
		glVertex2f(-1.05,0.0);
		glVertex2f(-1.45,0.0);
	glEnd();
	
	glColor3f (0.10,0.10,0.7);
	glRasterPos2f(-1.415,0.18);
	Write("Camera View");
	glRasterPos2f(-1.41,0.06);
	Write("(Color Cube)");
	
	//Button 9
	glColor3f (0.74,0.74,0.74);
	glBegin(GL_QUADS);
		glVertex2f(-1.95,-0.1);
		glVertex2f(-1.55,-0.1);
		glVertex2f(-1.55,-0.4);
		glVertex2f(-1.95,-0.4);
	glEnd();
	
	glColor3f (0.10,0.10,0.7);
	glRasterPos2f(-1.915,-0.22);
	Write("Scanline Area");
	glRasterPos2f(-1.88,-0.34);
	Write("Filling Algo");
	
	//Button 10
	glColor3f (0.74,0.74,0.74);
	glBegin(GL_QUADS);
		glVertex2f(-1.45,-0.1);
		glVertex2f(-1.05,-0.1);
		glVertex2f(-1.05,-0.4);
		glVertex2f(-1.45,-0.4);
	glEnd();
	
	glColor3f (0.10,0.10,0.7);
	glRasterPos2f(-1.4,-0.23);
	Write("Rectangular");
	glRasterPos2f(-1.32,-0.35);
	Write("Mesh");
	
	//Button Exit
	glColor3f (0.74,0.74,0.74);
	glBegin(GL_QUADS);
		glVertex2f(-1.95,-1.2);
		glVertex2f(-1.05,-1.2);
		glVertex2f(-1.05,-1.5);
		glVertex2f(-1.95,-1.5);
	glEnd();
	
	glColor3f (0.10,0.10,0.7);
	glRasterPos2f(-1.6,-1.4);
	WriteS("EXIT");	
	
	glColor3f(1.0,1.0,1.0);
    glRasterPos2f(0.15,0.67);
    Write("CG PROJECT USING OPENGL");
    glColor3f(1.0,1.0,1.0);
    glRasterPos2f(-0.1,0.47);
    Write("A MENU BASED PROJECT FOR LAB PROGRAMS");
	glColor3f(1.0,1.0,1.0);
    glRasterPos2f(0.05,0.27);
    Write("CLICK ON BUTTON FOR THE OUTPUT");
	glColor3f(1.0,1.0,1.0);
    glRasterPos2f(-0.5,-0.07);
    
	glFlush();
}
void keys(unsigned char key,int x,int y)
{
}

void mouse(int btn,int state,int x,int y)
{
	if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		//printf("%d %d\n",x,y);
		
		//      BUTTON 1
		if(x>16 && x<16+w && y>84 && y<84+h)
		{
			glutInitWindowSize(1010,689);
			glutInitWindowPosition(350,0);
			glutCreateWindow("3D Sierpinski Gasket");
			init1();
			glutDisplayFunc(display1);
			glEnable(GL_DEPTH_TEST);
			glutMainLoop();
		}
		
		//       BUTTON 2
		if(x>185 && x<185+w && y>84 && y<84+h)
		{
			glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
			glutInitWindowSize(1010,689);
			glutInitWindowPosition(350,0);
			glutCreateWindow("Liang Barsky");
			glutDisplayFunc(display2);
			init2();
			glutMainLoop();
		}
		
		//      BUTTON 3
		if(x>16 && x<16+w && y>154 && y<154+h)
		{
			glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
			glutInitWindowSize(1010,689);
			glutInitWindowPosition(350,0);
			glutCreateWindow("Spin a color cube");
			glutReshapeFunc(myReshape);
			glutDisplayFunc(display3);
			glutIdleFunc(spinCube);
			glutMouseFunc(mouse3);
			glEnable(GL_DEPTH_TEST);
			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3,GL_FLOAT, 0, vertices);
			glColorPointer(3, GL_FLOAT, 0, colors);
			glColor3f(1.0,1.0,1.0);
			glutMainLoop();
		}
		
		//      BUTTON 4
		if(x>185 && x<185+w && y>154 && y<154+h)
		{
			glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
			glutInitWindowSize(1010,689);
			glutInitWindowPosition(350,0);
			glutCreateWindow("House Rotation");
			glutDisplayFunc(display4);
			init4();
			glutMainLoop();
		}
		
		//      BUTTON 5
		if(x>16 && x<16+w && y>223 && y<223+h)
		{
			glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
			glutInitWindowSize(1010,689);
			glutInitWindowPosition(350,0);
			glutCreateWindow("Cohen-Sutherland Line Clipping");
			init5();
			glutDisplayFunc(display5);	
			glutMainLoop();
		}
		
		//      BUTTON 6
		if(x>185 && x<185+w && y>223 && y<223+h)
		{
			glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
			glutInitWindowPosition(350,0);
			glutInitWindowSize(1010,689);
			glutCreateWindow("Cylinder & ParralelPiped");
			init6();
			glutDisplayFunc(display6);
			glutMainLoop();
		}
		
		//      BUTTON 7
		if(x>16 && x<16+w && y>292 && y<292+h)
		{
				//glutInit(&argc, argv);
				glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
				glutInitWindowPosition(350,0);
				glutInitWindowSize(1010,689);
				glutCreateWindow("Teapot");
				glutDisplayFunc(displaySolid);
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
				glShadeModel(GL_SMOOTH);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_NORMALIZE);
				glClearColor(0.1,0.1,0.1,0.0);
				glViewport(0,0,640,480);
				glutMainLoop();
		}
		
		//      BUTTON 8
		if(x>185 && x<185+w && y>292 && y<292+h)
		{
			glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
			glutInitWindowPosition(350,0);
			glutInitWindowSize(1010,689);
			glutCreateWindow("Colorcube Viewer");
			glutReshapeFunc(myReshape8);
			glutDisplayFunc(display8);
			glutMouseFunc(mymouse8);
			glutKeyboardFunc(keys8);
			glEnable(GL_DEPTH_TEST);
			glutMainLoop();
	
		}
		
		//      BUTTON 9
		if(x>16 && x<16+w && y>361 && y<361+h)
		{
			glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
			glutInitWindowSize(1010,689);
			glutInitWindowPosition(350,0);
			glutCreateWindow("Scan line area filling algorithm");
			glutDisplayFunc(display9);
			init9();
			glutMainLoop();
		}
		
		//      BUTTON 10
		if(x>185 && x<185+w && y>361 && y<361+h)
		{
			glutInitWindowPosition(350,0);
			glutInitWindowSize(1010,689);
			glutCreateWindow("rectangular Mesh");
			init0();
			glutDisplayFunc(display0);
			glutMainLoop();
		}
		/*      EXIT BUTTON    */
		if(x>16 && x<320 && y>550 && y<605)
			exit(0);
		
	}
	
}
void init()
{
	glClearColor(1.0,1.0,1.0,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-2.0,2.0,-2.0,2.0);
	glMatrixMode(GL_MODELVIEW);
}
int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitWindowSize(1347,689);
	glutInitWindowPosition(0,0);
	glutCreateWindow("CG Project");
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keys);
	init();
	glutMainLoop();
	return 0;
}
