
// #pragma once

/*
	Заголовочный файл для класса ComputerClub, в котором добавляются
		все необходимые зависимости и определяются названия и тип используемых
		структур и методов/функций
*/

#ifndef COMPUTER_CLUB_H

#define COMPUTER_CLUB_H

#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class ComputerClub {
	
private:

	// Структура - общая информация о компьютерном клубе
	struct computerClubInfo {
		unsigned short tableNumber  = 0;   // Количество компьютерных столов
		unsigned short timeStart = 1440;   // Начало работы клуба
		unsigned short timeEnd   = 1440;   // Завершение работы клуба
		unsigned short costOfHour   = 0;   // Стоимость одного часа
		bool flagCorrectWork     = true;   // Флаг, показывающий корректность входного файла
	};

	// Структура - информация о каждом событии (входящим и исходящем)
	struct computerClubEvent {
		unsigned short time    = 1440;     // Время события
		unsigned short ID         = 0;     // ID события
		unsigned short tableIndex = 0;     // Номер стола для событий с ID == 2 и ID == 12
		string personName      = "\0";     // Имя клиента
	};

	// Преобразование строки формата XX:XX в количество минут и проверка результата на корректность
	static unsigned short checkTime(const string &eventTime, computerClubInfo &computerClubInfo, 
									const bool flagEndl, bool flagPrint);

	// Преобразование количества минут в строку формата XX:XX
	static string timeToString(unsigned short time);

	// Получение всей информации из входного файла
	static vector<computerClubEvent> openFile(const string &fname, computerClubInfo &computerClubInfo);

	// Вычисление выручки (с округлением до часа в большую сторону) для одного стола
	static pair<unsigned int, unsigned short>
		calculateCostOfTable(unsigned short timeEnd, unsigned short timeStart, unsigned short costOfHour);

	// Вывод в консоль (выходной файл) информации по конкретному событию
	static void printEvent(computerClubEvent &compEvent, unsigned short newEventID, unsigned short errorIndex);
	
	// Вывод в консоль (выходной файл) информации и подсчёт выручки для последних клиентов,
	//	которые ушли перед закрытием компьютерного клуба 
	static void checkLastClients(computerClubInfo &compClubInfo,
								 map<string, pair<unsigned short, unsigned short>> &clientToTableMap,
								 vector<pair <unsigned int, unsigned short>> &tablesInfo);

	// Последовательная обработка всех входящих событий
	static vector<pair<unsigned int, unsigned short>>
		eventProcessing(computerClubInfo &compClubInfo, vector<computerClubEvent> &compClubEventIn);

	// Вывод в консоль (выходной файл) информации (выручка и время работы) по всем столам компьютерного клуба
	static void printTablesInfo(vector<pair<unsigned int, unsigned short>> &tablesInfo);

public:

	// Главный файл класса ComputerClub
	static void computerClubMain(const string &fname);

};

#endif // COMPUTER_CLUB_H