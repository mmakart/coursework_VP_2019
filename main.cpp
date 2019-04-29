//Курсовая работа Макарцева Михаила Петровича
//Группы ПИб-18И2
//По предмету "Высокоуровневое программирование"
//Преподаватель Зырянова Светлана Анатольевна
//СибАДИ 2019

/*
- ввод нового элемента в массив
- печать всего массива
- поиск элемента в массиве
- сортировка массива по заданному полю
- сохранение массива в файле
- чтение массива из файла
*/

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

//Регионы РФ
struct Regions
{
    int code; //код региона
    string governor; //ФИО губернатора
    double area; //площадь
    int population; //население
    string regionalCenter; //региональный центр
};

void printPrompt();
void printCommands();
void printDB(Regions *reg, int size);
void addElement(Regions *reg, int &size, bool isLoaded);
void loadFromFile(Regions *reg, int &size, bool &isLoaded);

int main()
{
    char command; //Команда пользователя
    bool fileIsLoaded = false; //Загружен ли файл

    Regions *regions = nullptr; //База данных в оперативной памяти
    int size = 0; //Количество записей

    cout << "+------------------------------------------------+" << endl;
    cout << "| Программа, позволяющая работать с базой данных |" << endl;
    cout << "| субъектов Российской Федерации                 |" << endl;
    cout << "+------------------------------------------------+" << endl;
    cout << endl;

    printCommands();
    cout << endl;

    cout << "Введите команду. (Список команд можно посмотреть, введя \"h\")" << endl;

    while (true) 
    {
        printPrompt();
        cin >> command;

        //Условие выхода из цикла
        if (command == 'q')
            break;

        switch (command)
        {
            //Добавление элемента
            case '1':
                addElement(regions, size, fileIsLoaded);
                break;
            //Печать базы данных
            case '2':
                printDB(regions, size);
                break;
            //Поиск элемента по заданному полю
            case '3':
                cout << "Ищем элемент" << endl;
                break;
            //Сортировка по заданному полю
            case '4':
                cout << "Сортируем по заданному полю" << endl;
                break;
            //Сохранение в файл
            case '5':
                cout << "Сохранить" << endl;
                break;
            //Чтение из файла
            case '6':
                loadFromFile(regions, size, fileIsLoaded);
                break;
            //Помощь
            case 'h':
                printCommands();
                break;
            //Неизвестная команда
            default:
                cout << "Такой команды нет. Для просмотра доступных команд введите \"h\"" << endl;
        }

    }

    cout << "Завершение..." << endl;

    return 0;
}

//Приглашение ввести команду
void printPrompt()
{
    cout << "> ";
}

//Помощь. Список доступных команд
void printCommands()
{
    cout << "+------------------------------------------+" << endl;
    cout << "| 1 - ввод нового элемента в массив        |" << endl;
    cout << "| 2 - печать всего массива                 |" << endl;
    cout << "| 3 - поиск элемента в массиве             |" << endl;
    cout << "| 4 - сортировка массива по заданному полю |" << endl;
    cout << "| 5 - сохранение массива в файле           |" << endl;
    cout << "| 6 - чтение массива из файла              |" << endl;
    cout << "| q - выход из программы                   |" << endl;
    cout << "+------------------------------------------+" << endl;
}

void printDB(Regions *reg, int size)
{
    cout.width(21);
    cout << "Код региона" << '\t';
    cout << "ФИО губернатора" << setw(20) << '\t';
    cout << "Площадь, км^2" << '\t';
    cout << "Население, тыс. чел." << '\t';
    cout << "Областной центр" << endl;

    for (int i = 0; i < size; i++)
    {
        cout << reg[i].code << '\t';
        cout << reg[i].governor << '\t';
        cout << reg[i].area << '\t';
        cout << reg[i].population << '\t';
        cout << reg[i].regionalCenter << endl;
    }
}

void addElement(Regions *reg, int &size, bool isLoaded)
{
    if (isLoaded)
    {
        cout << "Заглушка. Если файл загружен." << endl;
    }
    else
    {
        cout << "Сначала откройте какой-то файл командой \"6\"" << endl;
    }
}

void loadFromFile(Regions *reg, int &size, bool &isLoaded)
{
    //Если текущий открытый файл не сохранён
    //Спросить, сохранять ли его
    //Выгрузить из памяти текущий массив
    //Спроси, какой файл загрузить
    //Открыть этот файл
    //Считай количество строк, т. е. записей
    //Создай динамический массив на кол-во записей
    //Скопируй всё содержимое строк
    ifstream fin;
    string path;

    cout << "Введите путь к файлу: " << endl;
    cin.ignore();
    getline(cin, path);

    isLoaded = true;
}
