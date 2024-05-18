
// #pragma once

/*
	������������ ���� ��� ������ ComputerClub, � ������� �����������
		��� ����������� ����������� � ������������ �������� � ��� ������������
		�������� � �������/�������
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

	// ��������� - ����� ���������� � ������������ �����
	struct computerClubInfo {
		unsigned short tableNumber  = 0;   // ���������� ������������ ������
		unsigned short timeStart = 1440;   // ������ ������ �����
		unsigned short timeEnd   = 1440;   // ���������� ������ �����
		unsigned short costOfHour   = 0;   // ��������� ������ ����
		bool flagCorrectWork     = true;   // ����, ������������ ������������ �������� �����
	};

	// ��������� - ���������� � ������ ������� (�������� � ���������)
	struct computerClubEvent {
		unsigned short time    = 1440;     // ����� �������
		unsigned short ID         = 0;     // ID �������
		unsigned short tableIndex = 0;     // ����� ����� ��� ������� � ID == 2 � ID == 12
		string personName      = "\0";     // ��� �������
	};

	// �������������� ������ ������� XX:XX � ���������� ����� � �������� ���������� �� ������������
	static unsigned short checkTime(const string &eventTime, computerClubInfo &computerClubInfo, 
									const bool flagEndl, bool flagPrint);

	// �������������� ���������� ����� � ������ ������� XX:XX
	static string timeToString(unsigned short time);

	// ��������� ���� ���������� �� �������� �����
	static vector<computerClubEvent> openFile(const string &fname, computerClubInfo &computerClubInfo);

	// ���������� ������� (� ����������� �� ���� � ������� �������) ��� ������ �����
	static pair<unsigned int, unsigned short>
		calculateCostOfTable(unsigned short timeEnd, unsigned short timeStart, unsigned short costOfHour);

	// ����� � ������� (�������� ����) ���������� �� ����������� �������
	static void printEvent(computerClubEvent &compEvent, unsigned short newEventID, unsigned short errorIndex);
	
	// ����� � ������� (�������� ����) ���������� � ������� ������� ��� ��������� ��������,
	//	������� ���� ����� ��������� ������������� ����� 
	static void checkLastClients(computerClubInfo &compClubInfo,
								 map<string, pair<unsigned short, unsigned short>> &clientToTableMap,
								 vector<pair <unsigned int, unsigned short>> &tablesInfo);

	// ���������������� ��������� ���� �������� �������
	static vector<pair<unsigned int, unsigned short>>
		eventProcessing(computerClubInfo &compClubInfo, vector<computerClubEvent> &compClubEventIn);

	// ����� � ������� (�������� ����) ���������� (������� � ����� ������) �� ���� ������ ������������� �����
	static void printTablesInfo(vector<pair<unsigned int, unsigned short>> &tablesInfo);

public:

	// ������� ���� ������ ComputerClub
	static void computerClubMain(const string &fname);

};

#endif // COMPUTER_CLUB_H