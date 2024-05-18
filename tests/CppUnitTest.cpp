
#include "CppUnitTest.h"

#include "..\include\ComputerClub.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CppUnitTest {

	TEST_CLASS(TimeToShort) {
		
		pair<unsigned short, unsigned short> eventTime;
		
	public:
		
		TEST_METHOD(Zero) {
			eventTime = make_pair(0, 0);
			unsigned short timeToShort = ComputerClub::timeToShort(eventTime);
			Assert::AreEqual(0, timeToShort, L"Error in TimeToShort with Zeros");
		}
		
	};
}