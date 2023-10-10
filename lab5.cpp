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
int * find_nums_between_minmax(vector<int> &sortedArray, pair <int,int> minMax){
    vector<int> numsBtwMinMax;
    numsBtwMinMax.resize(minMax.second - minMax.first);

    int semid = semget (IPC_PRIVATE,1,IPC_CREAT|0666);
    int shmid = shmget(IPC_PRIVATE,2,IPC_CREAT|0666);
    int shmid1 = shmget(IPC_PRIVATE,2*(minMax.second - minMax.first),IPC_CREAT|0666);
    if (semid < 0 || shmid < 0){
        cout << "Ошибка создания семафора...";
        return nullptr;
    }


    sembuf Plus1 = {0, 1, 0};
    sembuf Minus1 = {0, -1, 0};
    //sembuf MinusNumOfProc = {0, -NumOfProc, 0};

    semop(semid,&Plus1,1);
    int *k = (int*)shmat(shmid,NULL,0);
    int *arr = (int*)shmat(shmid1,NULL,0);
    //vector<int> numsBtwMinMax1
    int intervalForProc = sortedArray.size() / NumOfProc;
    *k = 0;

    for (int proc = 0; proc < NumOfProc; proc++){
        if (fork()==0){
            for (int i = proc*intervalForProc; (i < proc*intervalForProc+intervalForProc) && (i < sortedArray.size()-1) && (sortedArray[i] < minMax.second); i++){
                
                for (int j = sortedArray[i]+1; (j < sortedArray[i+1]) && (j < minMax.second); j++){
                    semop(semid,&Minus1,1);

                    numsBtwMinMax[*k] = j; 
                    arr[*k] = j;
                    *k = *k+1;
                    //cout << j << " ";
                    
                    semop(semid,&Plus1,1);
                }
            }
            exit(0);
        }
    }
    cout << endl;
    //system("ipcs -s");
    string temp = "ipcs -s -i " + to_string(semid);
    system(temp.c_str());
    wait(NULL);
    
    //
    
    /*for (int i = 0; (i < sortedArray.size()-1) && (sortedArray[i] < minMax.second);i++){
        for (int j = sortedArray[i]+1; (j < sortedArray[i+1]) && (j < minMax.second); j++){
            numsBtwMinMax.push_back(j);
        }
    }*/
    for (int i = 0; i < *k; i ++){
        cout << arr[i] << " ";
    }
    cout << endl;
    return arr;
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

    int * arr = find_nums_between_minmax(array,minMax);
    //delete[] arr;
    //print_nums(numsBetwMinMax);
    print_nums(array);
}



