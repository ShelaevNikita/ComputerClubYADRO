
#include "../include/ComputerClub.h"

unsigned short ComputerClub::timeToShort(pair<unsigned short, unsigned short> eventTime) {
    return eventTime.first * 60 + eventTime.second;
}

string ComputerClub::timeToString(unsigned short time) {
    unsigned short timeHour = time / 60;
    unsigned short timeMin = time % 60;
    return to_string(timeHour / 10) + to_string(timeHour % 10) + ":" + to_string(timeMin / 10) + to_string(timeMin % 10);
}

unsigned short ComputerClub::checkTime(const string &eventTime, 
                                       computerClubInfo &computerClubInfo, bool flagEndl) {
    pair<unsigned short, unsigned short> eventTimePair;
    eventTimePair.first  = stoi(eventTime.substr(0, 2));
    eventTimePair.second = stoi(eventTime.substr(3, 5));
    unsigned short timeTemp = timeToShort(eventTimePair);
    if (!computerClubInfo.flagCorrectWork || timeTemp >= 1440 || eventTime[2] != ':') {
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

vector<ComputerClub::computerClubEvent> 
ComputerClub::openFile(const string &fname, computerClubInfo &compClubInfo) {
	vector<computerClubEvent> eventIn;
	ifstream inputFile(fname);
    if (!inputFile.is_open()) {
        inputFile.close();
        cout << " ERROR: Not found inputFile!..." << endl;
        compClubInfo.flagCorrectWork = false;
        return eventIn;
    }

    string eventTime;
    computerClubEvent tempEvent;   

    inputFile >> compClubInfo.tableNumber >> eventTime;

    compClubInfo.timeStart = checkTime(eventTime, compClubInfo, false);

    inputFile >> eventTime >> compClubInfo.costOfHour;

    compClubInfo.timeEnd = checkTime(eventTime, compClubInfo, true);

    if (!compClubInfo.flagCorrectWork) {
        inputFile.close();
        return eventIn;
    }

    while (inputFile >> eventTime >> tempEvent.ID >> tempEvent.personName) {
        if (tempEvent.ID == 2)
            inputFile >> tempEvent.tableIndex;
        else
            tempEvent.tableIndex = 0;

        tempEvent.time = checkTime(eventTime, compClubInfo, false);

        if (!compClubInfo.flagCorrectWork || tempEvent.ID == 0 || tempEvent.ID > 4 ||
                (tempEvent.ID == 2 && (tempEvent.tableIndex == 0 || tempEvent.tableIndex > compClubInfo.tableNumber))) {
            cout << tempEvent.ID << " " << tempEvent.personName;
            if (tempEvent.ID == 2)
                cout << " " << tempEvent.tableIndex;
            cout << endl;

            inputFile.close();
            return eventIn;
        }
        eventIn.push_back(tempEvent);
    }
    inputFile.close();
    return eventIn;
}

void ComputerClub::printEvent(computerClubEvent &compEvent, unsigned short newEventID, unsigned short errorIndex) {
    
    const string errorMessage[6] = { "OK", "YouShallNotPass", "NotOpenYet",
                                     "PlaceIsBusy", "ClientUnknown", "ICanWaitNoLonger!" };
    compEvent.ID = newEventID;
    if (errorIndex > 0)
        compEvent.personName = errorMessage[errorIndex];

    cout << timeToString(compEvent.time) << " " << compEvent.ID << " " << compEvent.personName;
    if (compEvent.ID == 2 || compEvent.ID == 12)
        cout << " " << compEvent.tableIndex;
    cout << endl;

    return;
}

pair<short, unsigned int>
ComputerClub::calculateCostOfTable(unsigned short timeEnd, 
                                   unsigned short timeStart, unsigned short costOfHour) {
    short diffClientTime = timeEnd - timeStart;
    if (diffClientTime < 0)
        diffClientTime += 1440;

    unsigned short hourShort = diffClientTime / 60;
    if (hourShort % 60 != 0)
        hourShort++;

    return make_pair(diffClientTime, hourShort * costOfHour);
}

void ComputerClub::checkLastClients(computerClubInfo &compClubInfo,
                                    map<string, pair<unsigned short, unsigned short>> &clientToTableMap,
                                    vector<pair<unsigned short, unsigned int>> &tablesInfo) {
    vector<string> lastClients;
    for (auto itr = clientToTableMap.cbegin(); itr != clientToTableMap.cend(); itr++)
        lastClients.push_back(itr -> first);

    sort(lastClients.begin(), lastClients.end());

    for (auto &clubClient : lastClients) {
        pair<unsigned short, unsigned short> lastClientTable = clientToTableMap[clubClient];
        if (lastClientTable.first != 0) {
            pair<unsigned short, unsigned int> diffClientTimeCost = 
                calculateCostOfTable(compClubInfo.timeEnd, lastClientTable.second, compClubInfo.costOfHour);
            tablesInfo[lastClientTable.first].first  += diffClientTimeCost.first;
            tablesInfo[lastClientTable.first].second += diffClientTimeCost.second;

            computerClubEvent currentClubEvent;
            currentClubEvent.time = compClubInfo.timeEnd;
            currentClubEvent.personName = clubClient;           
            printEvent(currentClubEvent, 11, 0);
        }
    }
    return;
}

vector<pair<unsigned short, unsigned int>>
ComputerClub::eventProcessing(computerClubInfo &compClubInfo, vector<computerClubEvent> &compClubEventIn) {
    vector<bool> occupiedTables;
    vector<pair<unsigned short, unsigned int>> tablesInfo;
    queue<string> queueOfClients;
    map<string, pair<unsigned short, unsigned short>> clientToTableMap;

    unsigned short occupiedTablesCount = 0;
    
    for (unsigned short i = 0; i <= compClubInfo.tableNumber; i++) {
        occupiedTables.push_back(false);
        tablesInfo.push_back(make_pair(0, 0));
    }

    for (auto &currentClubEvent : compClubEventIn) {
        printEvent(currentClubEvent, currentClubEvent.ID, 0);
        switch (currentClubEvent.ID) {
            case 1:
                if (currentClubEvent.time < compClubInfo.timeStart || currentClubEvent.time > compClubInfo.timeEnd)
                    printEvent(currentClubEvent, 13, 2);
                else if (clientToTableMap.find(currentClubEvent.personName) != clientToTableMap.end())
                    printEvent(currentClubEvent, 13, 1);
                else
                    clientToTableMap[currentClubEvent.personName] = make_pair(0, currentClubEvent.time);
                break;

            case 2:
                if (clientToTableMap.find(currentClubEvent.personName) == clientToTableMap.end())
                    printEvent(currentClubEvent, 13, 4);
                else if (occupiedTables[currentClubEvent.tableIndex])
                    printEvent(currentClubEvent, 13, 3);
                else {
                    pair<unsigned short, unsigned short> prevClientTable = clientToTableMap[currentClubEvent.personName];                   
                    if (prevClientTable.first != 0) {
                        occupiedTables[prevClientTable.first] = false;

                        pair<unsigned short, unsigned int> diffClientTimeCost =
                            calculateCostOfTable(currentClubEvent.time, prevClientTable.second, compClubInfo.costOfHour);
                        tablesInfo[prevClientTable.first].first  += diffClientTimeCost.first;
                        tablesInfo[prevClientTable.first].second += diffClientTimeCost.second;
                    }
                    else
                        occupiedTablesCount++;

                    occupiedTables[currentClubEvent.tableIndex] = true;
                    clientToTableMap[currentClubEvent.personName] = make_pair(currentClubEvent.tableIndex, currentClubEvent.time);
                }
                break;

            case 3:
                if (occupiedTablesCount < compClubInfo.tableNumber)
                    printEvent(currentClubEvent, 13, 5);
                else if (queueOfClients.size() > compClubInfo.tableNumber)
                    printEvent(currentClubEvent, 11, 0);
                /*
                else if (clientToTableMap.find(currentClubEvent.personName) == clientToTableMap.end())
                    printEvent(currentClubEvent, 13, 4);
                */
                else
                    queueOfClients.push(currentClubEvent.personName);
                break;

            case 4:
                if (clientToTableMap.find(currentClubEvent.personName) == clientToTableMap.end())
                    printEvent(currentClubEvent, 13, 4);
                else {
                    pair<unsigned short, unsigned short> prevClientTable = clientToTableMap[currentClubEvent.personName];
                    if (prevClientTable.first == 0)
                        break;

                    occupiedTablesCount--;
                    occupiedTables[prevClientTable.first] = false;
                    clientToTableMap.erase(clientToTableMap.find(currentClubEvent.personName));

                    pair<unsigned short, unsigned int> diffClientTimeCost =
                        calculateCostOfTable(currentClubEvent.time, prevClientTable.second, compClubInfo.costOfHour);
                    tablesInfo[prevClientTable.first].first  += diffClientTimeCost.first;
                    tablesInfo[prevClientTable.first].second += diffClientTimeCost.second;                  

                    if (queueOfClients.size() > 0) {
                        string nextClient = queueOfClients.front();
                        queueOfClients.pop();

                        occupiedTablesCount++;
                        occupiedTables[prevClientTable.first] = true;
                        clientToTableMap[nextClient] = make_pair(prevClientTable.first, currentClubEvent.time);

                        currentClubEvent.personName = nextClient;
                        currentClubEvent.tableIndex = prevClientTable.first;
                        printEvent(currentClubEvent, 12, 0);
                    }
                }
                break;
        }
    }
    checkLastClients(compClubInfo, clientToTableMap, tablesInfo);
    return tablesInfo;
}

void ComputerClub::printTablesInfo(vector<pair<unsigned short, unsigned int>> &tablesInfo) {
    for (auto itr = 1; itr < tablesInfo.size(); itr++)
        cout << itr << " " << tablesInfo[itr].second << " " << timeToString(tablesInfo[itr].first) << endl;
    return;
}

void ComputerClub::computerClubMain(const string &fname) {
    computerClubInfo compClubInfo;
    vector<computerClubEvent> compClubEventIn = openFile(fname, compClubInfo);
    if (!compClubInfo.flagCorrectWork)
        return;

    cout << timeToString(compClubInfo.timeStart) << endl;

    vector<pair<unsigned short, unsigned int>> tablesInfo = eventProcessing(compClubInfo, compClubEventIn);

    cout << timeToString(compClubInfo.timeEnd) << endl;

    printTablesInfo(tablesInfo);

    return;
}