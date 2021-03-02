/*
 * SimplyAnomalyDetector.cpp
 *
 * Author: 207233222 Yeheli Frangi
 */
#include "SimpleAnomalyDetector.h"

//This function is the constructor of the class.
SimpleAnomalyDetector::SimpleAnomalyDetector() {
	this->changingThreshold = 0.9;
}
//This function is the distructor of the class.
SimpleAnomalyDetector::~SimpleAnomalyDetector() {
}

/**This function learn all the information she gets from the Time Series-
 * it takes each two featurs and return if they have the buggest corralation.
 * If they are - the function creat theeir max corralation, max Threshold and linear regression.
 * the function creat a vector fill with each corralation struct with all the parametars.
 * **/

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
	vector<string> keys =  ts.GetKeys();
	map <string, vector<float>> data =ts.getMap();
	Line lin_reg;
	string feature2;
	//check corralation for each feature.
	for(int i=0;i<keys.size();i++){
		float maxThreshold=0;
		float maxCorrlation=0;
		string feature1 = data.find(keys.at(i))->first;
		//compare corallation of feature1 to all the rest with the person function.
		for(int j=i+1;j<keys.size();j++){
			float* x = &data.find(keys.at(i))->second[0];
			float* y = &data.find(keys.at(j))->second[0];
			float match = pearson(x,y,data.find(keys.at(i))->second.size());
			//if there is a match from pearson.
			if(abs(match) > 0.5 && abs(match) > maxCorrlation ){
				maxCorrlation = match;
				feature2 = data.find(keys.at(j))->first;
			}
		}
		//if there was a corralation.
		if(maxCorrlation){
			vector<float> featureOne = data.find(feature1)->second;
			vector<float> featureTwo = data.find(feature2)->second;
			int size = featureOne.size();
			//creat all the points from the two featurs.
			Point** points= new Point*[size];
			for(int i=0;i<=size;i++){
				points[i] = new Point(featureOne[i],featureTwo[i]);
			}
			lin_reg = linear_reg(points,size);
			//for each point check the devision.
			for(int i =0;i<size;i++){
				Point p = Point (points[i]->x,points[i]->y);
				float threshold = dev(p,lin_reg);
				if(threshold>maxThreshold){
					maxThreshold = threshold;
				}
			}
			//delete all the points from the array.
			for(int i=0;i<=size;i++){
				delete points[i];
			}
			//delete the hole array.
			delete[] points;
			//creat a struct of the correlated features we found, and push it in the vector of corallations.
			correlatedFeatures theStruct = creatStructCorrelatedFeatures(feature1,feature2,maxCorrlation,lin_reg,1.1*maxThreshold);
			cf.push_back(theStruct);
		}
	}
	
}
/**
 * This function creat a struct of correlatedFeatures, and push all the members we found-
 * the two featurs, the corallation , the threshold and the line.  
 * **/
correlatedFeatures SimpleAnomalyDetector::creatStructCorrelatedFeatures(string feature1,string feature2,float corrlation,Line lin_reg,float threshold){
	correlatedFeatures theStruct;
	theStruct.feature1 = feature1;
	theStruct.feature2 = feature2;
	theStruct.corrlation = corrlation;
	theStruct.lin_reg = lin_reg;
	theStruct.threshold = threshold;
	return theStruct;
}

/**
 * This function check if there is an exception in the new information according to 
 * the Time series and the struct we learn from them, and report it.
 * **/
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	vector<AnomalyReport> reports;
	int sumOfStructs = cf.size();
	//check for each corralation = for each stuck
	for(int i=0;i<sumOfStructs;i++){
		//colacte the information.
		string feature1 = cf.at(i).feature1;
		string feature2 = cf.at(i).feature2;
		Line linear_reg = cf.at(i).lin_reg;
		float maxThreshold = cf.at(i).threshold;
		map <string, vector<float>> data =ts.getMap();	
		vector<float> feature1Vec = data.find(feature1)->second;
		vector<float> feature2Vec = data.find(feature2)->second;
		int size = feature1Vec.size();
		//creat all the points from the two featurs.
		Point** points = new Point*[size];
		for(int i=0;i<=size;i++){
			points[i] = new Point(feature1Vec[i],feature2Vec[i]);
		}
		// for each point check if there is an exception or not.
		for(int j = 0;j<size;++j){
			Point p = Point (points[j]->x,points[j]->y);
			int solution = isAnomaly(p,cf.at(i));
			if( solution == 1){
				//creat an anomaly report, and push it in the vector of reports.
				AnomalyReport information = AnomalyReport(feature1 + "-" + feature2,j+1);
				reports.push_back(information);
			}
		}
		//delete all the points from the array.
		for(int i=0;i<=size;i++){
			delete points[i];
		}
		//delete the hole array.
		delete[] points;

	}
	return reports;
}
/**
 * if the corralation of the featers is more that 0.5, this function check with dev if there is an anomaly at the point
 * */
int SimpleAnomalyDetector::isAnomaly(Point p,correlatedFeatures oneStruct){
	//for each point check the devision- if it bigger then the threshold that in the stuck.
	float threshold = dev(p,oneStruct.lin_reg);
	if(threshold>oneStruct.threshold){
		return 1;
	}
	return 0;
}
float SimpleAnomalyDetector::Getthreshold (){
	return changingThreshold;
}
void SimpleAnomalyDetector::Setthreshold (float newThreshold){
	changingThreshold=newThreshold;
}