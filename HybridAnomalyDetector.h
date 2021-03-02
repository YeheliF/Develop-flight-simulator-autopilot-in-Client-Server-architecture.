/*
 * HybridAnomalyDetector.h
 *
 * Author: 207233222 Yeheli Frangi
 */

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	//constructor
	HybridAnomalyDetector();
	//disconstructor
	virtual ~HybridAnomalyDetector();
	// function that update the values of the circle.
	virtual void learnNormal(const TimeSeries& ts);
	// function that checks if there is an anomaly in the circle.
	virtual int isAnomaly(Point p,correlatedFeatures oneStruct);

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
