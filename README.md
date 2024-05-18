
# ������������� ������ ������������� �����

## �������������
*������� �������� YADRO �� ����� ���������� � ������ ���������� �������� ������� :)*

## ������ ���������
��������� ����������� �� �� **C++** (***ISO C++ 17***).   
��������������� ����������� (����� ����������) � ������������� (������� �� ������) ����� �������������� 
`O(max(NLlogNL, K))` � `O(max(N, K))`, ��� ***N*** - ���������� ������ � ������������ �����,   
***L*** - ������� ���������� �������� � ������ �������� � ***K*** - ���������� �������� �������.   

����� �������������� �������� �����, ����� ��������������� ��������� ��������:   
`g++ -o test.exe ComputerClubStart.cpp include/ComputerClub.h src/ComputerClub.cpp`,   
��� `test.exe` - �������� ������������ �����.   

## ������������� �������
��� ������� ��������� ��������� �������� �� � �������� ��������� ��������� ������ �������� �������� �����.
������� ��� ����� �������� ������� (��� `examples/testFile1.txt` - �������� ���������� �����):
`./test.exe examples/testFile1.txt` - ��� ������ ���������� � ������� ���   
`./test.exe examples/testFile1.txt > output.txt` - ��� ������ ���������� � ����������� ���� (��������, `output.txt`).

## ������� ������������� �����
### �������� �������
+ ID1 - ������ ������: *<�����> 1 <��� �������>*
+ ID2 - ������ ��� �� ����: *<�����> 2 <��� �������> <����� �����>*
+ ID3 - ������ �������: *<�����> 3 <��� �������>*
+ ID4 - ������ ����: *<�����> 4 <��� �������>*

### ��������� �������
+ ID11 - ������ ����: *<�����> 11 <��� �������>*
+ ID12 - ������ ��� �� ����: *<�����> 12 <��� �������> <����� �����>*
+ ID13 - ������: *<�����> 13 <������>*

### ������
+ **YouShallNotPass** - ������ ��� ��������� � ������������ ����� (*ID1*)
+ **NotOpenYet** - ������ ������ � ��������� ���� (*ID1*)
+ **PlaceIsBusy** - ������ �������� ��������� �� ������� ���� (*ID2*)
+ **ClientUnknown** - ������ �� ��������� � ������������ ����� (*ID2*, *ID4*)
+ **ICanWaitNoLonger!** - � ����� ���� ��������� ����� (*ID3*)