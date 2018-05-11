#include "oglwidget.h"

#include <math.h>
#include <iostream>
#include <vector>

#include "vertex.h"
#include "quad.h"


static std::string path = "D:/Projekte/Qt/ComputerGraphicsProject2/data/";			// Ove Windows
//static std::string path = "/Users/ove/Documents/Qt/ComputerGraphicsProject2/data/";		// Ove MAC


static std::string meshFileName = "cube.obj";
static int meshSudivisions = 4;


static std::string surfaceFileName = "exampleSurface.txt";
static int surfaceResolution = 20;


static std::string curveFileName = "exampleCurve.txt";
static int curveResolution = 20;


static float scale = 3.0f;

static bool drawSurface = true;
static bool drawWireframe = false;

static bool doRotation = true;
static double alpha = 0;         // rotation angle


/// initialize Open GL lighting and projection matrix
void InitLightingAndProjection()    // to be executed once before drawing
{
    // light positions and colors
    GLfloat LightPosition1[4] = {10, 5,  10, 0};
    GLfloat LightPosition2[4] = {-5, 5, -10, 0};
    GLfloat ColorRedish[4]   = {1.0, 0.8, 0.8, 1};  // white with a little bit of red
    GLfloat ColorBlueish[4]  = {0.8, 0.8, 1.0, 1};  // white with a little bit of blue

    glEnable(GL_DEPTH_TEST);        // switch on z-buffer
    glDepthFunc(GL_LESS);

    glShadeModel(GL_SMOOTH);        // Gouraud shading
    //glShadeModel(GL_FLAT);

    glEnable(GL_LIGHTING);          // use lighting
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);      // draw both sides

    // define and switch on light 0
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  ColorRedish);
    glLightfv(GL_LIGHT0, GL_SPECULAR, ColorRedish);
    glEnable(GL_LIGHT0);

    // define and switch on light 1
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  ColorBlueish);
    glLightfv(GL_LIGHT1, GL_SPECULAR, ColorBlueish);
    glEnable(GL_LIGHT1);

    glMatrixMode(GL_PROJECTION);        // define camera projection
    glLoadIdentity();                   // reset matrix to identity (otherwise existing matrix will be multiplied with)
    glOrtho(-15, 15, -10, 10, -20, 30); // orthogonal projection (xmin xmax ymin ymax zmin zmax)
    //glFrustum(-10, 10, -8, 8, 2, 20); // perspective projektion
}

/// define material color properties for front and back side
void SetMaterialColor(int side, float r, float g, float b)
{
    float amb[4], dif[4], spe[4];
    int	i, mat;

    dif[0] = r; // diffuse color as defined by r,g, and b
    dif[1] = g;
    dif[2] = b;
    for (i = 0; i < 3; i++)
    {
        amb[i] = .1 * dif[i]; // ambient color is 10 percent of diffuse
        spe[i] = .5; // specular color is just white / gray
    }

    amb[3] = dif[3] = spe[3] = 1.0; // alpha component is always 1
    switch (side)
    {
        case 1:	mat = GL_FRONT; break;
        case 2:	mat = GL_BACK; break;
        default: mat = GL_FRONT_AND_BACK; break;
    }

    glMaterialfv(mat, GL_AMBIENT, amb); // define ambient, diffuse and specular components
    glMaterialfv(mat, GL_DIFFUSE, dif);
    glMaterialfv(mat, GL_SPECULAR, spe);
    glMaterialf(mat, GL_SHININESS, 50.0); // Phong constant for the size of highlights
}


OGLWidget::OGLWidget(QWidget *parent) : QOpenGLWidget(parent)   // constructor
{
	this->mesh = new Mesh(path+meshFileName, meshSudivisions);
	this->surface = new Surface(path+surfaceFileName, surfaceResolution);
	this->curve = new Curve(path+curveFileName, curveResolution);

	/// Setup the animation timer to fire every x msec
	this->animtimer = new QTimer(this);
	this->animtimer->start(50);

	/// Everytime the timer fires, the animation is going one step forward
	connect(this->animtimer, SIGNAL(timeout()), this, SLOT(stepAnimation()));

    animstep = 0;
}

OGLWidget::~OGLWidget() // destructor
{
	delete this->animtimer;

	delete this->mesh;
    delete this->surface;
	delete this->curve;
}


void OGLWidget::stepAnimation()
{
    animstep++;    // Increase animation steps
    update();      // Trigger redraw of scene with paintGL
}

void OGLWidget::initializeGL()  // initializations to be called once
{
    initializeOpenGLFunctions();
	InitLightingAndProjection(); // define light sources and projection
}

void OGLWidget::paintGL()       // draw everything, to be called repeatedly
{
    glEnable(GL_NORMALIZE);     // this is necessary when using glScale (keep normals to unit length)

	/// set background color
    glClearColor(0.8, 0.8, 1.0, 1.0); // bright blue
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/// draw the scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();               // Reset The Current Modelview Matrix
	glTranslated(-7.5, 0, -10.0);      // Move 10 units backwards in z, since camera is at origin
    glScaled(scale, scale, scale);  // scale objects
	glRotated(alpha, 1, 0, 0);
	if (doRotation)
		alpha += 1;					// continuous rotation

	/// define color: 1=front, 2=back, 3=both, followed by r, g, and b
    SetMaterialColor(1, 1.0, 0.2, 0.2); // front color is red
    SetMaterialColor(2, 0.2, 0.2, 1.0); // back color is blue

	//mesh->DrawMesh(drawSurface, drawWireframe);

	//this->surface->Draw(drawSurface, drawWireframe);
	//this->surface->DrawControlMesh();

	//this->curve->DrawCurve();
	//this->curve->DrawControlPoints();
	this->curve->Draw();

	/// make it appear (before this, it's hidden in the rear buffer)
    glFlush();
}

void OGLWidget::resizeGL(int w, int h) // called when window size is changed
{
	/// adjust viewport transform
    glViewport(0, 0, w, h);
}
