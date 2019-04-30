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
#include <cstring>
#include <iomanip>
#include <fstream>

//#define DEBUG

using namespace std;

//Регионы РФ
struct Regions
{
    int code; //код региона
    string governor; //ФИО губернатора
    float area; //площадь
    int population; //население
    char regionalCenter[32]; //региональный центр
};

void printPrompt();
void printCommands();
void printDB(Regions *reg, int size);
void addElement(Regions *&reg, int &size, bool isLoaded);
void loadFromFile(Regions *&reg, int &size, bool &isLoaded);

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
    const int colNumber = 5;
    int colWidth[colNumber] = {16, 31, 13, 13, 22}; //Ширина каждого столбца
    string tableHeaders[colNumber] =
    {
        "Regional code", "Governer", "Area, km^2", "Population", "Regional center"
    };

    //Вывод заголовков
    int tableWidth = 0;
    for (int i = 0; i < colNumber; i++) {
        tableWidth += colWidth[i];
    }
    cout << string(tableWidth, '=') << endl;
    for (int i = 0; i < colNumber; i++)
    {
        cout << left << setw(colWidth[i]) << tableHeaders[i];
    }
    cout << endl;
    cout << string(tableWidth, '-') << endl;

    //Вывод содержимого таблицы
    for (int i = 0; i < size; i++)
    {
        cout << setw(colWidth[0]) << reg[i].code;
        cout << setw(colWidth[1]) << reg[i].governor;
        cout << right << setw(colWidth[2] - 3) << reg[i].area << "   ";
        cout << right << setw(colWidth[3] - 3) << reg[i].population << "   ";
        cout << left << setw(colWidth[4]) << reg[i].regionalCenter;
        cout << endl;
    }
    cout << string(tableWidth, '-') << endl;
}

void addElement(Regions *&reg, int &size, bool isLoaded)
{
    if (isLoaded)
    {
        cout << "Заглушка. Если файл загружен." << endl;
    }
    else
    {
        cout << "Сначала загрузите какой-то файл командой \"6\"" << endl;
    }
}

void loadFromFile(Regions *&reg, int &size, bool &isLoaded)
{
    //Если текущий открытый файл не сохранён
    //Спросить, сохранять ли его
    //Выгрузить из памяти текущий массив
    //Спроси, какой файл загрузить
    //Открыть этот файл
    //Считай количество строк, т. е. записей
    //Создай динамический массив на кол-во записей
    //Скопируй всё содержимое строк

    static ifstream fin;
    string path;

    string currentLine; //Текущая строка из файла
    int lines = 0; //Счётчик строк в файле

    char exitLoad;

    //Если массив хранит несохранённые данные
    if (fin.is_open())
    {
        cout << "Предупреждение. Есть несохранённые данные." << endl;
        cout << "Продолжить? (y/n) ";
        cin >> exitLoad;
        if (exitLoad != 'y')
        {
            //Уходим отсюда
            return;
        }
        fin.close();
    }

    isLoaded = false;

    delete [] reg;
    reg = nullptr;

    cout << "Введите путь к файлу: " << endl;
    cin.ignore();
    getline(cin, path);

    fin.open(path);

    //Если такой файл существует на диске
    if(fin.is_open())
    {
        //Считать количество строк
        while (getline(fin, currentLine)) {
            lines++;
        }
#ifdef DEBUG
        cout << "lines=" << lines << endl;
#endif
        size = lines;

        fin.close();
        fin.open(path); //Чтобы считывать файл с начала

        //Выделить память под массив
        reg = new Regions[size];

        string tempGovernor; //Для временного хранения Ф, И, О

        //Парсить его из файла
        for (int i = 0; i < lines; i++)
        {
            fin >> reg[i].code;
            fin >> tempGovernor;
            reg[i].governor += tempGovernor + ' ';
            fin >> tempGovernor;
            reg[i].governor += tempGovernor + ' ';
            fin >> tempGovernor;
            reg[i].governor += tempGovernor;
            fin >> reg[i].area;
            fin >> reg[i].population;
            fin >> reg[i].regionalCenter;
        }

        isLoaded = true;

        //Не знаю, закрывать ли файл после окончания чтения
        fin.close();
    }
    else
    {
        cout << "Такого файла нет." << endl;
    }
}
