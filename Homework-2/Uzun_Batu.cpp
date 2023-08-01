/*********
   CTIS164 - Template Source Program
----------
STUDENT : Batu Uzun
SECTION :001
HOMEWORK:2
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
when game is paused or not started traffic light is red, otherwise green.
only one missile can be displayed on the screen.
all scores will be displayed on the left side of the screen in a descending order.
first 10 seconds of the game, every missile hit gives 50 points, after 10 seconds it gives 150 points.
on the screen, gamer can see how many cars he destroyed.
cars have random colors.
after one game is finished, total score of that round will be displayed on the right side of the screen.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#define WINDOW_WIDTH  1300
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  1 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer
#define MYTIMER 100
#define D2R 0.0174532
#define MAXCAR 5
#define GAME 100

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
int activeTimer = 0, duration = 20, missileX, rocketon = 0;
//activeTimer: Check if the timer is on. rocketon: checks if there is a rocket on the screen
//missileX: missile variable.

int  periodcheck = 0;
//1000 periodcheck = 1 duration

int cont = 0, k, random, carduration = 0, tipofthemissile[2];
//cont: checks if game is paused or not     random: random number, tipofthemissile: location of the missile

int destroyedcars = -5, totalscore[GAME] = { -250 }, gamecount = 0;
//gamecount: number of games

//store plane's location
typedef struct {
    int xA, yA;


}plane_t;
//store car's location and colors
typedef struct {
    int xB, yB, r, g, b, color[MAXCAR][3], locations[MAXCAR][2], initpos[MAXCAR];
    int carmove[MAXCAR], distance[MAXCAR];

}cars_t;

cars_t carinfo;
plane_t planeinfo;
//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

//
// To display onto window using OpenGL commands
//
void BackGround() {
    //draw the highway
    glColor3ub(64, 64, 64);
    glRectf(-650, -350, 650, 0);
    glColor3ub(255, 255, 255);
    glRectf(-650, -190, -550, -160);
    glRectf(-500, -190, -400, -160);
    glRectf(-350, -190, -250, -160);
    glRectf(-200, -190, -100, -160);
    glRectf(-50, -190, 50, -160);
    glRectf(100, -190, 200, -160);
    glRectf(250, -190, 350, -160);
    glRectf(400, -190, 500, -160);
    glRectf(550, -190, 650, -160);
    //draw some buildings
    glColor3ub(160, 160, 160);
    glRectf(-250, 0, -150, 300);
    glColor3ub(102, 51, 0);
    glRectf(-230, 50, -210, 70);
    glRectf(-230, 90, -210, 110);
    glRectf(-230, 130, -210, 150);
    glRectf(-230, 170, -210, 190);
    glRectf(-230, 210, -210, 230);
    glRectf(-230, 250, -210, 270);
    glRectf(-170, 50, -190, 70);
    glRectf(-170, 90, -190, 110);
    glRectf(-170, 130, -190, 150);
    glRectf(-170, 170, -190, 190);
    glRectf(-170, 210, -190, 230);
    glRectf(-170, 250, -190, 270);
    glColor3ub(0, 0, 0);
    glRectf(-210, 30, -190, 0);


    glColor3ub(204, 0, 102);
    glRectf(-350, 0, -500, 150);
    glColor3ub(102, 51, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(-350, 150);
    glVertex2f(-425, 250);
    glVertex2f(-500, 150);
    glEnd();
    glColor3ub(0, 0, 0);
    glRectf(-440, 40, -410, 0);
    glColor3ub(160, 160, 160);
    glRectf(-370, 60, -400, 90);
    glRectf(-450, 60, -480, 90);
    glRectf(-370, 110, -400, 140);
    glRectf(-450, 110, -480, 140);

    glColor3ub(160, 160, 160);
    glRectf(200, 0, 100, 300);
    glColor3ub(102, 51, 0);
    glRectf(180, 50, 160, 70);
    glRectf(180, 90, 160, 110);
    glRectf(180, 130, 160, 150);
    glRectf(180, 170, 160, 190);
    glRectf(180, 210, 160, 230);
    glRectf(180, 250, 160, 270);
    glRectf(120, 50, 140, 70);
    glRectf(120, 90, 140, 110);
    glRectf(120, 130, 140, 150);
    glRectf(120, 170, 140, 190);
    glRectf(120, 210, 140, 230);
    glRectf(120, 250, 140, 270);
    glColor3ub(0, 0, 0);
    glRectf(160, 30, 140, 0);

    glColor3ub(204, 0, 102);
    glRectf(350, 0, 500, 150);
    glColor3ub(102, 51, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(350, 150);
    glVertex2f(425, 250);
    glVertex2f(500, 150);
    glEnd();
    glColor3ub(0, 0, 0);
    glRectf(440, 40, 410, 0);
    glColor3ub(160, 160, 160);
    glRectf(370, 60, 400, 90);
    glRectf(450, 60, 480, 90);
    glRectf(370, 110, 400, 140);
    glRectf(450, 110, 480, 140);
    //way sign
    glColor3ub(0, 0, 0);
    glRectf(275, 0, 290, 60);
    glRectf(245, 60, 320, 90);

    glColor3ub(255, 255, 255);
    glRectf(250, 70, 290, 80);
    glBegin(GL_TRIANGLES);
    glVertex2f(290, 65);
    glVertex2f(290, 85);
    glVertex2f(315, 75);
    glColor3ub(0, 0, 0);
    glEnd();
    vprint(250, 70, GLUT_BITMAP_8_BY_13, "ONE WAY");
    //traffic lamb
    glLineWidth(2);
    glColor3ub(102, 51, 0);
    glRectf(600, 0, 620, 130);
    glColor3ub(255, 0, 0);
    circle(610, 110, 5);
    glColor3ub(0, 0, 0);
    circle_wire(610, 110, 5);
    circle_wire(610, 90, 5);
    circle_wire(610, 70, 5);
    //change traffic light
    if (activeTimer == 1)
    {
        glColor3ub(0, 255, 0);
        circle(610, 70, 5);
        glColor3ub(102, 51, 0);
        circle(610, 110, 5);
        glColor3ub(0, 0, 0);
        circle_wire(610, 110, 5);
        circle_wire(610, 90, 5);
        circle_wire(610, 70, 5);

    }
}
void informativeMessage() {
    glColor3ub(0, 0, 0);
    if (activeTimer == 0 && cont == 0)
    {
        vprint(400, 315, GLUT_BITMAP_8_BY_13, "F1: start the game");
        //if (gamecount == 0)
        vprint(400, 240, GLUT_BITMAP_8_BY_13, "totalscore: %d", totalscore[gamecount - 1]);
    }
    else if (activeTimer == 1)//if game is active
    {
        vprint(400, 315, GLUT_BITMAP_8_BY_13, "<spacebar>      Fire");
        vprint(400, 280, GLUT_BITMAP_8_BY_13, "remaining time: %d", duration);
        vprint(400, 300, GLUT_BITMAP_8_BY_13, "<F1>    pause/continue the game");
        vprint(400, 260, GLUT_BITMAP_8_BY_13, "cars destroyed: %d", destroyedcars);
        vprint(400, 240, GLUT_BITMAP_8_BY_13, "totalscore: %d", totalscore[gamecount]);
    }
    else
    {
        vprint(400, 315, GLUT_BITMAP_8_BY_13, "F1: continue the game");
    }
}
void targetobject(int xB, int yB, int r, int g, int b) {
    //draw main body

    glColor3ub(r, g, b);
    glRectf(0 + xB, 0 + yB, 200 + xB, 30 + yB);
    glRectf(40 + xB, 30 + yB, 160 + xB, 40 + yB);
    //draw tires
    glColor3ub(153, 255, 153);
    circle(60 + xB, -4.5 + yB, 20);
    circle(140 + xB, -4.5 + yB, 20);
    glColor3ub(255, 255, 255);
    circle_wire(60 + xB, -4.5 + yB, 20);
    circle_wire(140 + xB, -4.5 + yB, 20);
    //draw front and back of the car
    glColor3ub(r, g, b);
    glBegin(GL_TRIANGLES);
    glVertex2f(0 + xB, 30 + yB);
    glVertex2f(40 + xB, 40 + yB);
    glVertex2f(40 + xB, 30 + yB);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(160 + xB, 30 + yB);
    glVertex2f(160 + xB, 40 + yB);
    glVertex2f(200 + xB, 30 + yB);
    glEnd();
    //draw top side of the car
    glColor3ub(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(160 + xB, 40 + yB);
    glVertex2f(40 + xB, 40 + yB);
    glVertex2f(60 + xB, 80 + yB);
    glVertex2f(140 + xB, 80 + yB);
    glEnd();
    glColor3ub(128, 128, 128);
    //draw windows
    glBegin(GL_TRIANGLES);
    glVertex2f(140 + xB, 40 + yB);
    glVertex2f(140 + xB, 80 + yB);
    glVertex2f(160 + xB, 40 + yB);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(60 + xB, 40 + yB);
    glVertex2f(60 + xB, 80 + yB);
    glVertex2f(40 + xB, 40 + yB);
    glEnd();
    glRectf(65 + xB, 45 + yB, 95 + xB, 75 + yB);
    glRectf(105 + xB, 45 + yB, 135 + xB, 75 + yB);
    glColor3ub(0, 0, 0);
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2f(65 + xB, 45 + yB);
    glVertex2f(95 + xB, 45 + yB);
    glVertex2f(65 + xB, 75 + yB);
    glVertex2f(95 + xB, 75 + yB);
    glVertex2f(65 + xB, 45 + yB);
    glVertex2f(65 + xB, 75 + yB);
    glVertex2f(95 + xB, 45 + yB);
    glVertex2f(95 + xB, 75 + yB);

    glVertex2f(105 + xB, 45 + yB);
    glVertex2f(135 + xB, 45 + yB);
    glVertex2f(105 + xB, 75 + yB);
    glVertex2f(135 + xB, 75 + yB);
    glVertex2f(105 + xB, 45 + yB);
    glVertex2f(105 + xB, 75 + yB);
    glVertex2f(135 + xB, 45 + yB);
    glVertex2f(135 + xB, 75 + yB);
    glEnd();
    //exhaust
    glRectf(0 + xB, 15 + yB, -15 + xB, 7 + yB);
    //car lights
    glColor3ub(255, 255, 153);
    glRectf(200 + xB, 15 + yB, 210 + xB, 7 + yB);
}
void plane(int xA) {
    //body
    glColor3ub(51, 25, 0);
    glRectf(-100 + xA, 230, 100 + xA, 280);
    glBegin(GL_QUADS);
    glColor3ub(51, 51, 0);
    glVertex2f(-100 + xA, 280);
    glVertex2f(-100 + xA, 320);
    glVertex2f(-80 + xA, 320);
    glVertex2f(-30 + xA, 280);
    glEnd();
    //Wings
    glColor3ub(51, 51, 0);
    glBegin(GL_QUADS);
    glVertex2f(-45 + xA, 320);
    glVertex2f(-15 + xA, 280);
    glVertex2f(45 + xA, 280);
    glVertex2f(15 + xA, 320);
    glEnd();
    glColor3ub(51, 51, 0);

    glBegin(GL_QUADS);
    glVertex2f(-45 + xA, 180);
    glVertex2f(-15 + xA, 240);
    glVertex2f(45 + xA, 240);
    glVertex2f(15 + xA, 180);
    glEnd();
    //cockpit
    glColor3ub(0, 0, 0);

    glBegin(GL_QUADS);
    glVertex2f(100 + xA, 280);
    glVertex2f(100 + xA, 250);
    glVertex2f(150 + xA, 250);
    glVertex2f(130 + xA, 270);
    glEnd();

    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(130 + xA, 235);
    glVertex2f(100 + xA, 230);
    glVertex2f(100 + xA, 250);
    glVertex2f(150 + xA, 250);
    glEnd();
}
void missiles(int yA) {
    //draw the missile
    glColor3ub(96, 96, 96);
    glRectf(70 + missileX, 230 - yA, 90 + missileX, 170 - yA);
    glColor3ub(0, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(70 + missileX, 170 - yA);
    glVertex2f(90 + missileX, 170 - yA);
    glVertex2f(80 + missileX, 150 - yA);
    glEnd();
    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(90 + missileX, 230 - yA);
    glVertex2f(missileX + 90, 210 - yA);
    glVertex2f(100 + missileX, 220 - yA);
    glVertex2f(100 + missileX, 230 - yA);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(70 + missileX, 230 - yA);
    glVertex2f(missileX + 70, 210 - yA);
    glVertex2f(60 + missileX, 220 - yA);
    glVertex2f(60 + missileX, 230 - yA);
    glEnd();
}
void showhighestscores() {
    int pass = 1, sorted = 0, temp, printfYposition = 200, sortedpoints[GAME];
    //necessary variables for this function. No need them in other functions.
    for (int i = 0; i < gamecount; i++)//to save totalscore array's content, use another array
        sortedpoints[i] = totalscore[i];
    while (sorted == 0)//sort the values
    {
        sorted = 1;
        for (int i = 0; i < gamecount - pass; i++)
        {
            if (sortedpoints[i] < sortedpoints[i + 1])
            {
                temp = sortedpoints[i];
                sortedpoints[i] = sortedpoints[i + 1];
                sortedpoints[i + 1] = temp;
                sorted = 0;
            }
        }
        pass++;
    }
    glColor3ub(0, 0, 0);
    vprint(-600, printfYposition, GLUT_BITMAP_8_BY_13, "scores:");

    for (int i = 0; i < gamecount; i++) {
        printfYposition -= 30;
        vprint(-600, printfYposition, GLUT_BITMAP_8_BY_13, "%d", sortedpoints[i]);

    }

}
void display() {
    //
    // clear window to black
    //
    glClearColor(153. / 255, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    //informativeMessage();
    BackGround();
    plane(planeinfo.xA);

    if (rocketon == 1 && cont != 0)
    {
        missiles(planeinfo.yA);

    }

    for (k = 0; k < MAXCAR; k++)//rocket hit check
    {
        if (((tipofthemissile[0] >= carinfo.locations[k][0] + carinfo.carmove[k] && (tipofthemissile[0] <= carinfo.locations[k][0] + 200 + carinfo.carmove[k])) &&
            ((tipofthemissile[1] >= carinfo.locations[k][1]) && (tipofthemissile[1] <= carinfo.locations[k][1] + 100))))

        {
            carinfo.distance[k] = rand() % 500;
            carinfo.initpos[k] = -975;
            carinfo.locations[k][0] = carinfo.initpos[k] - carinfo.distance[k];
            random = rand() % 350;
            random *= -1;
            carinfo.locations[k][1] = random;
            carinfo.carmove[k] = 0;
            destroyedcars++;
            carinfo.r = rand() % 255;
            carinfo.g = rand() % 255;
            carinfo.b = rand() % 255;
            carinfo.color[k][0] = carinfo.r;
            carinfo.color[k][1] = carinfo.g;
            carinfo.color[k][2] = carinfo.b;

            if (duration < 10)
                totalscore[gamecount] += 150;
            else
                totalscore[gamecount] += 50;
        }
        if (carinfo.locations[k][0] + carinfo.carmove[k] > WINDOW_HEIGHT)//check if cars are on the screen
        {
            carinfo.distance[k] = rand() % 500;
            carinfo.initpos[k] = -975;
            carinfo.locations[k][0] = carinfo.initpos[k] - carinfo.distance[k];
            random = rand() % 350;
            random *= -1;
            carinfo.locations[k][1] = random;
            carinfo.carmove[k] = 0;

            carinfo.r = rand() % 255;
            carinfo.g = rand() % 255;
            carinfo.b = rand() % 255;
            carinfo.color[k][0] = carinfo.r;
            carinfo.color[k][1] = carinfo.g;
            carinfo.color[k][2] = carinfo.b;
        }

    }

    for (int i = 0; i < MAXCAR; i++)//draw cars
    {
        carinfo.xB = carinfo.locations[i][0] + carinfo.carmove[i];
        carinfo.yB = carinfo.locations[i][1];

        targetobject(carinfo.xB, carinfo.yB, carinfo.color[i][0], carinfo.color[i][1], carinfo.color[i][2]);
    }
    if (gamecount > 0)
        showhighestscores();

    informativeMessage();
    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
    if (key == ' ' && activeTimer == 1)//if space is pressed and the timer is on
    {
        if (rocketon == 0)
        {

            missileX = planeinfo.xA;

        }
        tipofthemissile[0] = 80 + missileX;
        rocketon = 1;

    }
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }
    if (key == GLUT_KEY_F1)
    {
        activeTimer = !activeTimer;
        cont++;
    }
    if (key == GLUT_KEY_RIGHT && planeinfo.xA < 500 && activeTimer == 1)//if right arrow key is pressed
        planeinfo.xA += 50;
    if (key == GLUT_KEY_LEFT && planeinfo.xA > -550 && activeTimer == 1)//if left arrow key is pressed
        planeinfo.xA -= 50;
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    if (activeTimer == 1)
    {
        periodcheck += 10;//calculate 1 sec
        if (periodcheck % 10000 == 0)
            duration--;
        if (rocketon == 1)//move missile
            planeinfo.yA += 1;
        tipofthemissile[1] = 170 - planeinfo.yA;//save missile y axis

        if (carduration == 0)//initialize car's position
            for (k = 0; k < MAXCAR; k++)
            {


                carinfo.initpos[k] = -975;
                carinfo.distance[k] = rand() % 500;
                if (k > 0)
                    carinfo.initpos[k] = carinfo.initpos[k] - carinfo.distance[k];
                carinfo.locations[k][0] = carinfo.initpos[k];

                random = rand() % 350;
                random *= -1;
                carinfo.locations[k][1] = random;


            }
        for (k = 0; k < MAXCAR; k++)//move the car
            carinfo.carmove[k] += 1;
        carduration++;

    }

    if (planeinfo.yA >= 230 + 350)//is rocket on the screen
    {
        rocketon = 0;
        planeinfo.yA = 0;
    }

    if (duration == 0)//when time is finished
    {

        cont = 0;
        planeinfo.yA = 0;
        planeinfo.xA = 0;
        rocketon = 0;
        carduration = 0;
        destroyedcars = 0;
        activeTimer = 0;
        gamecount++;
        duration = 20;
        for (k = 0; k < MAXCAR; k++)
        {
            carinfo.carmove[k] = 0;
            carinfo.distance[k] = 0;
            carinfo.initpos[k] = -975;
        }
    }
    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()
}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Batu Uzun");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);
    srand(time(NULL));
    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);
#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif
    Init();
    glutMainLoop();
}