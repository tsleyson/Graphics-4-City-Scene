#include "BezierCurve.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

// If we were doing higher-degree curves, we could cache the
// needed binomial coefficients by calculating them with the
// binum function at construction time and storing them in
// binom_table, but with just four it's quick and easy to
// hard-code it.
int BezierCurve::binom_table[4] = {1, 3, 3, 1};

BezierCurve::BezierCurve(float* controls)
{
	for (int k = 0; k < 8; k++)
		this->control_points[k] = controls[k];
}

vector<float>* BezierCurve::curve_coordinates(float du)
{
	// Add first control points.
	this->coords.push_back(this->control_points[0]);
	this->coords.push_back(this->control_points[1]);
	vector<float> x, y;
	// To make things a bit easier to work with, convert from the
	// OpenGL-friendly representation with alternating x and y
	// coords in one array to separate x and y arrays.
	for (int i = 0; i < 8; i++)
	{
		i % 2 == 0 ? x.push_back(this->control_points[i]) : y.push_back(this->control_points[i]);
	}
	for (float u = 0.0; u < 1.0; u += du)
    {
		float xu = 0.0, yu = 0.0;
		for (int i = 0; i <= this->degree; i++) 
			xu += this->basis(u, i, 3) * x[i];
		for (int i = 0; i <= this->degree; i++) 
			yu += this->basis(u, i, 3) * y[i];
		this->coords.push_back(xu); this->coords.push_back(yu);
    }
	// Add last control points.
	this->coords.push_back(this->control_points[6]);
	this->coords.push_back(this->control_points[7]);
	return &this->coords;
}

// Not actually used.
int BezierCurve::binomial(int n, int i)
{
	int nfac = 1, n_minus_i_fac = 1, i_fac = 1;
	for (int index = 2; index <= n; index++)
	{
		nfac *= index;	// Always do this.
		if (index <= (n - i))
			n_minus_i_fac *= index;
		if (index <= i)
			i_fac *= index;
		// This way we can calculate all three factorials as we go along
		// and save some time.
	}
	return nfac / (i_fac*n_minus_i_fac);
}

float BezierCurve::basis(float u, int i, int degree)
{
	return binom_table[i] * pow(u, i) * pow(1 - u, degree - i);
}

float BezierCurve::derivative(float u, float* control_points, int degree)
{
	static int binom_list[3] = {1, 2, 1};
		// We need a different set of binomial coefficents since we're evaluating the
		// basis function for degree - 1.
	vector<float> x, y;
	for (int i = 0; i < (degree+1)*2; i++)
	{
		i % 2 == 0 ? x.push_back(control_points[i]) : y.push_back(control_points[i]);
	}
	float xsum = 0.0, ysum = 0.0;
	for (int i = 0; i <= degree - 1; i++)
	{
		xsum += binom_list[i]* pow(u, i) * pow(1-u, degree-i) * degree * x[i+1] - x[i];
		ysum += binom_list[i]* pow(u, i) * pow(1-u, degree-i) * degree * y[i+1] - y[i];
	}
	return xsum+ysum;
}

float BezierCurve::derivative(float u)
{
	return derivative(u, this->control_points, this->degree);
}


void BezierSpline::join(BezierCurve& bc)
{
	BezierCurve modified(bc.get_points());
	this->constituents.push_back(modified);
	/*if (this->constituents.empty())
	{
		this->constituents.push_back(modified);
		return;		
	}
	this->join(modified.get_points());*/
}

/* Part of an abortive attempt to make this program
actually do what it was supposed to.
void BezierSpline::collect_points()
{
	vector<float> temp_coords;
	float* temp_pts;
	for (vector<BezierCurve>::iterator it = this->constituents.begin(); 
		 it != this->constituents.end();
		 ++it)
	{
		temp_pts = it->get_points();
		for (int i = 0; i < 8; i++)
		{
			temp_coords.push_back(temp_pts[i]);
		}
	}
	int point_count = 0;
	for (int i = 0; i < temp_coords.size(); i++)
	{
		this->points.push_back(temp_coords.at(i));
		if (i%2 == 0 && i > 0) ++point_count;
		if (point_count%2 == 0 && point_count > 0 && point_count+1 < temp_coords.size()/2)
		{
			float xmid = (temp_coords[i] + temp_coords[i+2])/2.0;
			float ymid = (temp_coords[i+1] + temp_coords[i+3])/2.0;
			this->points.push_back(xmid);
			this->points.push_back(ymid);
		}
	}
}*/

void BezierSpline::join(float* controls)
{
	BezierCurve modified(controls);
	this->constituents.push_back(modified);
	/*
	if (this->constituents.empty())
	{
		this->constituents.push_back(modified);
		return;		
	}
	float* next_points = controls;
	float* prev_points = this->constituents.back().get_points();
	float x1 = next_points[2], y1 = next_points[3];
	float x2 = prev_points[4], y2  = prev_points[5];
	// Make the midpoint between the penultimate point of the
	// previous curve and the second point of the next curve
	// the new shared join point.
	next_points[0] = prev_points[6] = (x1 + x2)/2.0;
	next_points[1] = prev_points[7] = (y1 + y2)/2.0;
	this->constituents.push_back(modified);*/
}


vector<float>* BezierSpline::curve_coordinates(float du)
{
	vector<BezierCurve>::iterator vit;
	vector<float>* temp;
	for (vit = 	this->constituents.begin(); vit != this->constituents.end(); ++vit)
	{
		temp = vit->curve_coordinates(du);
		for (unsigned int i = 0; i < temp->size(); i++)
		{
			this->coordinates.push_back(temp->at(i));
		}
	}
	return &this->coordinates;
}