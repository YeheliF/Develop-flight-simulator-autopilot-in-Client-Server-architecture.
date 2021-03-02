/*
 * minCircle.cpp
 *
 * Author: 207233222 Yeheli Frangi
 */

#include "minCircle.h"

using namespace std;




/*
 * finds the min circle from two points.
 */
Circle twoPoint(Point p1, Point p2) {
    float x = (p1.x + p2.x) / 2;
    float y = (p1.y + p2.y) / 2;
    float r = sqrt(powf((p1.x - p2.x), 2) + powf((p1.y - p2.y), 2) ) / 2;
    return Circle(Point(x, y), r);
}

/*
 * checks if given point is inside circle.
 */
bool isIn(Circle c, Point p) {
    float distance = powf((c.center.x - p.x), 2) + powf((c.center.y - p.y), 2);
    if (distance > (c.radius * c.radius)) {
        return false;
    }
    return true;
}
/**
 * return the distance between 2 points
 * */
float dist(Point a, Point b){
	float x2=(a.x-b.x)*(a.x-b.x);
	float y2=(a.y-b.y)*(a.y-b.y);
	return sqrt(x2+y2);
}
/**
 * The function creates a circle with three given points on its egde.
 * returns the circle.
 * ***this function is taken from Eli's class of minCircle.***
 */
Circle threePointsC(Point a, Point b, Point c) {
	// find the circumcenter of the triangle a,b,c	
	Point mAB((a.x+b.x)/2 , (a.y+b.y)/2); // mid point of line AB
	float slopAB = (b.y - a.y) / (b.x - a.x); // the slop of AB
	float pSlopAB = - 1/slopAB; // the perpendicular slop of AB
	// pSlop equation is:
	// y - mAB.y = pSlopAB * (x - mAB.x) ==> y = pSlopAB * (x - mAB.x) + mAB.y
	
	Point mBC((b.x+c.x)/2 , (b.y+c.y)/2); // mid point of line BC
	float slopBC = (c.y - b.y) / (c.x - b.x); // the slop of BC
	float pSlopBC = - 1/slopBC; // the perpendicular slop of BC
	// pSlop equation is:
	// y - mBC.y = pSlopBC * (x - mBC.x) ==> y = pSlopBC * (x - mBC.x) + mBC.y
	
	/*
	pSlopAB * (x - mAB.x) + mAB.y = pSlopBC * (x - mBC.x) + mBC.y
	pSlopAB*x - pSlopAB*mAB.x + mAB.y = pSlopBC*x - pSlopBC*mBC.x + mBC.y
	
	x*(pSlopAB - pSlopBC) = - pSlopBC*mBC.x + mBC.y + pSlopAB*mAB.x - mAB.y
	x = (- pSlopBC*mBC.x + mBC.y + pSlopAB*mAB.x - mAB.y) / (pSlopAB - pSlopBC);
	
	*/
	
	float x = (- pSlopBC*mBC.x + mBC.y + pSlopAB*mAB.x - mAB.y) / (pSlopAB - pSlopBC);
	float y = pSlopAB * (x - mAB.x) + mAB.y;
	Point center(x,y);
	float R=dist(center,a);
	
	return Circle(center,R);
}

/**
 * The function creates a trivial circle : 0 <= num of points for circle <= 3 .
 * returns the circle.
 */
Circle trivial(vector<Point> R) {
    
    //defualt cirlce 
    if (R.size() == 0) {
        return Circle(Point(0, 0), 0);
    }
    
    //if there is only one point for the circle
    if (R.size() == 1) {
        return Circle(R[0], 0);
    }
    
    // two points - the circle between them 
    if (R.size() == 2) {
        return twoPoint(R[0], R[1]);
    }
    
    //three points - checks for each circle of 2 points if the third is inside - yes - return minmum - no - create circle from 3 points.
    if (R.size() == 3) {
        Circle c1 = twoPoint(R[0], R[1]);
        Circle minC(Point(0, 0), INFINITY);
        if (isIn(c1, R[2])) {
            minC = c1.radius < minC.radius ? c1 : minC;
        }
        Circle c2 = twoPoint(R[0], R[2]);
        if (isIn(c2, R[1])) {
            minC = c2.radius < minC.radius ? c2 : minC;
        }
        Circle c3 = twoPoint(R[1], R[2]);
        if (isIn(c3, R[0])) {
            minC = c3.radius < minC.radius ? c3 : minC;
        }
        if (minC.radius != INFINITY) {
            return minC;
        }
        return threePointsC(R[0], R[1], R[2]);
    }
    return Circle(Point(0, 0), 0);
}


/**
 * This function is based of welzl's algorithm.
 * We will randomly choose (the random choosing will be the order of points in the vector) a point.
 * We will calculate the min circle of all the points in <vector> points without are random point.
 * If the returned circle includes the random point - we will return the circle,
 * if not - our random point is on the edge of the circle -
 * we wil return the min circle from the <vector> points without the random point and with the point on its edge.
 */
Circle minCircle(vector<Point> points, vector<Point> R) {

    //we can calculate the min circle in a trivial way
    if (points.size() == 0 || R.size() == 3) {
        return trivial(R);
    }

    //calculate the minCircle of the vector without the first point in vector and current R vector.
    Circle min = minCircle({points.cbegin() + 1, points.cend()}, R);

    //we will check if first point in vector is in the min circle we received.
    if (isIn(min, points.at(0))) {
        return min;
    }

    //point is not in the circle - is on edge - add to R vector and re-find min circle.
    R.push_back(points.at(0));
    return minCircle({points.cbegin() + 1, points.cend()}, R);
}

/**
 * The function will move the given Pointer array to a vector and then call the recursive minCircle function.
 */
Circle findMinCircle(Point** points,size_t size) {
    vector<Point> p;
    vector<Point> R;
    for (int i = 0; i < size; i++) {
        p.push_back(*points[i]);
    }
    return minCircle(p, R);
}

