
// #pragma once

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

	struct computerClubInfo {
		unsigned short tableNumber = 0;
		unsigned short timeStart = 1440;
		unsigned short timeEnd = 1440;
		unsigned short costOfHour = 0;
		bool flagCorrectWork = true;
	};

	struct computerClubEvent {
		unsigned short time = 1440;
		unsigned short ID = 0;
		unsigned short tableIndex = 0;
		string personName = "\0";
	};

	static unsigned short timeToShort(pair <unsigned short, unsigned short> eventTime);

	static string timeToString(unsigned short time);

	static unsigned short checkTime(const string &eventTime, computerClubInfo &computerClubInfo, const bool flagEndl);

	static vector<computerClubEvent> openFile(const string &fname, computerClubInfo &computerClubInfo);

	static void printEvent(computerClubEvent &compEvent, unsigned short newEventID, unsigned short errorIndex);

	static pair<short, unsigned int> calculateCostOfTable(unsigned short timeEnd,
														  unsigned short timeStart, unsigned short costOfHour);

	static void checkLastClients(computerClubInfo &compClubInfo,
								 map<string, pair<unsigned short, unsigned short>> &clientToTableMap,
								 vector<pair <unsigned short, unsigned int>> &tablesInfo);

	static vector<pair <unsigned short, unsigned int>>
		eventProcessing(computerClubInfo &compClubInfo, vector<computerClubEvent> &compClubEventIn);

	static void printTablesInfo(vector<pair<unsigned short, unsigned int>> &tablesInfo);

public:

	static void computerClubMain(const string &fname);

};

#endif // COMPUTER_CLUB_H