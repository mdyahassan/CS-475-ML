#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>
#include <cmath>
#include <random>




using namespace std;




struct Passenger {
 int pclass;
 int survived;
 int sex;
 double age;
};




vector<Passenger> read_data(string filename) {
 vector<Passenger> data;
 ifstream infile("titanic_project.csv");
 if (infile.fail()) {
     cout << "Failed to open file." << endl;
     exit(1);
 }
 string line;
 getline(infile, line); // skip header
 while (getline(infile, line)) {
     Passenger p;
     stringstream ss(line);
     string value;
     getline(ss, value, ',');
     getline(ss, value, ',');
     p.pclass = stoi(value);
     getline(ss, value, ',');
     p.survived = stoi(value);
     getline(ss, value, ',');
     p.sex = stoi(value);
     getline(ss, value, ',');
     p.age = stoi(value);
     data.push_back(p);




 }
 infile.close();
 return data;
}




vector<int> sample(int n, int k, int seed) {
 vector<int> idx(n);
 for (int i = 0; i < n; i++) {
     idx[i] = i;
 }
 mt19937 g(seed);
 shuffle(idx.begin(), idx.end(), g);
 idx.resize(k);
 return idx;
}




struct NaiveBayes {
 vector<int> class_counts;
 vector<vector<int> > feature_counts;
 vector<vector<double> > feature_probs;
 NaiveBayes() {}
 void train(vector<Passenger>& data) {
     int num_classes = 2;
     int num_features = 5;
     class_counts.resize(num_classes);
     feature_counts.resize(num_classes);
     feature_probs.resize(num_classes);
     for (int i = 0; i < num_classes; i++) {
         feature_counts[i].resize(num_features);
         feature_probs[i].resize(num_features);
     }
     for (auto& p : data) {
         class_counts[p.survived]++;
         feature_counts[p.survived][p.pclass]++;
         feature_counts[p.survived][p.sex]++;
         feature_counts[p.survived][p.age]++;
     }
     for (int i = 0; i < num_classes; i++) {
         int count = class_counts[i];
         for (int j = 0; j < num_features; j++){
                             feature_probs[i][j] = (double)feature_counts[i][j] / count;
         }
     }
 }
 int predict(Passenger& p) {
     int num_classes = 2;
     double class_probs[num_classes];
     for (int i = 0; i < num_classes; i++) {
         class_probs[i] = log((double)class_counts[i] / class_counts[0]);
         class_probs[i] += log(feature_probs[i][p.pclass]);
         class_probs[i] += log(feature_probs[i][p.survived]);
         class_probs[i] += log(feature_probs[i][p.sex]);
         class_probs[i] += log(feature_probs[i][p.age]);
      }
     if (class_probs[0] > class_probs[1]) {
         return 0;
     } else {
         return 1;
     }
 }
 vector<int> predict(vector<Passenger>& data) {
     vector<int> predictions;
     for (auto& p : data) {
         predictions.push_back(predict(p));
     }
     return predictions;
 }
};


// calculate sensitivity and specificity
void evaluate(vector<Passenger>& data, vector<int>& predictions, double& sensitivity, double& specificity) {
   int true_positives = 0, false_negatives = 0, true_negatives = 0, false_positives = 0;
   for (int i = 0; i < data.size(); i++) {
       if (predictions[i] == 1 && data[i].survived == 1) {
           true_positives++;
       } else if (predictions[i] == 0 && data[i].survived == 1) {
           false_negatives++;
       } else if (predictions[i] == 0 && data[i].survived == 0) {
           true_negatives++;
       } else {
           false_positives++;
       }
   }
   sensitivity = (double)true_positives / (true_positives + false_negatives);
   specificity = (double)true_negatives / (true_negatives + false_positives);
}




int main() {
 vector<Passenger> data = read_data("desktop/titanic_project.csv");
 int n = data.size();
 vector<int> idx = sample(n, (n * 0) + 800, 1234);
 vector<Passenger> train, test;
 for (int i = 0; i < n; i++) {
     if (find(idx.begin(), idx.end(), i) != idx.end()) {
         train.push_back(data[i]);
     } else {
         test.push_back(data[i]);
     }
 }
 auto start_time = chrono::steady_clock::now();
 NaiveBayes nb;
 nb.train(train);
 auto end_time = chrono::steady_clock::now();
 auto diff_time = end_time - start_time;
 auto training_time = chrono::duration <double, milli> (diff_time).count();
 vector<int> predictions = nb.predict(test);
 int num_correct = 0;
 for (int i = 0; i < test.size(); i++) {
     if (predictions[i] == test[i].survived) {
         num_correct++;
     }
 }
 double accuracy = (double)num_correct / test.size();
 cout << "Accuracy: " << accuracy << endl;


 // calculate sensitivity and specificity
   double sensitivity, specificity;
   evaluate(test, predictions, sensitivity, specificity);
   cout << "Sensitivity: " << sensitivity << endl;
   cout << "Specificity: " << specificity << endl;
     cout << "Training time: " << training_time << " seconds" << endl;


 return 0;
}





