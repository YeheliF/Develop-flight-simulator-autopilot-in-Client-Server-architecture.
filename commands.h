/*
 * commands.h
 *
 * Author: 207233222 Yeheli Frangi
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	
};

//struct that hold all the data to all the commands together.
struct allData{
	TimeSeries* learnNormal;
	TimeSeries* detect;
	HybridAnomalyDetector* oneReport = new HybridAnomalyDetector();
	vector<AnomalyReport> allReports;
};	

// the father of all the commands.
class Command{	
protected:
	DefaultIO* dio;
	string description;
	allData* all_data;	
public:
	Command(DefaultIO* dio,allData* data):dio(dio),all_data(data){}
	virtual void execute()=0;
	virtual ~Command(){}
	virtual void print(){
		dio->write(description);
	}
};

// option1
class UploadTimeSeries : public Command{
	public:
	UploadTimeSeries(DefaultIO* dio,allData* data): Command(dio,data){	
		this->description = "1.upload a time series csv file\n";
	}
	//upload files.
	void execute(){
		string opening1 = "Please upload your local train CSV file.\n";
		string opening2 = "Please upload your local test CSV file.\n";
		string closing = "Upload complete.\n";
		dio->write(opening1);
		ofstream anomalyTrain;
		//creat train file.
		anomalyTrain.open("anomalyTrain.csv");
		string input1;
		input1 = dio->read();
		while(input1 != "done"){
			anomalyTrain << input1 << endl;
			input1 = dio->read();
		}
		anomalyTrain.close();
		dio->write(closing);
		all_data->learnNormal =new TimeSeries("anomalyTrain.csv");
		ofstream anomalyTest;
		//creat test file.
		anomalyTest.open("anomalyTest.csv");
		string input2;
		dio->write(opening2);		
		input2 = dio->read();
		while(input2 != "done"){
			anomalyTest << input2 << endl;
			input2 = dio->read();
		}
		dio->write(closing);
		all_data->detect =new TimeSeries("anomalyTest.csv");
	}
	
};
//option 2
class AlgorithmSttings:public Command{
	public:
	AlgorithmSttings(DefaultIO* dio,allData* data): Command(dio,data){
		description = "2.algorithm settings\n";
	}
	// changing the algorithem -- update the threshold.
	void execute(){
		float currentThreshold =all_data->oneReport->Getthreshold();
		string opening1 = "The current correlation threshold is ";
		dio->write(opening1);
		dio->write(currentThreshold);
		dio->write("\n");
		string opening3 = "Type a new threshold\n";
		dio->write(opening3);

		float changingThreshold;
		changingThreshold = stof(dio->read());
		//if the input is invalid.
		while(changingThreshold < 0 || changingThreshold >1){
			string problem = "please choose a value between 0 and 1.\n";
			dio->write(problem);
			changingThreshold = stof(dio->read());
		}
		all_data->oneReport->Setthreshold(changingThreshold);
	}
};
//option 3
class DetectAnomalies:public Command{
	public:
	DetectAnomalies(DefaultIO* dio,allData* data): Command(dio,data){
		description = "3.detect anomalies\n";
	}
	// get the train and test of timeseries.
	void execute(){
		all_data->oneReport->learnNormal(*(all_data->learnNormal));
		all_data->allReports = all_data->oneReport->detect(*all_data->detect);
		string ending ="anomaly detection complete.\n";
		dio->write(ending);
	}
};
// option 4
class DisplayResult:public Command{
	public:
	DisplayResult(DefaultIO* dio,allData* data): Command(dio,data){
		description = "4.display results\n";
	}
	// print all the anomaly reports.
	void execute(){
		int size = all_data->allReports.size();
		for(int i=0;i<size;i++){
			dio->write(all_data->allReports.at(i).timeStep);
			dio->write("	 " + all_data->allReports.at(i).description + "\n");
		}
		dio->write("Done.\n");
	}
};
// option 5
class UploadAnomaliesAnalyzeResult:public Command{
	public:
	UploadAnomaliesAnalyzeResult(DefaultIO* dio,allData* data): Command(dio,data){
		description = "5.upload anomalies and analyze results\n";
	}
	// get input of execptions from the user.
	vector<pair<int,int>> GetExceptions(){
		dio->write("Please upload your local anomalies file.\n");
		string input = dio->read();
		string middle = ",";
		vector<pair<int,int>> myExceptions;
		while(input != "done"){
			int first = stoi(input.substr(0, input.find(middle)));
			int second = stoi(input.substr(input.find(middle)+1));
			myExceptions.push_back(make_pair(first,second));
			input = dio->read();
		}
		dio->write("Upload complete.\n");
		return myExceptions;
	}
// the range of all the exception we got from the user.
	int RangeOfExceptions(vector<pair<int,int>> exceptions ){
		int different=0;
		int sum=0;
		for(pair<int,int> one_pair : exceptions ){
			different = one_pair.second - one_pair.first + 1;
			sum += different;
		}
		return sum;
	}
	// update TP,FP,TN,FN with loops on the reports and the exceptions.
	void ExceptionVSreports(vector<pair<int,int>> reports, vector<pair<int,int>> exceptions , int n, int p, int N){
		float TP=0,FP=0,FN =0,TN =0,sumOfLines =0;
		bool flag = 0;
		vector<int> flagsOfExceptions(exceptions.size(),0);	
		// loop on all the reports	
		for(pair<int,int> one_report:reports){
			// the time of the report.
			int start = one_report.first;
			int end = one_report.second;
			// loop on all the exceptions
			for(int i=0;i<exceptions.size();i++){
				// ([)] , ([]).
				if(exceptions[i].first <= one_report.first){
					if(exceptions[i].second >= one_report.first){
						TP++;
						flag = 1;
						flagsOfExceptions[i] = 1;
						// update the time
						start = min(start,exceptions[i].first);
						end = max(end,exceptions[i].second);
					}
				}
				// [(]), [()]
				else{
					if(exceptions[i].first <= one_report.second){
						TP++;
						flag = 1;
						flagsOfExceptions[i] = 1;
						// update the time
						start = min(start,exceptions[i].first);
						end = max(end,exceptions[i].second);
					}
				}	
			}
			// if all exceptions didnt touch the report. 
			if(flag == 0){
				FP++;
			}
			flag = 0;
			sumOfLines += (end - start +1);
		}
		// go over all the exceptions that didnt touch a report.
		for(int j=0;j<flagsOfExceptions.size();j++){
			if(flagsOfExceptions[j] == 0){
				FN++;
				sumOfLines += (exceptions[j].second - exceptions[j].first + 1);
			}
		}
		TN = n - sumOfLines;

		int a = TP/p*1000;
		float truePositiveRate = ((float) a/1000);
		int b = FP/N*1000;
		float falseAlarmRate = ((float) b/1000);
	
		dio->write("True Positive Rate: ");
		dio->write(truePositiveRate);
		dio->write("\n");
		dio->write("False Positive Rate: ");
		dio->write(falseAlarmRate);
		dio->write("\n");
	}
	//analize the result between reports and exceptions.
	void execute(){
		vector<pair<int,int>> myReports;
		int firstTS = all_data->allReports.at(0).timeStep;
		int secondTS = firstTS;
		// organize all the reports i have.
		for(int i=0;i<all_data->allReports.size()-1;i++){
			string firstD = all_data->allReports.at(i).description;
			string secondD = all_data->allReports.at(i+1).description;
			int temp = all_data->allReports.at(i+1).timeStep;
			if(firstD == secondD && secondTS + 1 == temp){
				secondTS = temp;
			}
			else{
				myReports.push_back(make_pair(firstTS,secondTS));
				firstTS = all_data->allReports.at(i+1).timeStep;
				secondTS = all_data->allReports.at(i+1).timeStep;
			}
		}
		myReports.push_back(make_pair(firstTS,secondTS));
		sort(myReports.begin(),myReports.end());
		// get the exceptions.
		vector<pair<int,int>> exceptions = GetExceptions();
		sort(exceptions.begin(),exceptions.end());
		int p = exceptions.size();
		int n = all_data->learnNormal->NumOfFeature();
		int N = n - RangeOfExceptions(exceptions);
		int b = 0;
		ExceptionVSreports(myReports,exceptions,n,p,N);
	}
};
// option 6
class Exit:public Command{
	public:
	Exit(DefaultIO* dio,allData* data): Command(dio,data){
		description = "6.exit\n";
	}
	//delete all the data.
	void execute(){
		delete all_data->oneReport;
		delete all_data->learnNormal;
		delete all_data->detect;
	}
};


#endif /* COMMANDS_H_ */
