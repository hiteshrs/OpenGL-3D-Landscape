/***********************************************************
************************************************************
****		Bangalore Institute of Technology			****
****	Department of Computer Science & Engineering	****
****				VI Semester							****
****			  Graphics Package						****
****		  ***********************					****
****		  **** 3D LANDSCAPE  ****					****
****		  ***********************					****
****			   Submitted By							****
****	 1BI13CS063				HITESH RS				****
****	   for the academic year 2015-16				****
************************************************************
************************************************************
*/

#define _CRT_SECURE_NO_WARNINGS

#include<stdlib.h>
#include<GL/glut.h>
#include<stdio.h>
#include<math.h>

static bool info_display = true;
static bool plane_move = true;
static bool drone_mode = true;
static bool walk_Mode = false;
static bool fly_Mode = false;
static bool windmill_rotate = true;
static bool dayLight = true;
static bool start_simulation = true;
//static bool house1 = true, house2 = true, house3 = false, house4 = false, house5 = false;
//static bool windmill1 = true, windmill2 = true, windmill3 = false, windmill4 = false, windmill5 = false;

static bool house1 = true, house2 = true, house3 = true, house4 = true, house5 = true;
static bool windmill1 = true, windmill2 = true, windmill3 = true, windmill4 = true, windmill5 = true;

static GLfloat angle = 0;

//FPS MODE Variables
bool g_key[256];
bool g_shift_down = false;
bool g_fps_mode = false;
int g_viewport_width = 0;
int g_viewport_height = 0;
bool g_mouse_left_down = false;
bool g_mouse_right_down = false;

// Movement settings
const float g_translation_speed = 200;
const float g_rotation_speed = (3.1416) / 180 * 0.2;

float m_x = 5000, m_y = 80, m_z = -4000;		// Position
float m_lx, m_ly, m_lz;							// Direction vector of where we are looking at
float m_yaw = 20.0, m_pitch = 0.0;				// Various rotation angles
float m_strafe_lx, m_strafe_lz;					// Always 90 degree to direction vector

void Refresh()
{
	// Camera parameter according to Riegl's co-ordinate system
	// x/y for flat, z for height
	m_lx = cos(m_yaw) * cos(m_pitch);
	m_ly = sin(m_pitch);
	m_lz = sin(m_yaw) * cos(m_pitch);

	m_strafe_lx = cos(m_yaw - (3.1416/2));
	m_strafe_lz = sin(m_yaw - (3.1416 / 2));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(m_x, m_y, m_z, m_x + m_lx, m_y + m_ly, m_z + m_lz, 0.0, 1.0, 0.0);

	//printf("Camera: %f %f %f Direction vector: %f %f %f\n", m_x, m_y, m_z, m_lx, m_ly, m_lz);
}

void Move(float incr)
{
	float lx = cos(m_yaw)*cos(m_pitch);
	float ly = sin(m_pitch);
	float lz = sin(m_yaw)*cos(m_pitch);

	m_x = m_x + incr*lx;
	m_y = m_y + incr*ly;
	m_z = m_z + incr*lz;

	if (m_y <= 100)
	{
		m_y = 100;
	}
	if (walk_Mode)
	{
		if (m_y >= 150)
		{
			m_y = 150;
		}
	}

	Refresh();
}

void Strafe(float incr)
{
	m_x = m_x + incr*m_strafe_lx;
	m_z = m_z + incr*m_strafe_lz;

	Refresh();
}

void Fly(float incr)
{
	m_y = m_y + incr;

	Refresh();
}

void RotateYaw(float angle)
{
	m_yaw = (m_yaw + angle);
	if (m_yaw > 90.0)
	{
		m_yaw = (m_yaw + 90.0);
	}

	Refresh();
}

void RotatePitch(float angle)
{
	const float limit = 89.0 * (3.1416) / 180.0;

	m_pitch += angle;

	if (m_pitch < -limit)
		m_pitch = -limit;

	if (m_pitch > limit)
		m_pitch = limit;

	Refresh();
}


void drawHouse(GLfloat x, GLfloat y, GLfloat z, GLfloat R, GLfloat G, GLfloat B,GLfloat angle)
{
	GLfloat size = 50;
	GLfloat xscale = 3, yscale = 3, zscale = 5;

	glPushMatrix();					
	glTranslatef(x, (yscale*size/2), z);	//House Cube
	glRotatef(angle , 0, 1, 0);
	glScalef(xscale, yscale, zscale);

	glColor3f(R/255.0, G/255.0, B/255.0);
	glutSolidCube(size);


	glColor3f(0.92, 0.082, 0.094);
	glBegin(GL_POLYGON);					//House Top Front Triangle
	glVertex3f(-(size / 2), (size / 2), (size / 2));
	glVertex3f((size / 2), (size / 2), (size / 2));
	glVertex3f(0, size, (size / 2));
	glEnd();
	glColor3f(0, 0, 0);
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);					//House Top Front Triangle Border
	glVertex3f(-(size / 2), (size / 2), (size / 2));
	glVertex3f((size / 2), (size / 2), (size / 2));
	glVertex3f(0, size, (size / 2));
	glEnd();

	
	glColor3f(0.92, 0.082, 0.094);
	glBegin(GL_POLYGON);					//House Top Back Triangle
	glVertex3f(-(size / 2), (size / 2), -(size / 2));
	glVertex3f((size / 2), (size / 2), -(size / 2));
	glVertex3f(0, size, -(size / 2));
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);					//House Top Back Triangle Border
	glVertex3f(-(size / 2), (size / 2), -(size / 2));
	glVertex3f((size / 2), (size / 2), -(size / 2));
	glVertex3f(0, size, -(size / 2));
	glEnd();


	glColor3f(0.92, 0.082, 0.094);
	glBegin(GL_POLYGON);					//House Top left side panel
	glVertex3f(-(size / 2), (size / 2), -(size / 2));
	glVertex3f(0, size, -(size / 2));
	glVertex3f(0, size, (size / 2));
	glVertex3f(-(size / 2), (size / 2), (size / 2));
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);					//House Top left side panel Border
	glVertex3f(-(size / 2), (size / 2), -(size / 2));
	glVertex3f(0, size, -(size / 2));
	glVertex3f(0, size, (size / 2));
	glVertex3f(-(size / 2), (size / 2), (size / 2));
	glEnd();


	glColor3f(0.92, 0.082, 0.094);
	glBegin(GL_POLYGON);					//House Top right side panel
	glVertex3f((size / 2), (size / 2), -(size / 2));
	glVertex3f(0, size, -(size / 2));
	glVertex3f(0, size, (size / 2));
	glVertex3f((size / 2), (size / 2), (size / 2));
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);					//House Top right side panel Border
	glVertex3f((size / 2), (size / 2), -(size / 2));
	glVertex3f(0, size, -(size / 2));
	glVertex3f(0, size, (size / 2));
	glVertex3f((size / 2), (size / 2), (size / 2));
	glEnd();


	glColor3f(168.0 / 255.0, 65.0 / 255.0, 0.0 / 255.0);
	glBegin(GL_POLYGON);					//Door
	glVertex3f((size / 4), -(size / 2), (size / 2)+0.2);
	glVertex3f((size / 4), 0, (size / 2) + 0.2);
	glVertex3f(-(size / 4), 0, (size / 2) + 0.2);
	glVertex3f(-(size / 4), -(size / 2), (size / 2) + 0.2);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);					//Door Border
	glVertex3f((size / 4), -(size / 2), (size / 2) + 0.2);
	glVertex3f((size / 4), 0, (size / 2) + 0.2);
	glVertex3f(-(size / 4), 0, (size / 2) + 0.2);
	glVertex3f(-(size / 4), -(size / 2), (size / 2) + 0.2);
	glEnd();


	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);					//Left Front Windows
	glVertex3f(-(size / 2) - 0.2, -(size / 4), (size / 8));
	glVertex3f(-(size / 2) - 0.2, -(size / 4), (size / 4));
	glVertex3f(-(size / 2) - 0.2, (size / 4),  (size / 4));
	glVertex3f(-(size / 2) - 0.2, (size / 4),  (size / 8));
	glEnd(); 
	glBegin(GL_POLYGON);					//Left Back Windows
	glVertex3f(-(size / 2) - 0.2, -(size / 4), -(size / 8));
	glVertex3f(-(size / 2) - 0.2, -(size / 4), -(size / 4));
	glVertex3f(-(size / 2) - 0.2, (size / 4), -(size / 4));
	glVertex3f(-(size / 2) - 0.2, (size / 4), -(size / 8));
	glEnd();
	glBegin(GL_POLYGON);					//Right Front Windows
	glVertex3f((size / 2) + 0.2, -(size / 4), (size / 8));
	glVertex3f((size / 2) + 0.2, -(size / 4), (size / 4));
	glVertex3f((size / 2) + 0.2, (size / 4), (size / 4));
	glVertex3f((size / 2) + 0.2, (size / 4), (size / 8));
	glEnd();
	glBegin(GL_POLYGON);					//Right Back Windows
	glVertex3f((size / 2) + 0.2, -(size / 4), -(size / 8));
	glVertex3f((size / 2) + 0.2, -(size / 4), -(size / 4));
	glVertex3f((size / 2) + 0.2, (size / 4), -(size / 4));
	glVertex3f((size / 2) + 0.2, (size / 4), -(size / 8));
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);					//Left Front Windows Border
	glVertex3f(-(size / 2) - 0.2, -(size / 4), (size / 8));
	glVertex3f(-(size / 2) - 0.2, -(size / 4), (size / 4));
	glVertex3f(-(size / 2) - 0.2, (size / 4), (size / 4));
	glVertex3f(-(size / 2) - 0.2, (size / 4), (size / 8));
	glEnd();
	glBegin(GL_LINE_LOOP);					//Left Back Windows Border
	glVertex3f(-(size / 2) - 0.2, -(size / 4), -(size / 8));
	glVertex3f(-(size / 2) - 0.2, -(size / 4), -(size / 4));
	glVertex3f(-(size / 2) - 0.2, (size / 4), -(size / 4));
	glVertex3f(-(size / 2) - 0.2, (size / 4), -(size / 8));
	glEnd();
	glBegin(GL_LINE_LOOP);					//Right Front Windows Border
	glVertex3f((size / 2) + 0.2, -(size / 4), (size / 8));
	glVertex3f((size / 2) + 0.2, -(size / 4), (size / 4));
	glVertex3f((size / 2) + 0.2, (size / 4), (size / 4));
	glVertex3f((size / 2) + 0.2, (size / 4), (size / 8));
	glEnd();
	glBegin(GL_LINE_LOOP);					//Right Back Windows Border
	glVertex3f((size / 2) + 0.2, -(size / 4), -(size / 8));
	glVertex3f((size / 2) + 0.2, -(size / 4), -(size / 4));
	glVertex3f((size / 2) + 0.2, (size / 4), -(size / 4));
	glVertex3f((size / 2) + 0.2, (size / 4), -(size / 8));
	glEnd();

	
	glPopMatrix();
}

GLUquadricObj *Cylinder = gluNewQuadric();
void drawPlane()
{
	GLfloat size = 20;
	GLfloat xscale = 3, yscale = 3, zscale = 3;
	GLfloat x, y, z;
	x = 0;
	y = 0;
	z = 8000;

	glPushMatrix();
	glRotatef(-90, 0, 1, 0);
	glTranslatef(x, y + (yscale*size) + (yscale * 62), z);
	glRotatef(9, 1, 0, 0);
	glScalef(xscale, yscale, zscale);
	glColor3f(83.0 / 255.0, 135.0 / 255.0, 5.0 / 255.0);		//Yellow

	gluCylinder(Cylinder, size, size, size * 10, 10, 10);		//Cylinder

	glutSolidSphere(size, 10, 10);								//Sphere

	glTranslatef(0, 0, size*10);
	glutSolidCone(size, size * 7, 10, 10);						//Cone
	glTranslatef(0, 0, -size * 10);

	glTranslatef(0, 0, size * 3);
	glBegin(GL_POLYGON);										//Front Wings Layer 1
	glVertex3f(0, size / 2, 0);
	glVertex3f(size * 10, size / 2, size*1.5);
	glVertex3f(size * 10, size / 2, size*3);
	glVertex3f(0, size / 2, size * 3);
	glVertex3f(-size * 10, size / 2, size*3);
	glVertex3f(-size * 10, size / 2, size*1.5);
	glVertex3f(0, size / 2, 0);
	glEnd();
	glBegin(GL_POLYGON);										//Front Wings Layer 2
	glVertex3f(0, size / 2+5, 0);
	glVertex3f(size * 10, size / 2 + 5, size*1.5);
	glVertex3f(size * 10, size / 2 + 5, size * 3);
	glVertex3f(0, size / 2 + 5, size * 3);
	glVertex3f(-size * 10, size / 2 + 5, size * 3);
	glVertex3f(-size * 10, size / 2 + 5, size*1.5);
	glVertex3f(0, size / 2 + 5, 0);
	glEnd();
	glColor3f(0.0 / 255.0, 19.0 / 255.0, 161.0 / 255.0);			//Blue
	glBegin(GL_POLYGON);										//Front Wings thickness Fill
	glVertex3f(size * 10, size / 2 + 5, size*1.5);
	glVertex3f(-size * 10, size / 2 + 5, size*1.5);
	glVertex3f(-size * 10, size / 2 , size*1.5);
	glVertex3f(size * 10, size / 2 , size*1.5);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(size * 10, size / 2 + 5, size*1.5);
	glVertex3f(size * 10, size / 2 + 5, size * 3);
	glVertex3f(size * 10, size / 2 , size * 3);
	glVertex3f(size * 10, size / 2 , size*1.5);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(-size * 10, size / 2 + 5, size*1.5);
	glVertex3f(-size * 10, size / 2 + 5, size * 3);
	glVertex3f(-size * 10, size / 2, size * 3);
	glVertex3f(-size * 10, size / 2, size*1.5);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(-size * 10, size / 2 + 5, size*1.5);
	glVertex3f(0, size / 2 + 5, 0);
	glVertex3f(0, size / 2 , 0);
	glVertex3f(-size * 10, size / 2, size*1.5);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(size * 10, size / 2 + 5, size*1.5);
	glVertex3f(0, size / 2 + 5, 0);
	glVertex3f(0, size / 2, 0);
	glVertex3f(size * 10, size / 2, size*1.5);
	glEnd();

	glColor3f(83.0 / 255.0, 135.0 / 255.0, 5.0 / 255.0);		//Yellow
	glTranslatef(0, 0, size * 11);
	glScalef(0.5, 0.5, 0.5);
	glBegin(GL_POLYGON);										//Back Wings Layer 1
	glVertex3f(0, size / 2, 0);
	glVertex3f(size * 10, size / 2, size*1.5);
	glVertex3f(size * 10, size / 2, size * 3);
	glVertex3f(0, size / 2, size * 3);
	glVertex3f(-size * 10, size / 2, size * 3);
	glVertex3f(-size * 10, size / 2, size*1.5);
	glVertex3f(0, size / 2, 0);
	glEnd();
	glBegin(GL_POLYGON);										//Back Wings Layer 2
	glVertex3f(0, size / 2 + 5, 0);
	glVertex3f(size * 10, size / 2 + 5, size*1.5);
	glVertex3f(size * 10, size / 2 + 5, size * 3);
	glVertex3f(0, size / 2 + 5, size * 3);
	glVertex3f(-size * 10, size / 2 + 5, size * 3);
	glVertex3f(-size * 10, size / 2 + 5, size*1.5);
	glVertex3f(0, size / 2 + 5, 0);
	glEnd();
	glColor3f(0.0 / 255.0, 19.0 / 255.0, 161.0 / 255.0);			//Blue
	glBegin(GL_POLYGON);										//Back Wings thickness Fill
	glVertex3f(size * 10, size / 2 + 5, size*1.5);
	glVertex3f(-size * 10, size / 2 + 5, size*1.5);
	glVertex3f(-size * 10, size / 2, size*1.5);
	glVertex3f(size * 10, size / 2, size*1.5);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(size * 10, size / 2 + 5, size*1.5);
	glVertex3f(size * 10, size / 2 + 5, size * 3);
	glVertex3f(size * 10, size / 2, size * 3);
	glVertex3f(size * 10, size / 2, size*1.5);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(-size * 10, size / 2 + 5, size*1.5);
	glVertex3f(-size * 10, size / 2 + 5, size * 3);
	glVertex3f(-size * 10, size / 2, size * 3);
	glVertex3f(-size * 10, size / 2, size*1.5);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(-size * 10, size / 2 + 5, size*1.5);
	glVertex3f(0, size / 2 + 5, 0);
	glVertex3f(0, size / 2, 0);
	glVertex3f(-size * 10, size / 2, size*1.5);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(size * 10, size / 2 + 5, size*1.5);
	glVertex3f(0, size / 2 + 5, 0);
	glVertex3f(0, size / 2, 0);
	glVertex3f(size * 10, size / 2, size*1.5);
	glEnd();

	glColor3f(83.0 / 255.0, 135.0 / 255.0, 5.0 / 255.0);		//Yellow
	glTranslatef(0, 0, size*3);									
	glBegin(GL_POLYGON);										//Back Top Wing layer 1
	glVertex3f(size / 4, size / 2, 0);
	glVertex3f(size / 4, size / 2, -size*3);
	glVertex3f(size / 4, size *5, -size * 2);
	glVertex3f(size / 4, size *5, 0);
	glEnd();
	glBegin(GL_POLYGON);										//Back Top Wing layer 2
	glVertex3f(-size / 4, size / 2, 0);
	glVertex3f(-size / 4, size / 2, -size * 3);
	glVertex3f(-size / 4, size * 5, -size * 2);
	glVertex3f(-size / 4, size * 5, 0);
	glEnd();

	glColor3f(0.0 / 255.0, 19.0 / 255.0, 161.0 / 255.0);			//Blue
	glBegin(GL_POLYGON);										//Back Top Wing Thickness Fill
	glVertex3f(size / 4, size / 2, 0);
	glVertex3f(size / 4, size *5, 0);
	glVertex3f(-size / 4, size *5, 0);
	glVertex3f(-size / 4, size / 2, 0);
	glEnd();
	glBegin(GL_POLYGON);										//Back Top Wing Thickness Fill
	glVertex3f(size / 4, size / 2, -size * 3);
	glVertex3f(-size / 4, size / 2, -size * 3);
	glVertex3f(-size / 4, size * 5, -size * 2);
	glVertex3f(size / 4, size * 5, -size * 2);
	glEnd();
	glBegin(GL_POLYGON);										//Back Top Wing Thickness Fill
	glVertex3f(size / 4, size * 5, 0);
	glVertex3f(-size / 4, size * 5, 0);
	glVertex3f(-size / 4, size * 5, -size * 2);
	glVertex3f(size / 4, size * 5, -size * 2);
	glEnd();

	glColor3f(31.0 / 255.0, 26.0 / 255.0, 26.0 / 255.0);		//Brown
	glTranslatef(0, 0, -size * 33);
	glRotatef(80, 0, 0, 1);
	glutSolidSphere(size /2, 10, 10);							//Fan Centre
	glColor3f(199.0 / 255.0, 10.0 / 255.0, 10.0/255.0);			//Pink
	glBegin(GL_POLYGON);										//Fan Wings Layer 1
	glVertex3f(0, size / 2, 0);
	glVertex3f(size / 2, size * 4, 0);
	glVertex3f(0, size * 6, 0);
	glVertex3f(-size / 2, size * 4, 0);
	glVertex3f(size / 2, -size * 4, 0);
	glVertex3f(0, -size * 6, 0);
	glVertex3f(-size / 2, -size * 4, 0);
	glVertex3f(0, size / 2, 0);
	glEnd();
	glBegin(GL_POLYGON);										//Fan Wings Layer 1
	glVertex3f(0, size / 2, 2);
	glVertex3f(size / 2, size * 4, 2);
	glVertex3f(0, size * 6, 2);
	glVertex3f(-size / 2, size * 4, 2);
	glVertex3f(size / 2, -size * 4, 2);
	glVertex3f(0, -size * 6, 2);
	glVertex3f(-size / 2, -size * 4, 2);
	glVertex3f(0, size / 2, 2);
	glEnd();
	glRotatef(-80, 0, 0, 1);


	glColor3f(31.0 / 255.0, 26.0 / 255.0, 26.0 / 255.0);		//Brown
	glTranslatef(0, 0, size * 20);
	glRotatef(90, 1, 0, 0);
	gluCylinder(Cylinder, size / 10, size / 10, size * 5, 10, 10);		//Back Leg
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, -size*5, 0);
	glutSolidSphere(size / 2, 10, 10);									//Back Wheel
	glTranslatef(0, size * 5, 0);
	
	glColor3f(31.0 / 255.0, 26.0 / 255.0, 26.0 / 255.0);		//Brown
	glTranslatef(0, 0, -size * 15);
	glRotatef(90, 0, 1, 0);
	glRotatef(60, 1, 0, 0);
	gluCylinder(Cylinder, size / 10, size / 10, size * 8, 10, 10);		//Right Leg
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, -size * 8, 0);
	glutSolidSphere(size / 2, 10, 10);									//Right Wheel
	glTranslatef(0, size * 8, 0);
	glRotatef(90, 1, 0, 0);

	glColor3f(31.0 / 255.0, 26.0 / 255.0, 26.0 / 255.0);		//Brown
	glRotatef(-120, 1, 0, 0);
	glRotatef(180, 0, 1, 0);
	gluCylinder(Cylinder, size / 10, size / 10, size * 8, 10, 10);		//Left Leg
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, -size * 8, 0);
	glutSolidSphere(size / 2, 10, 10);									//Left Wheel
	glTranslatef(0, size * 8, 0);
	glRotatef(90, 1, 0, 0);


	glPopMatrix();


}

void drawSun()
{

	GLfloat size = 200;
	GLfloat xscale = 3, yscale = 3, zscale = 3;
	GLfloat x, y, z;
	x = 5000;
	y = 8000;
	z = 5000;

	glPushMatrix();
	glTranslatef(x, y + (yscale*size), z);	//Sun
	glScalef(xscale, yscale, zscale);
	glColor3f(1,1,0);
	glutSolidSphere(size, 20, 20);

	glPopMatrix();
}

void drawMoon()
{

	GLfloat size = 120;
	GLfloat xscale = 3, yscale = 3, zscale = 3;
	GLfloat x, y, z;
	x = 5000;
	y = 8000;
	z = 5000;

	glPushMatrix();
	glTranslatef(x, y + (yscale*size), z);	//Moon
	glScalef(xscale, yscale, zscale);
	glColor3f(180.0 / 255.0, 204.0 / 255.0, 217.0 / 255.0);
	glutSolidSphere(size, 10, 10);

	glPopMatrix();
}

void drawTree(GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat size = 7;
	GLfloat xscale = 3, yscale = 10, zscale = 3;

	glPushMatrix();
	glTranslatef(x, (yscale*size / 8), z);
	glScalef(xscale, yscale, zscale);
	glRotatef(90 * 3, 1, 0, 0);
	glColor3f(181.0/255.0, 75.0/255.0, 0);
	glutSolidCone(size, size * 5, 1000, 1000);	//Tree Pole
	glPopMatrix();


	glPushMatrix();
	glTranslatef(x, (yscale*size), z);
	glScalef(xscale, yscale, zscale);
	glRotatef(90 * 3, 1, 0, 0);
	glColor3f(26.0 / 255.0, 161.0 / 255.0, 13.0/255.0);
	glutSolidCone(size*6, size * 3, 8, 8);	//Tree Leaves Base 1
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, (yscale*size*2), z);
	glScalef(xscale, yscale, zscale);
	glRotatef(90 * 3, 1, 0, 0);
	glColor3f(88.0 / 255.0, 199.0 / 255.0, 78.0 / 255.0);
	glutSolidCone(size * 5, size * 3, 8, 8);	//Tree Leaves Base 2
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, (yscale*size * 3), z);
	glScalef(xscale, yscale, zscale);
	glRotatef(90 * 3, 1, 0, 0);
	glColor3f(26.0 / 255.0, 161.0 / 255.0, 13.0 / 255.0);
	glutSolidCone(size * 4, size * 2.5, 8, 8);	//Tree Leaves Base 3
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, (yscale*size * 4), z);
	glScalef(xscale, yscale, zscale);
	glRotatef(90 * 3, 1, 0, 0);
	glColor3f(88.0 / 255.0, 199.0 / 255.0, 78.0 / 255.0);
	glutSolidCone(size * 3, size * 2, 8, 8);	//Tree Leaves Base 4
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, (yscale*size * 5), z);
	glScalef(xscale, yscale, zscale);
	glRotatef(90 * 3, 1, 0, 0);
	glColor3f(26.0 / 255.0, 161.0 / 255.0, 13.0 / 255.0);
	glutSolidCone(size * 2, size * 1, 8, 8);	//Tree Leaves Base 5
	glPopMatrix();

}

void drawSingleBlade(GLfloat size)
{
	glBegin(GL_POLYGON);
	glVertex3f(-(size / 2), 0, (size / 8));
	glVertex3f((size / 2), 0, (size / 8));
	glVertex3f((size / 4), (size*10), (size / 8));
	glVertex3f(-(size / 4), (size * 10), (size / 8));
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-(size / 2), 0, (size / 4));
	glVertex3f((size / 2), 0, (size / 4));
	glVertex3f((size / 4), (size * 10), (size / 4));
	glVertex3f(-(size / 4), (size * 10), (size / 4));
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-(size / 2), 0, (size / 8));
	glVertex3f(-(size / 2), 0, (size / 4));
	glVertex3f((size / 2), 0, (size / 8));
	glVertex3f((size / 2), 0, (size / 4));
	glVertex3f((size / 4), (size * 10), (size / 8));
	glVertex3f((size / 4), (size * 10), (size / 4));
	glVertex3f(-(size / 4), (size * 10), (size / 8));
	glVertex3f(-(size / 4), (size * 10), (size / 4));
	glEnd();



}
void drawThreeBlades(GLfloat x, GLfloat y, GLfloat z, GLfloat size, GLfloat xscale, GLfloat yscale, GLfloat zscale)
{
	glPushMatrix();
	glScalef(xscale, yscale / (xscale - 1), zscale * 2);
	drawSingleBlade(size);
	glRotatef(120, 0, 0, 1);
	drawSingleBlade(size);
	glRotatef(120, 0, 0, 1);
	drawSingleBlade(size);
	glRotatef(120, 0, 0, 1);
	glPopMatrix();
}
void drawWindMill(GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat size = 5;
	GLfloat xscale = 3, yscale = 10, zscale = 3;
	
	glPushMatrix();
	glTranslatef(x, (yscale*size / 8), z);	
	glScalef(xscale, yscale, zscale);
	glRotatef(90*3, 1, 0, 0);
	glColor3f(0.823, 0.819, 0.89);
	glutSolidCone(size, size*15, 10, 10);	//WindMill Pole
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, (yscale*size/2)+(size*130), z);
	glScalef(xscale, yscale/(xscale-1), zscale*2);
	glutSolidCube(size * 2);					//Windmill Machine
	glPopMatrix();


	glPushMatrix();								//Windmill Blades
	glTranslatef(x, (yscale*size / 2) + (size * 130), z+size);
	if (windmill_rotate)
	{
		glRotatef(angle, 0, 0, 1);
	}
	drawThreeBlades(x, y, z, size, xscale, yscale, zscale);
	glPopMatrix();

}

void drawClouds()
{
	GLfloat size = 10;
	GLfloat xscale = 5, yscale = 5, zscale = 10;
	GLfloat x, y, z;
	x = 0;
	y = 0;
	z = 0;
	for (z = 1; z <= 1000; z += 500)
	{
		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GREATER, 0.0 / 255.0);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glColor4ub(255, 255, 255, 200);
		glTranslatef(x * 10, y + (yscale*size) + 3000, z * 10);	//Cloud 1
		glScalef(xscale, yscale, zscale);
		glutSolidSphere(size, 10, 10);
		glTranslatef(5, -4, -3);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(8, -4, 6);
		glutSolidSphere(size * 1, 10, 10);
		glTranslatef(10, 4, 2);
		glutSolidSphere(size*1.5, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.3, 10, 10);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GREATER, 0.0 / 255.0);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glColor4ub(255, 255, 255, 200);
		glTranslatef(x * 10 - 900, y + (yscale*size) + 3000, z * 10 - 600);	//Cloud 2
		glScalef(xscale, yscale, zscale);
		glutSolidSphere(size, 10, 10);
		glTranslatef(5, -4, -3);
		glutSolidSphere(size*1.8, 10, 10);
		glTranslatef(15, 9, 12);
		glutSolidSphere(size * 2.3, 10, 10);
		glTranslatef(19, 14, -16);
		glutSolidSphere(size * 1, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*2.7, 10, 10);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GREATER, 0.0 / 255.0);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glColor4ub(255, 255, 255, 200);
		glTranslatef(x * 10 - 600, y + (yscale*size) + 3000, z * 10 + 600);	//Cloud 3
		glScalef(xscale, yscale, zscale);
		glutSolidSphere(size, 10, 10);
		glTranslatef(5, -4, -3);
		glutSolidSphere(size*1.2, 10, 10);
		glTranslatef(15, 9, 2);
		glutSolidSphere(size * 1.3, 10, 10);
		glTranslatef(9, 14, -16);
		glutSolidSphere(size * 1, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.7, 10, 10);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GREATER, 0.0 / 255.0);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glColor4ub(255, 255, 255, 200);
		glTranslatef(x * 10 + 600, y + (yscale*size) + 3000, z * 10 + 600);	//Cloud 4
		glScalef(xscale, yscale, zscale);
		glutSolidSphere(size, 10, 10);
		glTranslatef(5, -4, -3);
		glutSolidSphere(size*1.2, 10, 10);
		glTranslatef(15, 9, 2);
		glutSolidSphere(size * 1.3, 10, 10);
		glTranslatef(9, 14, -16);
		glutSolidSphere(size * 1, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.7, 10, 10);
		glTranslatef(15, 9, 2);
		glutSolidSphere(size * 1.3, 10, 10);
		glTranslatef(9, 14, -16);
		glutSolidSphere(size * 1, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.7, 10, 10);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GREATER, 100.0 / 255.0);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glColor4ub(255, 255, 255, 200);
		glTranslatef(x * 10 + 600, y + (yscale*size) + 3000, z * 10 - 600);	//Cloud 5
		glScalef(xscale, yscale, zscale);
		glutSolidSphere(size, 10, 10);
		glTranslatef(5, -4, -3);
		glutSolidSphere(size*1.2, 10, 10);
		glTranslatef(15, 9, 2);
		glutSolidSphere(size * 1.3, 10, 10);
		glTranslatef(9, 14, -16);
		glutSolidSphere(size * 1, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.7, 10, 10);
		glTranslatef(15, 9, 2);
		glutSolidSphere(size * 1.3, 10, 10);
		glTranslatef(9, 14, -16);
		glutSolidSphere(size * 1, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.7, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.7, 10, 10);
		glTranslatef(-30, 9, 2);
		glutSolidSphere(size * 1.3, 10, 10);
		glTranslatef(9, 14, -16);
		glutSolidSphere(size * 1, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.7, 10, 10);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GREATER, 0.0 / 255.0);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glColor4ub(240, 240, 240, 200);
		glTranslatef(x * 10 - 1600, y + (yscale*size) + 3000, z * 10 + 1000);	//Cloud 6
		glRotatef(30, 0, 1, 0);
		glScalef(xscale, yscale, zscale);
		glutSolidSphere(size, 10, 10);
		glTranslatef(5, -4, -3);
		glutSolidSphere(size*1.2, 10, 10);
		glTranslatef(15, 9, 2);
		glutSolidSphere(size * 1.3, 10, 10);
		glTranslatef(9, 14, -16);
		glutSolidSphere(size * 1, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.7, 10, 10);
		glTranslatef(15, 9, 2);
		glutSolidSphere(size * 1.3, 10, 10);
		glTranslatef(9, 14, -16);
		glutSolidSphere(size * 1, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.7, 10, 10);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GREATER, 0.0 / 255.0);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glColor4ub(255, 255, 255, 200);
		glTranslatef(x * 10 + 1200, y + (yscale*size) + 3000, z * 10 + 1200);	//Cloud 7
		glScalef(xscale, yscale, zscale);
		glutSolidSphere(size, 10, 10);
		glTranslatef(5, -4, -3);
		glutSolidSphere(size*1.2, 10, 10);
		glTranslatef(15, 9, 2);
		glutSolidSphere(size * 1.3, 10, 10);
		glTranslatef(9, 14, -16);
		glutSolidSphere(size * 1, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.7, 10, 10);
		glTranslatef(15, 9, 2);
		glutSolidSphere(size * 1.3, 10, 10);
		glTranslatef(9, 14, -16);
		glutSolidSphere(size * 1, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.7, 10, 10);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GREATER, 100.0 / 255.0);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glColor4ub(255, 255, 255, 200);
		glTranslatef(x * 10 + 1400, y + (yscale*size) + 3000, z * 10 - 1600);	//Cloud 8
		glScalef(xscale, yscale, zscale);
		glutSolidSphere(size, 50, 50);
		glTranslatef(5, -4, -3);
		glutSolidSphere(size*1.2, 10, 10);
		glTranslatef(15, 9, 2);
		glutSolidSphere(size * 1.3, 10, 10);
		glTranslatef(9, 14, -16);
		glutSolidSphere(size * 1, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.7, 10, 10);
		glTranslatef(15, 9, 2);
		glutSolidSphere(size * 1.3, 10, 10);
		glTranslatef(9, 14, -16);
		glutSolidSphere(size * 1, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.7, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.7, 10, 10);
		glTranslatef(-30, 9, 2);
		glutSolidSphere(size * 1.3, 10, 10);
		glTranslatef(9, 14, -16);
		glutSolidSphere(size * 1, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.7, 10, 10);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glPopMatrix();

		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GREATER, 0.0 / 255.0);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glColor4ub(240, 240, 240, 200);
		glTranslatef(x * 10 - 3500, y + (yscale*size) + 3000, z * 10 + 2000);	//Cloud 9
		glRotatef(30, 0, 1, 0);
		glScalef(xscale, yscale, zscale);
		glutSolidSphere(size, 10, 10);
		glTranslatef(5, -4, -3);
		glutSolidSphere(size*1.2, 10, 10);
		glTranslatef(15, 9, 2);
		glutSolidSphere(size * 1.3, 10, 10);
		glTranslatef(9, 14, -16);
		glutSolidSphere(size * 1, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.7, 10, 10);
		glTranslatef(15, 9, 2);
		glutSolidSphere(size * 1.3, 10, 10);
		glTranslatef(9, 14, -16);
		glutSolidSphere(size * 1, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*0.8, 10, 10);
		glTranslatef(5, -7, 5);
		glutSolidSphere(size*1.7, 10, 10);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glPopMatrix();
	}
}

void drawRunway()
{
	GLfloat size = 10;
	GLfloat xscale = 500, yscale = 1, zscale = 150;
	GLfloat x, y, z;
	x = -8000;
	y = 1;
	z = 0;
	float i;

	glPushMatrix();
	glTranslatef(x, 2, z);
	glScalef(xscale, yscale, zscale);
	glColor3f(8.0 / 255.0, 8.0 / 255.0, 10.0 / 255.0);
	glutSolidCube(size);
	glColor3f(1, 1, 1);
	glTranslatef(-5.5, 10, 0);
	for (i = 0.0; i < 10.0; i += 1.0)
	{
		glTranslatef(1, 0, 0);
		glutSolidCube(size / 20);
	}
	glPushMatrix();
	glTranslatef(-4.5, 0, 4.5);
	glScalef(20, yscale, 1);
	glutSolidCube(size / 20);
	glTranslatef(0, 0, -9);
	glutSolidCube(size / 20);
	glPopMatrix();
	glPopMatrix();
}

void drawViewVolume()
{

	glLineWidth(2);
	glColor3f(0, 0, 1);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-500, 0, -500);
	glVertex3f(-500, 1000, -500);
	glVertex3f(-500, 1000, 500);
	glVertex3f(-500, 0, 500);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(500, 0, -500);
	glVertex3f(500, 1000, -500);
	glVertex3f(500, 1000, 500);
	glVertex3f(500, 0, 500);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(-500, 0, -500);
	glVertex3f(500, 0, -500);
	glVertex3f(-500, 1000, -500);
	glVertex3f(500, 1000, -500);
	glVertex3f(-500, 1000, 500);
	glVertex3f(500, 1000, 500);
	glVertex3f(-500, 0, 500);
	glVertex3f(500, 0, 500);
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(-100, 0, 0);
	glVertex3f(100, 0, 0);
	glVertex3f(0, -100, 0);
	glVertex3f(0, 100, 0);
	glVertex3f(0, 0, -100);
	glVertex3f(0, 0, 100);
	glEnd();

}

void drawPlaneWindow()
{
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 600);
	glDepthFunc(GL_ALWAYS);

	glColor3f(40.0 / 255.0, 40.0 / 255.0, 48.0 / 255.0);
	glBegin(GL_POLYGON);		//Window Divider
	glVertex2f(450, 600);
	glVertex2f(480, 0);
	glVertex2f(520, 0);
	glVertex2f(550, 600);
	glEnd();

	glBegin(GL_POLYGON);		//Top Window Panel
	glVertex2f(500, 480);
	glVertex2f(0, 600);
	glVertex2f(1000, 600);
	glVertex2f(500, 480);
	glEnd();

	glBegin(GL_POLYGON);		//Bottom Window Panel
	glVertex2f(500, 20);
	glVertex2f(0, 40);
	glVertex2f(0, 0);
	glVertex2f(1000, 0);
	glVertex2f(1000, 40);
	glVertex2f(500, 20);
	glEnd();

	glBegin(GL_POLYGON);		//Left Window Panel
	glVertex2f(10, 600);
	glVertex2f(0, 600);
	glVertex2f(0, 0);
	glVertex2f(30, 0);
	glEnd();

	glBegin(GL_POLYGON);		//Right Window Panel
	glVertex2f(990, 600);
	glVertex2f(1000, 600);
	glVertex2f(1000, 0);
	glVertex2f(970, 0);
	glEnd();
	
	
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.0 / 255.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glColor4ub(149, 151, 207, 50);
	glBegin(GL_POLYGON);		//Translucent Glass shade
	glVertex2f(0, 0);
	glVertex2f(0, 600);
	glVertex2f(1000, 600);
	glVertex2f(1000, 0);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glPopMatrix();
	glDepthFunc(GL_LESS);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glutPostRedisplay();
}

void drawText(GLfloat x, GLfloat y, GLfloat z, char *str)
{
	int i;
	
	glColor3f(1, 0, 0);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 600);
	if (info_display)
	{
		glColor3f(1, 1, 0);
		glBegin(GL_POLYGON);
		glVertex2f(0, 0);
		glVertex2f(1000, 0);
		glVertex2f(1000, 600);
		glVertex2f(0, 600);
		glEnd();
	}
	glDepthFunc(GL_ALWAYS);
	glColor3f(1, 0, 0);
	glRasterPos2f(x, y);
	for (i = 0; str[i] != '\0'; ++i)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	glDepthFunc(GL_LESS);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}

void drawStrokeText(GLfloat x, GLfloat y, GLfloat sx, GLfloat sy, char str[], GLfloat width, GLubyte R, GLubyte G, GLubyte B)
{
	int i;
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 600);
	if (info_display)
	{
		glColor3f(0, 0, 0);
		glBegin(GL_POLYGON);
		glVertex2f(0, 0);
		glVertex2f(1000, 0);
		glVertex2f(1000, 600);
		glVertex2f(0, 600);
		glEnd();
	}
	glDepthFunc(GL_ALWAYS);
	glColor3ub(R, G, B);
	glLineWidth(width);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(sx, sy, 0);
	for (i = 0; str[i] != '\0'; ++i)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	glPopMatrix();
	glDepthFunc(GL_LESS);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}

void spin()
{
	angle = (int)(angle + 20) % 360;
	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y)
{
	if (key == 27)exit(0);
	if (key == 'i')
	{
		info_display = !info_display;
		g_fps_mode = false;
	}
	if (key == ' ') 
	{
		plane_move = !plane_move;
		g_fps_mode = !g_fps_mode;

		if (info_display)
		{
			g_fps_mode = false;
		}

		if (g_fps_mode) 
		{
			glutSetCursor(GLUT_CURSOR_NONE);
			glutWarpPointer(g_viewport_width / 2, g_viewport_height / 2);
		}
		else 
		{
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		}
	}

	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) 
	{
		g_shift_down = true;
	}
	else 
	{
		g_shift_down = false;
	}

	g_key[key] = true;
	glutPostRedisplay();
}
void KeyboardUp(unsigned char key, int x, int y)
{
	g_key[key] = false;
}
void Timer(int value)
{
	if (g_fps_mode) {
		if (g_key['w'] || g_key['W']) {
			Move(g_translation_speed);
		}
		else if (g_key['s'] || g_key['S']) {
			Move(-g_translation_speed);
		}
		else if (g_key['a'] || g_key['A']) {
			Strafe(g_translation_speed);
		}
		else if (g_key['d'] || g_key['D']) {
			Strafe(-g_translation_speed);
		}
		else if (g_mouse_left_down) {
			Fly(-g_translation_speed);
		}
		else if (g_mouse_right_down) {
			Fly(g_translation_speed);
		}
	}

	glutTimerFunc(1, Timer, 0);
}
void mouse(int btn, int state, int x, int y)
{

	if (state == GLUT_DOWN) 
	{
		if (btn == GLUT_LEFT_BUTTON)
		{
			g_mouse_left_down = true;
		}
		else if (btn == GLUT_RIGHT_BUTTON)
		{
			g_mouse_right_down = true;
		}
	}
	else if (state == GLUT_UP) 
	{
		if (btn == GLUT_LEFT_BUTTON)
		{
			g_mouse_left_down = false;
		}
		else if (btn == GLUT_RIGHT_BUTTON)
		{
			g_mouse_right_down = false;
		}
	}

	glutPostRedisplay();
}
void MouseMotion(int x, int y)
{
	// This variable is hack to stop glutWarpPointer from triggering an event callback to Mouse(...)
	// This avoids it being called recursively and hanging up the event loop
	static bool just_warped = false;

	if (just_warped) {
		just_warped = false;
		return;
	}

	if (g_fps_mode) {
		int dx = x - g_viewport_width / 2;
		int dy = y - g_viewport_height / 2;

		if (dx) {
			RotateYaw(g_rotation_speed*dx);
		}

		if (dy) {
			RotatePitch(g_rotation_speed*dy);
		}

		glutWarpPointer(g_viewport_width / 2, g_viewport_height / 2);

		just_warped = true;
	}
}

void reshape(int w, int h)
{
	g_viewport_width = w;
	g_viewport_height = h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, (GLfloat)w/(GLfloat)h, 50, 50000);


	//glFrustum(-1000, 1000, -1000, 1000, 10, 100);
	//glOrtho(-1000, 1000, -500, 1500, -10000, 10000);

	glMatrixMode(GL_MODELVIEW);
}

void lighting()
{
	GLfloat mat_ambient[] = { 2, 2, 0.8, 1 };
	GLfloat mat_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat mat_specular[] = { 2, 2, 2, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };

	GLfloat lightintensity[] = { 1, 1, 0.7, 1.0 };
	GLfloat lightPosition[] = { 3900, 8900, 3900, 0 };

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glLightfv(GL_LIGHT0, GL_AMBIENT, mat_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, lightintensity);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightintensity);
}

void DroneMode()
{
	drone_mode = true;
	walk_Mode = false;
	fly_Mode = false;
	m_x = 0, m_y = 80, m_z = 5000;
	m_yaw = 0.0, m_pitch = 0.0;
}
void WalkMode()
{
	walk_Mode = true;
	fly_Mode = false;
	drone_mode = false;
	m_x = 0, m_y = 80, m_z = 5000;
	m_yaw = 0.0, m_pitch = 0.0;
}
void FlyMode()
{
	fly_Mode = true;
	walk_Mode = false;
	drone_mode = false;
	plane_move = true;
	m_x = -10000, m_y = 80, m_z = 0;
	m_yaw = 0.0, m_pitch = 0.0;
}
void House()
{
	//drawHouse(x, y, z, R, G, B, angle)
	if (house1)
		drawHouse(300, 0, -250, 9, 109, 143, 30);			//Sky Blue
	if (house2)
		drawHouse(8000, 0, 5500, 41, 21, 214, 60);			//Blue
	if (house3)
		drawHouse(1000, 0, 2000, 45, 134, 181, -45);		//Blue
	if (house4)
		drawHouse(-300, 0, 2500, 196, 123, 27, 120);		//Yellow
	if (house5)
		drawHouse(6000, 0, 4500, 60, 110, 0, 150);			//Green
}
void WindMill()
{
	//drawWindMill(x, y, z)
	if (windmill1)
		drawWindMill(2000, 0, -3000);
	if (windmill2)
		drawWindMill(5000, 0, -5000);
	if (windmill3)
		drawWindMill(3000, 0, -4000);
	if (windmill4)
		drawWindMill(4000, 0, -1000);
	if (windmill5)
		drawWindMill(3000, 0, -2000);
}
void SimulationMenu(int item)
{
	switch (item)
	{
	case 7: DroneMode(); break;
	case 1: WalkMode(); break;
	case 2: FlyMode(); break;
	case 3: dayLight = true; break;
	case 4:	dayLight = false; break;
	case 5: windmill_rotate = !windmill_rotate; break;
	case 6: exit(0);
	default:break;
	}
}
void AddMenu(int item)
{
	switch (item)
	{
	case 1: house1 = !house1; break;
	case 2: house2 = !house2; break;
	case 3: house3 = !house3; break;
	case 4: house4 = !house4; break;
	case 5: house5 = !house5; break;
	case 6: windmill1 = !windmill1; break;
	case 7: windmill2 = !windmill2; break;
	case 8: windmill3 = !windmill3; break;
	case 9: windmill4 = !windmill4; break;
	case 10: windmill5 = !windmill5; break;
	default:break;

	}
}

void Menu()
{

	int add_menu;

	add_menu = glutCreateMenu(AddMenu);
	glutAddMenuEntry("Add/Remove House 1", 1);
	glutAddMenuEntry("Add/Remove House 2", 2);
	glutAddMenuEntry("Add/Remove House 3", 3);
	glutAddMenuEntry("Add/Remove House 4", 4);
	glutAddMenuEntry("Add/Remove House 5", 5);
	glutAddMenuEntry("Add/Remove WindMill 1", 6);
	glutAddMenuEntry("Add/Remove WindMill 2", 7);
	glutAddMenuEntry("Add/Remove WindMill 3", 8);
	glutAddMenuEntry("Add/Remove WindMill 4", 9);
	glutAddMenuEntry("Add/Remove WindMill 5", 10);

	glutCreateMenu(SimulationMenu);
	glutAddMenuEntry("Drone Mode", 7);
	glutAddMenuEntry("Walking Simulation ", 1);
	glutAddMenuEntry("Aircraft Simulation", 2);
	glutAddMenuEntry("Day Mode", 3);
	glutAddMenuEntry("Night Mode", 4);
	glutAddMenuEntry("Start/Stop WindMill", 5);
	glutAddSubMenu("Add/Remove Objects", add_menu);
	glutAddMenuEntry("Exit          (ESC)", 6);


	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
}
void displayInfo()
{
	//drawText(200, 500,0, "HELLO WORLD");

	//drawStrokeText(x, y, sx, sy, str[], width, R, G, B);
	drawStrokeText(130, 530, 0.35, 0.4, "Bangalore Institute of Technology", 4, 0, 0, 255);
	drawStrokeText(170, 490, 0.2, 0.2, "Department of Computer Science & Engineering", 2.5, 0, 0, 255);
	drawStrokeText(190, 370, 0.6, 0.6, "3D LANDSCAPE", 20, 255, 0, 0);
	drawStrokeText(400, 310, 0.2, 0.2, "VI Semester", 2.2, 0, 255, 0);
	drawStrokeText(370, 280, 0.2, 0.2, "Graphics Package", 2.2, 0, 255, 0);
	drawStrokeText(400, 230, 0.2, 0.2, "Submitted By", 2.2, 0, 255, 255);
	drawStrokeText(230, 190, 0.2, 0.2, "1BI13CS063", 2.5, 255, 255, 0);
	drawStrokeText(600, 190, 0.2, 0.2, "HITESH RS", 2.5, 255, 255, 0);
	drawStrokeText(300, 130, 0.2, 0.2, "for the academic year 2015-16", 2.2, 255, 0, 255);
	

	drawStrokeText(280, 20, 0.15, 0.15, "(Press i to switch on/off Information Window)", 2, 255, 0, 0);
}
void display()
{
	char str[50];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (dayLight)
	{
		glClearColor(150.0 / 255.0, 201.0 / 255.0, 242.0 / 255.0, 1);
	}
	else
	{
		glClearColor(29.0 / 255.0, 63.0 / 255.0, 82.0 / 255.0, 1);
	}
	
	glLoadIdentity();

	lighting();

	Refresh();

	if (info_display)
	{
		displayInfo();
	}


	//drawViewVolume();

	glColor3f(2.0 / 255.0, 64.0 / 255.0, 0.0 / 255.0);				//World Base
	glBegin(GL_POLYGON);
	glVertex3f(-10000, 0, -10000);
	glVertex3f(-10000, 0, 10000);
	glVertex3f(10000, 0, 10000);
	glVertex3f(10000, 0, -10000);
	glEnd();

	if (dayLight)
	{
		drawSun();
	}
	else
	{
		drawMoon();
	}

	House();

	WindMill();


	//drawTree(x, y, z);
	drawTree(5000, 0, 5000);
	drawTree(4000, 0, 3000);
	drawTree(4000, 0, 2000);
	drawTree(3000, 0, 4000);
	//drawTree(7000, 0, 1000);
	//drawTree(3000, 0, 500);

	drawClouds();

	drawRunway();

	if (!fly_Mode)
	{
		drawPlane();
	}

	if (fly_Mode)
	{
		drawPlaneWindow();
		g_fps_mode = true;
		glutSetCursor(GLUT_CURSOR_NONE);
		if (plane_move)
		{
			Move(g_translation_speed);
		}
		
	}

	if (!info_display)
	{
		if (!g_fps_mode)
		{
			drawText(10, 580, 0, "Press SPACEBAR to start/stop movement");
		}
		else
		{
			if (drone_mode)
				drawText(10, 580, 0, "DRONE MODE: ON");
			if (walk_Mode)
				drawText(10, 580, 0, "WALK MODE: ON");
			if (fly_Mode)
				drawText(10, 580, 0, "AIRPLANE MODE: ON");
			drawText(10, 560, 0, "w: Forward");
			drawText(10, 540, 0, "a: Left");
			drawText(10, 520, 0, "d: Right");
			drawText(10, 500, 0, "s:Backward");
			drawText(10, 480, 0, "Mouse: Rotation");
		}
		if (fly_Mode)
		{
			sprintf(str, "Altitude: %f", m_y);
			drawText(440, 540, 0, str);
		}
	}

	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 600);
	glutCreateWindow("3D Landscape");
	glutDisplayFunc(display);
	

	Menu();
	//glutDisplayFunc(display);
	//glViewport(0, 0, 1200, 700);
	//glutMouseFunc(mouse);
	glutReshapeFunc(reshape);
	//glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboard);
	
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MouseMotion);
	glutKeyboardUpFunc(KeyboardUp);;
	glutTimerFunc(1, Timer, 0);

	glutIdleFunc(spin);
	glutFullScreen();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;
}
