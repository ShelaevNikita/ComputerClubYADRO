
/*
    Основной файл класса ComputerClub со всеми функциями/методами
*/

#include "../include/ComputerClub.h"

// Преобразование строки формата XX:XX в количество минут и проверка результата на корректность
unsigned short ComputerClub::checkTime(const string &eventTime, computerClubInfo &compClubInfo,
                                       bool flagEndl, bool flagPrint) {

    // Вычисление общего количества минут из строки формата XX:XX
    unsigned short timeTemp = stoi(eventTime.substr(0, 2)) * 60 + stoi(eventTime.substr(3, 5));

    // Проверка строки с временем и самого времени на корректность
    if (!compClubInfo.flagCorrectWork || timeTemp >= 1440 || 
        eventTime.length() != 5 || eventTime[2] != ':') {
        if (flagPrint) {
            cout << eventTime;
            if (flagEndl)
                cout << endl;
            else
                cout << " ";
        }
        compClubInfo.flagCorrectWork = false;
        return 0;
    }
    return timeTemp;
}

// Преобразование количества минут в строку формата XX:XX 
string ComputerClub::timeToString(unsigned short time) {
    unsigned short timeHour = time / 60;
    unsigned short timeMin  = time % 60;
    return to_string(timeHour / 10) + to_string(timeHour % 10) + ":" + to_string(timeMin / 10) + to_string(timeMin % 10);
}

// Получение всей информации из входного файла 
vector<ComputerClub::computerClubEvent> 
ComputerClub::openFile(const string &fname, computerClubInfo &compClubInfo) {

    // Вектор входящих событий
	vector<computerClubEvent> eventIn;
 
    ifstream inputFile(fname);

    // Если не удалось открыть входной файл
    if (!inputFile.is_open()) {            
        inputFile.close();
        cout << " ERROR: Not found inputFile!..." << endl;
        compClubInfo.flagCorrectWork = false;
        return eventIn;
    }
	
	// Строка для записи времени
    string eventTime;

	// Запись входящего события	
    computerClubEvent tempEvent;          

    inputFile >> compClubInfo.tableNumber;

    // Если количество столов == 0
    if (compClubInfo.tableNumber == 0) {
        cout << compClubInfo.tableNumber << endl;
        compClubInfo.flagCorrectWork = false;
        inputFile.close();
        return eventIn;
    }

    // Время начала работы компьютерного клуба
    inputFile >> eventTime;
    compClubInfo.timeStart = checkTime(eventTime, compClubInfo, false, true);

    // Время окончания работы компьютерного клуба
    inputFile >> eventTime;
    compClubInfo.timeEnd = checkTime(eventTime, compClubInfo, true, true);

    // Если формат входного файла некорректный
    if (!compClubInfo.flagCorrectWork) {
        inputFile.close();
        return eventIn;
    }

    inputFile >> compClubInfo.costOfHour;

    // Если стоимость одного часа == 0
    if (compClubInfo.costOfHour == 0) {
        cout << compClubInfo.costOfHour << endl;
        compClubInfo.flagCorrectWork = false;
        inputFile.close();
        return eventIn;
    }

    // Последовательная проверка и запись в вектор всех входящих событий
    while (inputFile >> eventTime >> tempEvent.ID >> tempEvent.personName) {
        if (tempEvent.ID == 2)
            inputFile >> tempEvent.tableIndex;
        else
            tempEvent.tableIndex = 0;

        // Время события
        tempEvent.time = checkTime(eventTime, compClubInfo, false, false);

        // Проверка события на корректность
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

// Вычисление выручки (с округлением до часа в большую сторону) для одного стола:
//  (( Время события с ID == 2 или ID == 12 ) - ( Время события с ID == 4 или ID == 11 )) * Стоимость одного часа
pair<unsigned int, unsigned short>
ComputerClub::calculateCostOfTable(unsigned short timeEnd, unsigned short timeStart, unsigned short costOfHour) {

    short diffClientTime = timeEnd - timeStart;
	
	// Если компьютерный клуб работает в ночное время или круглосуточно
	// 	может возникнуть отрицательная разница во времени
    if (diffClientTime < 0)                       
        diffClientTime += 1440;   

    unsigned short hourShort = diffClientTime / 60;
	
	// Округление до часа в большую сторону
    if (diffClientTime % 60 > 0)                  
        hourShort++;

    return { hourShort * costOfHour, diffClientTime };
}

// Вывод в консоль (выходной файл) информации по конкретному событию
void ComputerClub::printEvent(computerClubEvent &compEvent, unsigned short newEventID,
                              const string &errorMessage) {
		
	// Часто входящие и исходящие события различаются только ID
    compEvent.ID = newEventID;
	
	// Если событие с ID == 13 -> вывод названия ошибки
    if (compEvent.ID == 13)
        compEvent.personName = errorMessage;

    // Запись в консоль (выходной файл) информации по событию
    cout << timeToString(compEvent.time) << " " << compEvent.ID << " " << compEvent.personName;
    if (compEvent.ID == 2 || compEvent.ID == 12)
        cout << " " << compEvent.tableIndex;
    cout << endl;

    return;
}

// Вывод в консоль (выходной файл) информации и подсчёт выручки для последних клиентов,
//	которые ушли перед закрытием компьютерного клуба 
void ComputerClub::checkLastClients(const computerClubInfo &compClubInfo,
                                    const map<string, pair<unsigned short, unsigned short>> &clientToTableMap,
                                    vector<pair<unsigned int, unsigned short>> &tablesInfo) {

    // Информация о столе и времени клиента
    pair<unsigned short, unsigned short> clientInfo;
	
	// Информация о событии
	computerClubEvent currentClubEvent;
	
    // Последовательная обработка всех последних клиентов
    //  (ключи в словаре уже упорядочены по возрастанию - имена клиентов в алфавитном порядке)
    for (const auto &lastClient : clientToTableMap) {

        clientInfo = lastClient.second;

        // Если клиент не сидел за столом, он просто уходит
        if (clientInfo.first > 0) {

            // Вычисление выручки и времени занимания клиентом стола
            pair<unsigned short, unsigned int> diffClientTimeCost = 
                calculateCostOfTable(compClubInfo.timeEnd, clientInfo.second, compClubInfo.costOfHour);

            // Обновление информации для занимаемого стола
            tablesInfo[clientInfo.first].first  += diffClientTimeCost.first;
            tablesInfo[clientInfo.first].second += diffClientTimeCost.second;
        }
		
        // Вывод в консоль (выходной файл) события с ID == 11 и временем закрытия компьютерного клуба
        currentClubEvent.time       = compClubInfo.timeEnd;
        currentClubEvent.personName = lastClient.first;
        printEvent(currentClubEvent, 11, "\0");
    }
    return;
}

// Последовательная обработка всех входящих событий
vector<pair<unsigned int, unsigned short>>
ComputerClub::eventProcessing(const computerClubInfo &compClubInfo, 
                              vector<computerClubEvent> &compClubEventIn,
                              const map<errorType, string> &errorMessage) {

    // Вектор занимаемых компьютерных столов
    vector<bool> occupiedTables(compClubInfo.tableNumber + 1, false);

    // Вектор для записи информации о каждом компьютерном столе:
    //  (выручка; общее время работы)
    vector<pair<unsigned int, unsigned short>> tablesInfo(compClubInfo.tableNumber + 1, { 0, 0 });

    // Очередь из клиентов (события с ID == 3) размером не больше количества столов
    queue<string> queueOfClients;

    // Словарь для хранения информации о клиентах, которые находятся внутри компьютерного клуба:
    //  ключ - имя клиента, значение - (индекс стола; время в минутах, когда клиент сел за этот стол)
    map<string, pair<unsigned short, unsigned short>> clientToTableMap;

    // Счётчик занятых компьютерных столов
    unsigned short occupiedTablesCount = 0;

    // Цикл по всем входящим событиям
    for (auto &currentClubEvent : compClubEventIn) {

        // Запись в консоль (выходной файл) информации о входящем событии 
        printEvent(currentClubEvent, currentClubEvent.ID, "\0");

        // Если компьютерный клуб работает в ночное время или круглосуточно
        unsigned short flag = 0;
        if (compClubInfo.timeEnd <= compClubInfo.timeStart)
            flag = 1440;

        // Выбор направления обработки события в зависимости от его ID
        switch (currentClubEvent.ID) {
            case 1:
                if (currentClubEvent.time < compClubInfo.timeStart || currentClubEvent.time > compClubInfo.timeEnd + flag)
					// Ошибка "NotOpenYet"
                    printEvent(currentClubEvent, 13, errorMessage.at(NotOpenYet));
                else if (clientToTableMap.find(currentClubEvent.personName) != clientToTableMap.end())
					// Ошибка "YouShallNotPass"
                    printEvent(currentClubEvent, 13, errorMessage.at(YouShallNotPass));
                else
                    // Клиент зашёл внутрь компьютерного клуба, но ещё не сел за стол
                    clientToTableMap[currentClubEvent.personName] = { 0, currentClubEvent.time };

                break;

            case 2:
                if (clientToTableMap.find(currentClubEvent.personName) == clientToTableMap.end())
					// Ошибка "ClientUnknown"
                    printEvent(currentClubEvent, 13, errorMessage.at(ClientUnknown));
                else if (occupiedTables[currentClubEvent.tableIndex])
					// Ошибка "PlaceIsBusy"
                    printEvent(currentClubEvent, 13, errorMessage.at(PlaceIsBusy));
                else {
                    // Получение информации о предыдущем компьютерном столе клиента
                    pair<unsigned short, unsigned short> prevClientTable = clientToTableMap[currentClubEvent.personName];

                    // Если он уже сидел за столом
                    if (prevClientTable.first != 0) {

                        // Освобождение предыдущего стола
                        occupiedTables[prevClientTable.first] = false;

                        // Вычисление выручки и времени занимания клиентом стола
                        pair<unsigned int, unsigned short> diffClientTimeCost =
                            calculateCostOfTable(currentClubEvent.time, prevClientTable.second, compClubInfo.costOfHour);

                        // Обновление информации для предыдущего стола
                        tablesInfo[prevClientTable.first].first  += diffClientTimeCost.first;
                        tablesInfo[prevClientTable.first].second += diffClientTimeCost.second;
                    }
                    else
                        // Иначе клиент занимает стол
                        occupiedTablesCount++;

                    // Клиент садится за компьютерный стол
                    occupiedTables[currentClubEvent.tableIndex] = true;

                    // Обновление информации в используемом словаре
                    clientToTableMap[currentClubEvent.personName] = { currentClubEvent.tableIndex, currentClubEvent.time };
                }
                break;

            case 3:
                if (occupiedTablesCount < compClubInfo.tableNumber)
					// Ошибка "ICanWaitNoLonger!"
                    printEvent(currentClubEvent, 13, errorMessage.at(ICanWaitNoLonger));
                else if (clientToTableMap.find(currentClubEvent.personName) == clientToTableMap.end())
                    // Ошибка "ClientUnknown" (отсутствует в условии задания, но требуется для корректной работы программы)
                    printEvent(currentClubEvent, 13, errorMessage.at(ClientUnknown));
                else if (queueOfClients.size() == compClubInfo.tableNumber) {
                    // Очередь переполнена, и клиент просто уходит
                    clientToTableMap.erase(currentClubEvent.personName);
                    printEvent(currentClubEvent, 11, "\0");
                }
                else
                    // Добавление клиента в очередь
                    queueOfClients.push(currentClubEvent.personName);

                break;

            case 4:
                if (clientToTableMap.find(currentClubEvent.personName) == clientToTableMap.end())
                    // Ошибка "ClientUnknown"
                    printEvent(currentClubEvent, 13, errorMessage.at(ClientUnknown));
                else {
                    // Получение информации о компьютерном столе уходящего клиента
                    pair<unsigned short, unsigned short> prevClientTable = clientToTableMap[currentClubEvent.personName];

                    // Если уходящий клиент не сидел за столом, то он просто уходит
                    if (prevClientTable.first == 0)
                        break;

                    // Обновление информации о занимаемых столах и в словаре
                    occupiedTablesCount--;
                    occupiedTables[prevClientTable.first] = false;
                    clientToTableMap.erase(currentClubEvent.personName);

                    // Вычисление выручки и времени занимания клиентом стола
                    pair<unsigned short, unsigned int> diffClientTimeCost =
                        calculateCostOfTable(currentClubEvent.time, prevClientTable.second, compClubInfo.costOfHour);

                    // Обновление информации для освобождаемого стола
                    tablesInfo[prevClientTable.first].first  += diffClientTimeCost.first;
                    tablesInfo[prevClientTable.first].second += diffClientTimeCost.second;                  

                    // Если очередь пуста, то не будет исходящих событий с ID == 12
                    if (queueOfClients.empty())
                        break;

                    // Получение первого клиента из очереди
                    string nextClient = queueOfClients.front();
                    queueOfClients.pop();

                    // Обновление информации о столе, за который сел этот клиент
                    //  (клиент из очереди занимает только что освободившийся стол)
                    occupiedTablesCount++;
                    occupiedTables[prevClientTable.first] = true;
                    clientToTableMap[nextClient] = { prevClientTable.first, currentClubEvent.time };

                    // Вывод в консоль (выходной файл) события с ID == 12
                    currentClubEvent.personName = nextClient;
                    currentClubEvent.tableIndex = prevClientTable.first;
                    printEvent(currentClubEvent, 12, "\0");
                }
                break;
        }
    }
    // Обработка клиентов, находящихся в компьютерном клубе перед его закрытием
    checkLastClients(compClubInfo, clientToTableMap, tablesInfo);

    return tablesInfo;
}

// Вывод в консоль (выходной файл) информации (выручка и время работы) по всем столам компьютерного клуба
void ComputerClub::printTablesInfo(const vector<pair<unsigned int, unsigned short>> &tablesInfo) {
    for (auto itr = 1; itr < tablesInfo.size(); itr++)
        cout << itr << " " << tablesInfo[itr].first << " " << timeToString(tablesInfo[itr].second) << endl;
    return;
}

// Главный файл класса ComputerClub
void ComputerClub::computerClubMain(const string &fname) {

    computerClubInfo compClubInfo;

    vector<computerClubEvent> compClubEventIn = openFile(fname, compClubInfo);

    // Если входной файл не корректен, завершение работы программы
    if (!compClubInfo.flagCorrectWork)
        return;

    // Словарь, хранящий информацию об ошибках
    const map<errorType, string> errorMessage {
        { YouShallNotPass, "YouShallNotPass" },
        { NotOpenYet, "NotOpenYet" },
        { PlaceIsBusy, "PlaceIsBusy" },
        { ClientUnknown, "ClientUnknown" },
        { ICanWaitNoLonger, "ICanWaitNoLonger!" }
    };

    cout << timeToString(compClubInfo.timeStart) << endl;

    vector<pair<unsigned int, unsigned short>> tablesInfo = 
        eventProcessing(compClubInfo, compClubEventIn, errorMessage);

    cout << timeToString(compClubInfo.timeEnd) << endl;

    printTablesInfo(tablesInfo);

    return;
}