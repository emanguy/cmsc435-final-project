#include <string>
#include <iostream>

using namespace std;

class Triangle{
private:
    float X_CENTER;
    float Y_CENTER;
    string _market;
    string _demo;
    DDIMatrix* _matrix;

public:
	Triangle(float x, float y, string market, string demo, DDIMatrix* matrix);
	~Triangle();

	//draw distribution on the axis
	void render();
	void mark(float x_off, float y_off, eDataType type, float len, int rgb);
	void markDist(float min, float max, float avg, float std_dev, eDataType type, int len);
	void markAll();
	//draw axis
	void draw_axis();
	//draw triangle
	void draw_triangle();
};
