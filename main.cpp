//Курсовая работа Макарцева Михаила Петровича
//Группы ПИб-18И2
//По предмету "Высокоуровневое программирование"
//Преподаватель Зырянова Светлана Анатольевна
//СибАДИ 2019

/*
Функции программы:
- ввод нового элемента в массив
- печать всего массива
- поиск элемента в массиве
- сортировка массива по заданному полю (вставками)
- сохранение массива в файле
- чтение массива из файла
*/

#include <iostream>
#include <cstring>
#include <iomanip>
#include <fstream>
#include <vector>

//#define DEBUG

using namespace std;

//Регионы РФ
struct Regions
{
    int code; //код региона
    string governor; //ФИО губернатора
    int area; //площадь
    int population; //население
    string regionalCenter; //региональный центр
};

void printPrompt();
void printCommands();
void printTable(Regions *reg, int size);
void addElement(Regions *&reg, int &size, bool &isSaved);
void printContents(Regions *reg, int size);
bool isSorted(Regions *reg, int size, int field, bool ascending);
void insertionSort(Regions *reg, int size, int field, bool ascending);
void sortContents(Regions *reg, int size);
void findContents(Regions *reg, int size);
void saveToFile(Regions *reg, int size, bool &isSaved);
void loadFromFile(Regions *&reg, int &size, bool isSaved);

int main()
{
    char command; //Команда пользователя

    Regions *regions = nullptr; //База данных в оперативной памяти
    int size = 0; //Количество записей
    bool changesAreSaved = true;
    char confirmExit; //(y|n) Выйти без сохранения

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

        switch (command)
        {
            //Добавление элемента
            case '1':
                addElement(regions, size, changesAreSaved);
                break;
            //Печать базы данных
            case '2':
                printContents(regions, size);
                break;
            //Поиск элемента по заданному полю
            case '3':
                findContents(regions, size);
                break;
            //Сортировка по заданному полю
            case '4':
                sortContents(regions, size);
                break;
            //Сохранение в файл
            case '5':
                saveToFile(regions, size, changesAreSaved);
                break;
            //Чтение из файла
            case '6':
                loadFromFile(regions, size, changesAreSaved);
                break;
            //Помощь
            case 'h':
                printCommands();
                break;
            //Выход
            case 'q':
                if (changesAreSaved)
                    goto EXIT;

                cout << "Есть несохранённые изменения. Всё равно выйти? (y/n) ";
                cin >> confirmExit;

                if (confirmExit == 'y')
                    goto EXIT;
                else
                    break;
            //Неизвестная команда
            default:
                cout << "Такой команды нет. Для просмотра доступных команд введите \"h\"" << endl;
        }

    }

EXIT:

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

void printTable(Regions *reg, int size)
{
    const int colNumber = 5;
    static int colWidth[colNumber] = {16, 31, 13, 13, 22}; //Ширина каждого столбца
    static string tableHeaders[colNumber] =
    {
        "Regional code", "Governor", "Area, km^2", "Population", "Regional center"
    };

    int tableWidth = 0;
    for (int i = 0; i < colNumber; i++) {
        tableWidth += colWidth[i];
    }

    //Вывод заголовков
    cout << string(tableWidth, '=') << endl;
    for (int i = 0; i < colNumber; i++) {
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

void addElement(Regions *&reg, int &size, bool &isSaved)
{
    if (reg == nullptr)
    {
        cout << "Сначала загрузите в память какой-то файл командой \"6\"" << endl;
        return;
    }

    Regions *moreReg = new Regions[size + 1];
    Regions newReg;

    //Ввод нового элемента
    cout << "Введите элемент: " << endl;
    cout << "Код региона: ";
    cin >> newReg.code;
    cout << "ФИО губернатора: ";
    cin.ignore();
    getline(cin, newReg.governor);
    cout << "Площадь: ";
    cin >> newReg.area;
    cout << "Население: ";
    cin >> newReg.population;
    cout << "Областной центр: ";
    cin.ignore();
    getline(cin, newReg.regionalCenter);

    //Копирование существующих элементов в новый массив
    for (int i = 0; i < size; i++)
    {
        moreReg[i].code = reg[i].code;
        moreReg[i].governor = reg[i].governor;
        moreReg[i].area = reg[i].area;
        moreReg[i].population = reg[i].population;
        moreReg[i].regionalCenter = reg[i].regionalCenter;
    }
    //Добавление нового элемента последним
    moreReg[size].code = newReg.code;
    moreReg[size].governor = newReg.governor;
    moreReg[size].area = newReg.area;
    moreReg[size].population = newReg.population;
    moreReg[size].regionalCenter = newReg.regionalCenter;

    delete [] reg;
    reg = moreReg;
    size++;

    isSaved = false;
}

void printContents(Regions *reg, int size)
{
    if (reg == nullptr)
    {
        cout << "Сначала откройте файл." << endl;
        return;
    }
    printTable(reg, size);
}

bool isSorted(Regions *reg, int size, int field, bool ascending)
{
    switch (field)
    {
        case 0: //код
            if (ascending)
            {
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].code > reg[i+1].code)
                        return false;
            }
            else
            {
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].code < reg[i+1].code)
                        return false;
            }
            break;
        case 1: //губернатор
            if (ascending)
            {
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].governor > reg[i+1].governor)
                        return false;
            }
            else
            {
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].governor < reg[i+1].governor)
                        return false;
            }
            break;
        case 2: //площадь
            if (ascending)
            {
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].area > reg[i+1].area)
                        return false;
            }
            else
            {
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].area < reg[i+1].area)
                        return false;
            }
            break;
        case 3: //население
            if (ascending)
            {
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].population > reg[i+1].population)
                        return false;
            }
            else
            {
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].population < reg[i+1].population)
                        return false;
            }
            break;
        case 4: //центр
            if (ascending)
            {
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].regionalCenter > reg[i+1].regionalCenter)
                        return false;
            }
            else
            {
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].regionalCenter < reg[i+1].regionalCenter)
                        return false;
            }
            break;
    }
    return true;
}

void insertionSort(Regions *reg, int size, int field, bool ascending)
{
    switch (field)
    {
        case 0:
            for (int i = 1; i < size; i++)
            {
                Regions temp = reg[i];
                int j = i - 1;
                while (j >= 0 && (ascending ? reg[j].code > temp.code : reg[j].code < temp.code)) {
                    reg[j+1] = reg[j];
                    j--;
                }
                reg[j+1] = temp;
            }
            break;
        case 1:
            for (int i = 1; i < size; i++)
            {
                Regions temp = reg[i];
                int j = i - 1;
                while (j >= 0 && (ascending ? reg[j].governor > temp.governor : reg[j].governor < temp.governor)) {
                    reg[j+1] = reg[j];
                    j--;
                }
                reg[j+1] = temp;
            }
            break;
        case 2:
            for (int i = 1; i < size; i++)
            {
                Regions temp = reg[i];
                int j = i - 1;
                while (j >= 0 && (ascending ? reg[j].area > temp.area : reg[j].area < temp.area)) {
                    reg[j+1] = reg[j];
                    j--;
                }
                reg[j+1] = temp;
            }
            break;
        case 3:
            for (int i = 1; i < size; i++)
            {
                Regions temp = reg[i];
                int j = i - 1;
                while (j >= 0 && (ascending ? reg[j].population > temp.population : reg[j].population < temp.population)) {
                    reg[j+1] = reg[j];
                    j--;
                }
                reg[j+1] = temp;
            }
            break;
        case 4:
            for (int i = 1; i < size; i++)
            {
                Regions temp = reg[i];
                int j = i - 1;
                while (j >= 0 && (ascending ? reg[j].regionalCenter > temp.regionalCenter : reg[j].regionalCenter < temp.regionalCenter)) {
                    reg[j+1] = reg[j];
                    j--;
                }
                reg[j+1] = temp;
            }
            break;
    }
}

void sortContents(Regions *reg, int size)
{
    if (reg == nullptr)
    {
        cout << "Вы ещё не открыли никакого файла." << endl;
        return;
    }
    int field;
    bool ascending; //Сортировка по возрастанию
    char answerAscending; //(y|n) Ввод пользователя

    cout << "По какому полю сортировать?" << endl;
    cout << "1-код, 2-губернатор, 3-площадь, 4-население, 5-центр." << endl;
    cin >> field;

    if (field < 1 || field > 5)
    {
        cout << "Поля с таким номером не существует." << endl;
        return;
    }

    field--; //Нумерация начинается с нуля

    cout << "По возрастанию? Иначе по убыванию (y/n) " << endl;
    cin >> answerAscending;

    if (answerAscending == 'y')
        ascending = true;
    else if (answerAscending == 'n')
        ascending = false;
    else
    {
        cout << "Ожидался ввод \"y\" или \"n\"." << endl;
        return;
    }

    if (isSorted(reg, size, field, ascending))
    {
        cout << "Массив уже отсортирован по этому полю." << endl;
        return;
    }

    insertionSort(reg, size, field, ascending);
}

void findContents(Regions *reg, int size)
{
    int fieldSearch; //Поле, по которому надо искать
    string key; //Что ищет пользователь
    vector<Regions> foundEntries; //Найденные записи

    cout << "По какому полю искать?" << endl;
    cout << "1-код, 2-губернатор, 3-площадь, 4-население, 5-центр." << endl;
    cin >> fieldSearch;

    if (fieldSearch < 1 || fieldSearch > 5)
    {
        cout << "Поля с таким номером не существует." << endl;
        return;
    }

    fieldSearch--; //Нумерация начинается с нуля

    cout << "Ищем: " << endl;
    cin.ignore();
    getline(cin, key);

    switch (fieldSearch)
    {
        case 0: //код
            for (int i = 0; i < size; i++)
                if (reg[i].code == stoi(key))
                    foundEntries.push_back(reg[i]);
            break;
        case 1: //губернатор
            for (int i = 0; i < size; i++)
                if (reg[i].governor == key)
                    foundEntries.push_back(reg[i]);
            break;
        case 2: //площадь
            for (int i = 0; i < size; i++)
                if (reg[i].area == stoi(key))
                    foundEntries.push_back(reg[i]);
            break;
        case 3: //население
            for (int i = 0; i < size; i++)
                if (reg[i].population == stoi(key))
                    foundEntries.push_back(reg[i]);
            break;
        case 4: //центр
            for (int i = 0; i < size; i++)
                if (reg[i].regionalCenter == key)
                    foundEntries.push_back(reg[i]);
            break;
    }

    if (foundEntries.size() != 0)
    {
        cout << "Результаты: " << endl;

        //printTable работает с сырым указателем
        printTable(foundEntries.data(), foundEntries.size());
    }
    else
    {
        cout << "Ничего не найдено." << endl;
    }
}

void saveToFile(Regions *reg, int size, bool &isSaved)
{
    if (reg == nullptr)
    {
        cout << "Вы ещё ничего не открыли." << endl;
        return;
    }
    ofstream fout;
    string writePath;
    
    cout << "Введите имя файла." << endl;
    cin.ignore();
    getline(cin, writePath);

    fout.open(writePath);
    
    for (int i = 0; i < size; i++)
    {
        fout << reg[i].code << ' ';
        fout << reg[i].governor << ' ';
        fout << reg[i].area << ' ';
        fout << reg[i].population << ' ';
        fout << reg[i].regionalCenter << endl;
    }

    isSaved = true;

    fout.close();
}

void loadFromFile(Regions *&reg, int &size, bool isSaved)
{
    ifstream fin;
    string readPath; //Путь к открываемому файлу

    string currentLine; //Текущая строка файла
    int lines = 0; //Счётчик строк файла

    char confirmContinue;

    //Если в памяти есть несохранённые изменения
    if (!isSaved)
    {
        cout << "Предупреждение. Есть несохранённые данные." << endl;
        cout << "Продолжить? (y/n) ";
        cin >> confirmContinue;
        if (confirmContinue != 'y')
        {
            //Уходим отсюда
            return;
        }
    }

    //Очищаем память
    delete [] reg;
    reg = nullptr;

    cout << "Введите путь к файлу: " << endl;
    cin.ignore();
    getline(cin, readPath);

    fin.open(readPath);

    //Если такой файл существует на диске
    if (fin.is_open())
    {
        //Считать количество строк
        while (getline(fin, currentLine))
            lines++;

        size = lines;

        fin.close();
        fin.open(readPath); //Чтобы считывать файл с начала

        //Выделить память под массив
        reg = new Regions[size];

        string tempGovernor; //Для временного хранения Ф, И, О

        //Парсить файл
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

        fin.close();
    }
    else
    {
        cout << "Такого файла нет." << endl;
    }
}
