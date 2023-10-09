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

pair<int,int>find_min_and_max(vector<int> &array){
    int min = array[0];
    int max = array[0];
    for (int i : array){
        if (i<min)
            min = i;
        if (i>max)
            max = i;
    }
    return make_pair(min,max);
}

int find_min_pos(vector <int> &array, int min){
    for (int pos = 0; pos < array.size(); pos++){
        if (array[pos]== min){
            return pos;
        }
    }
    return 0;
}

vector<int> find_nums_between_minmax(vector<int> &sortedArray, pair <int,int> minMax){
    int pos = find_min_pos(sortedArray,minMax.first);
    vector<int> numsBtwMinMax;
    for (int i = pos; (i < sortedArray.size()-1) && (sortedArray[i] < minMax.second);i++){
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
    pair<int,int> minMax = find_min_and_max(array);
    sort(array.begin(), array.end());
    vector<int> numsBetwMinMax = find_nums_between_minmax(array,minMax);
    print_nums(numsBetwMinMax);
    print_nums(array);
}
