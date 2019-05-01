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

#define DEBUG

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

//Приглашение командной строки
void printPrompt();
//Помощь
void printCommands();
//Предупреждение о несохранённых данных
void AskContinue();
//Массив пуст
void printNoData();
//Если пользователь ввёл не y, и не n
void printExpectedYN();

//Очистить массив в памяти
void createNewContents(Regions *&reg, int &size, bool &isFromFile, bool &isSaved, string &path);

//Интерфейс + реализация добавления элемента
void addElement(Regions *&reg, int &size, bool &isSaved, bool isFromFile);

//Релизация удаления элемента по его номеру
void deleteContentsByNumber(Regions *&reg, int &size, int position);
//Интерфейс удаления элементов
void interfaceDeleteContents(Regions *&reg, int &size, bool &isSaved);

//Реализация вывода массива в табличной форме
void printTable(Regions *reg, int size);
void printTable(Regions *reg, int size, vector<int> positions);
//Интерфейс печати массива
void printContents(Regions *reg, int size);

//Является ли массив уже отсортированным
bool isSorted(Regions *reg, int size, int field, bool ascending);
//Реализация сортировки методом вставок
void insertionSort(Regions *reg, int size, int field, bool ascending);
//Интерфейс сортировки
void sortContents(Regions *reg, int size, bool &isSaved);

//Реализация поиска
vector<int> findContents(Regions *reg, int size, int field, string key);
//Интерфейс поиска
void interfaceFindContentsToShow(Regions *reg, int size);

//Реализация сохранения
void saveAs(Regions *reg, int size, string path);
//Интерфейс сохранения
void interfaceSave(Regions *reg, int size, bool &isSaved, bool &isFromFile, string &path);

//Интерфейс + реализация загрузки
void loadFromFile(Regions *&reg, int &size, bool &isSaved, bool &isFromFile, string &path);

int main()
{
    char command; //Команда пользователя

    Regions *regions = nullptr; //База данных в оперативной памяти
    int size = 0; //Количество записей
    bool changesAreSaved = true; //Сохранены ли изменения
    string filePath; //По какому пути открыт файл, чтобы сохранять с тем же именем
    char confirmExit; //(y|n) Выйти без сохранения
    bool arrayIsFromFile = false; //Для сохранения в файл с тем же именем

    cout << "+------------------------------------------------+" << endl;
    cout << "| Программа, позволяющая работать с базой данных |" << endl;
    cout << "| субъектов Российской Федерации                 |" << endl;
    cout << "+------------------------------------------------+" << endl;
    cout << endl;

    printCommands();
    cout << endl;

    cout << "Введите команду. (Список команд можно посмотреть, введя \"h\")." << endl;

    while (true) 
    {
        printPrompt();
        cin >> command;

        switch (command)
        {
            //Создание нового массива
            case 'c':
                createNewContents(regions, size, arrayIsFromFile, changesAreSaved, filePath);
                break;
            //Добавление элемента
            case 'a':
                addElement(regions, size, changesAreSaved, arrayIsFromFile);
                break;
            //Удаление записи из массива
            case 'd':
                interfaceDeleteContents(regions, size, changesAreSaved);
                break;
            //Печать базы данных
            case 'l':
                printContents(regions, size);
                break;
            //Поиск элемента по заданному полю
            case 'f':
                interfaceFindContentsToShow(regions, size);
                break;
            //Сортировка по заданному полю
            case 's':
                sortContents(regions, size, changesAreSaved);
                break;
            //Сохранение в файл
            case 'w':
                interfaceSave(regions, size, changesAreSaved, arrayIsFromFile, filePath);
                break;
            //Чтение из файла
            case 'r':
                loadFromFile(regions, size, changesAreSaved, arrayIsFromFile, filePath);
                break;
            //Помощь
            case 'h':
                printCommands();
                break;
            //Выход
            case 'q':
                if (changesAreSaved)
                    goto EXIT;

                AskContinue();
                cin >> confirmExit;

                if (confirmExit == 'y')
                    goto EXIT;
                else
                    break;
            //Отладочные данные
#ifdef DEBUG
            case '0':
                cout << "isSaved=" << changesAreSaved << " isFromFile=" << arrayIsFromFile << " path=" << filePath << endl;
                break;
#endif
            //Неизвестная команда
            default:
                cout << "Такой команды нет. Для просмотра доступных команд введите \"h\"" << endl;
        }

    }

EXIT:

    cout << "Завершение..." << endl;

    return 0;
}

//==========Приглашение ввести команду==========
void printPrompt()
{
    cout << "> ";
}

//==========Помощь. Список доступных команд==========
void printCommands()
{
    cout << "+------------------------------------------+" << endl;
    cout << "| c - создать новый пустой массив          |" << endl;
    cout << "| a - ввод нового элемента в массив        |" << endl;
    cout << "| d - удаление элемента по его позиции     |" << endl;
    cout << "| l - печать всего массива                 |" << endl;
    cout << "| f - поиск элемента в массиве             |" << endl;
    cout << "| s - сортировка массива по заданному полю |" << endl;
    cout << "| w - сохранение массива в файле           |" << endl;
    cout << "| r - чтение массива из файла              |" << endl;
    cout << "+------------------------------------------+" << endl;
    cout << "| h - список команд                        |" << endl;
    cout << "| q - выход из программы                   |" << endl;
    cout << "+------------------------------------------+" << endl;
}

void printNoData()
{
    cout << "Массив данных пуст." << endl;
}

void printExpectedYN()
{
    cout << "Ожидался ввод \"y\" или \"n\"." << endl;
}

void AskContinue()
{
    cout << "Есть несохранённые данные. Продолжить? (y/n) ";
}

void createNewContents(Regions *&reg, int &size, bool &isFromFile, bool &isSaved, string &path)
{
    if (reg == nullptr)
    {
        printNoData();
        return;
    }

    char confirmContinue; //(y|n) Продолжить

    if (!isSaved)
    {
        AskContinue();
        cin >> confirmContinue;
        if (confirmContinue == 'n')
            return;
        else if (confirmContinue != 'y')
        {
            printExpectedYN();
            return;
        }
    }

    delete [] reg; //Очистка памяти
    reg = nullptr;
    size = 0;
    path = "";
    isFromFile = false;
    isSaved = true; //т. к. нечему быть несохранённым

    cout << "Массив данных очистился." << endl;
}

//==========Печать данных, поданных на вход, в виде таблицы==========
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

//Печатать только те данные, номера котрых указаны в positions
void printTable(Regions *reg, int size, vector<int> positions)
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
    for (int i = 0; i < positions.size(); i++)
    {
        cout << setw(colWidth[0]) << reg[positions[i]].code;
        cout << setw(colWidth[1]) << reg[positions[i]].governor;
        cout << right << setw(colWidth[2] - 3) << reg[positions[i]].area << "   ";
        cout << right << setw(colWidth[3] - 3) << reg[positions[i]].population << "   ";
        cout << left << setw(colWidth[4]) << reg[positions[i]].regionalCenter;
        cout << endl;
    }
    cout << string(tableWidth, '-') << endl;
}

//==========1 - добавление нового элемента с консоли==========
void addElement(Regions *&reg, int &size, bool &isSaved, bool isFromFile)
{
    if (reg == nullptr && !isFromFile)
    {
        char answerCreateFirst; //(y|n) Ответ, добавлять ли первую запись
        printNoData();
        cout << "Хотите создать первую запись? (y/n) ";
        cin >> answerCreateFirst;

        if (answerCreateFirst == 'n')
        {
            return;
        }
        else if (answerCreateFirst != 'y')
        {
            printExpectedYN();
            return;
        }
    }

    Regions *moreReg = new Regions[size + 1];
    Regions newReg;

    //Ввод нового элемента
    cout << "Введите элемент." << endl;
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

void deleteContentsByNumber(Regions *&reg, int &size, int position)
{
    Regions *lessReg = new Regions[size - 1]; 
    
    for (int i = 0; i < position; i++)
        lessReg[i] = reg[i];
    for (int i = position; i < size-1; i++)
        lessReg[i] = reg[i+1];

    delete [] reg;
    reg = lessReg;
    size--;

    if (size == 0)
        reg = nullptr;
}

void interfaceDeleteContents(Regions *&reg, int &size, bool &isSaved)
{
    if (reg == nullptr)
    {
        printNoData();
        return;
    }

    cout << "Если удалять элемент не нужно, введите число, меньшее 1." << endl;

    int positionDelete; //Номер элемента для удаления

    cout << "Позиция: ";
    cin >> positionDelete;
    positionDelete--; //Нумерация начинается с нуля


    if (0 <= positionDelete && positionDelete < size)
    {
        //Для показа уже удалённого элемента
        Regions *oneDeletedElement = new Regions[1];
        oneDeletedElement[0] = reg[positionDelete];

        //Сам процесс удаления
        deleteContentsByNumber(reg, size, positionDelete);

        cout << "Элемент " << positionDelete + 1 << " удалён." << endl << endl;

        printTable(oneDeletedElement, 1);

        delete [] oneDeletedElement;

        isSaved = false;
    }
    else
    {
        cout << "Такого номера нет." << endl;
        return;
    }
}

//==========2 - Печать всего массива==========
void printContents(Regions *reg, int size)
{
    if (reg == nullptr)
    {
        printNoData();
        return;
    }
    printTable(reg, size);
}

//==========Отсортирован ли уже массив==========
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

//==========Реализация сортировки вставками по заданному полю==========
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

//Возвращает номера записей, соответствующих ключу
vector<int> findContents(Regions *reg, int size, int field, string key)
{
    vector<int> resultNumbers;
    switch (field)
    {
        case 0: //код
            for (int i = 0; i < size; i++)
                if (reg[i].code == stoi(key))
                    resultNumbers.push_back(i);
            break;
        case 1: //губернатор
            for (int i = 0; i < size; i++)
                if (reg[i].governor == key)
                    resultNumbers.push_back(i);
            break;
        case 2: //площадь
            for (int i = 0; i < size; i++)
                if (reg[i].area == stoi(key))
                    resultNumbers.push_back(i);
            break;
        case 3: //население
            for (int i = 0; i < size; i++)
                if (reg[i].population == stoi(key))
                    resultNumbers.push_back(i);
            break;
        case 4: //центр
            for (int i = 0; i < size; i++)
                if (reg[i].regionalCenter == key)
                    resultNumbers.push_back(i);
            break;
    }
    return resultNumbers;
}

//==========3 - поиск данных по точному соответствию==========
void interfaceFindContentsToShow(Regions *reg, int size)
{
    if (reg == nullptr)
    {
        printNoData();
        return;
    }

    int fieldSearch; //Поле, по которому надо искать
    string searchKey; //Что ищет пользователь
    vector<int> foundNumbers; //Найденные записи

    cout << "По какому полю искать?" << endl;
    cout << "1-код, 2-губернатор, 3-площадь, 4-население, 5-центр: ";
    cin >> fieldSearch;

    if (fieldSearch < 1 || fieldSearch > 5)
    {
        cout << "Поля с таким номером не существует." << endl;
        return;
    }

    fieldSearch--; //Нумерация начинается с нуля

    cout << "Ищем: ";
    cin.ignore();
    getline(cin, searchKey);

    foundNumbers = findContents(reg, size, fieldSearch, searchKey);

    if (foundNumbers.size() != 0)
    {
        cout << endl << "Результаты: " << endl << endl;

        printTable(reg, size, foundNumbers);
    }
    else
    {
        cout << "Ничего не найдено." << endl;
    }
}

//==========4 - сортировка по заданному полю==========
void sortContents(Regions *reg, int size, bool &isSaved)
{
    if (reg == nullptr)
    {
        printNoData();
        return;
    }
    int field;
    bool ascending; //Сортировка по возрастанию
    char answerAscending; //(y|n) Ввод пользователя

    cout << "По какому полю сортировать?" << endl;
    cout << "1-код, 2-губернатор, 3-площадь, 4-население, 5-центр: ";
    cin >> field;

    if (field < 1 || field > 5)
    {
        cout << "Поля с таким номером не существует." << endl;
        return;
    }

    field--; //Нумерация начинается с нуля

    cout << "По возрастанию? Иначе по убыванию (y/n) ";
    cin >> answerAscending;

    if (answerAscending == 'y')
        ascending = true;
    else if (answerAscending == 'n')
        ascending = false;
    else
    {
        printExpectedYN();
        return;
    }

    if (isSorted(reg, size, field, ascending))
    {
        cout << "Массив уже отсортирован по этому полю." << endl;
        return;
    }

    insertionSort(reg, size, field, ascending);

    isSaved = false;

    cout << endl << "Результат сортировки: " << endl << endl;
    printTable(reg, size);
}


//==========5 - сохранение в текстовый файл==========
void interfaceSave(Regions *reg, int size, bool &isSaved, bool &isFromFile, string &path)
{
    char confirmSave;

    if (reg == nullptr)
    {
        printNoData();
        cout << "Всё равно сохранить? (y/n) ";
        cin >> confirmSave;

        if (confirmSave == 'n')
            return;
        else if (confirmSave != 'y')
        {
            printExpectedYN();
            return;
        }
    }
    
    //Если файл с таким именем был открыт/сохранён ранее
    if (isFromFile)
    {
        cout << "Сохранить как \"" << path << "\"? ([y]es/save [a]s/[c]ancel) ";
        cin >> confirmSave;
        if (confirmSave == 'c')
            return;
        else if (confirmSave != 'y' && confirmSave != 'a')
        {
            cout << "Ожидался ввод \"y\", \"a\" или \"c\"." << endl;
            return;
        }
    }
    if (!isFromFile || confirmSave == 'a')
    {
        cout << "Введите путь к файлу: ";
        cin.ignore();
        getline(cin, path);
    }

    //Сам процесс сохранения
    saveAs(reg, size, path);

    isFromFile = true; //Потому что данные в памяти и в файле совпадают
    isSaved = true;

    cout << "Массив сохранён в файле \"" << path << "\"." << endl;
}

void saveAs(Regions *reg, int size, string path)
{
    ofstream fout;
    fout.open(path);
    
    for (int i = 0; i < size; i++)
    {
        fout << reg[i].code << ' ';
        fout << reg[i].governor << ' ';
        fout << reg[i].area << ' ';
        fout << reg[i].population << ' ';
        fout << reg[i].regionalCenter << endl;
    }
    fout.close();
}

//==========6 - чтение в память из файла==========
void loadFromFile(Regions *&reg, int &size, bool &isSaved, bool &isFromFile, string &path)
{
    ifstream fin;

    string currentLine; //Текущая строка файла
    int lines = 0; //Счётчик строк файла

    char confirmContinue;

    //Если в памяти есть несохранённые изменения
    if (!isSaved)
    {
        AskContinue();
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

    cout << "Введите путь к файлу: ";
    cin.ignore();
    getline(cin, path);

    fin.open(path);

    //Если такого файла нет на диске
    if (!fin.is_open())
    {
        cout << "Такого файла нет." << endl;
        return;
    }

    //Считать количество строк
    while (getline(fin, currentLine))
        lines++;

    size = lines;

    fin.close();
    fin.open(path); //Чтобы считывать файл с начала

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

    isSaved = true;
    isFromFile = true;

    fin.close();

    cout << "Чтение файла \"" << path << "\" закончено." << endl;

    cout << endl << "Содержимое:" << endl << endl;
    printTable(reg, size);
}
