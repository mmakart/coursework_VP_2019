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
void printPrompt(string path);
//Помощь
void printCommands();
//Предупреждение о несохранённых данных
void AskContinue();
//Массив пуст
void printNoData();
//Если пользователь ввёл не y, и не n
void printExpectedYN();
//Если пользователь ввёл не число
void printExpectedNumber();

//Очистить массив в памяти
void createNewContents(Regions *&reg, int &size, bool &isFromFile, bool &isSaved, string &path);

//Интерфейс + реализация добавления элемента
void addElement(Regions *&reg, int &size, bool &isSaved, bool isFromFile);

//Интерфейс + реализация редактирования полей элемента
void editContents(Regions *reg, int size, bool &isSaved);

//Релизация удаления элемента по его номеру
void deleteContentsByNumber(Regions *&reg, int &size, int position);
//Интерфейс удаления элементов
void interfaceDeleteContents(Regions *&reg, int &size, bool &isSaved);

//Реализация вывода массива в табличной форме
void printTable(Regions *reg, int size);
void printTable(Regions *reg, int size, vector<int> positions);
void printTable(Regions reg);
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

//Реализация сохранения, возвращает код успеха/ошибки
int saveAs(Regions *reg, int size, string path);
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
    bool arrayIsFromFile = false; //Для сохранения в файл с тем же именем
    string filePath; //По какому пути открыт файл, чтобы сохранять с тем же именем

    char confirmExit; //(y|n) Выйти без сохранения

    cout << "+------------------------------------------------+" << endl;
    cout << "| Программа, позволяющая работать с базой данных |" << endl;
    cout << "| субъектов Российской Федерации                 |" << endl;
    cout << "+------------------------------------------------+" << endl;
    cout << endl;

    //Первоначальный вывод списка команд
    printCommands();

    cout << "Введите команду. (Список команд можно посмотреть, введя \"h\")." << endl;

    while (true) 
    {
        //Приглашение на ввод команды
        printPrompt(filePath);
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
            //Редактирование элемента
            case 'e':
                editContents(regions, size, changesAreSaved);
                break;
            //Удаление записи из массива
            case 'd':
                interfaceDeleteContents(regions, size, changesAreSaved);
                break;
            //Печать массива
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
                cout << "nullptr=" << (regions == nullptr);
                cout << " size=" << size;
                cout << " isSaved=" << changesAreSaved;
                cout << " isFromFile=" << arrayIsFromFile;
                cout << " path=" << filePath << endl;
                break;
#endif
            //Неизвестная команда
            default:
                cout << "Такой команды нет. Для просмотра доступных команд введите \"h\"." << endl;
        }

    }

EXIT:

    delete [] regions;
    cout << "Завершение..." << endl;

    return 0;
}

//========== Приглашение ввести команду ==========
void printPrompt(string path)
{
    cout << path << "> ";
}

//========== Помощь. Список доступных команд ==========
void printCommands()
{
    cout << "+-------------------------------------------------+" << endl;
    cout << "| [c]reate - создать новый пустой массив          |" << endl;
    cout << "| [a]dd    - ввод нового элемента в массив        |" << endl;
    cout << "| [e]dit   - редактирование элемента              |" << endl;
    cout << "| [d]elete - удаление элемента по его позиции     |" << endl;
    cout << "| [l]ist   - печать всего массива                 |" << endl;
    cout << "| [f]ind   - поиск элемента в массиве             |" << endl;
    cout << "| [s]ort   - сортировка массива по заданному полю |" << endl;
    cout << "| [w]rite  - сохранение массива в файле           |" << endl;
    cout << "| [r]ead   - чтение массива из файла              |" << endl;
    cout << "+-------------------------------------------------+" << endl;
    cout << "| [h]elp   - список команд                        |" << endl;
    cout << "| [q]uit   - выход из программы                   |" << endl;
    cout << "+-------------------------------------------------+" << endl;
}

void printNoData()
{
    cout << "Массив данных пуст." << endl;
}

void printExpectedNumber()
{
    cout << "Ошибка считывания числа: неверный формат." << endl;
}

void printExpectedYN()
{
    cout << "Ожидался ввод \"y\" или \"n\"." << endl;
}

void AskContinue()
{
    cout << "Есть несохранённые данные. Продолжить? (y/n) ";
}

void printBadFileData(string path, int position)
{
    cout << "Ошибка чтения файла \"" << path << "\"." << endl;
    cout << "Одно из полей в строке " << position + 1 << " должно быть числом." << endl;
}

//========== Очистка текущего массива и создание нового ==========
void createNewContents(Regions *&reg, int &size, bool &isFromFile, bool &isSaved, string &path)
{
    if (reg == nullptr && !isFromFile)
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

//========== Печать данных, поданных на вход, в виде таблицы ==========
void printTable(Regions *reg, int size)
{
    const int colNumber = 5; //Количество столбцов
    static int colWidth[colNumber] = {16, 31, 13, 13, 22}; //Ширина каждого столбца
    static string tableHeaders[colNumber] =
    {
        "Regional code", "Governor", "Area, km^2", "Population", "Regional center"
    };

    //Подсчёт ширины таблицы
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

//========== Печатать только те данные, номера которых указаны в positions ==========
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

//Печать в виде таблицы единственного элемента
void printTable(Regions reg)
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
    cout << setw(colWidth[0]) << reg.code;
    cout << setw(colWidth[1]) << reg.governor;
    cout << right << setw(colWidth[2] - 3) << reg.area << "   ";
    cout << right << setw(colWidth[3] - 3) << reg.population << "   ";
    cout << left << setw(colWidth[4]) << reg.regionalCenter;
    cout << endl;

    cout << string(tableWidth, '-') << endl;
}

//========== 1 - добавление нового элемента с консоли ==========
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

    //Новый массив данных с размером на единицу большим
    Regions *moreReg = new Regions[size + 1];
    //Добавляемый элемент
    Regions newReg;

    //Временные переменные
    string family, name, patronymic, strCode, strArea, strPopulation;

    //Ввод нового элемента
    cout << "Введите элемент." << endl;

    cout << "Код региона: ";
    cin >> strCode;
    //Проверка на то, что введённые данные - число
    try {
        newReg.code = stoi(strCode);
    }
    catch(exception ex) {
        printExpectedNumber();
        return;
    }

    cout << "Фамилия губернатора: ";
    cin >> family;
    cout << "Имя губернатора: ";
    cin >> name;
    cout << "Отчество губернатора: ";
    cin >> patronymic;
    newReg.governor = family + " " + name + " " + patronymic;

    cout << "Площадь: ";
    cin >> strArea;
    //Проверка на то, что введённые данные - число
    try {
        newReg.area = stoi(strArea);
    }
    catch(exception ex) {
        printExpectedNumber();
        return;
    }
    cout << "Население: ";
    cin >> strPopulation;
    //Проверка на то, что введённые данные - число
    try {
        newReg.population = stoi(strPopulation);
    }
    catch(exception ex) {
        printExpectedNumber();
        return;
    }
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

    //Очистка старого ненужного массива
    delete [] reg;
    //Теперь он указывает на новые данные
    reg = moreReg;
    size++;

    //Теперь есть несохранённые изменения
    isSaved = false;
}

//========== Редактирование элемента ==========
void editContents(Regions *reg, int size, bool &isSaved)
{
    if (reg == nullptr)
    {
        printNoData();
        return;
    }

    //Номер записи для редактирования
    int positionEdit;
    //Для проверки на правильность ввода числа
    string strPositionEdit;

    //Поле записи для редактирования
    int fieldEdit;
    //Для проверки на правильность ввода числа
    string strFieldEdit;

    //Временные переменные для неприкосновенности основного массива при ошибке
    int code;
    string governor;
    int area, population;
    string regionalCenter;

    //Временные переменные для проверки на то,
    //что введённые данные были числом
    string strCode, strArea, strPopulation;

    //Для временного хранения Ф, И, О по отдельности
    string family, name, patronymic;
    //Для хранения номера редактируемого элемента
    vector<int> vectorPositionEdit;

    cout << "Какую запись редактировать? ";
    cin >> strPositionEdit;
    //Проверка на то, что введённые данные - число
    try {
        positionEdit = stoi(strPositionEdit);
    }
    catch (exception ex) {
        printExpectedNumber();
        return;
    }

    positionEdit--; //Нумерация начинается с нуля

    if (positionEdit < 0 || positionEdit >= size)
    {
        cout << "Такой записи нет." << endl;
        return;
    }

    //Для показа пользователю редактируемого элемента отдельно
    Regions oneEditedElement = reg[positionEdit];
    vectorPositionEdit.push_back(positionEdit);

    cout << "Вы выбрали запись: " << endl;
    printTable(reg, size, vectorPositionEdit);

    cout << "Какое поле заменить?" << endl;
    cout << "1-код, 2-губернатор, 3-площадь, 4-население, 5-центр: ";
    cin >> strFieldEdit;
    //Проверка на то, что введённые данные - число
    try {
        fieldEdit = stoi(strFieldEdit);
    }
    catch (exception ex) {
        printExpectedNumber();
        return;
    }

    fieldEdit--; //Нумерация начинается с нуля

    if (fieldEdit < 0 || fieldEdit >= 5)
    {
        cout << "Такого поля нет." << endl;
        return;
    }

    cout << "Введите новое значение: ";

    switch (fieldEdit)
    {
        //Код региона
        case 0:
            cin >> strCode;
            //Проверка на то, что ввёденные данные - число
            try {
                code = stoi(strCode);
            }
            catch (exception ex) {
                printExpectedNumber();
                return;
            }
            reg[positionEdit].code = code; 
            break;
        //ФИО губернатора
        case 1:
            cout << "Фамилия: ";
            cin >> family;
            cout << "Имя: ";
            cin >> name;
            cout << "Отчество: ";
            cin >> patronymic;
            governor = family + ' ' + name + ' ' + patronymic;
            reg[positionEdit].governor = governor; 
            break;
        //Площадь
        case 2:
            cin >> strArea;
            //Проверка на то, что ввёденные данные - число
            try {
                area = stoi(strArea);
            }
            catch (exception ex) {
                printExpectedNumber();
                return;
            }
            reg[positionEdit].area = area; 
            break;
        //Население
        case 3:
            cin >> strPopulation;
            //Проверка на то, что ввёденные данные - число
            try {
                population = stoi(strPopulation);
            }
            catch (exception ex) {
                printExpectedNumber();
                return;
            }
            reg[positionEdit].population = population; 
            break;
        //Областной центр
        case 4:
            cin.ignore();
            getline(cin, regionalCenter);
            reg[positionEdit].regionalCenter = regionalCenter; 
            break;
    }

    //Для печати этого нового изменённого элемента
    oneEditedElement = reg[positionEdit];

    cout << "Отредактированная запись: " << endl;
    printTable(reg, size, vectorPositionEdit);

    isSaved = false;
}

//========== Релизация процесса удаления элемента ==========
void deleteContentsByNumber(Regions *&reg, int &size, int position)
{
    Regions *lessReg = new Regions[size - 1]; 
    
    //До позиции удаляемого элемента расположение данных не меняется
    for (int i = 0; i < position; i++)
        lessReg[i] = reg[i];
    //Все последующие элементы смещаются на одну позицию
    for (int i = position; i < size-1; i++)
        lessReg[i] = reg[i+1];

    //Очистка старого ненужного массива
    delete [] reg;
    //Теперь он указывает на новые данные
    reg = lessReg;
    size--;

    //Защита от отрицательного размера
    if (size == 0)
        reg = nullptr;
}

//========== Удаление элемента ==========
void interfaceDeleteContents(Regions *&reg, int &size, bool &isSaved)
{
    if (reg == nullptr)
    {
        printNoData();
        return;
    }

    cout << "Введите позицию от 1 до " << size << ". (0 для выхода)." << endl;

    int positionDelete; //Номер элемента для удаления
    vector<int> vectorPositionDelete;
    vectorPositionDelete.push_back(positionDelete);

    cout << "Позиция: ";
    cin >> positionDelete;
    positionDelete--; //Нумерация начинается с нуля

    if (positionDelete < 0 || positionDelete >= size)
    {
        cout << "Такого элемента нет." << endl;
        return;
    }

    //Для показа уже удалённого элемента
    Regions oneDeletedElement = reg[positionDelete];

    //Сам процесс удаления
    deleteContentsByNumber(reg, size, positionDelete);

    cout << "Элемент " << positionDelete + 1 << " удалён." << endl;

    //Печать удалённого элемента
    printTable(oneDeletedElement);
    cout << "Количество записей: " << size << endl;

    isSaved = false;
}

//========== 2 - Печать всего массива ==========
void printContents(Regions *reg, int size)
{
    if (reg == nullptr)
    {
        printNoData();
        return;
    }
    printTable(reg, size);
    cout << "Количество записей: " << size << endl;
}

//========== Отсортирован ли уже массив ==========
bool isSorted(Regions *reg, int size, int field, bool ascending)
{
    switch (field)
    {
        case 0: //код
            if (ascending)
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].code > reg[i+1].code)
                        return false;
            else
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].code < reg[i+1].code)
                        return false;
            break;
        case 1: //губернатор
            if (ascending)
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].governor > reg[i+1].governor)
                        return false;
            else
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].governor < reg[i+1].governor)
                        return false;
            break;
        case 2: //площадь
            if (ascending)
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].area > reg[i+1].area)
                        return false;
            else
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].area < reg[i+1].area)
                        return false;
            break;
        case 3: //население
            if (ascending)
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].population > reg[i+1].population)
                        return false;
            else
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].population < reg[i+1].population)
                        return false;
            break;
        case 4: //центр
            if (ascending)
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].regionalCenter > reg[i+1].regionalCenter)
                        return false;
            else
                for (int i = 0; i < size - 1; i++)
                    if (reg[i].regionalCenter < reg[i+1].regionalCenter)
                        return false;
            break;
    }
    return true;
}

//========== Реализация сортировки вставками по заданному полю ==========
void insertionSort(Regions *reg, int size, int field, bool ascending)
{
    switch (field)
    {
        //Код региона
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
        //ФИО губернатора
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
        //Площадь
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
        //Население
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
        //Областной центр
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
                if (reg[i].governor.find(key) != string::npos)
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
                if (reg[i].regionalCenter.find(key) != string::npos)
                    resultNumbers.push_back(i);
            break;
    }
    return resultNumbers;
}

//========== 3 - поиск данных по точному соответствию ==========
void interfaceFindContentsToShow(Regions *reg, int size)
{
    if (reg == nullptr)
    {
        printNoData();
        return;
    }

    int fieldSearch; //Поле, по которому надо искать
    string strFieldSearch; //Для прорверки на правильность ввода
    string searchKey; //Что ищет пользователь
    //Массив позиций в массиве найденных элементов
    vector<int> foundNumbers;

    cout << "По какому полю искать?" << endl;
    cout << "1-код, 2-губернатор, 3-площадь, 4-население, 5-центр: ";
    cin >> strFieldSearch;
    //Проверка на то, что ввёденные данные - число
    try {
        fieldSearch = stoi(strFieldSearch);
    }
    catch (exception ex) {
        printExpectedNumber();
        return;
    }

    if (fieldSearch < 1 || fieldSearch > 5)
    {
        cout << "Поля с таким номером не существует." << endl;
        return;
    }

    fieldSearch--; //Нумерация начинается с нуля

    cout << "Введите ключ поиска: ";
    cin.ignore();
    getline(cin, searchKey);

    foundNumbers = findContents(reg, size, fieldSearch, searchKey);

    if (foundNumbers.size() == 0)
    {
        cout << "Ничего не найдено." << endl;
        return;
    }
    cout << "Результаты поиска: " << endl;
    printTable(reg, size, foundNumbers);
}

//========== 4 - сортировка по заданному полю ==========
void sortContents(Regions *reg, int size, bool &isSaved)
{
    if (reg == nullptr)
    {
        printNoData();
        return;
    }
    int fieldSort; //Поле, по которому сортировать
    string strFieldSort; //Для проверки на правильность ввода
    bool ascending; //Сортировка по возрастанию
    char answerAscending; //(y|n) Ввод пользователя

    cout << "По какому полю сортировать?" << endl;
    cout << "1-код, 2-губернатор, 3-площадь, 4-население, 5-центр: ";
    cin >> strFieldSort;
    //Проверка на то, что считываемые данные - число
    try {
        fieldSort = stoi(strFieldSort);
    }
    catch(exception ex) {
        printExpectedNumber();
        return;
    }

    if (fieldSort < 1 || fieldSort > 5)
    {
        cout << "Поля с таким номером не существует." << endl;
        return;
    }

    fieldSort--; //Нумерация начинается с нуля

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

    if (isSorted(reg, size, fieldSort, ascending))
    {
        cout << "Массив уже отсортирован по этому полю." << endl;
        return;
    }

    //Сам процесс сортировки
    insertionSort(reg, size, fieldSort, ascending);

    isSaved = false;

    cout << "Результат сортировки: " << endl;
    printTable(reg, size);
}


//========== 5 - сохранение в текстовый файл ==========
void interfaceSave(Regions *reg, int size, bool &isSaved, bool &isFromFile, string &path)
{
    //Подтверждение сохранения пустого массива
    char confirmSaveEmpty;
    //Выбор сохранения
    char answerSave;
 
    //Для неприкосновенности переменной path в случае ошибки
    string savePath;

    //Удалось ли открыть файл для сохранения
    bool success;

    if (reg == nullptr)
    {
        printNoData();
        cout << "Всё равно сохранить? (y/n) ";
        cin >> confirmSaveEmpty;

        if (confirmSaveEmpty == 'n')
            return;
        else if (confirmSaveEmpty != 'y')
        {
            printExpectedYN();
            return;
        }
    }
    
    //Если файл с таким именем был открыт/сохранён ранее
    if (isFromFile)
    {
        cout << "Сохранить как \"" << path << "\" ([y]es/save [a]s/[c]ancel)? ";
        cin >> answerSave;
        if (answerSave == 'c')
            return;
        else if (answerSave != 'y' && answerSave != 'a')
        {
            cout << "Ожидался ввод \"y\", \"a\" или \"c\"." << endl;
            return;
        }
        savePath = path;
    }
    else if (answerSave == 'a')
    {
        cout << "Введите имя файла или полный путь к нему: ";
        cin.ignore();
        getline(cin, savePath);
    }

    //Сам процесс сохранения
    success = (saveAs(reg, size, savePath) == 0 ? true : false);

    if (!success)
    {
        cout << "Не удалось сохранить файл по пути \"" << savePath << "\"." << endl;
        return;
    }
    isFromFile = true; //Потому что данные в памяти и в файле совпадают
    isSaved = true;
    path = savePath;

    cout << "Массив сохранён в файле \"" << savePath << "\"." << endl;
}

//========== Реализация процесса сохранения ==========
int saveAs(Regions *reg, int size, string path)
{
    ofstream fout;
    fout.open(path);

    //Если не получилось открыть файл для записи
    if (!fout.is_open())
        return -1;
    
    for (int i = 0; i < size; i++)
    {
        fout << reg[i].code << ' ';
        fout << reg[i].governor << ' ';
        fout << reg[i].area << ' ';
        fout << reg[i].population << ' ';
        fout << reg[i].regionalCenter << endl;
    }
    fout.close();

    return 0;
}

//==========6 - чтение в память из файла==========
void loadFromFile(Regions *&reg, int &size, bool &isSaved, bool &isFromFile, string &path)
{
    ifstream fin;

    string currentLine; //Текущая строка файла
    int lines = 0; //Счётчик строк файла

    //Для неприкосновенности переменной path в случае ошибки
    string readPath;

    //Подтвердить, что несохранённые даннные могут быть потеряны
    char confirmContinue;

    //Если в памяти есть несохранённые изменения
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

    cout << "Введите имя файла или полный путь к нему: ";
    cin.ignore();
    getline(cin, readPath);

    fin.open(readPath);

    //Если такого файла нет на диске
    if (!fin.is_open())
    {
        cout << "Такого файла нет." << endl;
        return;
    }

    //Считать количество строк
    while (getline(fin, currentLine))
        lines++;

    fin.close();
    fin.open(readPath); //Чтобы снова считывать файл с начала

    //Временный массив для неприкосновенности текущих данных
    //в случае ошибки чтения
    Regions *dangerReg = new Regions[lines];

    //Временные переменные
    string tempGovernor, strCode, strArea, strPopulation;

    //Парсить файл
    for (int i = 0; i < lines; i++)
    {
        fin >> strCode;
        //Проверка на то, что считываемые данные - число
        try {
            dangerReg[i].code = stoi(strCode);
        }
        catch (exception ex) {
            printBadFileData(readPath, i);
            delete [] dangerReg;
            fin.close();
            return;
        }

        fin >> tempGovernor;
        dangerReg[i].governor += tempGovernor + ' ';
        fin >> tempGovernor;
        dangerReg[i].governor += tempGovernor + ' ';
        fin >> tempGovernor;
        dangerReg[i].governor += tempGovernor;
        fin >> strArea;

        //Проверка на то, что считываемые данные - число
        try {
            dangerReg[i].area = stoi(strArea);
        }
        catch (exception ex) {
            printBadFileData(readPath, i);
            delete [] dangerReg;
            fin.close();
            return;
        }
        fin >> strPopulation;

        //Проверка на то, что считываемые данные - число
        try {
            dangerReg[i].population = stoi(strPopulation);
        }
        catch (exception ex) {
            printBadFileData(readPath, i);
            delete [] dangerReg;
            fin.close();
            return;
        }
        fin >> dangerReg[i].regionalCenter;
    }

    //Очищаем оригинальный массив
    delete [] reg;
    //Теперь он указывает уже на проверенные считанные данные
    reg = dangerReg;

    size = lines;
    isSaved = true;
    isFromFile = true;
    path = readPath;

    fin.close();

    cout << "Чтение файла \"" << readPath << "\" успешно завершено." << endl;

    cout << "Содержимое:" << endl;
    printTable(reg, size);
    cout << "Количество записей: " << size << endl;
}
