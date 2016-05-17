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

#include "Triangle.h"


Triangle::Triangle(float x, float y, string market, string demo, DDIMatrix* matrix)
{
	this->X_CENTER=x;
	this->Y_CENTER=y;
  this->_market = market;
  this->_demo = demo;
  this->_matrix = matrix;
}
Triangle::~Triangle()
{
}

void Triangle::render(){
  this->draw_axis();
  this->draw_triangle();
  this->markAll();
}

void Triangle::draw_axis()
{
	float x1 = this->X_CENTER;
  float y1 = this->Y_CENTER - 200;
  float sin60 = sqrt(3)/2;
  float w = sin60 * 200;
  float x2 = this->X_CENTER - w;
  float y2 = this->Y_CENTER + 100;
  float x3 = this->X_CENTER + w;
  float y3 = this->Y_CENTER + 100;
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);
  glVertex2f(x1, y1);
  glVertex2f(this->X_CENTER, this->Y_CENTER);
  glEnd();
  glBegin(GL_LINES);
  glVertex2f(x2, y2);
  glVertex2f(this->X_CENTER, this->Y_CENTER);
  glEnd();
  glBegin(GL_LINES);
  glVertex2f(x3, y3);
  glVertex2f(this->X_CENTER, this->Y_CENTER);
  glEnd();
  glFlush();
}

void Triangle::draw_triangle(){
  float inter = _matrix->getDDI(this->_market, this->_demo, INTERIM);
  float plan = _matrix->getDDI(this->_market, this->_demo, PLANNING);
  float final = _matrix->getDDI(this->_market, this->_demo, FINAL);
  float sin60 = sqrt(3)/2;
  float interx = this->X_CENTER;
  float intery = this->Y_CENTER - inter;
  float planx = this->X_CENTER - plan * sin60;
  float plany = this->Y_CENTER + plan * 0.5;
  float finalx = this->X_CENTER + final * sin60;
  float finaly = this->Y_CENTER + final * 0.5; 
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

void Triangle::mark(float x_off, float y_off, eDataType type, float len, int rgb){
  float color1 =0, color2 = 0, color3 = 0; 
  float sinx, cosx, sinmark, cosmark;
  float sin60 = sqrt(3)/2;
  if(type == INTERIM){
    sinmark = 1;
    cosmark = 0;
  } else if(type == PLANNING){
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

  float x_mark = this->X_CENTER + x_off;
  float y_mark = this->Y_CENTER + y_off;
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

void Triangle::markDist(float min, float max, float avg, float std_dev, eDataType type, int len){

  float sinx, cosx;
  float sin60 = sqrt(3)/2;
  if(type == INTERIM){
    sinx = 0;
    cosx = -1;
  } else if(type == PLANNING){
    sinx = sin60;
    cosx = 0.5;
  } else {
    sinx = -sin60;
    cosx = 0.5; 
  } 
  float std2 = 2 * std_dev;
  float minx = sinx * min;
  float miny = cosx * min;
  this->mark(minx, miny, type, len, 1);
  float maxx = sinx * max;
  float maxy = cosx * max;
  this->mark(maxx, maxy, type, len, 1);
  float avgx = sinx * avg;
  float avgy = cosx * avg;
  this->mark(avgx, avgy, type, len, 2);
  float std1x = avgx + sinx * std2;
  float std1y = avgy + cosx * std2;
  float std2x = avgx - sinx * std2;
  float std2y = avgy - cosx * std2;
  this->mark(std1x, std1y, type, len, 3);
  this->mark(std2x, std2y, type, len, 3);
}

  void Triangle::markAll(){
    float inter_min, inter_max, inter_avg, inter_std;
    float plan_min, plan_max, plan_avg, plan_std;
    float final_min, final_max, final_avg, final_std;
    inter_min = _matrix->getMMin(_demo, INTERIM);
    inter_max = _matrix->getMMax(_demo, INTERIM);
    inter_avg = _matrix->getMAvg(_demo, INTERIM);
    inter_std = _matrix->getMStd(_demo, INTERIM);
    plan_min = _matrix->getMMin(_demo, PLANNING);
    plan_max = _matrix->getMMax(_demo, PLANNING);
    plan_avg = _matrix->getMAvg(_demo, PLANNING);
    plan_std = _matrix->getMStd(_demo, PLANNING);
    final_min = _matrix->getMMin(_demo, FINAL);
    final_max = _matrix->getMMax(_demo, FINAL);
    final_avg = _matrix->getMAvg(_demo, FINAL);
    final_std = _matrix->getMStd(_demo, FINAL);
    this->markDist(inter_min, inter_max, inter_avg, inter_std, INTERIM, 5);
    this->markDist(plan_min, plan_max, plan_avg, plan_std, PLANNING, 5);
    this->markDist(final_min, final_max, final_avg, final_std, FINAL, 5);
  }
