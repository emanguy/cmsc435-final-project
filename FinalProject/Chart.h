// Name: Jason de Bernard
// File: Chart.h
// Email: d105@umbc.edu
// Description: Graph class that has a bunch of bar graphs

#ifndef CHART_H
#define CHART_H

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "DDIMatrix.h"
#include "BarChart.h"

#include <GL/glut.h>

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

using namespace std;

enum eGraphType{MARKET, DEMO};

bool compareByAlpha(const BarChart &a, const BarChart &b);
bool compareByInter(const BarChart &a, const BarChart &b);
bool compareByFinal(const BarChart &a, const BarChart &b);
bool compareByPlan(const BarChart &a, const BarChart &b);

class Chart
{

 public:

  // Constructor
  Chart();

  // Use this to set up the chart based on the window size or whatever inputs
  Chart(float width, float height);

  // Renders all bar graphs in vector
  void Render();

  // Sets list that will be shown in bar graph
  void setList(DDIMatrix matrix);

  // Sets the type as MARKET or DEMO
  void setType(eGraphType id_type);

  // Sets the name of list as one of the market or demo names
  void setName(string id_name);

  // Sets Z value and sets bar widths
  void setDimensions();

  // Different kind of vector sorting
  void sortAlpha();
  void sortInter();
  void sortFinal();
  void sortPlan();

  // Some getters for name and type in case they become necessary
  string getName();
  eGraphType getType();

  // In case for whatever reason you want the chart dimensions
  float getChartWidth();
  float getChartHeight();

 private:

  // A list of all of the bar charts
  vector<BarChart> c_barCharts;

  // X and Y values of chart on screen (top left)
  float c_absX, c_absY;

  // Dimensions of chart for render
  float c_width, c_height;

  // Space between graphs and width of bars
  float c_spacing, c_planWidth, c_interWidth, c_finalWidth, c_barChartWidth;

  // Market ID or DEMO ID
  eGraphType m_type;

  // Market or Demo name
  string m_name;

};

#endif
