#include "point.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <FreeImage/FreeImage.h>
#include<GL/freeglut.h>
#endif

using namespace std;
float xpos = -10;
bool displayAxis=true;
void makeCat();
bool displayModel = true;

bool displayWireMesh;
bool displaySolid;
//bool displayAxis;
bool orthViewOn;
bool perViewOn = false;

bool rotateModelX;
bool rotateModelY;
bool rotateModelZ;
bool translatemodelX;
bool translatemodelY;
bool translatemodelZ;

bool color;
bool sphereOn = false;

float redBodyRobot = 0.0f;
float greenBodyRobot = 0.0f;
float blueBodyRobot = 0.0f;
float cattail;
float catleg;
float linespeed;
float robotBodyPosX;
float robotBodyPosY;
float robotBodyPosZ;
float cameraOrth = 8.0;
float cameraPer = 2.3;
float positionX[10]; float positionZ[10];
float positionY[10];
float modelRotationY[10];
float modelRotationZ[10];
float modelRotationX[10];
float xRotationAngle = 0.0f;
float yRotationAngle = 0.0f;
float zRotationAngle = 0.0f;
bool moveX, moveY, moveZ;
static int shoulderAngle = 0, elbowAngle = 0;
static int upperlegjoint = 0, lowerlegjoint = 0;
static int hipAngle = 0, kneeAngle = 0;
float rinit = 0;
bool baxis = true;
bool displayCat;
bool drawRobot;
bool drawCat;
// Animation Variables
bool playAnimation=true ;
bool visiblePath ;
bool linePath;
bool circlePath;
bool resetOn = false;
float a = -1;
float r, g, b;
int aniSeconds = 0;
int aniState = 0;
int speed = 0;
// GLOBAL VARIABLES ////////////////////////////////////////////////////////////

static size_t windowWidth = 640;
static size_t windowHeight = 480;
static float aspectRatio;

GLint leftMouseButton, rightMouseButton;    //status of the mouse buttons
int mouseX = 0, mouseY = 0;                 //last known X and Y of the mouse
//bool sphereOn = false;                      //show the camera radius sphere


//note to students reading this code:
//  yes, I should really be more object-oriented with this code.
//  a lot of this would be simplified and better encapsulated inside
//  of a Camera class. don't let your code get this ugly!
enum cameraList { CAMERA_INNER = 0, CAMERA_OUTER = 1 };
enum cameraList currentCamera = CAMERA_OUTER;

#define USING_INNER (currentCamera == CAMERA_INNER)
//#define USING_INNER (currentCamera == CAMERA_INNER)
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_CLAMP_TO_BORDER 0x812D
#define GL_MIRRORED_REPEAT 0x8370
void drawrobot();
//void Drawbox();
float M_PI = 3.141592;

Point outerCamTPR;
Point outerCamXYZ;

Point innerCamXYZ;
Point innerCamTPR;
Point innerCamDir;

GLuint texID[5];
char* textureFileNames[5] = {	// File names for the files from which texture images are loaded
    (char*)"textures/earth.png",
    (char*)"textures/marble.jpg",
    (char*)"textures/body.jpg",
    (char*)"textures/grass.jpg",
    (char*)"textures/cat3.jpg"

};

typedef struct
{
    int X;
    int Y;
    int Z;
    double U;
    double V;
}VERTICES;

const double PI = 3.1415926535897;
const int space = 11;
const int VertexCount = (90 / space) * (360 / space) * 4;
VERTICES VERTEX[VertexCount];

static void QuadBox(GLfloat size, GLenum type)
{
    static GLfloat n[6][3] =
    {
      {-1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0},
      {0.0, 0.0, 1.0},
      {0.0, 0.0, -1.0}
    };
    static GLint faces[6][4] =
    {
      {0, 1, 2, 3},
      {3, 2, 6, 7},
      {7, 6, 5, 4},
      {4, 5, 1, 0},
      {5, 6, 2, 1},
      {7, 4, 0, 3}
    };
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    for (i = 5; i >= 0; i--) {
        glBegin(type);
        glNormal3fv(&n[i][0]);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(&v[faces[i][0]][0]);
        glTexCoord2f(0.0, 1.0);
        glVertex3fv(&v[faces[i][1]][0]);
        glTexCoord2f(1.0, 1.0);
        glVertex3fv(&v[faces[i][2]][0]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }

}

void CreateSphere(double R, double H, double K, double Z) {
    int n;
    double a;
    double b;
    n = 0;
    for (b = 0; b <= 90 - space; b += space) {

        for (a = 0; a <= 360 - space; a += space)
        {
            VERTEX[n].X = R * sin((a) / 180 * PI) * sin((b) / 180 * PI) - H;
            VERTEX[n].Y = R * cos((a) / 180 * PI) * sin((b) / 180 * PI) + K;
            VERTEX[n].Z = R * cos((b) / 180 * PI) - Z;
            VERTEX[n].V = (2 * b) / 360;
            VERTEX[n].U = (a) / 360;

            n++;
            VERTEX[n].X = R * sin((a) / 180 * PI) * sin((b + space) / 180 * PI) - H;
            VERTEX[n].Y = R * cos((a) / 180 * PI) * sin((b + space) / 180 * PI) + K;
            VERTEX[n].Z = R * cos((b + space) / 180 * PI) - Z;
            VERTEX[n].V = (2 * (b + space)) / 360;
            VERTEX[n].U = (a) / 360;
            n++;
            VERTEX[n].X = R * sin((a + space) / 180 * PI) * sin((b) / 180 * PI) - H;
            VERTEX[n].Y = R * cos((a + space) / 180 * PI) * sin((b) / 180 * PI) + K;
            VERTEX[n].Z = R * cos((b) / 180 * PI) - Z;
            VERTEX[n].V = (2 * b) / 360;
            VERTEX[n].U = (a + space) / 360;
            n++;
            VERTEX[n].X = R * sin((a + space) / 180 * PI) * sin((b + space) / 180 * PI) - H;
            VERTEX[n].Y = R * cos((a + space) / 180 * PI) * sin((b + space) / 180 * PI) + K;
            VERTEX[n].Z = R * cos((b + space) / 180 * PI) - Z;
            VERTEX[n].V = (2 * (b + space)) / 360;
            VERTEX[n].U = (a + space) / 360;
            n++;
        }
    }
}

void DisplaySphere(double R)
{

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    int b;
    glScalef(0.0125 * R, 0.0125 * R, 0.0125 * R);
    //glRotatef (90, 1, 0, 0);
    glBegin(GL_TRIANGLE_STRIP);
    for (b = 0; b < VertexCount; b++)
    {
        glTexCoord2f(VERTEX[b].U, VERTEX[b].V);
        glVertex3f(VERTEX[b].X, VERTEX[b].Y, -VERTEX[b].Z);
    }



    for (b = 0; b < VertexCount; b++)
    {

        glTexCoord2f(VERTEX[b].U, -VERTEX[b].V);

        glVertex3f(VERTEX[b].X, VERTEX[b].Y, VERTEX[b].Z);

    }

    glEnd();
}

void DrawSphere(double size)
{
    CreateSphere(size, 0, 0, 0);
    DisplaySphere(5);
}

void Drawbox(double w, double h, double l) {
    glPushMatrix();
    glScalef(w, h, l);
    QuadBox(1, GL_QUADS);
    glPopMatrix();

}

void loadTextures() {
    int i;
    glGenTextures(5, texID); // Get the texture object IDs.
    for (i = 0; i < 5; i++) {
        void* imgData; // Pointer to image color data read from the file.
        int imgWidth; // The width of the image that was read.
        int imgHeight; // The height.
        FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(textureFileNames[i]);
        if (format == FIF_UNKNOWN) {
            printf("Unknown file type for texture image file %s\n", textureFileNames[i]);
            continue;
        }
        FIBITMAP* bitmap = FreeImage_Load(format, textureFileNames[i], 0); // Read image from file.
        if (!bitmap) {
            printf("Failed to load image %s\n", textureFileNames[i]);
            continue;
        }
        FIBITMAP* bitmap2 = FreeImage_ConvertTo24Bits(bitmap); // Convert to RGB or BGR format
        FreeImage_Unload(bitmap);
        imgData = FreeImage_GetBits(bitmap2); // Grab the data we need from the bitmap.
        imgWidth = FreeImage_GetWidth(bitmap2);
        imgHeight = FreeImage_GetHeight(bitmap2);
        if (imgData) {
            printf("Texture image loaded from file %s, size %dx%d\n",
                textureFileNames[i], imgWidth, imgHeight);
            glBindTexture(GL_TEXTURE_2D, texID[i]); // Will load image data into texture object #i
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_BGR_EXT,
                GL_UNSIGNED_BYTE, imgData);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Required since there are no mipmaps.
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        }
        else {
            printf("Failed to get texture data from %s\n", textureFileNames[i]);
        } // end of else
    } // end of for loop
} // end of LoadTextures()


void makeGrassPlane() {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[3]); // Bind texture
    glPushMatrix();
    glTranslatef(0.0f, -5, 0.0f);
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void specialfunctions(int key, int, int) {
    switch (key)
    {
    case GLUT_KEY_LEFT: if (currentCamera == CAMERA_INNER)

        innerCamDir.x -= 2;
        //innerCamXYZ.x -= 1;
    case GLUT_KEY_RIGHT:if (currentCamera == CAMERA_INNER) innerCamDir.x += 1;
        //innerCamXYZ.x += 1;
    case GLUT_KEY_UP:if (currentCamera == CAMERA_INNER) innerCamDir.y += 2;
        //innerCamXYZ.y += 1;
    case GLUT_KEY_DOWN:if (currentCamera == CAMERA_INNER) innerCamDir.y -= 1;
        //innerCamXYZ.y -= 1;
    defualt: return;
    }
    glutPostRedisplay();

}
// recomputeOrientation() //////////////////////////////////////////////////////
//
// This function updates the camera's position in cartesian coordinates based 
//  on its position in spherical coordinates. Should be called every time 
//  either camera's spherical coordinates are updated.
//
////////////////////////////////////////////////////////////////////////////////
void timer(int v) {
    {
        if (playAnimation)
        {



            if (displayModel) {

                if (circlePath) {
                    //glTranslatef(10, 0, 0);
                        //glRotatef(speed, 0, 0, 1);

                    speed += 3;
                    //speed++;
                }
                else {
                    speed -= 0.5;
                }


                if (linePath)
                {
                    if (linespeed < 10) {
                        linespeed += 15;
                    }
                    else {
                        linespeed -= 0.1;

                    }


                }
            }






            if (aniSeconds <= 30 && aniState == 0)
            {
                shoulderAngle += 2;
                elbowAngle += 2;
                hipAngle += 1;
                cattail += 1;
                catleg += 1;
                aniSeconds++;
                if (aniSeconds >= 30)
                    aniState = 1;



            }
            else
            {
                shoulderAngle -= 2;
                elbowAngle -= 2;
                hipAngle -= 1;
                cattail -= 1;
                catleg -= 1;
                aniSeconds--;
                if (aniSeconds <= -30)
                    aniState = 0;
            }
            if (aniSeconds <= 30 && aniState == 0)
            {
                upperlegjoint += 1;
                lowerlegjoint += 0.1;
                hipAngle -= 1;

                kneeAngle += 1;
                aniSeconds++;
                if (aniSeconds >= 30)
                    aniState = 1;
            }
            else
            {
                upperlegjoint -= 1;
                lowerlegjoint -= 1;
                hipAngle -= 1;
                kneeAngle -= 1;
                aniSeconds--;
                if (aniSeconds <= -30)
                    aniState = 0;
            }






            // Make it so you can toggle the animation so that it plays and doesn't play.

            // Within the if statement for handling the animation toggle make two seperate conditions for the circle path and the linear path.

            // Within the circle path you'll want to update a travel speed variable for the circle. Something like this: speed = speed + speedIncrement.

            // For the linear path you want to update a variable the same way you did the circle path but have if statements to limit and reset the value so it doesn't get too high. 

            // Then use the variables you made and update here as the rotation value for the circle path and as the trasnlate value for the direction you want the robot to go.

        }
    }





    glutPostRedisplay();
    glutTimerFunc(1500 / 60, timer, v);

}

void wireBox(GLdouble width, GLdouble height, GLdouble depth)
{
    glPushMatrix();
    glScalef(width, height, depth);
    glutWireCube(1.0);
    glPopMatrix();
}

void recomputeOrientation(Point& xyz, Point& tpr)
{
    xyz.x = tpr.z * sinf(tpr.x) * sinf(tpr.y);
    xyz.z = tpr.z * -cosf(tpr.x) * sinf(tpr.y);
    xyz.y = tpr.z * -cosf(tpr.y);
    glutPostRedisplay();
}

// resizeWindow() //////////////////////////////////////////////////////////////
//
//  GLUT callback for window resizing. Resets GL_PROJECTION matrix and viewport.
//
////////////////////////////////////////////////////////////////////////////////
void resizeWindow(int w, int h)
{
    aspectRatio = w / (float)h;

    windowWidth = w;
    windowHeight = h;

    //update the viewport to fill the window
    glViewport(0, 0, w, h);

    //update the projection matrix with the new window properties
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspectRatio, 0.1, 100000);

    glutPostRedisplay();
}



// mouseCallback() /////////////////////////////////////////////////////////////
//
//  GLUT callback for mouse clicks. We save the state of the mouse button
//      when this is called so that we can check the status of the mouse
//      buttons inside the motion callback (whether they are up or down).
//
////////////////////////////////////////////////////////////////////////////////
void mouseCallback(int button, int state, int thisX, int thisY)
{
    //update the left and right mouse button states, if applicable
    if (button == GLUT_LEFT_BUTTON)
        leftMouseButton = state;
    else if (button == GLUT_RIGHT_BUTTON)
        rightMouseButton = state;

    //and update the last seen X and Y coordinates of the mouse
    mouseX = thisX;
    mouseY = thisY;
}

// mouseMotion() ///////////////////////////////////////////////////////////////
//
//  GLUT callback for mouse movement. We update the current camera's spherical
//      coordinates based on how much the user has moved the mouse in the
//      X or Y directions (in screen space) and whether they have held down
//      the left or right mouse buttons. If the user hasn't held down any
//      buttons, the function just updates the last seen mouse X and Y coords.
//
////////////////////////////////////////////////////////////////////////////////
void mouseMotion(int x, int y)
{
    if (leftMouseButton == GLUT_DOWN)
    {
        Point* curTPR = (USING_INNER ? &innerCamTPR : &outerCamTPR);      //just for conciseness below
        curTPR->x += (x - mouseX) * 0.005;
        curTPR->y += (USING_INNER ? -1 : 1) * (y - mouseY) * 0.005;

        // make sure that phi stays within the range (0, M_PI)
        if (curTPR->y <= 0)
            curTPR->y = 0 + 0.001;
        if (curTPR->y >= M_PI)
            curTPR->y = M_PI - 0.001;

        //update camera (x,y,z) based on (radius,theta,phi)
        if (USING_INNER)
        {
            recomputeOrientation(innerCamDir, innerCamTPR);
            innerCamDir.normalize();
        }
        else {
            recomputeOrientation(outerCamXYZ, outerCamTPR);
        }
    }
    else if (rightMouseButton == GLUT_DOWN && !USING_INNER) {
        double totalChangeSq = (x - mouseX) + (y - mouseY);

        Point* curTPR = &outerCamTPR;      //just for conciseness below
        curTPR->z += totalChangeSq * 0.01;

        //limit the camera radius to some reasonable values so the user can't get lost
        if (curTPR->z < 2.0)
            curTPR->z = 2.0;
        if (curTPR->z > 10.0 * (currentCamera + 1))
            curTPR->z = 10.0 * (currentCamera + 1);

        //update camera (x,y,z) based on (radius,theta,phi)
        recomputeOrientation(outerCamXYZ, outerCamTPR);
    }

    mouseX = x;
    mouseY = y;
}



// initScene() /////////////////////////////////////////////////////////////////
//
//  A basic scene initialization function; should be called once after the
//      OpenGL context has been created. Doesn't need to be called further.
//
////////////////////////////////////////////////////////////////////////////////
void initScene()
{
    glEnable(GL_DEPTH_TEST);

    float lightCol[4] = { 1, 1, 1, 1 };
    float ambientCol[4] = { 0.3, 0.3, 0.3, 1.0 };
    float lPosition[4] = { 10, 10, 10, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, lPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightCol);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientCol);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_POINT_SMOOTH);

    glShadeModel(GL_SMOOTH);
    //glShadeModel(GL_FLAT);

    glutPostRedisplay();
}


// drawSceneElements() /////////////////////////////////////////////////////////
//
//  Because we'll be drawing the scene twice from different viewpoints,
//      we encapsulate the code to draw the scene here, so that we can just
//      call this function twice once the projection and modelview matrices
//      have been set appropriately.
//
////////////////////////////////////////////////////////////////////////////////
void drawSceneElements(void)
{
    glDisable(GL_LIGHTING);
    //draw a simple grid under the teapot
    glColor3f(1, 1, 1);

    //glColor3f(1, 1, 1);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

   // glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(125, 0, 125);
    glScalef(6, 1, 6);
    // Ground Grid
    if (displayCat) {
        displayModel = false;
        {
            glPushMatrix();

            glTranslatef(4, 1, 2);
            makeCat();
            glPopMatrix();
        }

    }
    if (displayWireMesh)
        for (int dir = 0; dir < 10; dir++)
        {
            glPushMatrix();
            glTranslatef(0, -4.3, 0);
            for (int i = -50; i < 6; i++)
            {

                glBegin(GL_LINE_STRIP);
                for (int j = -50; j < 6; j++) {

                    // glBindTexture(GL_TEXTURE_2D, texID[2]);
                    glVertex3f(dir < 1 ? i : j, -0.73f, dir < 1 ? j : i);


                }
                glEnd();

            }
            glPopMatrix();
        }

    else
        makeGrassPlane();

    glPopMatrix();


    //and then draw the teapot itself!


    //glEnable(GL_LIGHTING);


    //see documentation for glutSolidTeapot; glutSolidTeapot must be called with 
    //a different winding set. there is a known 'bug' that results in the 
    //winding of the teapot to be backwards.

    glPushMatrix();
    glTranslatef(-110, 0, 50);
    glScalef(2, 2, 2);
    // Trees
    glFrontFace(GL_CW);
    for (float i = 1; i < 10; i++) {
        for (float j = -10; j < 6; j++)
        {

            glPushMatrix();
            //glbegin(GL_
            //)
            glTranslatef(i * 10, 0, j * 10);
            //float a = rand() % (max - 100 / min / 100);
            glColor3d(0.5f, 0.35f, 0.05f);
            //glPushMatrix();

            Drawbox(1, 5, 1);

            glColor3d(0, 1, 0);
            glTranslatef(0, 0 + 2.0f, 0);
            DrawSphere(40);
            glPopMatrix();
            //glPushMatrix();

        }
        glFrontFace(GL_CCW);
    }
    glPopMatrix();

    drawrobot();
    makeCat();
}

void drawrobot() {

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glEnable(GL_TEXTURE_2D);
    if (displayModel)
    {

        // Body
        if (displayWireMesh) // Wire Mesh
        {
            glPushMatrix();
            glTranslatef(0, 0, 0);
            glColor3f(1.0, 1.0, 1.0);
            //Drawbox(2, 2, 2);
            glutWireCube(2);
            glPopMatrix();
        }

        else // Solid
        {
            glBindTexture(GL_TEXTURE_2D, texID[0]);
            glPushMatrix();
            glTranslatef(0, 0, 0);

            if (color)
                glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
            else
                glColor3f(1.0f, 1.0f, 1.0f);

            Drawbox(2, 2, 2);
            glPopMatrix();
        }

        // Head
        if (displayWireMesh) // Wire Mesh
        {
            glPushMatrix();
            glColor3f(1.0f, 1.0f, 1.0f);
            glTranslatef(0, 0 + 2.0f, 0);
            //DrawSphere(17);
            glutWireSphere(1.0f, 10, 10);
            glPopMatrix();
        }

        else // Solid
        {
            glBindTexture(GL_TEXTURE_2D, texID[1]);
            glPushMatrix();
            if (color)
                glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
            else
                glColor3f(1.0f, 1.0f, 1.0f);
            glTranslatef(0, 0 + 2.0f, 0);
            DrawSphere(17);
            glPopMatrix();
        }

        if (displayWireMesh) // Wire Mesh rightarm
        {

            glPushMatrix();
            glTranslatef(1, 0.6, 0);
            glRotatef(-90, 0.0, 0.0, 1.0);
            glRotatef((GLfloat)shoulderAngle, 0.0, -1.0, 0.0);
            glTranslatef(1, 0.0, 0);
            if (color)
                glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
            else
                glColor3f(1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glScalef(2, 0.5, 0.5);
            glutWireCube(1);
            glPopMatrix();


            // Lower right arm

            glTranslatef(1.0, 0, 0);
            glRotatef(90, 0.0, -1.0, 0.0);
            glRotatef((GLfloat)elbowAngle, 0.0, 1.0, 0.0);
            glTranslatef(1, 0.0, 0);
            if (color)
                glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
            else
                glColor3f(1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glScalef(2, 0.5, 0.5);
            glutWireCube(1);
            glPopMatrix();
            glPopMatrix();
        }

        else // Solid
        {
            glBindTexture(GL_TEXTURE_2D, texID[1]);
            glPushMatrix();
            glTranslatef(1, 0.6, 0);
            glRotatef(-90, 0.0, 0.0, 1.0);
            glRotatef((GLfloat)shoulderAngle, 0.0, -1.0, 0.0);
            glTranslatef(1, 0.0, 0);
            if (color)
                glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
            else
                glColor3f(1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glScalef(2, 0.5, 0.5);
            Drawbox(1, 1, 1);
            glPopMatrix();


            // Lower right arm
            glBindTexture(GL_TEXTURE_2D, texID[2]);
            glTranslatef(1.0, 0, 0);
            glRotatef(90, 0.0, -1.0, 0.0);
            glRotatef((GLfloat)elbowAngle, 0.0, 1.0, 0.0);
            //glRotatef(40, 0.0, 1.0, 0.0);
            glTranslatef(1, 0.0, 0);
            glColor3f(0, 0.1, 0);
            if (color)
                glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
            else
                glColor3f(1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glScalef(2, 0.5, 0.5);
            Drawbox(1, 1, 1);
            glPopMatrix();
            glPopMatrix();
        }




        if (displayWireMesh) // Wire Mesh leftarm
        {
           // glBindTexture(GL_TEXTURE_2D, texID[0]);
            glPushMatrix();
            glTranslatef(-1, 0.6, 0);
            glRotatef(90, 0.0, 0.0, 1.0);
            glRotatef(-(GLfloat)shoulderAngle, 0.0, 1.0, 0.0);
            glTranslatef(-1, 0.0, 0);
            if (color)
                glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
            else
                glColor3f(1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glScalef(2, 0.5, 0.5);
            glutWireCube(1);
            glPopMatrix();

            //glBindTexture(GL_TEXTURE_2D, texID[0]);
            // Lower left arm
            glTranslatef(-1.0, 0, 0);
            glRotatef(-90, 1, 0, 0);
            glRotatef(90, 0.0, 0.0, 1.0);
            glRotatef(-(GLfloat)elbowAngle, 0.0, 0.0, 1.0);
            //glTranslatef(0, 1, 0);
            //elbowAngle = 0;
            //glRotatef(-(GLfloat)elbowAngle, 0.0, 0, 0.0);
            //glRotatef(0, 0.0, 1.0, 0.0);
            glTranslatef(-1.0, 0.0, 0);
            if (color)
                glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
            else
                glColor3f(1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glScalef(2, 0.5, 0.5);
            glutWireCube(1);
            glPopMatrix();
            glPopMatrix();
        }

        // Solid
        else
        {
            //glBindTexture(GL_TEXTURE_2D, texID[1]);
            glBindTexture(GL_TEXTURE_2D, texID[0]);
            glPushMatrix();
            glTranslatef(-1, 0.6, 0);
            glRotatef(90, 0.0, 0.0, 1.0);
            glRotatef(-(GLfloat)shoulderAngle, 0.0, 1.0, 0.0);
            //glRotatef(-240, 0.0, 0.0, 1.0);
            glTranslatef(-1, 0.0, 0);
            if (color)
                glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
            else
                glColor3f(1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glScalef(2, 0.5, 0.5);
            Drawbox(1, 1, 1);
            glPopMatrix();


            // Lower left arm
            glBindTexture(GL_TEXTURE_2D, texID[3]);
            //glBindTexture(GL_TEXTURE_2D, texID[0]);
            glTranslatef(-1.0, 0, 0);
            glRotatef(-90, 0, -1.0, 0.0);
            //glRotatef(-45, 1, 0, 0);
            glRotatef(-(GLfloat)elbowAngle, 0.0, 1.0, 0.0);
            //glRotatef(-240, 0.0, 0.0, 1.0);
            glTranslatef(-1.0, 0.0, 0);
            if (color)
                glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
            else
                glColor3f(1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glScalef(2, 0.5, 0.5);
            Drawbox(1, 1, 1);
            glPopMatrix();
            
            glPopMatrix();
            
        }


        if (displayWireMesh)//wire mesh left leg
        {
            glPushMatrix();
            glTranslatef(0.5, -1, 0);
            glRotatef((GLfloat)upperlegjoint, 1.0, 0.0, 0.0);
            glTranslatef(0, -1, 0);
            glColor3f(1, 1, 1);
            glScalef(0.5, 2, 0.5);
            glutWireCube(1.0f);



            glTranslatef(0.5, -1, 0);
            glRotatef(-10, 1, 0, 0);
            glRotatef(-(GLfloat)lowerlegjoint, 1.0, 0.0, 0.0);// (3) rotate
            glTranslatef(-0.5, -1, 0);
            glColor3f(1.0, 0.0, 0.0);
            glScalef(1, 1, 1);
            glTranslatef(0, 1.0, 0);
            glutWireCube(1.0f); // (1) draw the lower leg box
            glPopMatrix();
        }
        else {
            //glBindTexture(GL_TEXTURE_2D, texID[1]);         
            glBindTexture(GL_TEXTURE_2D, texID[0]);
            glPushMatrix();
            glTranslatef(0.5, -1, 0);
            glRotatef((GLfloat)upperlegjoint, 1.0, 0.0, 0.0);
            glTranslatef(0, -1, 0);
            if (color)
                glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
            else
                glColor3f(1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glScalef(0.5, 2, 0.5);
            Drawbox(1, 1, 1);
            glPopMatrix();

            //glPushMatrix();
            //glBindTexture(GL_TEXTURE_2D, texID[1]);
            glBindTexture(GL_TEXTURE_2D, texID[0]);
            glTranslatef(-0.5, -1, 0);
            glRotatef(-10, 1, 0, 0);
            glRotatef(-(GLfloat)lowerlegjoint, 1.0, 0.0, 0.0); // (3) rotate

            glTranslatef(0.5, -1, 0);
            if (color)
                glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
            else
                glColor3f(1.0f, 1.0f, 1.0f);

            glPushMatrix();
            glScalef(0.5, 2, 0.5);
            Drawbox(1, 1, 1);
            glPopMatrix();

            glPopMatrix();

            glDisable(GL_TEXTURE_2D);

        }if (displayWireMesh)//wire mesh right leg
        {
            glPushMatrix();
            glTranslatef(-1.5, -1, 0);
            glRotatef(-(GLfloat)upperlegjoint, 1.0, 0.0, 0.0);
            glTranslatef(1, -1, 0);
            glColor3f(1, 1, 1);
            glPushMatrix();
            glScalef(0.5, 2, 0.5);
            glutWireCube(1);
            //glPopMatrix();


            glTranslatef(-0.5, 0, 0);
            glRotatef(-(GLfloat)lowerlegjoint, 1.0, 0.0, 0.0);// (3) rotate
            glTranslatef(0.5,-1, 0);
            glColor3f(1.0, 0.0, 0.0);
            glPushMatrix();
            glScalef(1, 1, 1);
            glutWireCube(1);
            glPopMatrix();
            glPopMatrix();
            glPopMatrix();

        }
        else {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texID[1]);
            glPushMatrix();
            glTranslatef(-0.5, -1, 0);
            glRotatef(-(GLfloat)upperlegjoint, 1.0, 0.0, 0.0);
            glTranslatef(0, -1, 0);
            if (color)
                glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
            else
                glColor3f(1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glScalef(0.5, 2, 0.5);
            Drawbox(1, 1, 1);
            glPopMatrix();

            //glPushMatrix();
            //glBindTexture(GL_TEXTURE_2D, texID[1]);
            glTranslatef(-0.5, -1, 0);
            glRotatef(-(GLfloat)lowerlegjoint, 1.0, 0.0, 0.0); // (3) rotate
            glTranslatef(0.5, -1, 0);
            if (color)
                glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
            else
                glColor3f(1.0f, 1.0f, 1.0f);

            glPushMatrix();
            glScalef(0.5, 2, 0.5);
            Drawbox(1, 1, 1);
            glPopMatrix();

            glPopMatrix();


        }
    }

    glDisable(GL_TEXTURE_2D);
}



void makeCat() {
    {

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

        glEnable(GL_TEXTURE_2D);
        if (displayCat)
            displayModel = false;
        {
            if (displayWireMesh) // Wire Mesh for cat head
            {
                glPushMatrix();
                glColor3f(1.0f, 1.0f, 1.0f);
                glTranslatef(0 + 3.0f, 0 + 2.0f, 0);
                glutWireSphere(1.0f, 8, 7);
                glPopMatrix();
            }

            else // Solid
            {
                glBindTexture(GL_TEXTURE_2D, texID[4]);
                glPushMatrix();
                if (color)
                    glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
                else
                    glColor3f(1.0f, 1.0f, 1.0f);
                glTranslatef(0 + 3.0f, 0 + 2.0f, 0);
                DrawSphere(15.0f);
                glPopMatrix();
            }
            if (displayWireMesh) // Wire Mesh for cat body
            {
                glPushMatrix();
                glTranslatef(5, 2, 0);
                glScalef(1.4, 0.5, 0);
                glColor3f(1, 0, 0);
                glutWireCube(2.5f);
                glPopMatrix();
            }

            else // Solid cat
            {
                glBindTexture(GL_TEXTURE_2D, texID[3]);
                glPushMatrix();
                glTranslatef(5, 2, 0);
                glScalef(1.4, 0.5, 0);
                if (color)
                    glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
                else
                    glColor3f(1.0f, 1.0f, 1.0f);

                glutSolidCube(2.5f);
                glPopMatrix();
            }
            if (displayWireMesh) // Wire Mesh for cat left leg1
            {
                glPushMatrix();
                glTranslatef(4, 1, 0);
                glScalef(0.2, 1.0, 0);
                glColor3f(1, 0, 0);
                glutWireCube(2.5f);
                glPopMatrix();
            }

            else // Solid cat
            {
                glBindTexture(GL_TEXTURE_2D, texID[4]);
                glPushMatrix();
                glTranslatef(4, 1, 0);
                glScalef(0.2, 1.0, 0);
                if (color)
                    glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
                else
                    glColor3f(1.0f, 1.0f, 1.0f);

                glutSolidCube(2.5f);
                glPopMatrix();
            }
            if (displayWireMesh) // Wire Mesh for cat rightleg1
            {
                glPushMatrix();
                glTranslatef(4.7, 1, 0);
                glScalef(0.2, 1.0, 0);
                glColor3f(1, 0, 0);
                glutWireCube(2.5f);
                glPopMatrix();
            }

            else // Solid cat
            {
                glPushMatrix();
                glTranslatef(4.7, 1, 0);
                glScalef(0.2, 1.0, 0);
                if (color)
                    glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
                else
                    glColor3f(1.0f, 1.0f, 1.0f);

                glutSolidCube(2.5f);
                glPopMatrix();
            }
            if (displayWireMesh) // Wire Mesh for cat left leg2
            {
                glPushMatrix();
                glTranslatef(5.8, 1, 0);
                glScalef(0.2, 1.0, 0);
                glColor3f(1, 0, 0);
                glutWireCube(2.5f);
                glPopMatrix();
            }

            else // Solid cat
            {
                glPushMatrix();
                glTranslatef(5.8, 1, 0);
                glScalef(0.2, 1.0, 0);
                if (color)
                    glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
                else
                    glColor3f(1.0f, 1.0f, 1.0f);

                glutSolidCube(2.5f);
                glPopMatrix();
            }if (displayWireMesh) // Wire Mesh for cat right leg2
            {
                glPushMatrix();
                glTranslatef(6.5, 1, 0);
                glScalef(0.2, 1.0, 0);
                glColor3f(1, 0, 0);
                glutWireCube(2.5f);
                glPopMatrix();
            }

            else // Solid cat
            {
                glPushMatrix();
                glTranslatef(6.5, 1, 0);
                glScalef(0.2, 1.0, 0);
                if (color)
                    glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
                else
                    glColor3f(1.0f, 1.0f, 1.0f);

                glutSolidCube(2.5f);
                glPopMatrix();
            }
            if (displayWireMesh) // Wire Mesh for cat Tail
            {
                glPushMatrix();
                glTranslatef(7, 2, 0);
                glScalef(0.5, 0.1, 0);
                glColor3f(1, 0, 0);
                glutWireCube(2.5f);
                glPopMatrix();
            }

            else // Solid cat
            {
                glPushMatrix();
                glTranslatef(7, 2, 0);
                glScalef(0.5, 0.1, 0);
                if (color)
                    glColor3f(redBodyRobot, greenBodyRobot, blueBodyRobot);
                else
                    glColor3f(1.0f, 1.0f, 1.0f);

                glutSolidCube(2.5f);
                glPopMatrix();
            }
        }glDisable(GL_TEXTURE_2D);
    }

}

// drawInnerCamera() ///////////////////////////////////////////////////////////
//
//  Draws a representation of the inner camera in space. This should only be
//      called when rendering the scene from the POV of the outer camera,
//      so that we can visualize where the inner camera is positioned
//      and what it is looking at.
//
////////////////////////////////////////////////////////////////////////////////
void drawInnerCamera()
{
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(innerCamXYZ.x, innerCamXYZ.y, innerCamXYZ.z);
    glRotatef(-innerCamTPR.x * 180.0 / M_PI, 0, 1, 0);
    glRotatef(innerCamTPR.y * 180.0 / M_PI, 1, 0, 0);

    glScalef(0.5, 0.1, 0.5);


    glColor3f(1, 0, 0);
    glutWireCube(3.0f);

    //draw the reels on top of the camera...
    for (int currentReel = 0; currentReel < 2; currentReel++)
    {
        float radius = 0.25f;
        int resolution = 32;
        Point reelCenter = Point(0, -0.25 + (currentReel == 0 ? 0 : 0.5), -0.5);
        glBegin(GL_LINES);
        Point s = reelCenter - Point(0, 0.25, 0);
        glVertex3f(s.x, s.y, s.z);
        for (int i = 0; i < resolution; i++)
        {
            float ex = -cosf(i / (float)resolution * M_PI);
            float why = sinf(i / (float)resolution * M_PI);
            Point p = Point(0, ex * radius, -why * radius * 3) + reelCenter;
            glVertex3f(p.x, p.y, p.z);  //end of this line...
            glVertex3f(p.x, p.y, p.z);  //and start of the next
        }
        Point f = reelCenter + Point(0, 0.25, 0);
        glVertex3f(f.x, f.y, f.z);
        glEnd();
    }

    //and just draw the lens shield manually because 
    //i don't want to think about shear matrices.
    //clockwise looking from behind the camera:
    float lensOff = 0.3f;
    float lensOut = 0.2f;
    Point v0 = Point(0.5, 0.5, -0.5);
    Point v1 = Point(-0.5, 0.5, -0.5);
    Point v2 = Point(-0.5, 0.5, 0.5);
    Point v3 = Point(0.5, 0.5, 0.5);

    Point l0 = v0 + Point(lensOut, 0, 0) + Point(0, lensOut, 0) + Point(0, 0, -lensOff);
    Point l1 = v1 + Point(-lensOut, 0, 0) + Point(0, lensOut, 0) + Point(0, 0, -lensOff);
    Point l2 = v2 + Point(-lensOut, 0, 0) + Point(0, lensOut, 0) + Point(0, 0, lensOff);
    Point l3 = v3 + Point(lensOut, 0, 0) + Point(0, lensOut, 0) + Point(0, 0, lensOff);


    glBegin(GL_LINE_STRIP);
    l0.glVertex();
    l1.glVertex();
    l2.glVertex();
    l3.glVertex();
    l0.glVertex();
    glEnd();

    //and connect the two
    glBegin(GL_LINES);
    v0.glVertex();  l0.glVertex();
    v1.glVertex();  l1.glVertex();
    v2.glVertex();  l2.glVertex();
    v3.glVertex();  l3.glVertex();
    glEnd();


    if (sphereOn)
    {
        //draw a point at the center of the camera
        glColor3f(1, 0, 0);
        glPointSize(10);
        glBegin(GL_POINTS);
        Point(0, 0, 0).glVertex();
        glEnd();
        glPopMatrix();

        //do the same set of transformations, but without the scale..
        glPushMatrix();
        glTranslatef(innerCamXYZ.x, innerCamXYZ.y, innerCamXYZ.z);
        glRotatef(-innerCamTPR.x * 180.0 / M_PI, 0, 1, 0);
        glRotatef(innerCamTPR.y * 180.0 / M_PI, 1, 0, 0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glColor4f(1, 1, 1, 0.3);
        glutSolidSphere(1, 32, 32);

        glDisable(GL_DEPTH_TEST);
        glColor3f(1, 0, 0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, -1, 0);
        glEnd();
        glColor3f(0, 0, 1);
        glBegin(GL_POINTS);
        glVertex3f(0, -1, 0);
        glEnd();
        glEnable(GL_DEPTH_TEST);

        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);


        glPopMatrix();
    }
    else {
        glPopMatrix();
    }


    glPopAttrib();
}

// renderCallback() ////////////////////////////////////////////////////////////
//
//  GLUT callback for scene rendering. Sets up the modelview matrix, renders
//      a teapot to the back buffer, and switches the back buffer with the
//      front buffer (what the user sees).
//
////////////////////////////////////////////////////////////////////////////////
void renderCallback(void)
{
    //clear the render buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float borderWidth = 3;
    //start with the code from the outer camera, which covers the whole screen!
    glViewport(0, 0, windowWidth, windowHeight);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);    glPushMatrix(); glLoadIdentity();   gluOrtho2D(0, 1, 0, 1);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    if (currentCamera == CAMERA_OUTER)
        glColor3f(1, 0, 0);
    else
        glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glVertex2f(0, 0); glVertex2f(0, 1); glVertex2f(1, 1); glVertex2f(1, 0);
    glEnd();
    glViewport(borderWidth, borderWidth, windowWidth - borderWidth * 2, windowHeight - borderWidth * 2);
    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(0, 0); glVertex2f(0, 1); glVertex2f(1, 1); glVertex2f(1, 0);
    glEnd();
  
    glMatrixMode(GL_PROJECTION);    glPopMatrix();
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    //update the modelview matrix based on the camera's position
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(outerCamXYZ.x, outerCamXYZ.y, outerCamXYZ.z,
        0, 0, 0,
        0, 1, 0);

    drawSceneElements();
    if (displayAxis == true)
    {
        glBegin(GL_LINES);
        glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(10, 0, 0);
        glTranslatef(0, 1, 1);
        glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 10, 0);
        glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 10);
        glEnd();
    }
    drawInnerCamera();
    
    //drawrobot();

    ///     draw border and background for preview box in upper corner  //////////////////////

        //next, do the code for the inner camera, which only sets in the top-right
        //corner!
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    //step 1: set the projection matrix using gluOrtho2D -- but save it first!
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1, 0, 1);

    //step 2: clear the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //step 3: set the viewport matrix a little larger than needed...
    glViewport(2 * windowWidth / 3.0 - borderWidth, 2 * windowHeight / 3.0 - borderWidth,
        windowWidth / 3.0 + borderWidth, windowHeight / 3.0 + borderWidth);
    //step 3a: and fill it with a white rectangle!
    if (currentCamera == CAMERA_OUTER)
        glColor3f(1, 1, 1);
    else
        glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(0, 0); glVertex2f(0, 1); glVertex2f(1, 1); glVertex2f(1, 0);
    glEnd();

    //step 4: trim the viewport window to the size we want it...
    glViewport(2 * windowWidth / 3.0, 2 * windowHeight / 3.0,
        windowWidth / 3.0, windowHeight / 3.0);
    //step 4a: and color it black! the padding we gave it before is now a border.
    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(0, 0); glVertex2f(0, 1); glVertex2f(1, 1); glVertex2f(1, 0);
    glEnd();

    //before rendering the scene in the corner, pop the old projection matrix back
    //and re-enable lighting!
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    ///     begin drawing scene in upper corner //////////////////////////////////////////////

    glViewport(2 * windowWidth / 3.0, 2 * windowHeight / 3.0,
        windowWidth / 3.0, windowHeight / 3.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(innerCamXYZ.x, innerCamXYZ.y, innerCamXYZ.z,      //camera is located at (x,y,z)
        innerCamXYZ.x + innerCamDir.x,                  //looking at a point that is
        innerCamXYZ.y + innerCamDir.y,                  //one unit along its direction
        innerCamXYZ.z + innerCamDir.z,
        0.0f, 1.0f, 0.0f);                                //up vector is (0,1,0) (positive Y)

    glClear(GL_DEPTH_BUFFER_BIT);                   //ensure that the overlay is always on top!


    drawSceneElements();
   drawrobot();
   makeCat();
   if (displayAxis == true)
   {
       glBegin(GL_LINES);
       glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(10, 0, 0);
       glTranslatef(0, 1, 1);
       glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 10, 0);
       glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 10);
       glEnd();
   }
    //push the back buffer to the screen
    glutSwapBuffers();
}


// normalKeys() ////////////////////////////////////////////////////////////////
//
//  GLUT keyboard callback.
//
////////////////////////////////////////////////////////////////////////////////
void normalKeys(unsigned char key, int x, int y)
{
    switch (key) {
    case '1': // Display a wireframe(mesh only) model
        displayWireMesh = !displayWireMesh;
        glutPostRedisplay();
        break;
    case 'a':
        playAnimation = !playAnimation;
        glutPostRedisplay();
        break;
    case '2': // Display a solid model
        displayAxis = !displayAxis;
        glutPostRedisplay();
        break;

 
   

  
    case 'i':
        currentCamera = CAMERA_INNER;
        glutPostRedisplay();
        break;
    case 'o':
        currentCamera = CAMERA_OUTER;
        glutPostRedisplay();
        break;



    case 'esc': // escape
        exit(27);

    }
}




// main() //////////////////////////////////////////////////////////////////////
//
//  Program entry point. Does not process command line arguments.
//
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
    //create a double-buffered GLUT window at (50,50) with predefined windowsize
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("double cameras... woahhhhh double cameras");
    //create a double-buffered GLUT window at (50,50) with predefined windowsize
    printf("\n\
----------------------------------------------------------------------------\n\
a. '1': toggle (wire/solid) to display wireframe or solid display of the scene. Note \n\
that the robot and plane should be displayed as textured models for the solid\n\
mode.\n\
b.'2' : toggle on / off to display the three axes(axes only)\n\
c.'a' : robot animation toggle ON / OFF\n\
d.'i' : select the inner camera\n\
e.'o' : select the outer camera\n\
f.four arrow keys : move the fly camera(inner camera) without changing the\n\
orientation determined by the mouse.Note that the inner camera viewport(upper\n\
    right view) should be updated when the inner camera is in motion.\n\
g.'ESC' : terminate the program\n\
----------------------------------------------------------------------------\n");

    //give the camera a 'pretty' starting point!
    innerCamXYZ = Point(5, 5, 5);
    innerCamTPR = Point(-M_PI / 4.0, M_PI / 4.0, 1);
    recomputeOrientation(innerCamDir, innerCamTPR);
    innerCamDir.normalize();

    outerCamTPR = Point(1.50, 2.0, 14.0);
    outerCamXYZ = Point(0, 0, 0);
    recomputeOrientation(outerCamXYZ, outerCamTPR);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    //register callback functions...
    loadTextures();
    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
    glutKeyboardFunc(normalKeys);
    glutDisplayFunc(renderCallback);
    glutReshapeFunc(resizeWindow);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(0, timer, 0);
    glutSpecialFunc(specialfunctions);
    //glutKeyboardFunc(normalKeys);
    //do some basic OpenGL setup
    initScene();

    //and enter the GLUT loop, never to exit.
    glutMainLoop();

    return(0);
}