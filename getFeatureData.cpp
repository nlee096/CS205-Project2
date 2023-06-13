
#include <iostream>
#include <set>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string.h>
#include <cmath>
#include <limits>
#include <chrono>
#include <stdlib.h> // for rand
#include <time.h>  // for seed

using namespace std;
using namespace std::chrono;

int num_features;
int num_instances;
const int MAX_INSTANCES = 4000;
const int MAX_FEATURES = 80;
double data[MAX_INSTANCES][MAX_FEATURES+1];

void normalize(){
	double sum;
	double variance;
	double mean[num_features];
	double std[num_features];
	cout << "Normalizing ... " << endl;
	for(int i = 1; i < num_features+1; i++){
		sum = 0;
		for(int j = 0; j < num_instances; j++){
			sum += data[j][i];
		}
		mean[i-1] = sum / (double)num_instances;
	}

	double tempMean;
	for(int i = 1; i < num_features+1; i++){
		variance = 0;
		tempMean = mean[i-1];
		for(int j = 0; j < num_instances; j++){
			variance += pow(data[j][i] - tempMean, 2);
		}
		std[i-1] = sqrt(variance/ (double)(num_instances-1));
	}

	for(int i = 0; i < num_instances; i++){
		for(int j = 1; j < num_features+1; j++){
			data[i][j] = (data[i][j] - mean[j-1]) / std[j-1];
		}
	}
	cout << "Done" << endl;
}

int main()
{
    cout << "Welcome to Nathan Lee's and Sahas Poyekar's Feature Selection Algorithm." << endl;
    cout << "Select which data set:" << endl;
    cout << "1. Test small dataset 32" << endl;
    cout << "2. Test small dataset 33" << endl;
    cout << "3. Test large dataset 32" << endl;
    cout << "4. Test large dataset 33" << endl;
    cout << "5. CS170_small_Data__6.txt" << endl;
    cout << "6. CS170_large_Data__4.txt" << endl;
    cout << "7. CS170_XXXlarge_Data__10.txt" << endl;
    cout << "8. Type in your own" << endl;
    // cout << "9. real world dataset" << endl;

    int max;
    int fileOption;
    string fileName;
    cin >> fileOption;
    if(fileOption == 1){
    	fileName = "CS170_small_Data__32.txt";
        max = 10;
    }
    else if(fileOption ==2){
    	fileName = "CS170_small_Data__33.txt";
        max = 10;
    }
    else if(fileOption ==3){
    	fileName = "CS170_large_Data__32.txt";
        max = 20;
    }
    else if(fileOption ==4){
    	fileName = "CS170_large_Data__33.txt";
        max = 20;
    }
    else if(fileOption ==5){
        fileName = "CS170_small_Data__6.txt";
        max = 10;
    }
    else if(fileOption ==6){
        fileName = "CS170_large_Data__4.txt";
        max = 20;
    }
    else if(fileOption ==7){
        fileName = "CS170_XXXlarge_Data__10.txt";
        max = 80;
    }
    else if(fileOption ==8){
        cin >> fileName;
    }
    // else if(fileOption ==8){
    //     fileName = "data.txt";
    // }
    else{
    	cout << "invlid file input" << endl;
    	exit(1);
    }
    ifstream file;
    file.open(fileName.c_str());
    if(!file){
    	cout << "file failed to open" << endl;
    	exit(1);
    }
    string instance;
    int instance_num = 0;
    int feature_num = 0;
    double value;
    if(file.is_open()){
    	// cout << "Reading ..." << endl;
    	while(!file.eof()){
    	    feature_num = 0;
            getline(file, instance);
            stringstream ss;
            ss.str(instance);
            ss.precision(8); 
            while(ss >> value){
                data[instance_num][feature_num] = value;
			    feature_num++;
            }
            if(instance_num == 0){
            	num_features = feature_num;
            }
            instance_num++;
    	}
    }

    num_instances = instance_num - 1;
    num_features--;
    // cout << "Done" << endl;
    // cout << num_instances << " instances" << endl;
    // cout << num_features << " features" << endl;
    cout << "This dataset has " << num_features << " features (not including the class attribute), with " << num_instances << " instance." << endl;

    cout << "Do you want to normalize? (0 = no, 1 = yes)" << endl;
    int norm;
    cin >> norm;
    if(norm == 1){
    	normalize();
    }
    else if(norm != 0){
    	cout << "invalid input" << endl;
    	exit(1);
    }

    cout << "Type the 2 feature numbers (separated by space)." << endl;
    int feat1;
    int feat2;
    cin >> feat1;
    cin >> feat2;
    while(feat1 > max || feat2 > max){
        cout << "max feature number is " << max << endl;
        cout << "Enter 2 new numbers" << endl;
        cin >> feat1;
        cin >> feat2;
    }

    ofstream MyFile("data.txt");
    for(int i = 0; i < num_instances; i++){
        string line = "";
        line = line + to_string(data[i][0]) + " ";
        line = line + to_string(data[i][feat1]) + " "; 
        line = line + to_string(data[i][feat2]); 
        MyFile << line << endl;
    }

    MyFile.close();

    
    return 0;
}
