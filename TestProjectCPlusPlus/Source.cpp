#include <stdio.h>
#include <windows.h>
#define BUFLEN 32
#include <iostream>
#include <string>
using namespace std;


double get_value(HANDLE m2, HANDLE m, HANDLE s2, const string& message) {
	char buffer[BUFLEN];
	unsigned long n;
	WriteFile(m2, message.c_str(), strlen(message.c_str()), &n, NULL);
	ReleaseSemaphore(s2, 1, NULL);
	WaitForSingleObject(s2, INFINITE);
	ReadFile(m, &buffer, BUFLEN, &n, NULL);
	return atof(buffer);
}

template<typename T>
void input_correctly_number(T& aa)
{
	while (!(cin >> aa) || cin.peek() != '\n')
	{
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
		cout << "������� �������� ��������. ��������� �������: ";
	}
}

void Limitations(double min, double& value)
{
	while (value < min)
	{
		cout << "�������� ������ ���� �������������. ��������� ����: ";
		input_correctly_number(value);
	}
}

int main() {
	setlocale(LC_ALL, "RUS");
	double a = 0, b = 0, result = 0;
	cout << "������� �������: ������ ������ �������� ��������\n";
	cout << "������� �������� ���������� a: ";
	input_correctly_number(a);
	Limitations(1, a);
	cout << "������� �������� ���������� b: ";
	input_correctly_number(b);
	Limitations(1, b);
	STARTUPINFO si;
	PROCESS_INFORMATION pi1, pi2, pi3;
	CreateSemaphore(NULL, 0, 1, L"mysem_child1");
	CreateSemaphore(NULL, 0, 1, L"mysem_child2");
	CreateSemaphore(NULL, 0, 1, L"mysem_child3");
	ZeroMemory(&si, sizeof(STARTUPINFO));	
	CreateProcess(L"C:\\Users\\�����\\source\\repos\\TestProjectCPlusPlus\\x64\\Debug\\Child1.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi1);
	CreateProcess(L"C:\\Users\\�����\\source\\repos\\TestProjectCPlusPlus\\x64\\Debug\\Child2.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi2);
	CreateProcess(L"C:\\Users\\�����\\source\\repos\\TestProjectCPlusPlus\\x64\\Debug\\Child3.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi3);
	HANDLE m = CreateMailslot(L"\\\\.\\mailslot\\myslot", 0, MAILSLOT_WAIT_FOREVER, NULL);
	HANDLE s2 = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, L"mysem_child1");
	HANDLE m2 = CreateFile(L"\\\\.\\mailslot\\myslot_square", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 0);
	cout << "������� �������: ���������� ��������� �������� #1 �������� ���������� a\n";
	a = get_value(m2, m, s2, to_string(a));
	cout << "������� �������: �������� �������� ���������� a, ����������� � �������: " << a << "\n";
	cout << "������� �������: ���������� ��������� �������� #1 �������� ���������� b\n";
	b = get_value(m2, m, s2, to_string(b));
	cout << "������� �������: �������� �������� ���������� b, ����������� � �������: " << b << "\n";
	cout << "������� �������: ���������� ��������� �������� #2 �������� ���������� a � b\n";
	s2 = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, L"mysem_child2");
	m2 = CreateFile(L"\\\\.\\mailslot\\myslot_sum", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 0);
	result = get_value(m2, m, s2, to_string(a) + " " + to_string(b) + "");
	cout << "������� �������: �������� �������� ����� ��������� ���������� a � b: " << result << "\n";
	cout << "������� �������: ���������� ��������� �������� #3 �������� ����� ���������� a � b\n";
	s2 = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, L"mysem_child3");
	m2 = CreateFile(L"\\\\.\\mailslot\\myslot_sqrt", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 0);
	result = get_value(m2, m, s2, to_string(result));
	cout << "������� �������: �������� �������� ����������� ����� ����� ��������� ���������� a � b: " << result << "\n";
	cout << "�������� ��������� " << result << "\n";
}