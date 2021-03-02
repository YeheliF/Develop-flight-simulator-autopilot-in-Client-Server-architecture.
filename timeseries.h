/*
 * timeseries.h
 *
 * Author: 207233222 Yeheli Frangi
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <array>
#include <map>
using namespace std;

class TimeSeries{

public:
	//constructor - creat TimeSeries.
	TimeSeries(const char* CSVfileName);
	//This function is the distructor of the class.
	~TimeSeries();
	//copy constructor.
	TimeSeries(const TimeSeries& ts);
	//creat all the keys of the map from the first line of the file.
	vector<string> CreatKeys(const char* CSVfileName);
	// an access to the map- all the features and thier vectors.
	map <string, vector<float>> getMap() const;
	// an access to the keys- all the features.
	vector<string> GetKeys() const;
	vector<float> GetFeatureVector(string key) const;
	int NumOfFeature();
private:
	//the map- all the features and thier vectors.
	map <string, vector<float>> data;
	//the vector of keys - all the keys of the map.
	vector<string> keys;
};





#endif /* TIMESERIES_H_ */
