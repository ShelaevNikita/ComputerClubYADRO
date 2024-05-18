
/*
    �������� ���� ������ ComputerClub �� ����� ���������/��������
*/

#include "../include/ComputerClub.h"

// �������������� ������ ������� XX:XX � ���������� ����� � �������� ���������� �� ������������
unsigned short ComputerClub::checkTime(const string &eventTime, computerClubInfo &computerClubInfo,
                                       bool flagEndl, bool flagPrint) {

    // ���� ������ ������ � �������� �� ���������
    if (eventTime.length() != 5) {
        computerClubInfo.flagCorrectWork = false;
        return 1440;
    }

    // ���������� ����� � ����� �� ������ ������� XX:XX
    pair<unsigned short, unsigned short> eventTimePair
        { stoi(eventTime.substr(0, 2)), stoi(eventTime.substr(3, 5)) };
    
    // ����� ���������� �����
    unsigned short timeTemp = eventTimePair.first * 60 + eventTimePair.second; 

    // �������� ������� ������� �� ������������
    if (flagPrint && (!computerClubInfo.flagCorrectWork || timeTemp >= 1440 || eventTime[2] != ':')) {
        cout << eventTime;
        if (flagEndl)
            cout << endl;
        else
            cout << " ";

        computerClubInfo.flagCorrectWork = false;
        return 0;
    }
    return timeTemp;
}

// �������������� ���������� ����� � ������ ������� XX:XX 
string ComputerClub::timeToString(unsigned short time) {
    unsigned short timeHour = time / 60;
    unsigned short timeMin  = time % 60;
    return to_string(timeHour / 10) + to_string(timeHour % 10) + ":" + to_string(timeMin / 10) + to_string(timeMin % 10);
}

// ��������� ���� ���������� �� �������� ����� 
vector<ComputerClub::computerClubEvent> 
ComputerClub::openFile(const string &fname, computerClubInfo &compClubInfo) {

    // ������ �������� �������
	vector<computerClubEvent> eventIn;
 
    ifstream inputFile(fname);

    // ���� �� ������� ������� ������� ����
    if (!inputFile.is_open()) {            
        inputFile.close();
        cout << " ERROR: Not found inputFile!..." << endl;
        compClubInfo.flagCorrectWork = false;
        return eventIn;
    }

    string eventTime;                      // ������ ��� ������ �������
    computerClubEvent tempEvent;           // �������� �������

    inputFile >> compClubInfo.tableNumber;

    // ���� ���������� ������ == 0
    if (compClubInfo.tableNumber == 0) {
        cout << compClubInfo.tableNumber << endl;
        compClubInfo.flagCorrectWork = false;
        inputFile.close();
        return eventIn;
    }

    // ����� ������ ������ ������������� �����
    inputFile >> eventTime;
    compClubInfo.timeStart = checkTime(eventTime, compClubInfo, false, true);

    // ����� ��������� ������ ������������� �����
    inputFile >> eventTime;
    compClubInfo.timeEnd = checkTime(eventTime, compClubInfo, true, true);

    // ���� ������ �������� ����� ������������
    if (!compClubInfo.flagCorrectWork) {
        inputFile.close();
        return eventIn;
    }

    inputFile >> compClubInfo.costOfHour;

    // ���� ��������� ������ ���� == 0
    if (compClubInfo.costOfHour == 0) {
        cout << compClubInfo.costOfHour << endl;
        compClubInfo.flagCorrectWork = false;
        inputFile.close();
        return eventIn;
    }

    // ���������������� �������� � ������ � ������ ���� �������� �������
    while (inputFile >> eventTime >> tempEvent.ID >> tempEvent.personName) {
        if (tempEvent.ID == 2)
            inputFile >> tempEvent.tableIndex;
        else
            tempEvent.tableIndex = 0;

        // ����� �������
        tempEvent.time = checkTime(eventTime, compClubInfo, false, false);

        // �������� ������� �� ������������
        if (!compClubInfo.flagCorrectWork || tempEvent.ID == 0 || tempEvent.ID > 4 ||
                (tempEvent.ID == 2 && (tempEvent.tableIndex == 0 || tempEvent.tableIndex > compClubInfo.tableNumber))) {

            cout << eventTime << " " << tempEvent.ID << " " << tempEvent.personName;
            if (tempEvent.ID == 2)
                cout << " " << tempEvent.tableIndex;
            cout << endl;

            compClubInfo.flagCorrectWork = false;
            inputFile.close();
            return eventIn;
        }
        eventIn.push_back(tempEvent);
    }
    inputFile.close();
    return eventIn;
}

// ���������� ������� (� ����������� �� ���� � ������� �������) ��� ������ �����:
//  (( ����� ������� � ID == 2 ��� ID == 12 ) - ( ����� ������� � ID == 4 ��� ID == 11 )) * ��������� ������ ����
pair<unsigned int, unsigned short>
ComputerClub::calculateCostOfTable(unsigned short timeEnd, unsigned short timeStart, unsigned short costOfHour) {

    short diffClientTime = timeEnd - timeStart;
    if (diffClientTime < 0)                       // ���� ������������ ���� �������� � ������ ����� ��� �������������
        diffClientTime += 1440;                   //    ����� ���������� ������������� ������� �� �������

    unsigned short hourShort = diffClientTime / 60;
    if (hourShort % 60 != 0)                      // ���������� �� ���� � ������� �������
        hourShort++;

    return { hourShort * costOfHour, diffClientTime };
}

// ����� � ������� (�������� ����) ���������� �� ����������� �������
void ComputerClub::printEvent(computerClubEvent &compEvent, unsigned short newEventID, unsigned short errorIndex) {
    
    // ������ ���� ��������� �� ������
    const string errorMessage[6] { "OK", "YouShallNotPass", "NotOpenYet",
                                   "PlaceIsBusy", "ClientUnknown", "ICanWaitNoLonger!" };

    compEvent.ID = newEventID;   // ����� �������� � ��������� ������� ����������� ������ ID
    if (compEvent.ID == 13)      // ���� ������� � ID == 13 -> ����� �������� ������
        compEvent.personName = errorMessage[errorIndex];

    // ������ � ������� (�������� ����) ���������� �� �������
    cout << timeToString(compEvent.time) << " " << compEvent.ID << " " << compEvent.personName;
    if (compEvent.ID == 2 || compEvent.ID == 12)
        cout << " " << compEvent.tableIndex;
    cout << endl;

    return;
}

// ����� � ������� (�������� ����) ���������� � ������� ������� ��� ��������� ��������,
//	������� ���� ����� ��������� ������������� ����� 
void ComputerClub::checkLastClients(computerClubInfo &compClubInfo,
                                    map<string, pair<unsigned short, unsigned short>> &clientToTableMap,
                                    vector<pair<unsigned int, unsigned short>> &tablesInfo) {

    vector<string> lastClients;                    // ������ ���� ��������� ��������
    for (const auto &[clientName, clientTable] : clientToTableMap)
        lastClients.push_back(clientName);

    sort(lastClients.begin(), lastClients.end());  // ���������� �������� � ���������� ������� (in-place, �� �� NLlogNL)

    // ���������������� ��������� ���� ��������� ��������
    for (auto &clubClient : lastClients) {

        // ��������� ���������� �� ����������� �������� ����� �� �������
        pair<unsigned short, unsigned short> lastClientTable = clientToTableMap[clubClient];

        // ���� ������ ��� ����� �� ������, ����� �� ������ ������
        if (lastClientTable.first != 0) {

            // ���������� ������� � ������� ��������� �������� �����
            pair<unsigned short, unsigned int> diffClientTimeCost = 
                calculateCostOfTable(compClubInfo.timeEnd, lastClientTable.second, compClubInfo.costOfHour);

            // ���������� ���������� ��� ����������� �����
            tablesInfo[lastClientTable.first].first  += diffClientTimeCost.first;
            tablesInfo[lastClientTable.first].second += diffClientTimeCost.second;

            // ����� � ������� (�������� ����) ������� � ID == 11 � �������� �������� ������������� �����
            computerClubEvent currentClubEvent;
            currentClubEvent.time = compClubInfo.timeEnd;
            currentClubEvent.personName = clubClient;           
            printEvent(currentClubEvent, 11, 0);
        }
    }
    return;
}

// ���������������� ��������� ���� �������� �������
vector<pair<unsigned int, unsigned short>>
ComputerClub::eventProcessing(computerClubInfo &compClubInfo, vector<computerClubEvent> &compClubEventIn) {

    // ������ ���������� ������������ ������
    vector<bool> occupiedTables;

    // ������ ��� ������ ���������� � ������ ������������ �����:
    //  (�������; ����� ����� ������)
    vector<pair<unsigned int, unsigned short>> tablesInfo;

    // ������� �� �������� (������� � ID == 3) �������� �� ������ ���������� ������
    queue<string> queueOfClients;

    // ������� ��� �������� ���������� � ��������, ������� ����� �� ������������� �������:
    //  ���� - ��� �������, �������� = (������ �����; ����� � �������, ����� ������ ��� �� ���� ����)
    map<string, pair<unsigned short, unsigned short>> clientToTableMap;

    // ������� ������� ����������� ������
    unsigned short occupiedTablesCount = 0;
    
    // ��������� ������������� ���� ������������ ��������
    for (unsigned short i = 0; i <= compClubInfo.tableNumber; i++) {
        occupiedTables.push_back(false);
        tablesInfo.push_back({ 0, 0 });
    }

    // ���� �� ���� �������� ��������
    for (auto &currentClubEvent : compClubEventIn) {

        // ������ � ������� (�������� ����) ���������� � �������� ������� 
        printEvent(currentClubEvent, currentClubEvent.ID, 0);

        // ���� ������������ ���� �������� � ������ ����� ��� �������������
        unsigned short flag = 0;
        if (compClubInfo.timeEnd <= compClubInfo.timeStart)
            flag = 1440;

        // ����� ����������� ��������� ������� � ����������� �� ��� ID
        switch (currentClubEvent.ID) {
            case 1:
                if (currentClubEvent.time < compClubInfo.timeStart || currentClubEvent.time > flag + compClubInfo.timeEnd)
                    printEvent(currentClubEvent, 13, 2);  // ������ "NotOpenYet"
                else if (clientToTableMap.find(currentClubEvent.personName) != clientToTableMap.end())
                    printEvent(currentClubEvent, 13, 1);  // ������ "YouShallNotPass"
                else
                    // ������ ����� ������ ������������� �����, �� ��� �� ��� �� ����
                    clientToTableMap[currentClubEvent.personName] = { 0, currentClubEvent.time };

                break;

            case 2:
                if (clientToTableMap.find(currentClubEvent.personName) == clientToTableMap.end())
                    printEvent(currentClubEvent, 13, 4);  // ������ "ClientUnknown"
                else if (occupiedTables[currentClubEvent.tableIndex])
                    printEvent(currentClubEvent, 13, 3);  // ������ "PlaceIsBusy"
                else {
                    // ��������� ���������� � ���������� ������������ ����� �������
                    pair<unsigned short, unsigned short> prevClientTable = clientToTableMap[currentClubEvent.personName];

                    // ���� �� ��� ����� �� ������
                    if (prevClientTable.first != 0) {

                        // ������������ ����������� �����
                        occupiedTables[prevClientTable.first] = false;

                        // ���������� ������� � ������� ��������� �������� �����
                        pair<unsigned int, unsigned short> diffClientTimeCost =
                            calculateCostOfTable(currentClubEvent.time, prevClientTable.second, compClubInfo.costOfHour);

                        // ���������� ���������� ��� ����������� �����
                        tablesInfo[prevClientTable.first].first  += diffClientTimeCost.first;
                        tablesInfo[prevClientTable.first].second += diffClientTimeCost.second;
                    }
                    else
                        occupiedTablesCount++;

                    // ������ ��� �� ������ ������������ ����
                    occupiedTables[currentClubEvent.tableIndex] = true;

                    // ���������� ���������� � ������������ �������
                    clientToTableMap[currentClubEvent.personName] = { currentClubEvent.tableIndex, currentClubEvent.time };
                }
                break;

            case 3:
                if (occupiedTablesCount < compClubInfo.tableNumber)
                    printEvent(currentClubEvent, 13, 5);    // ������ "ICanWaitNoLonger!"
                else if (queueOfClients.size() > compClubInfo.tableNumber)
                    printEvent(currentClubEvent, 11, 0);    // ������� �����������, � ������ ������ ������
                /*
                else if (clientToTableMap.find(currentClubEvent.personName) == clientToTableMap.end())
                    printEvent(currentClubEvent, 13, 4);    // ������ "ClientUnknown" (����������� � ������� �������)
                */
                else
                    // ���������� ������� � �������
                    queueOfClients.push(currentClubEvent.personName);

                break;

            case 4:
                if (clientToTableMap.find(currentClubEvent.personName) == clientToTableMap.end())
                    printEvent(currentClubEvent, 13, 4);     // ������ "ClientUnknown"
                else {
                    // ��������� ���������� � ������������ ����� ��������� �������
                    pair<unsigned short, unsigned short> prevClientTable = clientToTableMap[currentClubEvent.personName];

                    // ���� �������� ������ �� ����� �� ������, �� �� ������ ������
                    if (prevClientTable.first == 0)
                        break;

                    // ���������� ���������� � ���������� ������ � � �������
                    occupiedTablesCount--;
                    occupiedTables[prevClientTable.first] = false;
                    clientToTableMap.erase(currentClubEvent.personName);

                    // ���������� ������� � ������� ��������� �������� �����
                    pair<unsigned short, unsigned int> diffClientTimeCost =
                        calculateCostOfTable(currentClubEvent.time, prevClientTable.second, compClubInfo.costOfHour);

                    // ���������� ���������� ��� �������������� �����
                    tablesInfo[prevClientTable.first].first  += diffClientTimeCost.first;
                    tablesInfo[prevClientTable.first].second += diffClientTimeCost.second;                  

                    // ���� ������� �����, �� �� ����� ��������� ������� � ID == 12
                    if (queueOfClients.size() == 0)
                        break;

                    // ��������� ������� ������� �� �������
                    string nextClient = queueOfClients.front();
                    queueOfClients.pop();

                    // ���������� ���������� � �����, �� ������� ��� ���� ������
                    //  (������ �� ������� �������� ������ ��� �������������� ����)
                    occupiedTablesCount++;
                    occupiedTables[prevClientTable.first] = true;
                    clientToTableMap[nextClient] = { prevClientTable.first, currentClubEvent.time };

                    // ����� � ������� (�������� ����) ������� � ID == 12
                    currentClubEvent.personName = nextClient;
                    currentClubEvent.tableIndex = prevClientTable.first;
                    printEvent(currentClubEvent, 12, 0);
                }
                break;
        }
    }
    // ��������� ��������, ����������� � ������������ ����� ����� ��� ���������
    checkLastClients(compClubInfo, clientToTableMap, tablesInfo);

    return tablesInfo;
}

// ����� � ������� (�������� ����) ���������� (������� � ����� ������) �� ���� ������ ������������� �����
void ComputerClub::printTablesInfo(vector<pair<unsigned int, unsigned short>> &tablesInfo) {
    for (auto itr = 1; itr < tablesInfo.size(); itr++)
        cout << itr << " " << tablesInfo[itr].first << " " << timeToString(tablesInfo[itr].second) << endl;
    return;
}

// ������� ���� ������ ComputerClub
void ComputerClub::computerClubMain(const string &fname) {
    computerClubInfo compClubInfo;
    vector<computerClubEvent> compClubEventIn = openFile(fname, compClubInfo);

    // ���� ������� ���� �� ���������, ���������� ������ ���������
    if (!compClubInfo.flagCorrectWork)
        return;

    cout << timeToString(compClubInfo.timeStart) << endl;

    vector<pair<unsigned int, unsigned short>> tablesInfo = eventProcessing(compClubInfo, compClubEventIn);

    cout << timeToString(compClubInfo.timeEnd) << endl;

    printTablesInfo(tablesInfo);

    return;
}