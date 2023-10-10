#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> read_array(fstream &in, int n){
    vector<int> array;
    for (int i=0; i<n; i++){
        int num;
        in >> num;
        array.push_back(num);
    }
    return array;
}
vector<int> find_nums_between_minmax(vector<int> &sortedArray, pair <int,int> minMax){
    vector<int> numsBtwMinMax;
    for (int i = 0; (i < sortedArray.size()-1) && (sortedArray[i] < minMax.second);i++){
        for (int j = sortedArray[i]+1; (j < sortedArray[i+1]) && (j < minMax.second); j++){
            numsBtwMinMax.push_back(j);
        }
    }
    return numsBtwMinMax;
}

void print_nums(vector <int> &array){
    for (int i : array){
        cout << i << ", ";
    }
    cout << "\b\b  \n";
}

int main(){
    fstream input("input_lab5");
    if (!input.is_open()){
        cout << "Не удалось открыть файл input_lab5\n";
        return -2;
    }
    int n;
    input >> n;
    vector<int> array = read_array(input,n);
    
    sort(array.begin(), array.end());
    pair<int,int> minMax(array[0],array[array.size()-1]);
    vector<int> numsBetwMinMax = find_nums_between_minmax(array,minMax);
    print_nums(numsBetwMinMax);
    print_nums(array);
}
