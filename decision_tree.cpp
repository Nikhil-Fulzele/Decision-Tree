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

//Method for spliting the line in vector of attributes
vector<string> split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str);
    string tok;

    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
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
    //for(long i = 0; i < file_size; ++i)
    //    cout << file_info[i] << "\n";
    //cout << training_size << " " << testing_size;

    //Creating training dataset
    ofstream write_train_file;
    write_train_file.open ("training.data");
    for(long i=0; i < training_size; ++i)
        write_train_file << file_info[i] << "\n";
    write_train_file.close();

    //Creating training dataset
    ofstream write_test_file;
    write_test_file.open ("test.data");
    for(long i=training_size; i < file_size; ++i)
        write_test_file << file_info[i] << "\n";
    write_test_file.close();

}

int main () {
    string file_name; //name of the input file (dataset)

    //Reading the dataset file name
    cout << "Enter the file name : ";
    cin >> file_name;

    // Calling split_dataset to generate training and testing files
    split_dataset(file_name);

    return 0;
}
