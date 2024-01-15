/*
* 24.1.7
* CommandPrmpt_One
* ���α׷� ����: ��� ������Ʈ�� ���
* �ָ��� ����
* 1. ��ɾ �߰��� �� ����Ǵ� �κ�
* 2. ��ɾ��� ��ҹ��ڸ� �������� �ʱ� ���ؼ� ����� �κ�
* 3. ��ɾ� EXIT�� �ԷµǾ��� �� ���α׷� ������
*/

/*
* 24.1.8
* CommandPrompt_Two
* ����: �ڽ� ���μ��� ���� �ڵ� �߰� + �߰� ��� ����
*/

/*
* 24.1.14
* CommandPrompt_Three
* ����: echo, start ��ɾ� �߰�
* start: �Էµ� ��ɾ ������ ��� ������Ʈ�� ��� �� �����ϵ��� �䱸�ϴ� ���, �ɼ� �߰��� ����
* echo: ���ڿ��� ���� ���, ��ɾ� ���� ���ڿ��� �״�� ����ϴ� ���
* ���μ��� ���� �� ���ڿ� �Ű������� ���޿� �Ű澵 ��
*/

// Test

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>	//�ѱ� �Է��� ���� �������
#include <Windows.h>

#define STR_LEN	256
#define CMD_TOKEN_NUM 10

//�������� ���ڿ� ����
TCHAR ERROR_CMD[] = _T("'%s'��(��) ������ �� �ִ� ���α׷��� �ƴմϴ�. \n");

//��ū ���� ���ڿ�
TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" ,\t\n");	// ���⵵ seps�� ���� ��, �� �־ ���� ���� ������

//�Լ� ����
int CmdTokenize(void);
int CmdProcessing(int);
TCHAR* StrLower(TCHAR*);

int _tmain(int argc, TCHAR* argv[])
{
	//�����ڵ� ���
	//�ѱ� �Է��� �����ϰ� �ϱ� ����
	_tsetlocale(LC_ALL, _T("Korean"));

	/* �Ű����� ���ڰ� �ִ� ��� */
	if (argc > 2)
	{
		for (int i = 1; i < argc; i++)
			_tcscpy(cmdTokenList[i - 1], argv[i]);
		// �������ϸ��� ������ �� 
		CmdProcessing(argc - 1);
	}

	DWORD isExit = 0;

	while (1)
	{
		int tokenNum = CmdTokenize();

		// Enter �Է� ó���� ���� �κ�
		if (tokenNum == 0)
			continue;

		isExit = CmdProcessing(tokenNum);
		if (isExit == TRUE)
		{
			_fputts(_T("��ɾ� ó���� �����մϴ�. \n"), stdout);
			break;
		}
	}
	return 0;
}

/*
	�Լ�: int CmdTokenize(void)
	���: ����ڷκ��� ������ �Է¹޴� ��쿡 ��ū�� �и��Ѵ�
			main �Լ��� ���ڷ� ���޵Ǵ� ��쿡�� �Է��� �ʿ���⿡ CmdProcessing�� �и�
	��ȯ��: ��ū�� ����
*/
int CmdTokenize(void)
{
	TCHAR* token;

	_fputts(_T("Best command prompt>> "), stdout);
	_fgetts(cmdString, STR_LEN, stdin);	//_getts()�Լ� ��� ���

	token = _tcstok(cmdString, seps);
	int tokenNum = 0;

	//��ū���� �и��ϴ� ����
	while (token != NULL)
	{
		// StrLower�Լ��� ���� ��ҹ����� ������ ����
		_tcscpy(cmdTokenList[tokenNum++], StrLower(token));
		token = _tcstok(NULL, seps);
	}
	return tokenNum;
}

/*
	�Լ�: int CmdProcessing(void)
	���: ��ū�� ������ ���ڷ� ���� ��, ���ÿ� ���� ��ɾ��� ó���� ����
		exit�� �ԷµǸ� TRUE�� ��ȯ�ϰ�, ���α׷��� ����� �̾�����.
	��ȯ��: ��ɾ�
	�Ű�����: ��ū�� ����
*/
int CmdProcessing(int tokenNum)
{
	BOOL isRun;			// ���� ����� ���μ����� ���翩��

	// ���μ��� ������ ���� �� ���� ����ü ����
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);

	TCHAR cmdStringWithOptions[STR_LEN] = { 0, };	// ���ο� ���μ��� ���� �� ���޵Ǵ� �̸�
	TCHAR optString[STR_LEN] = { 0, };

	if (!_tcscmp(cmdTokenList[0], _T("exit")))
	{
		//���α׷� ���Ḧ ���� ��ȯ��
		return TRUE;
	}
	else if (!_tcscmp(cmdTokenList[0], _T("start")))
	{
		// ���ο� ������Ʈ�� ��� �� ����
		// ���������� �̸� : CmdPrompt.exe
		// ������ �ɼ� �߰� �ʿ�
		if (tokenNum > 1)
		{
			// start ��ɾ �����ϰ� ���ڿ� �籸��
			for (int i = 1; i < tokenNum; i++)
				// ���ڿ��� ���ο� ���ڿ��� ������ �� ���� �Լ�
				_stprintf(optString, _T("%s %s"), optString, cmdTokenList[i]);
			_stprintf(cmdStringWithOptions, _T("%s %s"), _T("CmdPromptProject.exe"), optString);
		}
		else
			// start ��ɾ ����ϴ� ���
			_stprintf(cmdStringWithOptions, _T("%s"), _T("CmdPromptProject.exe"));

		isRun = CreateProcess(NULL, cmdStringWithOptions, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else if (!_tcscmp(cmdTokenList[0], _T("echo")))
	{
		// ��ɾ� �� ���ڿ��� �״�� ���
		for (int i = 1; i < tokenNum; i++)
			_stprintf(optString, _T("%s %s"), optString, cmdTokenList[i]);
		_tprintf(_T("echo message: %s\n"), optString);
	}
	else
	{
		_tcscpy(cmdStringWithOptions, cmdTokenList[0]);

		// Check Point1 : ���ڿ� ó���� ���� for��
		for (int i = 1; i < tokenNum; i++)
			_stprintf(cmdStringWithOptions, _T("%s %s"), cmdStringWithOptions, cmdTokenList[i]);


		// ���� �߰��� ��ɾ �ƴ� �� ǥ�� �˻���ο� �����ϴ� ���������� ����
		isRun = CreateProcess(NULL, cmdStringWithOptions, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if (isRun == 0)
		{
			DWORD error = GetLastError();
			_tprintf(_T("CreateProcess�� ���� %lu�� �����߽��ϴ�.\n"), error);
			_tprintf(ERROR_CMD, cmdTokenList[0]);
		}
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