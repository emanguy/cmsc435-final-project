// Jason de Bernard
// File: BarChart.h
// Email: d105@umbc.edu
// Description: Bar charts that show DDI info

#ifndef BarChart_H
#define BarChart_H

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <string>

using namespace std;

class BarChart
{

 public:
  BarChart();

  // Setters for private parameters
  void setFinalZ(float z);

  void setPlanZ(float z);

  void setInterZ(float z);

  void setPlan(float height);

  void setInter(float height);

  void setFinal(float height);

  void setName(string name);


  // Getters for private parameters
  string getName() const;

  float getPlan() const;

  float getInter() const;

  float getFinal() const;

  float getFinalZ();

  float getPlanZ();

  float getInterZ();

 private:

  // Z values of each bar chart
  float intZ, planZ, finalZ;

  //Dimensions of 3 bars
  float intHeight, planHeight, finalHeight;

  // Market or Demo name
  string bar_name;

};

#endif
