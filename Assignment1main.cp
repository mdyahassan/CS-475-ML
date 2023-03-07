#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

double finSum(vector<double> n) {
    double sum ;
    for (int i=0; i < n.size(); i++) {
        sum += n[i];
    }
    return sum;
}

double finMean(vector<double> n) {
    double size = n.size();
    double sum = finSum(n);
    double mean = sum / size;
    return mean;
}

double finMedian(vector<double> n) {
    int size = n.size();
    double median;
    sort(n.begin(), n.end());
    if (size % 2 == 0) {
        median = (n[size / 2 - 1] + n[size / 2]) / 2;
    }
    else {
        median = n[size / 2];
    }
    return median;
}

double finRange(vector<double> n) {
    double max = -1e10;
    double min = 1e10;

    double range;

    for (int i=0; i < n.size(); i++) {
        if (n[i] > max) {
            max = n[i];
        }

        if (n[i] < min) {
            min = n[i];
        }
    }
   return range = max - min;
   
}



int main() {
    // initialize two vectors for two columns 'rm' and 'medv', and filename
    ifstream inFS;
    string line;
    string rm_in, medv_in;
    const int MAX_LEN = 1000;
    vector<double> rm(MAX_LEN);
    vector<double> medv(MAX_LEN);
    string filename = "boston.csn";

   //Try to open file
    cout << "Opening file Boston.csn." << endl;

    inFS.open("Boston.csn");
    if (!inFS.is_open()) {
        cout << "Could not open file Boston.csn." << endl;
        return 1; //1 indicates error
    }

    // Can now use inFS stream like cin stream
    //Bostoncsn should contain two doubles

    cout << "Reading line 1" << endl;
    getline(inFS, line);

    //echo heading
    cout << "heading: " << line << endl;

    int numObsernations = 0;
    while (inFS.good()){

        getline(inFS, rm_in, ',');
        getline(inFS, medv_in, '\n');

        rm.at(numObsernations) = stof(rm_in);
        medv.at(numObsernations) = stof(medv_in);

        numObsernations++;
    }

    rm.resize(numObsernations);
    medv.resize(numObsernations);
    
  

    
    cout << "New Length" << rm.size() << endl;
    cout << "Closing the file" << endl;
    inFS.close();
    
    cout << "Number of records: " << numObsernations << endl;
    
    // Compute rm
    cout << "Sum of rm : " << finSum(rm) << endl;
    cout << "Mean of rm : " << finMean(rm) << endl;
    cout << "Median of rm : " << finMedian(rm) << endl;
    cout << "Range of rm : " << finRange(rm) << endl;

    // Compute medv
    cout << "Sum of medv : " << finSum(medv) << endl;
    cout << "Mean of medv : " << finMean(medv) << endl;
    cout << "Median of medv : " << finMedian(medv) << endl;
    cout << "Range of medv : " << finRange(medv) << endl;

   

    // Compute covariance and correlation
    
    cout << "Covariance = " << finCovariance(rm, medv) << endl;
    cout << "Correlation = " << finCorrelation(rm, medv) << endl;

}
