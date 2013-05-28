/* BezierCurve.h
*  Contains class BezierCurve, which contains all the math necessary to draw
*  Bezier curve; the rendering class can just read off all the data.
*  I wish Unicode was supported for class names so I could name the class
*  with the proper accent mark on 'Bezier'.
*/

#include <vector>
using std::vector;

#ifndef BEZIER_CURVE_H
#define BEZIER_CURVE_H

class BezierCurve
{
	private:
		vector<float> coords;
		static const int degree = 3;	// Stuck at cubic.
		float control_points[8];		// Even indices are x coords, odd are y coords; 4 points.
		float basis(float u, int i, int degree);
		int binomial(int n, int i);
		static int binom_table[4];
	public:
		// Could use either direct Bernstein polynomial method or
		// de Casteljau's to construct the curve; using Bernstein
		// polynomial here.
		vector<float>* curve_coordinates(float du);
			// Give an increment, du, for the parameter u, which determines how many
			// points we actually calculate. Get back a vector of floats that specify
			// the coordinates for these points, with even indices being x coordinates
			// and odd indices being y coordinates. Make sure to store it somewhere if
			// you want to reuse it, because there's no way to directly access it;
			// this is to avoid stale data mistakes.
		BezierCurve(float* controls);
			// controls is in the same format as the control_points member.
		float* get_points() { return this->control_points; } 
		float derivative(float u);
		static float derivative(float u, float* controls, int degree);
};

class BezierSpline
{
	private:
		vector<BezierCurve> constituents;
			// Store all the curves that have been joined to make up the spline.
			// When we need to actually draw the spline, just loop over this and
			// call curve_coordinates for each one.
		vector<float> coordinates;
		vector<float> points;
	public:
		void join(BezierCurve&);
			// Joins another Bezier curve to the spline.
		void join(float*);
		void collect_points();
		vector<float>* curve_coordinates(float du);
			// Calculates the coordinates for the Bezier spline. 
};
#endif