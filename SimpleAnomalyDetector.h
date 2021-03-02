/*
 * SimplyAnomalyDetector.h
 *
 * Author: 207233222 Yeheli Frangi
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

/**
 * struct with the correlated Features and all the information about them -
 * the two featurs, the corallation , the threshold and the regression line. 
 * **/
struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
	float centerX;
	float centerY;
	float radius;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
	vector<correlatedFeatures> cf;
	float changingThreshold;
public:
	//This function is the constructor of the class.
	SimpleAnomalyDetector();
	//This function is the distructor of the class.
	virtual ~SimpleAnomalyDetector();
	//this function learn all the information from the Time Series, if there is a corralation, she update the struct.
	virtual void learnNormal(const TimeSeries& ts);
	//This function return a vector with all the points that have an exception according to the struct we learned.
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	float Getthreshold ();
	void Setthreshold (float newThreshold);

	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}
	
	/**this function creat the struct with the corralation and update all the information -
	 * the two featurs, the corallation , the threshold and the regression line.
	**/
	correlatedFeatures creatStructCorrelatedFeatures(string feature1,string feature2,float corrlation,Line lin_reg,float threshold);
	//check if there is an anomalies.
	virtual int isAnomaly(Point p,correlatedFeatures oneStruct);
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
