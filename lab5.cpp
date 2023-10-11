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

void delete_shared_mem(int id, void * mem){
    shmdt(mem);
    struct shmid_ds info;
    shmctl(id,IPC_RMID,&info);
}

int * find_nums_between_minmax(vector<int> &sortedArray, pair <int,int> minMax){

    int semid = semget (IPC_PRIVATE,1,IPC_CREAT|0666);
    int shmid = shmget(IPC_PRIVATE,2,IPC_CREAT|0666);
    int shmid1 = shmget(IPC_PRIVATE,2*(minMax.second - minMax.first),IPC_CREAT|0666);

    if (semid < 0 || shmid < 0){
        cout << "Ошибка создания семафора...";
        return nullptr;
    }


    sembuf Plus1 = {0, 1, 0};
    sembuf Minus1 = {0, -1, 0};

    semop(semid,&Plus1,1);
    int *k = (int*)shmat(shmid,NULL,0);
    int * arr = (int*)shmat(shmid1,NULL,0);


    int intervalForProc = sortedArray.size() / (NumOfProc-1);
    *k = 2;

    for (int proc = 0; proc < NumOfProc-1; proc++){
        if (fork()==0){
            for (int i = proc*intervalForProc; (i < proc*intervalForProc+intervalForProc) && (i < sortedArray.size()-1) && (sortedArray[i] < minMax.second); i++){
                
                for (int j = sortedArray[i]+1; (j < sortedArray[i+1]) && (j < minMax.second); j++){
                    semop(semid,&Minus1,1);

                    arr[*k] = j;
                    *k = *k+1;
                    
                    semop(semid,&Plus1,1);
                }
            }
            exit(0);
        }
    }
    for (int i = (NumOfProc-1)*intervalForProc; (i < sortedArray.size()-1) && (sortedArray[i] < minMax.second); i++){
                
                for (int j = sortedArray[i]+1; (j < sortedArray[i+1]) && (j < minMax.second); j++){
                    semop(semid,&Minus1,1);

                    arr[*k] = j;
                    *k = *k+1;
                    
                    semop(semid,&Plus1,1);
                }    
    }
    

    string temp = "ipcs -s -i " + to_string(semid);
    system(temp.c_str());
    wait(NULL);

    arr[0] = shmid1;
    arr[1] = *k - 2;

    semctl(semid, 0, IPC_RMID);

    delete_shared_mem(shmid,k);
    //delete_shared_mem(shmid1,arr);

    return arr;
}

void print_array(vector <int> &array){
    for (int i : array){
        cout << i << ", ";
    }
    cout << "\b\b  \n";
}
void print_array(int * arr, int n){
    for (int i = 0; i < n; i ++){
        cout << arr[i] << ", ";
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
    int * numsBtwMinMax = find_nums_between_minmax(array,minMax);
    if (numsBtwMinMax != nullptr){
        sort(numsBtwMinMax+2,numsBtwMinMax+numsBtwMinMax[1]+2);
        print_array(numsBtwMinMax+2,numsBtwMinMax[1]);
    }
    
    print_array(array);

    delete_shared_mem(numsBtwMinMax[0],numsBtwMinMax);

    return 0;
}



