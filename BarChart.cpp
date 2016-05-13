// Name: Jason de Bernard
// File: BarChart.cpp
// Email: d105@umbc.edu
// Description: Smaller bar charts within larger chart

#include "BarChart.h"

BarChart::BarChart(){

}

// Sets the height of bar based on planning ddi
void BarChart::setPlan(float height){

  planHeight = height;

}

// Sets height of bar based on interim ddi
void BarChart::setInter(float height){

  intHeight = height;

}

// Sets height of bar based on final ddi
void BarChart::setFinal(float height){

  finalHeight = height;

}

// Sets the name of the bar chart given the market or demo name
void BarChart::setName(string name){

  bar_name = name;

}


// Sets z value for planning bar (for depth)
void BarChart::setPlanZ(float z){

  planZ = z;

}

// Sets z value for interim bar (for depth)
void BarChart::setInterZ(float z){

  intZ = z;

}

// Sets z value for final bar (for depth)
void BarChart::setFinalZ(float z){

  finalZ = z;

}

// Returns name of current market or demo
string BarChart::getName() const{

  return bar_name;

}

// Returns value of planning ddi for current bar
float BarChart::getPlan() const{

  return planHeight;

}

// Returns value for interim ddi for current bar
float BarChart::getInter() const{

  return intHeight;

}

// Returns value for final ddi for current bar
float BarChart::getFinal() const{

  return finalHeight;

}

// Depth value for planning bar
float BarChart::getPlanZ(){

  return planZ;

}

// Depth value for interim bar
float BarChart::getInterZ(){

  return intZ;

}

// Depth value for final bar
float BarChart::getFinalZ(){

  return finalZ;

}



