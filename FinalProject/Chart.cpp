// Name: Jason de Bernard
// File: Chart.cpp
// Email: d105@umbc.edu
// Description: Graph functions that set parameters for rendering

#include "Chart.h"
#include "BarChart.h"
#include <GL/glut.h>
#include <GL/gl.h>

Chart::Chart(){

  // Dimensions of outer chart
  c_absX = 0;
  c_absY = 0;
  c_width = 500.0;
  c_height = 500.0;

  // Default names set on
  m_type = DEMO;
  m_name = "O6+";

}

Chart::Chart(float width, float height){

  // Dimensions of outer chart
  c_absX = 0;
  c_absY = 0;
  c_width = width;
  c_height = height;

  // Default names set on. Set these with setType and setName or nothing will render
  //m_type = DEMO;
  //m_name = "O6+";

}

/* Function Name: Render()
/  Description: Call this when you are ready to render all of the bar charts
/  onto the screen. This function will loop through the entire vector and
/  place the bar charts accordingly.
*/
void Chart::Render(QGLWidget* renderArea){

  // Size of list of bar charts
  int vecSize = c_barCharts.size();

  // Render label
  QString label(m_type == DEMO? "Demographic: " : "Market: ");
  label += + m_name.c_str();
  glColor3f(1, 1, 0);
  renderArea->renderText(c_absX, c_absY + c_height - 10, label);

  for(int i = 0; i < vecSize; i++){

    // First bar = Planning
    glPushMatrix();

     glTranslatef(c_absX + i*(c_barChartWidth+c_spacing) + c_spacing, c_absY + (c_height/2), c_barCharts.at(i).getPlanZ());

    glBegin(GL_QUADS);  
      glColor3f(1, 0, 0);
      glVertex3f(0, 0, 0);     // Bottom left     
      glVertex3f(0, -c_barCharts.at(i).getPlan(), 0);     // Top left
      glVertex3f(c_planWidth, -c_barCharts.at(i).getPlan(), 0);     // Top right
      glVertex3f(c_planWidth, 0, 0);       // Bottom right
    glEnd();
  glPopMatrix();

  // Second Bar = Interim
  glPushMatrix();

   glTranslatef(c_absX + i*(c_spacing + c_barChartWidth) + c_spacing + (c_planWidth/2), c_absY + c_height/2, c_barCharts.at(i).getInterZ());

    glBegin(GL_QUADS);  
      glColor3f(0, 1, 0);
      glVertex3f(0, 0, 0);     // Bottom left     
      glVertex3f(0, -c_barCharts.at(i).getInter(), 0);     // Top left
      glVertex3f(c_interWidth, -c_barCharts.at(i).getInter(), 0);     // Top right
      glVertex3f(c_interWidth, 0, 0);       // Bottom right
    glEnd();
  glPopMatrix();

  // Third Bar = Final
  glPushMatrix();

    glTranslatef(c_absX + i*(c_barChartWidth + c_spacing) + c_spacing + (c_planWidth/2) + (c_interWidth/3), c_absY + (c_height/2), c_barCharts.at(i).getFinalZ());

    glBegin(GL_QUADS);  
      glColor3f(0, 0, 1);
      glVertex3f(0, 0, 0);     // Bottom left     
      glVertex3f(0, -c_barCharts.at(i).getFinal(), 0);     // Top left
      glVertex3f(c_finalWidth, -c_barCharts.at(i).getFinal(), 0);     // Top right
      glVertex3f(c_finalWidth, 0, 0);       // Bottom right
    glEnd();
  glPopMatrix();

  }

}

/* Function: setList
/ Description: This just pulls a list from the DDI data based given the
/ name of the market or demo and whether you are pulling a market or demo
/ type of list. Make sure there is a name and type before calling this.
*/
void Chart::setList(DDIMatrix matrix){

  // Empty previous list of bar charts
  c_barCharts.clear();

  // Quick list of strings for market or demo data
  list<string> stringList;

  // Market chosen by user (list of demos)
  if(m_type == MARKET){
    stringList = matrix.getDemo(m_name);
    ListS::iterator list_front = stringList.begin();

    for( ; list_front != stringList.end(); list_front++){
    BarChart barChart;

	barChart.setPlan(matrix.getDDI(m_name, *list_front, PLANNING));
	barChart.setInter(matrix.getDDI(m_name, *list_front, INTERIM));
	barChart.setFinal(matrix.getDDI(m_name, *list_front, FINAL));

	barChart.setName(*list_front);

	c_barCharts.push_back(barChart);
      }
  }

  // Demo chosen by user (list of markets)
  else if(m_type == DEMO){
    stringList = matrix.getMarket(m_name);
    ListS::iterator list_front = stringList.begin();

    for( ; list_front != stringList.end(); list_front++){
      BarChart barChart;

      barChart.setPlan(matrix.getDDI(*list_front, m_name, PLANNING));
      barChart.setInter(matrix.getDDI(*list_front, m_name,  INTERIM));
      barChart.setFinal(matrix.getDDI(*list_front, m_name, FINAL));

      barChart.setName(*list_front);

      c_barCharts.push_back(barChart);
    }
  }

}


void Chart::setType(eGraphType id_type){

  // Make sure the radio button calls this to set the bar graph type
  m_type = id_type;

}


void Chart::setName(string id_name){

  // Make sure the drop down calls this to set the name
  m_name = id_name;

}

/* Function: setDimensions
/  Description: Make sure to call this once the user has selected
/  all of the information that they want to show. This will count
/  the number of bar graphs and set the widths accordingly. This also
/  goes through and sets the z dimensions (or depth) based on the highest
/  values so the higher DDI is pushed further back.
*/
void Chart::setDimensions(){

  // Size of list of charts
  int chartSize = c_barCharts.size();
  
  float barChartWidth = (float)(c_width / chartSize);

  // Spacing between the charts
  c_spacing = barChartWidth / 5;

  // Set width of each region of barcharts
  c_barChartWidth = ((c_width - c_spacing) / chartSize) - c_spacing;

  // Setting width of charts
  c_planWidth = c_barChartWidth / 2;
  c_interWidth = (c_barChartWidth / 3) + (c_planWidth/2);
  c_finalWidth = (c_barChartWidth / 6) + (c_interWidth/3);

  for(int i = 0; i < chartSize; i++){
    
    // Highest is interim
    if((c_barCharts.at(i).getInter() >= c_barCharts.at(i).getFinal()) && (c_barCharts.at(i).getInter() >= c_barCharts.at(i).getPlan())){
	
	c_barCharts.at(i).setInterZ(0.0);

	// Inter > Final > Plan
	if(c_barCharts.at(i).getFinal() >= c_barCharts.at(i).getPlan()){
	  c_barCharts.at(i).setFinalZ(-0.1);
	  c_barCharts.at(i).setPlanZ(-0.2);
	}

	// Inter > Plan > Final
	else{
	  c_barCharts.at(i).setFinalZ(-0.2);
	  c_barCharts.at(i).setPlanZ(-0.1);
	}
      }

    // Highest is planning
    else if((c_barCharts.at(i).getPlan() >= c_barCharts.at(i).getFinal()) && (c_barCharts.at(i).getPlan() >= c_barCharts.at(i).getInter())){

	c_barCharts.at(i).setPlanZ(0.0);
	  
	  // Plan > Final > Inter
	if(c_barCharts.at(i).getFinal() >= c_barCharts.at(i).getInter()){
	  c_barCharts.at(i).setInterZ(-0.2);
	  c_barCharts.at(i).setFinalZ(-0.1);
	}
	
	// Plan > Inter > Final
	else{
	  c_barCharts.at(i).setInterZ(-0.1);
	  c_barCharts.at(i).setFinalZ(-0.2);
	}
      }

    // Highest is final
    else{
	
      c_barCharts.at(i).setFinalZ(0.0);

      // Final > Inter > Plan
      if(c_barCharts.at(i).getFinal() >= c_barCharts.at(i).getPlan()){
	c_barCharts.at(i).setInterZ(-0.1);
	c_barCharts.at(i).setPlanZ(-0.2);
      }

      // Final > Plan > Inter
      else{
	c_barCharts.at(i).setInterZ(-0.2);
	c_barCharts.at(i).setPlanZ(-0.1);
      }
	
    }

  }

}

void Chart::setWidth(float width)
{
    c_width = width;
}

void Chart::setHeight(float height)
{
    c_height = height;
}

void Chart::setAbsX(float absX)
{
    c_absX = absX;
}

void Chart::setAbsY(float absY)
{
    c_absY = absY;
}

void Chart::sortAlpha(){

  sort(c_barCharts.begin(), c_barCharts.end(), compareByAlpha);

}

void Chart::sortInter(){

  sort(c_barCharts.begin(), c_barCharts.end(), compareByInter);

}

void Chart::sortFinal(){

  sort(c_barCharts.begin(), c_barCharts.end(), compareByFinal);

}

void Chart::sortPlan(){

  sort(c_barCharts.begin(), c_barCharts.end(), compareByPlan);

}


/*** This point on is all getters. The function return will show what is being gotten***/
string Chart::getName(){

  return m_name;

}

eGraphType Chart::getType(){

  return m_type;

}

float Chart::getChartWidth(){

  return c_width;

}

float Chart::getChartHeight(){

  return c_height;

}

float Chart::getChartY()
{
    return c_absY;
}

float Chart::getChartX()
{
    return c_absX;
}


//*** Except for these non-class functions for sorting! ***//
bool compareByAlpha(const BarChart &a, const BarChart &b)
{
  return a.getName() < b.getName();
}

bool compareByInter(const BarChart &a, const BarChart &b)
{
  return a.getInter() < b.getInter();
}

bool compareByFinal(const BarChart &a, const BarChart &b)
{
  return a.getFinal() < b.getFinal();
}

bool compareByPlan(const BarChart &a, const BarChart &b)
{
  return a.getPlan() < b.getPlan();
}
