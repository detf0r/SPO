#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <limits>

using namespace std;

void printMenu(){
    cout << "\nВведите цифру:\n"
         << "1. Выполнить введенную команду\n"
         << "2. Запустить на исполнение файл со сценарием\n"
         << "3. Сделать все файлы указанной директории исполняемыми\n"
         << "4. Выход\n";
}
void shellComandExec(){
    system("clear");
    
    cout << "Вы выбрали:\n1. Выполнить введенную команду\n\nВведите команду:\n";
    int st = 0;
    if (fork() == 0){
        char input[100];
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.getline(input,sizeof(input));
        system(input);
        exit(0);
    }
    wait(&st);
}

void scriptFileExec(){
    system("clear");
    
    cout << "Вы выбрали:\n2. Запустить на исполнение файл со сценарием\n\n"
         << "Введите путь файла и параметры через пробел:\n";
    
    int st = 0;
    if (fork()==0){
        char input[150];
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.getline(input,sizeof(input));
        system(input);
        exit(0);
    }
    wait(&st);
}

void makeDirectoryFilesUsable(){
    system("clear");
    
    cout << "Вы выбрали:\n3. Сделать все файлы указанной директории исполняемыми\n\n"
         << "Введите путь до директории в одинарных кавычках:\n";
    int st = 0;

    if (fork() == 0){
        char input [200];

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.getline(input,sizeof(input));

        string strPath(input);
        size_t startPos = strPath.find_first_of("'")+1;
        size_t endPos = strPath.find_last_of("'");

        string command = "chmod +x ";
        string path = strPath.substr(startPos,endPos - startPos) + "/*";
        command += path;


        cout << "Все файлы по пути: "<< path << " будут изменены\nПродолжить? (y/n): ";
        char answer;
        cin >> answer;
        if (answer == 'y'){
            system(command.c_str());
        }
        
        exit(0);
    }
    wait(&st);
}

int main() {
    setlocale(LC_ALL,"russian");

    while(true){
        
        printMenu();

        int choice = 0;
        cin >> choice;

        switch (choice)
        {
        case 1:
            shellComandExec();
            break;
        case 2:
            scriptFileExec();
            break;
        case 3:
            makeDirectoryFilesUsable();
            break;
        case 4:
            cout << "Выход...\n";
            exit(0);
            break;
        default:
            cout << "Ввод некорректен";
            break;
        }
    }
    return 0;
}