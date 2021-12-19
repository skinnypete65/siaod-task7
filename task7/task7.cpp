#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>

#define maxWindow 10

using namespace std;

struct code {
	int off;
	int len;
	char nextChar;
};

struct Node {
	int index;
	string data;
	Node* next;
};

string RLEEncode(string sourceStr);
double calculateCompressionKoef(int sourceLength, int encodeLength);
void LZ77(string str);
string LZ78(string input, int option);

vector <string> split(string str, char delimiter);
bool delete_Node(Node* head, Node* to_delete);
Node* search_Node(Node* head, int index);
Node* search_Node(Node* head, string data);
void insert_Node(Node* head, int index, string data);
void st_Node(Node* head, int index, string data);


string LZ78(string input, int option);

int main() {
	setlocale(LC_ALL, "rus");

	double compressionKoef;
	
	cout << "Задание 1\n";
	string RLESourceString1 = "aaaaabbbbfewwwwwwwwuuuuuuuiiii";
	string RLEEncodeString1 = RLEEncode(RLESourceString1);

	cout << "Исходный текст: " << RLESourceString1 << "\n";
	cout << "Закодированный текст: " << RLEEncodeString1 << "\n";
	cout << "Исходная длина: " << RLESourceString1.length() << ",  длина, сжатого текста: " << RLEEncodeString1.length() << "\n";
	compressionKoef = calculateCompressionKoef(RLESourceString1.length(), RLEEncodeString1.length());
	printf("Коэффициент сжатия: %.1f%%", compressionKoef);
	cout << "\n\n";

	string RLESourceString2 = "asdfhedfgs";
	string RLEEncodeString2 = RLEEncode(RLESourceString2);

	cout << "Исходный текст: " << RLESourceString2 << "\n";
	cout << "Закодированный текст: " << RLEEncodeString2 << "\n";
	cout << "Исходная длина: " << RLESourceString2.length() << ",  длина, сжатого текста: " << RLEEncodeString2.length() << "\n";
	compressionKoef = calculateCompressionKoef(RLESourceString2.length(), RLEEncodeString2.length());
	printf("Коэффициент сжатия: %.1f%%", compressionKoef);
	cout << "\n\n";
	
	cout << "Задание 2\n";
	cout << "LZ77:\n";
	LZ77("010110110010101010111");
	cout << "\n\nLZ78:\n";
	string input = "webwerbweberweberweb";
	cout << "Исходная строка: " << input << endl;

	string result;
	result = LZ78(input, 1);

	cout << "Закодированная строка:" << result << endl;

	result = LZ78(result, 2);

	cout << "Декодированная строка: " << result << endl;

}

string RLEEncode(string sourceStr) {
	string encodeStr = "";
	int charAmount = 1;
	for (int i = 0; i < sourceStr.length() - 1; i++) {
		if (sourceStr[i] == sourceStr[i + 1]) {
			charAmount++;
		}
		else {
			encodeStr += sourceStr[i] + to_string(charAmount);
			charAmount = 1;
		}
	}
	encodeStr += sourceStr[sourceStr.length() - 1] + to_string(charAmount);
	return encodeStr;
}

double calculateCompressionKoef(int sourceLength, int encodeLength) {
	return (1 - ((double)encodeLength / sourceLength)) * 100.0;
}

void LZ77(string str) {

	vector<code> vt;

	cout << "Исходная строка: " << str << endl;
	unsigned int window1, window2;

	for (unsigned int i = 0; i < str.size(); i++) {
		if (i + 1 <= maxWindow) window1 = i; // Определяем размер окна запроса 
		else window1 = maxWindow;

		if (i + window1 < str.length()) window2 = window1;
		else window2 = str.length() - i;

		string str1 = str.substr(i - window1, window1);
		string str2 = str.substr(i, window2);
		

		int off = -1;
		while (true) {
			if (window2 == 0) break; // Пустая строка, прямой выход 
			string str3 = str2.substr(0, window2);
			off = str1.find(str3); // Не найдено, выкл. = -1 

			if (off != -1) break; // Нашли, выходим 
			window2--;
			if (window2 <= 0) break;
		}

		if (off != -1) {
			code cd;
			cd.len = window2;
			cd.off = window1 - off;
			cd.nextChar = str[i + window2];
			vt.push_back(cd);
			i += window2;
		}
		else {
			code cd;
			cd.len = 0;
			cd.off = 0;
			cd.nextChar = str[i + window2];
			vt.push_back(cd);
		}
	}

	cout << "Закодированная строка: ";
	for (unsigned int i = 0; i < vt.size(); i++) { // Результат кодирования 
		printf("(%d,%d,%c)", vt[i].off, vt[i].len, vt[i].nextChar);
	}
	printf("\n");

	// декодирование 
	string strOut;
	for (unsigned int i = 0; i < vt.size(); i++) {
		if (vt[i].len == 0) {
			strOut += vt[i].nextChar;
		}
		else {
			int len = strOut.length();
			len -= vt[i].off;
			string temp = strOut.substr(len, vt[i].len);
			strOut += temp + vt[i].nextChar;
		}
	}
	cout << "Раскодированная строка: " << strOut;
}

string LZ78(string input, int option)
{
	if (option == 1)
	{
		Node* dictionary = new Node;
		string word, result;
		int length, last_seen, index = 1;

		length = (int)input.length();
		word = input[0];
		st_Node(dictionary, 1, word);
		result += "0," + word;

		for (int i = 1; i < length; i++)
		{
			string data;
			data = input[i];

		re_check:
			Node* search = search_Node(dictionary, data);

			if (search)
			{
				i++;
				data += input[i];
				last_seen = search->index;
				goto re_check;
			}
			else
			{
				char zero;
				if (input[i] == ' ')
					zero = '0';
				else
					zero = input[i];

				if ((int)data.length() < 2)
					result += " " + to_string(0) + "," + zero;
				else
					result += " " + to_string(last_seen) + "," + zero;

				index++;
				if (i != length)
					insert_Node(dictionary, index, data);
			}
		}

		return result;
	}
	if (option == 2)
	{
		Node* dictionary = new Node;
		string result;

		vector <string> s_input = split(input, ' ');
		int zz = 2;
		for (int i = 0; i < s_input.size(); i++)
		{
			vector <string> ss_input = split(s_input[i], ',');

			if (i == 0)
			{
				st_Node(dictionary, 1, ss_input[1]);
				result += ss_input[1];
			}
			else
			{
				Node* serched;
				string get_search = ss_input[1];
				serched = search_Node(dictionary, stoi(ss_input[0]));
				if (serched)
				{
					result += serched->data + get_search;
					get_search = serched->data + split(s_input[i], ',')[1];
					insert_Node(dictionary, zz, get_search);
				}
				else
				{
					if (stoi(ss_input[0]) == 0)
						insert_Node(dictionary, zz, get_search);
					else
						insert_Node(dictionary, zz, get_search);

					result += get_search;
				}
				zz++;
			}
		}

		if (result[(int)result.length() - 1] == '0')
			result = result.substr(0, result.size() - 1);

		return result;
	}
}

void st_Node(Node* head, int index, string data) {
	head->index = index;
	head->data = data;
	head->next = NULL;
}

void insert_Node(Node* head, int index, string data) {
	Node* new_Node = new Node;
	new_Node->index = index;
	new_Node->data = data;
	new_Node->next = NULL;

	Node* curr = head;
	while (curr != NULL)
	{
		if (curr->next == NULL)
		{
			curr->next = new_Node;
			return;
		}
		curr = curr->next;
	}
}

Node* search_Node(Node* head, string data)
{
	Node* curr = head;
	while (curr != NULL)
	{
		if (data.compare(curr->data) == 0)
			return curr;
		else
			curr = curr->next;
	}
	return NULL;
}

Node* search_Node(Node* head, int index)
{
	Node* curr = head;
	while (curr != NULL)
	{
		if (index == curr->index)
			return curr;
		else
			curr = curr->next;
	}
	return NULL;
}

bool delete_Node(Node* head, Node* to_delete) {
	if (to_delete == NULL)
		return false;
	else if (to_delete == head)
	{
		head = to_delete->next;
		delete to_delete;
		return true;
	}
	else {
		Node* curr = head;
		while (curr)
		{
			if (curr->next == to_delete)
			{
				curr->next = to_delete->next;
				delete to_delete;
				return true;
			}
			curr = curr->next;
		}
		return false;
	}
}

vector <string> split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}