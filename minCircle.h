/*
 * minCircle.h
 *
 * Author: 207233222 Yeheli Frangi
 */

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <math.h>
#include "anomaly_detection_util.h"

using namespace std;

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};

/*
 * finds the min circle from two points.
 */
Circle twoPoint(Point p1, Point p2);

/*
 * checks if given point is inside circle.
 */
bool isIn(Circle c, Point p);

/**
 * The function creates a circle with three given points on its egde.
 * returns the circle.
 */
Circle threePointsC(Point p1, Point p2, Point p3);


/**
 * This function is based of welzl's algorithm.
 * We will randomly choose (the random choosing will be the order of points in the vector) a point.
 * We will calculate the min circle of all the points in <vector> points without are random point.
 * If the returned circle includes the random point - we will return the circle,
 * if not - our random point is on the edge of the circle -
 * we wil return the min circle from the <vector> points without the random point and with the point on its edge.
 */
Circle minCircle(vector<Point> points, vector<Point> R);

/**
 * The function will move the given Pointer array to a vector and then call the recursive minCircle function.
 */
Circle findMinCircle(Point** points,size_t size);


#endif /* MINCIRCLE_H_ */
