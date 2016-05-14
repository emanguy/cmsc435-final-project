#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>
#include <GL/glut.h>
#include <string>
#include "DDIMatrix.h"

using namespace std;

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

//coordinates of the center of the axis
float XCENTER = 300, YCENTER = 300;

//distribution of interim, planning, final
float inter_min, inter_max, inter_avg, inter_std;
float plan_min, plan_max, plan_avg, plan_std;
float final_min, final_max, final_avg, final_std;

//vertex of the triangle
float inter, plan, final;

//draw distribution on the axis
void mark(float x, float y, float x_off, float y_off, int dir, float len, int rgb);
void markDist(float x, float y, float min, float max, float avg, float std_dev, int dir, int len);

//draw axis
void draw_axis(float x, float y);

//draw triangle
void draw_triangle(float x, float y, float inter, float plan, float final);

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT);
    draw_axis(XCENTER, YCENTER);
    markDist(XCENTER, YCENTER, inter_min, inter_max, inter_avg, inter_std, 0, 5);//interim
    markDist(XCENTER, YCENTER, plan_min, plan_max, plan_avg, plan_std, 1, 5);//plan
    markDist(XCENTER, YCENTER, final_min, final_max, final_avg, final_std, 2, 5);//final
    draw_triangle(XCENTER, YCENTER, inter, plan, final);
} 

void reshape(int w, int h)
{
   int width=w;
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 600.0, 500.0, 0.0, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

int main(int argc, char **argv)
{
   DDIMatrix matrix("interim.csv", "planning.csv", "final.csv");
   string market = "059";
   string demo = "O6+";

   //generate distribution of interim
   inter_min = matrix.getMMin(demo, INTERIM);
   inter_max = matrix.getMMax(demo, INTERIM);
   inter_avg = matrix.getMAvg(demo, INTERIM);
   inter_std = matrix.getMStd(demo, INTERIM);
   cout << "inter_min:" << inter_min << endl;
   cout << "inter_max:" << inter_max << endl;
   cout << "inter_avg:" << inter_avg << endl;
   cout << "inter_std:" << inter_std << endl;

   //generate distribution of planning
   plan_min = matrix.getMMin(demo, PLANNING);
   plan_max = matrix.getMMax(demo, PLANNING);
   plan_avg = matrix.getMAvg(demo, PLANNING);
   plan_std = matrix.getMStd(demo, PLANNING);
   cout << "plan_min:" << plan_min << endl;
   cout << "plan_max:" << plan_max << endl;
   cout << "plan_avg:" << plan_avg << endl;
   cout << "plan_std:" << plan_std << endl;

   //generate distribution of final
   final_min = matrix.getMMin(demo, FINAL);
   final_max = matrix.getMMax(demo, FINAL);
   final_avg = matrix.getMAvg(demo, FINAL);
   final_std = matrix.getMStd(demo, FINAL);
   cout << "final_min:" << final_min << endl;
   cout << "final_max:" << final_max << endl;
   cout << "final_avg:" << final_avg << endl;
   cout << "final_std:" << final_std << endl;

   //generate triangle
   inter = matrix.getDDI(market, demo, INTERIM);
   plan = matrix.getDDI(market, demo, PLANNING);
   final = matrix.getDDI(market, demo, FINAL);

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);

   glutDisplayFunc(display);

   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}

void mark(float x, float y, float x_off, float y_off, int dir, float len, int rgb){
  float color1 =0, color2 = 0, color3 = 0; 
  float sinx, cosx, sinmark, cosmark;
  float sin60 = sqrt(3)/2;
  if(dir == 0){
    sinmark = 1;
    cosmark = 0;
  } else if(dir == 2){
    sinmark = 0.5;
    cosmark = -sin60;
  } else {   
    sinmark = -0.5;
    cosmark = -sin60;
  }

  if(rgb == 1){
    color1 = 1;
  } else if(rgb == 2){
    color2 = 1;
  } else {   
    color3 = 1;
  }

  float x_mark = x + x_off;
  float y_mark = y + y_off;
  float x1_mark = x_mark + sinmark * len;
  float y1_mark = y_mark + cosmark * len;
  float x2_mark = x_mark - sinmark * len;
  float y2_mark = y_mark - cosmark * len;
  glColor3f(color1, color2, color3);
  glBegin(GL_LINES);
  glVertex2f(x1_mark, y1_mark);
  glVertex2f(x2_mark, y2_mark);
  glEnd();
  glFlush();
}

void markDist(float x, float y, float min, float max, float avg, float std_dev, int dir, int len){
  float sinx, cosx;
  float sin60 = sqrt(3)/2;
  if(dir == 0){
    sinx = 0;
    cosx = -1;
  } else if(dir == 2){
    sinx = sin60;
    cosx = 0.5;
  } else {
    sinx = -sin60;
    cosx = 0.5; 
  } 
  float std2 = 2 * std_dev;
  float minx = sinx * min;
  float miny = cosx * min;
  mark(x, y, minx, miny, dir, len, 1);
  float maxx = sinx * max;
  float maxy = cosx * max;
  mark(x, y, maxx, maxy, dir, len, 1);
  float avgx = sinx * avg;
  float avgy = cosx * avg;
  mark(x, y, avgx, avgy, dir, len, 2);
  float std1x = avgx + sinx * std2;
  float std1y = avgy + cosx * std2;
  float std2x = avgx - sinx * std2;
  float std2y = avgy - cosx * std2;
  mark(x, y, std1x, std1y, dir, len, 3);
  mark(x, y, std2x, std2y, dir, len, 3);
}

void draw_axis(float x, float y){    
  float x1 = x;
  float y1 = y - 200;
  float sin60 = sqrt(3)/2;
  float w = sin60 * 200;
  float x2 = x - w;
  float y2 = y + 100;
  float x3 = x + w;
  float y3 = y + 100;
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);
  glVertex2f(x1, y1);
  glVertex2f(x, y);
  glEnd();
  glBegin(GL_LINES);
  glVertex2f(x2, y2);
  glVertex2f(x, y);
  glEnd();
  glBegin(GL_LINES);
  glVertex2f(x3, y3);
  glVertex2f(x, y);
  glEnd();
  glFlush();
}

void draw_triangle(float x, float y, float inter, float plan, float final){
  float sin60 = sqrt(3)/2;
  float interx = x;
  float intery = y - inter;
  float planx = x - plan * sin60;
  float plany = y + plan * 0.5;
  float finalx = x + final * sin60;
  float finaly = y + final * 0.5; 
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);
  glVertex2f(planx, plany);
  glVertex2f(interx, intery);
  glEnd();
  glBegin(GL_LINES);
  glVertex2f(finalx, finaly);
  glVertex2f(interx, intery);
  glEnd();
  glBegin(GL_LINES);
  glVertex2f(planx, plany);
  glVertex2f(finalx, finaly);
  glEnd();
  glFlush();
}
