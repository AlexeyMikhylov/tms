#include "turing.h"

// Инициализация машины
void init(TuringMachine& tm, string filename)
{
	tm.row = loadFile(filename, tm, tm.programm, tm.row);

	controlPanel(tm);
}

// Чтение файла
int loadFile(string filename, TuringMachine& tm, string data[][MAX_COLS], int row)
{
	ifstream file(filename);

	if (!file.is_open())
	{
		cerr << "Error while opening file" << endl;
		exit(0);
	}

	string line;
	while (getline(file, line) && row < MAX_ROWS)
	{
		stringstream ss(line);
		string cell;
		
		tm.col = 0;

		while (getline(ss, cell, ',') && tm.col < MAX_COLS)
		{
			data[row][tm.col] = cell;
			tm.col++;
		}

		row++;
	}
	cout << "Programm loaded succesfully." << endl;
	file.close();

	getAlphabet(tm);

	return row;
}

// Получение алфавита машины
void getAlphabet(TuringMachine& tm)
{
	string alphabet_str;

	for (int i = 1; i < tm.col; i++)
	{
		alphabet_str += tm.programm[0][i];
	}

	tm.alphabet_len = alphabet_str.length();

	tm.alphabet = sizeCharArray(tm.alphabet_len + 1, tm.alphabet);
	tm.alphabet = stringToChar(alphabet_str, tm.alphabet, 1);
	tm.alphabet[0] = ' ';
}

// Выделение памяти символьного массива
char* sizeCharArray(int length, char* array)
{
	array = new char[length];

	return array;
}

// Перевод строки в символьный массив
char* stringToChar(string str, char* arr, int i)
{
	int n = str.length();
	arr = new char[n + 1];

	auto first = str.begin();
	auto last = str.end();

	copy(first, last, arr + i);

	return arr;
}

// Интерфейс программы
void controlPanel(TuringMachine& tm)
{
	int command;

	cout <<
		"\n[1] - run turing machine"
		"\n[0] - exit"
		"\n\nInput command: ";

	if (cin >> command)
	{
		cout << "\n";
		switch (command)
		{
		case 1:
			enterWord(tm);
			controlPanel(tm);
		case 0:
			exit(0);
		default:
			cout << "wrong input" << endl;
			controlPanel(tm);
		}
	}
	else
	{
		cout << "Invalid input" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		controlPanel(tm);
	}
}

// Ввод слова
void enterWord(TuringMachine& tm)
{
	tm.q = 1;
	tm.headPosition = 1;
	tm.counter = 0;

	string word;
	cout << "Enter word: ";
	cin >> word;

	int n = word.length();

	tm.tape_len = n + 2;

	tm.tape = sizeCharArray(tm.tape_len, tm.tape);

	tm.tape = stringToChar(word, tm.tape, 1);

	tm.tape[0] = '_';
	tm.tape[tm.tape_len - 1] = '_';

	if (checkWord(tm) == 1)
	{
		cout << "Wrong word" << endl;
	}
	else
	{
		runMT(tm);
	}
}

// Проверка слова
int checkWord(TuringMachine& tm)
{
	int flag;

	for (int i = 0; i < tm.tape_len; i++)
	{
		flag = 1;

		for (int j = 0; j <= tm.alphabet_len; j++)
		{
			if (tm.tape[i] == tm.alphabet[j])
			{
				flag = 0;
				//continue; <- 
			}
		}

		if (flag == 1)
		{
			return 1;
		}
	}

	return 0;
}

// Запуск машины
void runMT(TuringMachine& tm)
{
	while (tm.q != 0)
	{
		outputState(tm);
		runMTiter(tm);
	}

	outputState(tm);

	cout << "\n\nCompleted" << endl;

	controlPanel(tm);
}

// Вывод ленты конфигурации
void outputState(TuringMachine& tm)
{
	char* head;
	head = new char[tm.tape_len];

	for (int i = 0; i < tm.tape_len; i++)
	{
		head[i] = ' ';
	}

	head[tm.headPosition] = 'v';

	cout << "\n" << ++tm.counter << ") " << "\n";

	cout << "      ";
	for (int i = 0; i < tm.tape_len; i++)
	{
		cout << head[i] << "";
	}

	cout << endl;

	cout << "      ";
	for (int i = 0; i < tm.tape_len; i++)
	{
		cout << tm.tape[i] << "";
	}

	if (tm.q != 0)
	{
		tm.a = tm.tape[tm.headPosition];
		cout << "              config: q" << tm.q
			<< ", head = " << tm.a << ", next: "
			<< tm.programm[tm.q][getIndexByValue(tm.alphabet, tm.alphabet_len + 1, tm.a)];
	}
	else
	{
		cout << "              end";
	}
}

// Запуск одной итерации машины
void runMTiter(TuringMachine& tm)
{
	int symbol;
	tm.a = tm.tape[tm.headPosition];

	symbol = getIndexByValue(tm.alphabet, tm.alphabet_len + 1, tm.a);

	tm.command = tm.programm[tm.q][symbol];

	tm.command_ = sizeCharArray(5, tm.command_);

	tm.command_ = stringToChar(tm.command, tm.command_, 0);
	if (tm.command_[4] - '0' < 0 || tm.command_[4] - '0' > 9)
	{
		tm.command_[4] = '-';
	}

	// Замена символа
	tm.tape[tm.headPosition] = tm.command[0];

	// Движение головки
	if (tm.command_[1] == 'l')
	{
		tm.headPosition--;
	}
	else if (tm.command_[1] == 'r')
	{
		tm.headPosition++;
	}
	else if (tm.command_[1] == 's')
	{
		tm.headPosition = tm.headPosition;
	}
	else
	{
		cout << "Wrong command\n" << endl;
		exit(0);
	}

	// Новое состояние головки
	if (tm.command_[4] != '-')
	{
		tm.q = (tm.command_[3] - '0') * 10 + tm.command_[4] - '0';
	}
	else
	{
		tm.q = (tm.command_[3] - '0');
	}

	// Расширение ленты
	extendTape(tm);
}

// Получение индекса элемента по значению
int getIndexByValue(char* array, int length, char value)
{
	if (array == nullptr)
	{
		return -1;
	}
	for (int i = 0; i < length; ++i)
	{
		if (array[i] == value)
		{
			return i;
		}
	}
	return -1;
}

// Расширение ленты
void extendTape(TuringMachine& tm)
{
	// Имитация бесконечной ленты справа
	if (tm.headPosition == tm.tape_len)
	{
		resizeTape(tm);
		tm.tape[tm.tape_len - 1] = '_';
	}
	else if (tm.headPosition <= 0)
	{
		resizeTape(tm);

		for (int i = tm.tape_len - 1; i >= 0; i--)
		{
			tm.tape[i + 1] = tm.tape[i];
			tm.tape[i] = ' ';
		}

		tm.tape[0] = '_';
		tm.headPosition++;
	}

	// Имитация бесконечной ленты слева
	if (tm.tape[tm.tape_len - 1] != '_')
	{
		resizeTape(tm);
		tm.tape[tm.tape_len - 1] = '_';
	}
}

// Увеличение ленты
void resizeTape(TuringMachine& tm)
{
	// Копируем ленту в буффер
	char* newArray = new char[tm.tape_len];
	for (int i = 0; i < tm.tape_len; i++)
	{
		newArray[i] = tm.tape[i];
	}

	// Увеличиваем размер ленты на 1
	tm.tape_len++;
	tm.tape = new char[tm.tape_len];

	// Заполняем по умолчанию
	for (int i = 0; i < tm.tape_len; i++)
	{
		tm.tape[i] = '_';
	}

	// Возвращаем содержимое буфера на ленту
	for (int i = 0; i < tm.tape_len - 1; i++)
	{
		tm.tape[i] = newArray[i];
	}

	// Высвобождаем память
	delete[] newArray;
}
