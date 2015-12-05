// C++ code for implementing Decision Tree C4.5 algorithm
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<set>
#include <map>
#include<sstream>
#include<algorithm>
#include<random>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<cstdio>

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

// Method for calculating the freq(Ci,S)
double calculate_freq(vector<string> attribute_classes, string check_class){
    double freq = 0;
//    cout << "Third" << "\n";
    for(long i = 0; i < attribute_classes.size(); i++){
        if(attribute_classes[i] == check_class){
            freq += 1;
        }
    }
    return freq;
}

// Method for calculating information conveyed by complete attrubute set : info(T)
double cal_info_convey_given_set(vector<string> attribute_classes, double attribute_size){
    double info_conveyed = 0, freq = 0, freq_ratio = 0;
    set<string> unique_classes;
    set<string>::iterator it;
    cout << "attribute_size : " <<attribute_size<<"\n";
    //Generating unique classes
    for(long i = 0; i < attribute_size; i++){
        unique_classes.insert(attribute_classes[i]);
    }
    for(it = unique_classes.begin(); it != unique_classes.end(); it++){
        //cout << *it << "\n";
        freq = calculate_freq(attribute_classes,*it);
        cout << "freq : " << freq << "\n";
        freq_ratio = freq/attribute_size;
        cout << "freq_ratio : " << freq_ratio << "\n";
        info_conveyed += (freq_ratio*log2(freq_ratio));
        //cout << freq << " : " << freq_ratio << " : "  << info_conveyed << "\n";
    }
    info_conveyed = -info_conveyed;
    return info_conveyed;
}

// Method for calculating information conveyed by this attribute set : info_x(T)
vector<double> cal_info_convey_this_attribut_set(vector<string> attribute_classes, vector<string> attribute_values){
    double info_conveyed = 0;
    double split_info = 0;
    double attri_ratio = 0;
    double attribute_size = attribute_values.size();
    vector<double> info_and_split;

    map<string, vector<string>> split_category;
    map<string, vector<string>>::iterator it;
    for(long i = 0; i < attribute_values.size(); i++){
        split_category[attribute_values[i]].push_back(attribute_classes[i]);
    }
    for(it = split_category.begin(); it != split_category.end(); ++it){
        attri_ratio = it->second.size()/attribute_size;
        cout << "it->second.size() : " << it->second.size() << "attribute_size : " << attribute_size <<"\n";
        info_conveyed += (attri_ratio*cal_info_convey_given_set(it->second,it->second.size()));
        split_info += (attri_ratio*log2(attri_ratio));
    }
    split_info = -split_info;
    info_and_split.push_back(info_conveyed);
    info_and_split.push_back(split_info);

    return info_and_split;
}

// Method to calculate gain ratio
double get_gain_ratio(vector<string> attribute_classes, vector<string> attribute_values){
    double gain = 0, gain_ratio = 0, info = 0, info_x = 0, split_info;
    vector<double> store_info_x;
    info = cal_info_convey_given_set(attribute_classes,attribute_values.size());
    cout << "INFO : " << info << "\n";
    store_info_x = cal_info_convey_this_attribut_set(attribute_classes,attribute_values);
    info_x = store_info_x[0];
    cout << "INFO_X : " << info_x << "\n";
    gain = info - info_x;
    cout << "gain : " << gain << "\n";
    split_info = store_info_x[1];
    cout << "split_info : " << split_info << "\n";
    gain_ratio = gain/split_info;
    return gain_ratio;
}

//Method to differentiate categorical data from neumeric data
// Logic: If the number of unique attribute_values is more than some threshold then it is neumeric data
bool is_neumeric(vector<string> attribute_values,long deciding_threshold){
    set<string> values;
    for(long i = 0; i < attribute_values.size();i++){
        values.insert(attribute_values[i]);
        if(values.size() > deciding_threshold){
            return true;
        }
    }
    return false;
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

// Method for generating transpose of a 2D vector assuming it is a square 2d vector
vector<vector<string>> get_transpose_vector(vector<vector<string>> input_2d_vector){
    long row_size = input_2d_vector.size();
    long col_size = input_2d_vector[0].size();

    vector<vector<string>> transpose_vector(col_size,vector<string>(row_size));
    for(long i=0;i<row_size; i++) {
        for (long j=0; j<col_size; j++){
	        transpose_vector[j][i] = input_2d_vector[i][j];
        }
    }
    return transpose_vector;
}

// Method for generating feature vector for training set
vector<vector<string>> generate_feature_vector(){
    vector<vector<string>> feature_vector, transpose_feature_vector;
    string  file_name = "play_dataset.data";
    ifstream training_file(file_name);
    string line;
    if(training_file.is_open())
    {
        while (getline(training_file, line))
            feature_vector.push_back(split(line,','));
        training_file.close();
    }
    else
        cout << "Unable to open file";

    transpose_feature_vector = get_transpose_vector(feature_vector);
    return transpose_feature_vector;
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
    double feature_size = 0;

    //Reading the dataset file name
    //cout << "Enter the file name : ";
    //cin >> file_name;
    file_name = "dataset.data";
    //Calling split_dataset to generate training and testing files
    split_dataset(file_name);

    //generating feature vector for testing data
    feature_vector = generate_feature_vector();
//    for (int i = 0; i < feature_vector.size(); i++){
//        for (int j = 0; j < feature_vector[i].size(); j++){
//            string str = feature_vector[i][j];
//            str.erase(remove(str.begin(), str.end(), ' '),str.end());
//            cout << str << ",";
//        }
//        cout << "\n";
//    }
//    for (int i = 0; i < feature_vector.size(); i++){
//            cout << (is_neumeric(feature_vector[i],10)? "Yes" : "No");
//    }
    double feature_col_size = feature_vector.size();
    double feature_row_size = feature_vector[0].size();
    for(int i = 0; i < feature_row_size; i ++){
        //cout << feature_vector[feature_col_size-1][i] << "\n";
    }
    cout << get_gain_ratio(feature_vector[feature_col_size-1],feature_vector[0]);
    return 0;
}
