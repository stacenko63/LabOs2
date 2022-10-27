#include <stdio.h>
#include <windows.h>
#define BUFLEN 32
#include <iostream>
#include <string>
using namespace std;


int main() {

	setlocale(LC_ALL, "RUS");
	char buffer[BUFLEN];
	unsigned long n;
	HANDLE s = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, L"mysem_child3");
	HANDLE m = CreateMailslot(L"\\\\.\\mailslot\\myslot_sqrt", 0, MAILSLOT_WAIT_FOREVER, NULL);


	while (true) {

		WaitForSingleObject(s, INFINITE);
		cout << "������� 3: ���������� ����������� ����� ����� ������\n";
		ReadFile(m, &buffer, BUFLEN, &n, NULL);
		
		cout << "������� 3: �� ����������� ����� �� �������� �������� ��������� ������\n";
		double tmp = atof(buffer);
		tmp = sqrt(tmp);
		cout << "������� 3: ��������� ���������� ����������� �����: " << tmp << "\n";

		HANDLE m2 = CreateFile(L"\\\\.\\mailslot\\myslot", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, 0);
		cout << "������� 3: ������������ ����� ���������� ������� �������� ��������\n";
		WriteFile(m2, to_string(tmp).c_str(), strlen(to_string(tmp).c_str()), &n, NULL);
		ReleaseSemaphore(s, 1, NULL);

	}
}
