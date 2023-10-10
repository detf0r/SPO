#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

const int NumOfProc = 5;

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
    int semid = semget (IPC_PRIVATE,1,IPC_CREAT|0666);
    int shmid = shmget(IPC_PRIVATE,2,IPC_CREAT|0666);
    if (semid < 0 || shmid < 0){
        cout << "Ошибка создания семафора...";
        return numsBtwMinMax;
    }


    sembuf Plus1 = {0, 1, 0};
    sembuf Minus1 = {0, -1, 0};
    sembuf MinusNumOfProc = {0, -NumOfProc, 0};

    semop(semid,&Plus1,1);
    auto mem = (vector<int>*)shmat(shmid,NULL,0);

    int intervalForProc = sortedArray.size() / NumOfProc;
    
    for (int proc = 0; proc < NumOfProc; proc++){
        if (fork()==0){
            for (int i = proc*intervalForProc; (i < proc*intervalForProc+intervalForProc) && (i < sortedArray.size()-1) && (sortedArray[i] < minMax.second); i++){
                semop(semid,&Plus1,1);
                for (int j = sortedArray[i]+1; (j < sortedArray[i+1]) && (j < minMax.second); j++){
                    numsBtwMinMax.push_back(j);
                }
            }
        }
    }

    
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
