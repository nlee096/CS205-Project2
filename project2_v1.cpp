
#include <iostream>
#include <set>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>
#include <chrono>
// #include <stdlib.h> // for rand
// #include <time.h>  // for seed

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

double cross_validation(set<int> &feature_set){
    int nn;
    double distance;
    double nnDist;
    int correct = 0;

    if(feature_set.empty()){
        // below is for the case that we do not know the number of classifications 
        vector <int> common;
        vector <int> occurance;
        common.push_back(data[0][0]);
        occurance.push_back(1);
        int label;
        bool newLabel;
        for(int i = 1; i < num_instances; i++){
            label = data[i][0];
            newLabel = true;
            for(int j = 0; j < common.size(); j++){
                if(common.at(j) == label){
                    occurance.at(j) = occurance.at(j) + 1;
                    newLabel = false;
                    break;
                }
            }
            if(newLabel){
                common.push_back(label);
                occurance.push_back(1);
            }
        }
        int max = 0;
        int index = -1;
        for(int i = 0; i < occurance.size(); i++){
            if(occurance.at(i) > max){
                max = occurance.at(i);
                index = i;
            }
        }
        
        // code below assumes only 2 classifications (1, 2)
        // int one = 0;
        // int two = 0;
        // int label;
        // for(int i = 0; i < num_instances; i++){
        //     label = data[i][0];
        //     if(label == 1){
        //         one++;
        //     }
        //     else{
        //         two++;
        //     }
        // }
        // int max = (one >= two)? one : two;
        return (double)max/(double)num_instances;
    }   

    double label;
    double temp;
    double diff;
    set<int>::iterator it;
    int index;
    int k;

    for(int i = 0; i < num_instances; i++){
        label = data[i][0];
        nn = -1;
        distance = 0;
        nnDist = numeric_limits<double>::max();
        
        for(k = 0; k < num_instances; k++){
            if(k == i){continue;}

            // Distance does not need to be sqrt to compare
            // the smallest distance will also have the smallest distance^2
            // less calculations = faster time
            distance = 0;
            for(it = feature_set.begin(); it!=feature_set.end(); ++it){
                index = *it;
                diff = (data[i][index] - data[k][index]);
                distance += (diff * diff);
            }
            distance = sqrt(distance);
            if(distance < nnDist){
                nn = k;
                nnDist = distance;
            }
        }
        if(label == data[nn][0]){
            correct++;
        }
    }
    return (double)correct/num_instances;
}

/*
double cross_validation(){
    double random = (double) rand() / RAND_MAX;
    return random * 100;
}
*/

void forward_search(/*no input needed for now*/){
    auto ss = high_resolution_clock::now();
    set<int> best_set;
    double best_acc = cross_validation(best_set);
    set<int> current_set;
    
    set<int>::iterator it;
    set<int>::iterator temp;
    
    // cout << "Using no features I get an accuracy of " << (best_acc*100) << "%"<< endl;
    cout << "Running nearest neighbor with no features, using \"Leave-one-out\" evaluation, I get an accuracy of " << (best_acc*100) << "%"<< endl;
    cout << "Beginning search" << endl << endl;
    
    double last_acc = best_acc;
    
    for(int i = 1; i <= num_features; i++){
        // auto step = high_resolution_clock::now();
        // cout << "on the " << i << " level of the search tree." << endl;
        int add_feature = 0;
        double curr_best = 0;
        
        for(int j = 1; j <= num_features; j++){
            if(current_set.find(j) != current_set.end()){
                //cout << "-> "<< j << " is already in the set" << endl;
                continue;
            }
            
            //double accuracy = cross_validation();
            //cout << "--considering adding the " << j << " feature with accuracy " << accuracy << "%" << endl;
            cout << "Using feature(s) {";
            set<int> temp_set = current_set;
            temp_set.insert(j);
            for (it = temp_set.begin(); it!=temp_set.end();){
                cout << *it;
                temp = ++it;
                if(temp != temp_set.end()){
                    cout << ", ";
                }
            }
            cout << "} accuracy is ";

            //auto start = high_resolution_clock::now();
            double accuracy = cross_validation(temp_set);
            //auto stop = high_resolution_clock::now();
            //auto duration = duration_cast<microseconds>(stop - start);

            //cout << accuracy << "%. Time : " << duration.count() << " microseconds" << endl;
            cout << (accuracy*100) << "%" << endl;
            
            if(accuracy > curr_best){
                curr_best = accuracy;
                add_feature=j;
            }
        }
        // auto halt = high_resolution_clock::now();
        // auto time = duration_cast<microseconds>(halt - step);
        // cout << time.count() << " microseconds" << endl;
        
        current_set.insert(add_feature);
        //cout << "On level " << i << " feature " << add_feature <<  " was added to current set" << endl << endl;
        cout << "Feature set {";
            for (it = current_set.begin(); it!=current_set.end();){
                cout << *it;
                temp = ++it;
                if(temp != current_set.end()){
                    cout << ", ";
                }
            }
        cout << "} was best, accuracy is " << curr_best << "%" << endl;
        if(curr_best > best_acc){
            best_acc = curr_best;
            best_set = current_set;
        }
        else{
            if (last_acc > curr_best){
                cout << "(Warning, Accuracy has decreased from last set!)" << endl;
            }
            cout << "(Warning, accuracy is lower than the best set!)" << endl;
        }
        last_acc = curr_best;
        cout << endl;
    }
    cout << "Finished search!! The best feature subset is {";
    for (it = best_set.begin(); it!=best_set.end();){
        cout << *it;
        temp = ++it;
        if(temp != best_set.end()){
            cout << ", ";
        }
    }
    cout << "}, which has an accuracy of " << (best_acc*100) << "%" << endl;
    auto se = high_resolution_clock::now();
    auto total = duration_cast<microseconds>(se - ss);
    cout << "total time : " << (double(total.count()) / 1000000) << " seconds" << endl;
}

void back_elim(/*no input needed for now*/){
    auto ss = high_resolution_clock::now();
    set<int> best_set;
    for(int i = 1; i <= num_features; i++){
        best_set.insert(i);
    }
    
    set<int> current_set = best_set;

    auto aft_start = high_resolution_clock::now();
    double best_acc = cross_validation(current_set);
    auto aft_stop = high_resolution_clock::now();
    auto aft_time = duration_cast<microseconds>(aft_stop - aft_start);
    
    set<int>::iterator it;
    set<int>::iterator temp;
    
    // cout << "Using all features I get an accuracy of " << (best_acc*100) << "%"<< endl;
    cout << "Running nearest neighbor with all "<< num_features << " features, using \"Leave-one-out\" evaluation, I get an accuracy of " << (best_acc*100) << "%"<< endl;
    cout << "time : " << (double(aft_time.count()) / 1000000) << " seconds" << endl;
    cout << "Beginning search" << endl << endl;
    
    double last_acc = best_acc;

    for(int i = 1; i <= num_features; i++){
        //auto step = high_resolution_clock::now();
        //cout << "on the " << i << " level of the search tree." << endl;
        int add_feature = 0;
        double curr_best = 0;
        
        for(int j = 1; j <= num_features; j++){
            if(current_set.find(j) == current_set.end()){
                //cout << "-> "<< j << " is not in the set" << endl;
                continue;
            }
            
            //double accuracy = cross_validation();
            //cout << "--considering removing the " << j << " feature with accuracy " << accuracy << "%" << endl;
            
            cout << "Using feature(s) {";
            set<int> temp_set = current_set;
            temp_set.erase(j);
            for (it = temp_set.begin(); it!=temp_set.end();){
                cout << *it;
                temp = ++it;
                if(temp != temp_set.end()){
                    cout << ", ";
                }
            }
            cout << "} accuracy is ";

            //auto start = high_resolution_clock::now();
            double accuracy = cross_validation(temp_set);
            //auto stop = high_resolution_clock::now();
            //auto duration = duration_cast<microseconds>(stop - start);
            
            //cout << accuracy << "%. Time : " << duration.count() << " microseconds" << endl;
            cout << (accuracy*100) << "%" << endl;
            
            if(accuracy > curr_best){
                curr_best = accuracy;
                add_feature=j;
            }
        }
        //auto halt = high_resolution_clock::now();
        //auto time = duration_cast<microseconds>(halt - step);
        //cout << time.count() << " microseconds" << endl;
        
        current_set.erase(add_feature);
        //cout << "On level " << i << " feature " << add_feature <<  " was removed from current set" << endl << endl;
        cout << "Feature set {";
            for (it = current_set.begin(); it!=current_set.end();){
                cout << *it;
                temp = ++it;
                if(temp != current_set.end()){
                    cout << ", ";
                }
            }
        cout << "} was best, accuracy is " << curr_best << "%" << endl;
            
        if(curr_best >= best_acc){
            best_acc = curr_best;
            best_set = current_set;
        }
        else{
            if (last_acc > curr_best){
                cout << "(Warning, Accuracy has decreased from last set!)" << endl;
            }
            cout << "(Warning, accuracy is lower than the best set!)" << endl;
        }
        last_acc = curr_best;
        cout << endl;
    }
    cout << "Finished search!! The best feature subset is {";
    for (it = best_set.begin(); it!=best_set.end();){
        cout << *it;
        temp = ++it;
        if(temp != best_set.end()){
            cout << ", ";
        }
    }
    cout << "}, which has an accuracy of " << (best_acc*100) << "%" << endl;
    auto se = high_resolution_clock::now();
    auto total = duration_cast<microseconds>(se - ss);
    cout << "total time : " << (double(total.count()) / 1000000) << " seconds" << endl;
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

    int fileOption;
    string fileName;
    cin >> fileOption;
    if(fileOption == 1){
        fileName = "CS170_small_Data__32.txt";
    }
    else if(fileOption ==2){
        fileName = "CS170_small_Data__33.txt";
    }
    else if(fileOption ==3){
        fileName = "CS170_large_Data__32.txt";
    }
    else if(fileOption ==4){
        fileName = "CS170_large_Data__33.txt";
    }
    else if(fileOption ==5){
        fileName = "CS170_small_Data__6.txt";
    }
    else if(fileOption ==6){
        fileName = "CS170_large_Data__4.txt";
    }
    else if(fileOption ==7){
        fileName = "CS170_XXXlarge_Data__10.txt";
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

    cout << "Type the number of the algorithm you want to run." << endl
        << "1 : Forward Selection" << endl
        << "2 : Backward Elimination" << endl;
    
    int option;
    cin >> option;
    srand(time(NULL));
    if(option == 1){
        forward_search();
    }
    else if(option == 2){
        back_elim();
    }
    else{
        cout << "invalid input" << endl;
        exit(1);
    }
    return 0;
}
