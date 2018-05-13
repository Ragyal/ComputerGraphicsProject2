#include "oglwidget.h"

static std::string path = "D:/Projekte/Qt/ComputerGraphicsProject2/data/";			// Ove Windows
//static std::string path = "/Users/ove/Documents/Qt/ComputerGraphicsProject2/data/";		// Ove MAC


/// Catmull Clark Subdivision
static QVector3D meshTranslation = QVector3D(1.5, 0, 0);
static float meshScale = 2.5f;
static bool meshDoRotation = true;
static double meshAlpha = 90;

static std::string meshFileName = "cube.obj";
static int meshSudivisions = 4;
static bool meshDrawSurface = true;
static bool meshDrawWireframe = false;

/// Bezier Surface
static QVector3D surfaceTranslation = QVector3D(-12, 0, 0);
static float surfaceScale = 2.5f;
static bool surfaceDoRotation = false;
static double surfaceAlpha = 90;

static std::string surfaceFileName = "exampleSurface.txt";
static int surfaceResolution = 100;
static bool surfaceDrawSurface = true;
static bool surfaceDrawWireframe = false;

/// Bezier Curve and Rotation Object
static QVector3D curveTranslation = QVector3D(9, 7, 0);
static float curveScale = 2.5f;
static bool curveDoRotation = true;
static double curveAlpha = 0;

static std::string curveFileName = "exampleCurve.txt";
static int curveResolution = 20;
static bool curveDrawSurface = true;
static bool curveDrawWireframe = false;


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
	this->animtimer->start(25);

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

	/// define color: 1=front, 2=back, 3=both, followed by r, g, and b
	SetMaterialColor(1, 1.0, 0.2, 0.2); // front color is red
	SetMaterialColor(2, 0.2, 0.2, 1.0); // back color is blue

	/// draw the scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();               // Reset The Current Modelview Matrix

	/// Catmull Clark Subdivision
	glPushMatrix();
	glTranslated(meshTranslation.x(), meshTranslation.y(), meshTranslation.z());
	glScaled(meshScale, meshScale, meshScale);
	glRotated(meshAlpha, 0, 1, 1);

	mesh->DrawMesh(meshDrawSurface, meshDrawWireframe);
	glPopMatrix();

	/// Bezier Surface
	glPushMatrix();
	glTranslated(surfaceTranslation.x(), surfaceTranslation.y(), surfaceTranslation.z());
	glScaled(surfaceScale, surfaceScale, surfaceScale);
	glRotated(45, 1, 1, 0);
	glRotated(surfaceAlpha, 1, 0, 0);

	this->surface->Draw(surfaceDrawSurface, surfaceDrawWireframe);
	this->surface->DrawControlMesh();
	glPopMatrix();

	/// Bezier Curve and Rotation Object
	glPushMatrix();
	glTranslated(curveTranslation.x(), curveTranslation.y(), curveTranslation.z());
	glScaled(curveScale, curveScale, curveScale);
	glRotated(90, 0, -1, -1);
	glRotated(curveAlpha, 1, 0, 0);

	//this->curve->DrawCurve();
	//this->curve->DrawControlPoints();
	this->curve->Draw(curveDrawSurface, curveDrawWireframe);
	glPopMatrix();

	/// make it appear (before this, it's hidden in the rear buffer)
    glFlush();

	if (meshDoRotation)
		meshAlpha += 1;
	if (surfaceDoRotation)
		surfaceAlpha += 1;
	if (curveDoRotation)
		curveAlpha += 1;
}

void OGLWidget::resizeGL(int w, int h) // called when window size is changed
{
	/// adjust viewport transform
    glViewport(0, 0, w, h);
}
