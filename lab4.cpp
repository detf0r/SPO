#include <unistd.h>
#include <iostream>
#include <sys/wait.h>

using namespace std;

void printMenu(){
    cout << "Введите цифру:\n"
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
        cin.getline(input,sizeof(input));
        system(input);
        exit(0);
    }
    wait(&st);
}

int main()
{
    setlocale(LC_ALL,"russian");

    while(true){
        system("clear");
        printMenu();

        int choice = 0;
        cin >> choice;

        switch (choice)
        {
        case 1:
            shellComandExec();
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            exit(0);
            break;
        default:
            cout << "Ввод некорректен";
            break;
        }
    }
    return 0;
}