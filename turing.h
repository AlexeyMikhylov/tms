#ifndef TURING_H
#define TURING_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

using namespace std;

// Константы //

const int MAX_ROWS = 100;
const int MAX_COLS = 100;

// Структура //

struct TuringMachine
{
    string programm[MAX_ROWS][MAX_COLS];
    int row = 0;
    int col = 0;

    string command;
    char* command_ = nullptr;
    char a = ' ';
    int q = 1;
    int headPosition = 1;

    char* tape = nullptr;
    int tape_len = 0;

    char* alphabet = nullptr;
    int alphabet_len = 0;

    int counter = 0;
};

// Функции //

// Инициализация машины
void init(TuringMachine& tm, string filename);

// Чтение файла
int loadFile(string filename, TuringMachine& tm, string data[][MAX_COLS], int row);

// Получение алфавита машины
void getAlphabet(TuringMachine& tm);

// Выделение памяти символьного массива
char* sizeCharArray(int length, char* array);

// Перевод строки в символьный массив
char* stringToChar(string str, char* arr, int i);

// Интерфейс программы
void controlPanel(TuringMachine& tm);

// Ввод слова
void enterWord(TuringMachine& tm);

// Проверка слова
int checkWord(TuringMachine& tm);

// Запуск машины
void runMT(TuringMachine& tm);

// Вывод ленты конфигурации
void outputState(TuringMachine& tm);

// Запуск одной итерации машины
void runMTiter(TuringMachine& tm);

// Получение индекса элемента по значению
int getIndexByValue(char* array, int length, char value);

// Расширение ленты
void extendTape(TuringMachine& tm);

// Увеличение ленты
void resizeTape(TuringMachine& tm);

#endif
