/*
 * CLI.cpp
 *
 * Author: 207233222 Yeheli Frangi
 */
#include "CLI.h"
//constructor
CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
}

void CLI::start(){
    allData all_data;
    // creat all the classes of options, and upload to the vector of commands.
    UploadTimeSeries upload_time_series = UploadTimeSeries(this->dio,&all_data);
    commands.push_back(&upload_time_series);
    AlgorithmSttings algorithm_sttings = AlgorithmSttings(this->dio,&all_data);
    commands.push_back(&algorithm_sttings); 
    DetectAnomalies detect_anomalies = DetectAnomalies(this->dio,&all_data);
    commands.push_back(&detect_anomalies); 
    DisplayResult display_result = DisplayResult(this->dio,&all_data);
    commands.push_back(&display_result); 
    UploadAnomaliesAnalyzeResult upload_anomalies_analyze_result = UploadAnomaliesAnalyzeResult(this->dio,&all_data);
    commands.push_back(&upload_anomalies_analyze_result); 
    Exit exit = Exit(this->dio,&all_data);
    commands.push_back(&exit); 
    string openning = "Welcome to the Anomaly Detection Server.\nPlease choose an option:\n";
    int size = commands.size();
    int option=0;
    // print the menu untiol we exit.
    while (option != 6){
        dio->write(openning);    
        for (int i=0; i<size;i++) {
            commands.at(i)->print();
        }

        option = stoi(dio->read());
        // do the option.
        commands.at(option-1)->execute();
    }
}


CLI::~CLI() {
    
}

