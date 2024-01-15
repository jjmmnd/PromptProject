/*
* 24.1.7
* CommandPrmpt_One
* 프로그램 설명: 명령 프롬포트의 골격
* 주목할 사항
* 1. 명령어를 추가할 때 변경되는 부분
* 2. 명령어의 대소문자를 구분하지 않기 위해서 고려된 부분
* 3. 명령어 EXIT가 입력되었을 때 프로그램 종료방식
*/

/*
* 24.1.8
* CommandPrompt_Two
* 수정: 자식 프로세스 생성 코드 추가 + 추가 기능 수행
*/

/*
* 24.1.14
* CommandPrompt_Three
* 수정: echo, start 명령어 추가
* start: 입력된 명령어를 별도의 명령 프롬포트를 띄운 후 실행하도록 요구하는 기능, 옵션 추가도 가능
* echo: 문자열을 에코 출력, 명령어 뒤의 문자열을 그대로 출력하는 기능
* 프로세스 생성 시 문자열 매개변수의 전달에 신경쓸 것
*/

// Test

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>	//한글 입력을 위한 헤더파일
#include <Windows.h>

#define STR_LEN	256
#define CMD_TOKEN_NUM 10

//전역변수 문자열 선언
TCHAR ERROR_CMD[] = _T("'%s'은(는) 실행할 수 있는 프로그램이 아닙니다. \n");

//토큰 관련 문자열
TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" ,\t\n");	// 띄어쓰기도 seps에 넣을 것, 안 넣어서 한참 오류 수정함

//함수 선언
int CmdTokenize(void);
int CmdProcessing(int);
TCHAR* StrLower(TCHAR*);

int _tmain(int argc, TCHAR* argv[])
{
	//유니코드 기반
	//한글 입력을 가능하게 하기 위해
	_tsetlocale(LC_ALL, _T("Korean"));

	/* 매개변수 인자가 있는 경우 */
	if (argc > 2)
	{
		for (int i = 1; i < argc; i++)
			_tcscpy(cmdTokenList[i - 1], argv[i]);
		// 실행파일명을 제외한 것 
		CmdProcessing(argc - 1);
	}

	DWORD isExit = 0;

	while (1)
	{
		int tokenNum = CmdTokenize();

		// Enter 입력 처리를 위한 부분
		if (tokenNum == 0)
			continue;

		isExit = CmdProcessing(tokenNum);
		if (isExit == TRUE)
		{
			_fputts(_T("명령어 처리를 종료합니다. \n"), stdout);
			break;
		}
	}
	return 0;
}

/*
	함수: int CmdTokenize(void)
	기능: 사용자로부터 선택을 입력받는 경우에 토큰을 분리한다
			main 함수의 인자로 전달되는 경우에는 입력이 필요없기에 CmdProcessing과 분리
	반환값: 토큰의 개수
*/
int CmdTokenize(void)
{
	TCHAR* token;

	_fputts(_T("Best command prompt>> "), stdout);
	_fgetts(cmdString, STR_LEN, stdin);	//_getts()함수 대신 사용

	token = _tcstok(cmdString, seps);
	int tokenNum = 0;

	//토큰으로 분리하는 과정
	while (token != NULL)
	{
		// StrLower함수를 통해 대소문자의 구분을 없앰
		_tcscpy(cmdTokenList[tokenNum++], StrLower(token));
		token = _tcstok(NULL, seps);
	}
	return tokenNum;
}

/*
	함수: int CmdProcessing(void)
	기능: 토큰의 개수를 인자로 받은 후, 선택에 따른 명령어의 처리를 수행
		exit가 입력되면 TRUE를 반환하고, 프로그램의 종료로 이어진다.
	반환값: 명령어
	매개변수: 토큰의 개수
*/
int CmdProcessing(int tokenNum)
{
	BOOL isRun;			// 새로 만드는 프로세스의 존재여부

	// 프로세스 생성을 위한 두 개의 구조체 선언
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);

	TCHAR cmdStringWithOptions[STR_LEN] = { 0, };	// 새로운 프로세스 생성 시 전달되는 이름
	TCHAR optString[STR_LEN] = { 0, };

	if (!_tcscmp(cmdTokenList[0], _T("exit")))
	{
		//프로그램 종료를 위한 반환값
		return TRUE;
	}
	else if (!_tcscmp(cmdTokenList[0], _T("start")))
	{
		// 새로운 프롬포트를 띄운 후 실행
		// 실행파일의 이름 : CmdPrompt.exe
		// 별도의 옵션 추가 필요
		if (tokenNum > 1)
		{
			// start 명령어를 제외하고 문자열 재구성
			for (int i = 1; i < tokenNum; i++)
				// 문자열에 새로운 문자열을 저장할 때 쓰는 함수
				_stprintf(optString, _T("%s %s"), optString, cmdTokenList[i]);
			_stprintf(cmdStringWithOptions, _T("%s %s"), _T("CmdPromptProject.exe"), optString);
		}
		else
			// start 명령어만 사용하는 경우
			_stprintf(cmdStringWithOptions, _T("%s"), _T("CmdPromptProject.exe"));

		isRun = CreateProcess(NULL, cmdStringWithOptions, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else if (!_tcscmp(cmdTokenList[0], _T("echo")))
	{
		// 명령어 뒤 문자열을 그대로 출력
		for (int i = 1; i < tokenNum; i++)
			_stprintf(optString, _T("%s %s"), optString, cmdTokenList[i]);
		_tprintf(_T("echo message: %s\n"), optString);
	}
	else
	{
		_tcscpy(cmdStringWithOptions, cmdTokenList[0]);

		// Check Point1 : 문자열 처리를 위한 for문
		for (int i = 1; i < tokenNum; i++)
			_stprintf(cmdStringWithOptions, _T("%s %s"), cmdStringWithOptions, cmdTokenList[i]);


		// 직접 추가한 명령어가 아닐 시 표준 검색경로에 존재하는 실행파일을 실행
		isRun = CreateProcess(NULL, cmdStringWithOptions, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if (isRun == 0)
		{
			DWORD error = GetLastError();
			_tprintf(_T("CreateProcess가 오류 %lu로 실패했습니다.\n"), error);
			_tprintf(ERROR_CMD, cmdTokenList[0]);
		}
	}
	return 0;
}

/*
	함수: TCHAR* StrLower(TCHAR *pStr)
	기능: 문자열 내에 존재하는 모든 대문자를 소문자로 변경한다.
	반환값: 변경된 문자열의 포인터
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