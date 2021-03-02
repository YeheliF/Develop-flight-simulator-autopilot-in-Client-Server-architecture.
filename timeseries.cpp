/*
 * timeseries.cpp
 *
 * Author: 207233222 Yeheli Frangi
 */
#include "timeseries.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <map> 

//constructor - creat TimeSeries.
TimeSeries::TimeSeries(const char* CSVfileName){
    ifstream theFile;
    theFile.open(CSVfileName);
    //keys is global variable - save all the features.
    keys = CreatKeys(CSVfileName);
    int size = keys.size();
    vector<float> vectorsArray[size];
    string value, line;
    getline(theFile,line);
    //creat a vector for each feature from the file.
     while (getline(theFile,line)){
        int j;
        stringstream eachLine(line);
        //in each line, divides the information between the vectors.
        for(j=0; j <size-1 ;j++){
            getline(eachLine, value, ',');
            vectorsArray[j].push_back(stof(value));
        }
        getline(eachLine, value, '\n');
        vectorsArray[size-1].push_back(stof(value));
    }
    // insert to the map the feature and his vector.
    for(int i=0;i<size;i++){
        data.insert(pair <string,vector<float>>(keys[i], vectorsArray[i]));   
    }
    theFile.close();
}

//This function is the distructor of the class.
TimeSeries::~TimeSeries() {
}

//copy constructor.
TimeSeries::TimeSeries(const TimeSeries& ts){
    data = ts.data;
}

// an access to the map- all the features and thier vectors.
map <string, vector<float>> TimeSeries:: getMap() const{
    return this->data;
}

// an access to the keys- all the features.
vector<string> TimeSeries::GetKeys() const{
    return this->keys;
}

//From the first line in the file, creat all the features - keys for the map.
vector<string> TimeSeries:: CreatKeys(const char* CSVfileName){
    ifstream theFile;
    string line1;
	theFile.open(CSVfileName);
	if (theFile.is_open()){
		getline(theFile , line1);
    }
    stringstream firstLine(line1);
    string key;
    vector<string> keys;
    int counter=0;
    //fo over the first line of the file, and creat the keys.
    while(firstLine.good()){
        counter++;
        getline(firstLine,key,',');
        keys.push_back(key);
    }
    theFile.close();
    return keys;
}
vector<float> TimeSeries:: GetFeatureVector(string key) const{
    vector<float> featureVector= data.find(key)->second;
    return featureVector;
}
int TimeSeries::NumOfFeature(){
    return data.begin()->second.size();
}
