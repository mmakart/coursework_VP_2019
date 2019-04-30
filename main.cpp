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
    char governor[3][32]; //ФИО губернатора
    float area; //площадь
    float population; //население
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
    int colWidth[colNumber] = {12, 31, 19, 21, 16}; //Ширина каждого столбца
    int colData[colNumber] = {}; //Длина содержимого 
    int colSpaces[colNumber] = {}; //Сколько пробелов поставить до/после данных
    char *tableHeaders[colNumber] =
    { "Код региона", "ФИО губернатора", "Площадь, тыс. км^2", "Население, тыс. чел.", "Областной центр" };
    for (int i = 0; i < colNumber; i++) {
        colData[i] = strlen(tableHeaders[i]);
    }
    for (int i = 0; i < colNumber; i++) {
        colSpaces[i] = colWidth[i] - colData[i];
    }
    /*for (int i = 0; i < colNumber; i++) {
        cout << tableHeaders[i] << string(colSpaces[i], '#');
    }*/
    cout << setw(12) << tableHeaders[0];
    cout << setw(31) << tableHeaders[1];
    cout << setw(19) << tableHeaders[2];
    cout << setw(21) << tableHeaders[3];
    cout << setw(16) << tableHeaders[4];

    for (int i = 0; i < size; i++)
    {
        
        cout << setw(12) << reg[i].code;
        cout << setw(31) << reg[i].governor[0] << " " << reg[i].governor[1] << " " << reg[i].governor[2];
        cout << setw(19) << reg[i].area;
        cout << setw(21) << reg[i].population;
        cout << setw(16) << reg[i].regionalCenter << endl;
    }
}

void addElement(Regions *&reg, int &size, bool isLoaded)
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

    if (fin.is_open())
    {
        cout << "Предупреждение. Есть открытый файл." << endl;
        cout << "Выйти из режима загрузки файла? (y/n) ";
        cin >> exitLoad;
        if (exitLoad != 'n')
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

    if(fin.is_open())
    {
        //Считать количество строк
        while (getline(fin, currentLine))
        {
            lines++;
        }
        lines /= 7;
#ifdef DEBUG
        cout << "lines=" << lines << endl;
#endif
        size = lines;

        fin.close();
        fin.open(path);

        //Выделить память под массив
        reg = new Regions[lines];

        //Парсить его из файла
        for (int i = 0; i < lines; i++)
        {
            fin >> reg[i].code;
            //fin.ignore();
            //getline(fin, reg[i].governor[0], 32);
            //getline(fin, reg[i].governor[1], 32);
            //getline(fin, reg[i].governor[2], 32);
            fin >> reg[i].governor[0];
            fin >> reg[i].governor[1];
            fin >> reg[i].governor[2];
            fin >> reg[i].area;
            fin >> reg[i].population;
            fin >> reg[i].regionalCenter;
        }
#ifdef DEBUG
        cout << "reg[0].code=" << reg[0].code << endl;
        cout << "reg[0].governer[0]=" << reg[0].governor[0] << endl;
        cout << "reg[0].governer[1]=" << reg[0].governor[1] << endl;
        cout << "reg[0].governer[2]=" << reg[0].governor[2] << endl;
        cout << "reg[0].area=" << reg[0].area << endl;
        cout << "reg[0].population=" << reg[0].population << endl;
        cout << "reg[0].regionalCenter=" << reg[0].regionalCenter << endl;

        cout << "reg[1].code=" << reg[1].code << endl;
        cout << "reg[1].governer[0]=" << reg[1].governor[0] << endl;
        cout << "reg[1].governer[1]=" << reg[1].governor[1] << endl;
        cout << "reg[1].governer[2]=" << reg[1].governor[2] << endl;
        cout << "reg[1].area=" << reg[1].area << endl;
        cout << "reg[1].population=" << reg[1].population << endl;
        cout << "reg[1].regionalCenter=" << reg[1].regionalCenter << endl;
#endif

        isLoaded = true;
    }
    else
    {
        cout << "Такого файла нет" << endl;
    }
}
