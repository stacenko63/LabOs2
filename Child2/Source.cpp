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
	HANDLE s = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, L"mysem_child2");
	HANDLE m = CreateMailslot(L"\\\\.\\mailslot\\myslot_sum", 0, MAILSLOT_WAIT_FOREVER, NULL);


	while (true) {

		WaitForSingleObject(s, INFINITE); 
		cout << "������� 2: �������� ����� ������\n";
		ReadFile(m, &buffer, BUFLEN, &n, NULL); 
		cout << "������� 2: ���������� ����� �� �������� �������� ����������\n";
		string a_ = "", b_ = "";
		int i = 0;
		while (buffer[i] != ' ') {
			a_ += buffer[i];
			i++;
		}
		i++;
		while (i != n) {
			b_ += buffer[i];
			i++;
		}
		cout << "������� 2: ���������� ����� �� �������� �������� ���������� " << a_ << " " << b_ << "\n";
		double result = stod(a_) + stod(b_);

		cout << "������� 2: ��������� ��������: " << result << "\n";
		HANDLE m2 = CreateFile(L"\\\\.\\mailslot\\myslot", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, 0);
		cout << "������� 2: ������������ ����� ���������� ������� �������� ��������\n";
		WriteFile(m2, to_string(result).c_str(), strlen(to_string(result).c_str()), &n, NULL);
		ReleaseSemaphore(s, 1, NULL);

	}
}
