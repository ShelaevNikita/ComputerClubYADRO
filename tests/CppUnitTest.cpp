
#include "CppUnitTest.h"

#include "..\include\ComputerClub.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CppUnitTest {

	TEST_CLASS(CheckTime) {
		
		string eventTime;
		computerClubInfo compClubInfo;
		unsigned short timeToShort;
		
	public:
		
		TEST_METHOD(Zero) {
			eventTime   = "00:00";
			timeToShort = ComputerClub::checkTime(eventTime, compClubInfo, false, false);
			Assert::AreEqual(0, timeToShort, L"Error in CheckTime with Zeros");
		}
		
		TEST_METHOD(IncorrectTime) {
			eventTime   = "24:00";
			timeToShort = ComputerClub::checkTime(eventTime, compClubInfo, false, false);
			Assert::AreEqual(false, compClubInfo.flagCorrectWork, L"Error in CheckTime with IncorrectTime");
			Assert::AreEqual(0, timeToShort, L"Error in CheckTime with IncorrectTime");
		}
		
		TEST_METHOD(IncorrectFormat) {
			eventTime   = "4:00";
			timeToShort = ComputerClub::checkTime(eventTime, compClubInfo, false, false);
			Assert::AreEqual(false, compClubInfo.flagCorrectWork, L"Error in CheckTime with IncorrectFormat");
			Assert::AreEqual(1440, timeToShort, L"Error in CheckTime with IncorrectFormat");
		}		
	};
	
	TEST_CLASS(TimeToString) {
		
		string eventTime;
		unsigned short timeToShort;
		
	public:
		
		TEST_METHOD(Zero) {
			timeToShort = 0;
			eventTime   = ComputerClub::timeToString(timeToShort);
			Assert::AreEqual("00:00", eventTime, L"Error in TimeToString with Zeros");
		}
		
		TEST_METHOD(IncorrectTime) {
			timeToShort = 1440;
			eventTime   = ComputerClub::timeToString(timeToShort);
			Assert::AreEqual("24:00", eventTime, L"Error in TimeToString with IncorrectTime");
		}
		
		TEST_METHOD(OneZero) {
			timeToShort = 548;
			eventTime   = ComputerClub::timeToString(timeToShort);
			Assert::AreEqual("09:08", eventTime, L"Error in TimeToString with OneZero");
		}		
	};
	
	TEST_CLASS(CalculateCostOfTable) {
		
		unsigned short timeEnd, timeStart, costOfHour;
		pair<unsigned int, unsigned short> diffClientTimeCost;	
		
	public:
		
		TEST_METHOD(Zeros) {
			timeEnd    = 0;
			timeStart  = 0;
			costOfHour = 1;
			diffClientTimeCost = ComputerClub::calculateCostOfTable(timeEnd, timeStart, costOfHour);
			Assert::AreEqual(0, diffClientTimeCost.first,  L"Error in CalculateCostOfTable with Zeros");
			Assert::AreEqual(0, diffClientTimeCost.second, L"Error in CalculateCostOfTable with Zeros");
		}
		
		TEST_METHOD(EqualTime) {
			timeEnd    = 524;
			timeStart  = 524;
			costOfHour = 10;
			diffClientTimeCost = ComputerClub::calculateCostOfTable(timeEnd, timeStart, costOfHour);
			Assert::AreEqual(0, diffClientTimeCost.first,  L"Error in CalculateCostOfTable with EqualTime");
			Assert::AreEqual(0, diffClientTimeCost.second, L"Error in CalculateCostOfTable with EqualTime");
		}
		
		TEST_METHOD(ZeroCost) {
			timeEnd    = 1048;
			timeStart  = 524;
			costOfHour = 0;
			diffClientTimeCost = ComputerClub::calculateCostOfTable(timeEnd, timeStart, costOfHour);
			Assert::AreEqual(0,   diffClientTimeCost.first,  L"Error in CalculateCostOfTable with ZeroCost");
			Assert::AreEqual(524, diffClientTimeCost.second, L"Error in CalculateCostOfTable with ZeroCost");
		}

		TEST_METHOD(StartToEnd) {
			timeEnd    = 524;
			timeStart  = 1028;
			costOfHour = 10;
			diffClientTimeCost = ComputerClub::calculateCostOfTable(timeEnd, timeStart, costOfHour);
			Assert::AreEqual(160, diffClientTimeCost.first,  L"Error in CalculateCostOfTable with StartToEnd");
			Assert::AreEqual(936, diffClientTimeCost.second, L"Error in CalculateCostOfTable with StartToEnd");
		}

		TEST_METHOD(RoundToMore) {
			timeEnd    = 1048;
			timeStart  = 524;
			costOfHour = 10;
			diffClientTimeCost = ComputerClub::calculateCostOfTable(timeEnd, timeStart, costOfHour);
			Assert::AreEqual(90,  diffClientTimeCost.first,  L"Error in CalculateCostOfTable with RoundToMore");
			Assert::AreEqual(524, diffClientTimeCost.second, L"Error in CalculateCostOfTable with RoundToMore");
		}
		
		TEST_METHOD(RoundToEqual) {
			timeEnd    = 1064;
			timeStart  = 524;
			costOfHour = 11;
			diffClientTimeCost = ComputerClub::calculateCostOfTable(timeEnd, timeStart, costOfHour);
			Assert::AreEqual(99,  diffClientTimeCost.first,  L"Error in CalculateCostOfTable with RoundToEqual");
			Assert::AreEqual(540, diffClientTimeCost.second, L"Error in CalculateCostOfTable with RoundToEqual");
		}
		
		TEST_METHOD(RoundToEqualStartToEnd) {
			timeEnd    = 560;
			timeStart  = 1040;
			costOfHour = 10;
			diffClientTimeCost = ComputerClub::calculateCostOfTable(timeEnd, timeStart, costOfHour);
			Assert::AreEqual(160, diffClientTimeCost.first,  L"Error in CalculateCostOfTable with RoundToEqualStartToEnd");
			Assert::AreEqual(960, diffClientTimeCost.second, L"Error in CalculateCostOfTable with RoundToEqualStartToEnd");
		}			
	};
}