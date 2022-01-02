#include<Gl/glut.h>
#include<math.h>
#include<ctime>

#define PI 3.1416
#define X 0
#define Y 1
#define R 0
#define G 1
#define B 2
#define RGB 3
#define radius 220
#define innerRadius 10
#define secondLenght 190
#define minuteLength 150
#define hourLenght 100
#define steps 360

//all required functions
void display();
void reshape(int, int);
void drawCircle(int, float color[RGB]);
void drawMinuteHourMarks();
void drawSecondHand();
void drawHourHand();
void drawMinuteHand();
void drawLines(float point1[2], float point2[2], float, float color[RGB]);
void drawMarks();
void redraw();
void drawClock();
float radian(float);
void ddaLineDrawing(float point1[2], float point2[2], float pointSize, float color[RGB]);

float circleCenterCoordinate[2] = {250, 250};
float minuteColor[RGB] = {0.0, 1.0, 0.0};
float hourColor[RGB] = {1.0, 1.0, 1.0};
float borderColor[RGB] = {0.275, 0.813, 0.424};
float innerColor[RGB] = {0.4, 0.2, 0.3};
float centerColor[RGB] = {0.1, 0.1,0.1};
float secColor[RGB] = {1.0, 0.0, 0.0};
float secAngle=0, minuteAngle=0, hourAngle=0;

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowPosition(500,100);
    glutInitWindowSize(500,500);
    glutCreateWindow("Analog Clock");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    drawClock();
}

void drawClock()
{
    drawCircle(radius+8, borderColor);
    drawCircle(radius, innerColor);
    drawMarks();
    redraw();
    drawCircle(innerRadius, centerColor);
    glFlush();
}

void reshape(int width, int height)
{
    glViewport(0,0,width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
    glMatrixMode(GL_MODELVIEW);
}

void drawCircle(int r, float color[RGB])
{
    float theta;
    float x, y;
    glColor3fv(color);
    glBegin(GL_POLYGON);
    for(int i=0; i<=steps; i++)
    {
        //in radian
        theta = i*PI/180;
        x = circleCenterCoordinate[X]+r*sin(theta);
        y = circleCenterCoordinate[Y]+r*cos(theta);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawMarks()
{
    float theta;
    float point1[2], point2[2];
    //for minutes
    for(int i=0; i<=steps; i+=6)
    {
        theta = i*PI/180;
        point1[X] = circleCenterCoordinate[X]+radius*sin(theta);
        point1[Y] = circleCenterCoordinate[Y]+radius*cos(theta);
        point2[X] = circleCenterCoordinate[X]+(radius-10)*sin(theta);
        point2[Y] = circleCenterCoordinate[X]+(radius-10)*cos(theta);
        drawLines(point1, point2, 2.0, minuteColor);
    }
    //for Hour
    for(int i=0; i<=360; i+=30)
    {
        theta = i*PI/180;
        point1[X] = circleCenterCoordinate[X]+radius*cos(theta);
        point1[Y] = circleCenterCoordinate[Y]+radius*sin(theta);
        point2[X] = circleCenterCoordinate[X]+(radius-30)*cos(theta);
        point2[Y] = circleCenterCoordinate[X]+(radius-30)*sin(theta);
        drawLines(point1, point2, 4.0, hourColor);
    }
}
/*void drawLines(float point1[2], float point2[2], float pointSize, float color[RGB])
{

    glPointSize(5.0);
    glColor3fv(color);
    glBegin(GL_LINE_STRIP);
    glVertex2f(point1[X], point1[Y]);
    glVertex2f(point2[X], point2[Y]);
    glEnd();
}*/

void drawLines(float point1[2], float point2[2], float pointSize, float color[RGB])
{
    float d, m, x1, x2, y1, y2, dx, dy, step, Xinc, Yinc;
    x1=point1[X];
    y1=point1[Y];
    x2=point2[X];
    y2=point2[Y];
    float x = x1;
    float y = y1;
    dx = x2 - x1;
    dy = y2 - y1;
    step = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    Xinc = dx/step;
    Yinc = dy/step;
    glColor3fv(color);
    glPointSize(pointSize);
    glBegin(GL_POINTS);
    for (int i=0; i<step; i++)
    {
        glVertex2f(x,y);
        x += Xinc;
        y += Yinc;
    }
    glEnd();
}

//timer function
void redraw()
{
    time_t now = time(0);
    tm* ltm = localtime(&now);
    //in radian
    secAngle = ltm->tm_sec*6;
    minuteAngle = ltm->tm_min*6+(ltm->tm_sec*6/60);
    hourAngle = ltm->tm_hour*30+(ltm->tm_min*6/12);
    float point1[2] = {250, 250};
    float point2[2];
    //hour hand
    point1[X] = circleCenterCoordinate[X]+innerRadius*sin(radian(hourAngle));
    point1[Y] = circleCenterCoordinate[Y]+innerRadius*cos(radian(hourAngle));
    point2[X] = circleCenterCoordinate[X]+hourLenght*sin(radian(hourAngle));
    point2[Y] = circleCenterCoordinate[Y]+hourLenght*cos(radian(hourAngle));
    drawLines(point1, point2, 5.0, hourColor);
    //minute hand
    point1[X] = circleCenterCoordinate[X]+innerRadius*sin(radian(minuteAngle));
    point1[Y] = circleCenterCoordinate[Y]+innerRadius*cos(radian(minuteAngle));
    point2[X] = circleCenterCoordinate[X]+minuteLength*sin(radian(minuteAngle));
    point2[Y] = circleCenterCoordinate[Y]+minuteLength*cos(radian(minuteAngle));
    drawLines(point1, point2, 3.0, minuteColor);
    //second hand
    point1[X] = circleCenterCoordinate[X]+innerRadius*sin(radian(secAngle));
    point1[Y] = circleCenterCoordinate[Y]+innerRadius*cos(radian(secAngle));
    point2[X] = circleCenterCoordinate[X]+secondLenght*sin(radian(secAngle));
    point2[Y] = circleCenterCoordinate[Y]+secondLenght*cos(radian(secAngle));
    drawLines(point1, point2, 1.5, secColor);
    glutPostRedisplay();
}

//function for converting theta into radian
float radian(float angle)
{
    return angle*PI/180;
}
