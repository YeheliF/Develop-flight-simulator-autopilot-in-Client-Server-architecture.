/*
 * HybridAnomalyDetector.cpp
 *
 * Author: 207233222 Yeheli Frangi
 */
#include "HybridAnomalyDetector.h"
//the constructor of the class.
HybridAnomalyDetector::HybridAnomalyDetector() {
	
}
//the disconstructor of the class.
HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}
/**
 * this function checks if we need to include the center and the radius of the circle -
 * in case that the corralation is smaller than 0.9. 
 * */
void HybridAnomalyDetector::learnNormal(const TimeSeries& ts){
	SimpleAnomalyDetector::learnNormal(ts);
	int sizeAll = cf.size();
	// go over all the corollations
	for(int j =0;j<sizeAll;++j){
		//if the corrlation is smaller than 0.9 - we need to chack with the minimum circle.
		if(cf.at(j).corrlation < changingThreshold){
			vector<float> feature1Vec = ts.GetFeatureVector(cf.at(j).feature1);
			vector<float> feature2Vec = ts.GetFeatureVector(cf.at(j).feature2);
			int size = feature1Vec.size();
			// creat all the points from the two featurs
			Point** points = new Point*[size];
			for(int i=0;i<=size;i++){
				points[i] = new Point(feature1Vec[i],feature2Vec[i]);
			}
			Circle c = findMinCircle(points,size);
			// update the struct
			cf.at(j).centerX = c.center.x;
			cf.at(j).centerY = c.center.y;
			cf.at(j).radius = 1.1*c.radius;
			//delete all the points from the array.
			for(int i=0;i<=size;i++){
				delete points[i];
			}
			//delete the hole array.
			delete[] points;
		}
	}
}
// this function checks if there is an anomaly in the circle
int HybridAnomalyDetector::isAnomaly(Point p,correlatedFeatures oneStruct){
	if(oneStruct.corrlation<changingThreshold){
		//check if the point is in the min circle we created.
		if(isIn(Circle(Point(oneStruct.centerX,oneStruct.centerY),oneStruct.radius),p)){
			return 0;
		}
		return 1;
	}
	//if the corralation is bigger than 0.9, we need to do the regular check.
	return SimpleAnomalyDetector::isAnomaly(p,oneStruct);
}
