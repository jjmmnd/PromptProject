/*
* 24.1.7
* CommandPrmpt_One.cpp
* ���α׷� ����: ��� ������Ʈ�� ���
* �ָ��� ����
* 1. ��ɾ �߰��� �� ����Ǵ� �κ�
* 2. ��ɾ��� ��ҹ��ڸ� �������� �ʱ� ���ؼ� ����� �κ�
* 3. ��ɾ� EXIT�� �ԷµǾ��� �� ���α׷� ������
*/

/*
* 24.1.8
* CommandPrompt_Two.cpp
* ����: �ڽ� ���μ��� ���� �ڵ� �߰� + �߰� ��� ����
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>	//�ѱ� �Է��� ���� �������
#include <Windows.h>

#define STR_LEN	256
#define CMD_TOKEN_NUM	10

//�������� ���ڿ� ����
TCHAR ERROR_CMD[] = _T("'%s'��(��) ������ �� �ִ� ���α׷��� �ƴմϴ�. \n");

//�Լ� ����
int CmdProcessing(void);
TCHAR* StrLower(TCHAR*);


int _tmain(int argc, TCHAR* argv[])
{

	//�����ڵ� ���
	//�ѱ� �Է��� �����ϰ� �ϱ� ����
	_tsetlocale(LC_ALL, _T("Korean"));

	DWORD isExit;
	while (1)
	{
		isExit = CmdProcessing();
		if (isExit == TRUE)
		{
			_fputts(_T("��ɾ� ó���� �����մϴ�. \n"), stdout);
			break;
		}
	}
	return 0;
}

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(",\t\n");	//?? ��ū �и��� ���� �����ڸ� ����

/*
* �Լ�: TCHAR int CmdProcessing(void)
* ���: ��ɾ �Է� �޾Ƽ� �ش� ��ɾ �����Ǿ� �ִ� ����� ����
* exit�� �ԷµǸ� TRUE�� ��ȯ�ϰ�, ���α׷��� ����� �̾�����.
* ��ȯ��: ��ɾ�
*/
int CmdProcessing(void)
{
	_fputts(_T("Best command prompt>> "), stdout);
	_getts_s(cmdString);	//_getts()�Լ� ��� ���

	/*
		���μ��� ������ ���� �� ���� ����ü �ʱ�ȭ
	*/
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi = { 0, };
	//����ü ������ ũ�� ����, �̷��� ȣȯ���� ���� ��
	si.cb = sizeof(si);


	/*
	* char* strtok(char* s1, char* s2)
	* 1. s1���ڿ����� s2�����ڸ� ���� ��ū�� ����
	* 2. ��ū: Ư�� �����ڷ� �и��Ǵ� �ּ� ���� ���
	* 3. ��ȯ������ ���ڿ��� ������ or NULL
	* 4. �ϳ��� ��ū���� ����
	*/
	TCHAR* token = _tcstok(cmdString, seps);
	int tokenNum = 0;

	//��ū���� �и��ϴ� ����
	while (token != NULL)
	{
	
		// token�� NULL�� �־ ���ѷ����� ����?????????
		// StrLower�Լ��� ���� ��ҹ����� ������ ����
		_tcscpy(cmdTokenList[tokenNum++], StrLower(token));
		token = _tcstok(NULL, seps);
	}

	if (!_tcscmp(cmdTokenList[0], _T("exit")))
	{
		return TRUE;	//���α׷� ���Ḧ ���� ��ȯ��
	}
	else if (!_tcscmp(cmdTokenList[0], _T("�߰� �Ǵ� ��ɾ� 1")))
	{
		//���� ���� �߰��� ��ɾ��� �ڵ� ����
	}
	else
	{
		BOOL newProcess = CreateProcess(NULL, cmdTokenList[0], NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
		if(newProcess == false)
			_tprintf(ERROR_CMD, cmdTokenList[0]);
	}
	return 0;
}

/*
	�Լ�: TCHAR* StrLower(TCHAR *pStr)
	���: ���ڿ� ���� �����ϴ� ��� �빮�ڸ� �ҹ��ڷ� �����Ѵ�.
	��ȯ��: ����� ���ڿ��� ������
*/
TCHAR* StrLower(TCHAR* pStr)
{
	TCHAR* ret = pStr;

	while (*pStr)
	{
		if (_istupper(*pStr))
			*pStr = _totlower(*pStr);
		pStr++;
	}

	return ret;
}