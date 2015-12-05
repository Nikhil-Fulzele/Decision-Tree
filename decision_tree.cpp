// C++ code for implementing Decision Tree C4.5 algorithm
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<algorithm>
#include<random>
#include<cmath>
#include<ctime>

using namespace std;

//Defining tree node
struct node{
    bool isLeave;
    string final_cases;
    long dataset_size;
    long correct_classify;
    long incorrect_classify;
    double error_value;
    double success_rate;
    vector<struct node*> child;
}tree_node;

//Method to differentiate categorical data from neumeric data
// Logic: if the attribute_value is integer and number of unique attribute_values is more than some threshold then it is neumeric data
bool is_neumeric(vector<string> attribute_values,long deciding_threshold){
    set<string> values;
    for(long i; i < attribute_values.size();i++){
        values.insert(attribute_values[i]);
    }
}

//Method for spliting the line in vector of attributes
vector<string> split(string str, char delimiter) {
    vector<string> row_attributes_values;
    stringstream ss(str);
    string tok;

    while(getline(ss, tok, delimiter)) {
        row_attributes_values.push_back(tok);
    }

    return row_attributes_values;
}

// Method for generating feature vector for training set
vector<vector<string>> generate_feature_vector(){
    vector<vector<string>> final_feature_vector;
    string  file_name = "training.data";
    ifstream training_file(file_name);
    string line;
    if(training_file.is_open())
    {
        while (getline(training_file, line))
            final_feature_vector.push_back(split(line,','));
        training_file.close();
    }
    else
        cout << "Unable to open file";
    return final_feature_vector;
}

//Generate Training set and Testing set and save them to training.data and testing.data
void split_dataset(string file_name){
    long file_size;
    vector<string> file_info;
    string line;
    srand ( unsigned ( std::time(0) ) );

    // Process for reading file
    ifstream training_file(file_name);
    if(training_file.is_open())
    {
        while (getline(training_file, line))
            file_info.push_back(line);
        training_file.close();
    }
    else
        cout << "Unable to open file";

    file_size = file_info.size();

    // Calculating ratio for spliting
    long training_size, testing_size;
    training_size = file_size*70/100;
    testing_size = file_size-training_size;

    // using built-in random generator:
    random_shuffle ( file_info.begin(), file_info.end() );

    //Creating training dataset
    ofstream write_train_file;
    write_train_file.open ("training.data");
    for(long i=0; i < training_size; ++i)
        write_train_file << file_info[i] << "\n";
    write_train_file.close();

    //Creating testing dataset
    ofstream write_test_file;
    write_test_file.open ("test.data");
    for(long i=training_size; i < file_size; ++i)
        write_test_file << file_info[i] << "\n";
    write_test_file.close();

}

int main () {
    string file_name; //name of the input file (dataset)
    vector<vector<string>> feature_vector;

    //Reading the dataset file name
    cout << "Enter the file name : ";
    cin >> file_name;

    //Calling split_dataset to generate training and testing files
    split_dataset(file_name);

    //generating feature vector for testing data
    feature_vector = generate_feature_vector();
    for (int i = 0; i < feature_vector.size(); i++){
        for (int j = 0; j < feature_vector[j].size(); j++){
            cout << feature_vector[i][j] << " ";
        }
        cout << "\n";
    }

    return 0;
}
