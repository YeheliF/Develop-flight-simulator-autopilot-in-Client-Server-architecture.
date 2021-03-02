/*
 * animaly_detection_util.cpp
 *
 * Author: 207233222 Yeheli Frangi
 */

#include <math.h>
#include "anomaly_detection_util.h"

//return the avrage of a list of floats.
float avg(float* x, int size){
	float sum = 0;
	for (int i = 0; i < size; i++) {
		sum += x[i];
	}
	return (sum / size);
}

// returns the variance of X and Y
float var(float* x, int size){
	float sum = 0;
	for (int i = 0; i < size; i++) {
		sum += pow(x[i],2);
	}
	float part1 = sum / size;
	float part2 = pow(avg(x, size), 2);
	return (part1 - part2);
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
	float part2 = (avg(x, size) * avg(y, size));
	float sum=0;
	float mult[size];
	for (int i = 0; i < size; i++) {
		mult[i] = (x[i]*y[i]);
	}
	for (int i = 0; i < size; i++) {
		sum += mult[i];
	}
	float part1 = (sum / size);
	return (part1 - part2);
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
	float part1 = cov(x, y, size);
	float part2 = sqrt(var(x,size)* var(y, size));
	return (part1 / part2);

}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){
	float xArr[size];
	float yArr[size];
	for (int i = 0; i < size; i++) {
		xArr[i] = points[i]->x;
		yArr[i] = points[i]->y;
	}
	float a = (cov(xArr, yArr, size) / (var(xArr, size)));
	float xAvg = avg(xArr, size);
	float yAvg = avg(yArr, size);
	float b = (yAvg - a * xAvg);
	return Line(a,b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
	Line line = linear_reg(points, size);
	float yline = line.f(p.x);
	return (fabs(yline - p.y));
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
	float yline = l.f(p.x);
	return (fabs(yline - p.y));
}