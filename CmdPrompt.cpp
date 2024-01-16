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

/*
* 24.1.16
* CommandPrompt_Four
* 수정: lp, kp 명령어 추가
* lp: 현재 실행중인 프로세스 정보를 출력
* kp: 이름으로 프로세스를 종료 
*/

/*
* CommandPrompt_Five
* 수정: sort 명령어 추가
* 1. sort --> 문자열 정렬
* 2. sort > sort.dat --> 출력 리다이렉션 
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>	//한글 입력을 위한 헤더파일
#include <Windows.h>
#include <TlHelp32.h>

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
void ListProcess();
void KillProcess();

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
		// exit가 입력되면 
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
		// 실행파일의 이름 : CmdPromptProject.exe
		// 별도의 옵션 추가 필요
		if (tokenNum > 1)
		{
			// start 명령어를 제외하고 문자열 재구성
			for (int i = 1; i < tokenNum; i++)
			{
				// 문자열에 새로운 문자열을 저장할 때 쓰는 함수
				_stprintf(optString, _T("%s %s"), optString, cmdTokenList[i]);
			}
			_stprintf(cmdStringWithOptions, _T("%s %s"), _T("CmdPromptProject.exe"), optString);
		}
		// start 명령어만 사용하는 경우
		else
			_stprintf(cmdStringWithOptions, _T("%s"), _T("CmdPromptProject.exe"));

		isRun = CreateProcess(
			NULL, cmdStringWithOptions, NULL, NULL, 
			TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else if (!_tcscmp(cmdTokenList[0], _T("echo")))
	{
		// 명령어 뒤 문자열을 그대로 출력
		for (int i = 1; i < tokenNum; i++)
		{
			_stprintf(optString, _T("%s %s"), optString, cmdTokenList[i]);
		}
		_tprintf(_T("echo message: %s\n"), optString);
	}
	else if (!_tcscmp(cmdTokenList[0], _T("lp")))
	{
		ListProcess();
	}
	else if (!_tcscmp(cmdTokenList[0], _T("kp")))
	{
		if (tokenNum < 2)
		{
			_tprintf(_T("usage: kp <process name> \n"));
			return 0;
		}

		KillProcess();
	}
	else if (!_tcscmp(cmdTokenList[0], _T("sort")))
	{
		// sort 명령어 뒤에 '> 파일명'이 오는 경우 
		if (!_tcscmp(cmdTokenList[1], _T(">")))
		{
			// 리다이렉션의 대상 = 파일의 핸들은 상속 가능하도록
			// 파일이 자식 프로세스 리다이렉션 대상이 된다 == 핸들의 상속
			SECURITY_ATTRIBUTES fileSec = { sizeof(SECURITY_ATTRIBUTES),NULL,TRUE };

			HANDLE hFile = CreateFile(
				cmdTokenList[2], GENERIC_WRITE, FILE_SHARE_READ, &fileSec, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			si.hStdOutput = hFile;							// 출력 리다이렉션만 구현
			si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);	// 표준 입력을 리다이렉션하는 용도
			si.hStdError = GetStdHandle(STD_ERROR_HANDLE);	// 표준 에러
			si.dwFlags |= STARTF_USESTDHANDLES;				// 리다이렉션 정보를 반영하기 위해 

			SetCurrentDirectory(_T("C:\\Users\\82104\\source\\repos\\sort\\Debug"));

			isRun = CreateProcess(NULL, cmdTokenList[0], NULL, NULL,
				TRUE, NULL, NULL, NULL, &si, &pi);

			// 자식 프로세스가 종료될 때까지 대기
			WaitForSingleObject(pi.hProcess, INFINITE);

			// 리다이렉션 파일의 핸들 반환
			CloseHandle(hFile);
		}
		else
		{
			isRun = CreateProcess(NULL, cmdTokenList[0], NULL, NULL,
				TRUE, NULL, NULL, NULL, &si, &pi);

			WaitForSingleObject(pi.hProcess, INFINITE);
		}
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
	{
		_tcscpy(cmdStringWithOptions, cmdTokenList[0]);

		// Check Point1 : 문자열 처리를 위한 for문
		for (int i = 1; i < tokenNum; i++)
			_stprintf(cmdStringWithOptions, _T("%s %s"), cmdStringWithOptions, cmdTokenList[i]);


		// 직접 추가한 명령어가 아닐 시 표준 검색경로에 존재하는 실행파일을 실행
		isRun = CreateProcess(
			NULL, cmdStringWithOptions, NULL, NULL, 
			TRUE, NULL, NULL, NULL, &si, &pi);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if (isRun == 0)
		{
			// 에러번호를 알기 위한 출력문 
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

/*
	함수: void ListProcess()
	기능: 현재 실행중인 프로세스의 정보를 출력
*/
void ListProcess()
{
	// 프로세스 정보를 얻기 위한 구조체 변수
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// 현재 실행중인 프로세스의 스냅샷을 찍기
	HANDLE processSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (processSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("CreateToolhelp32Snapshot error\n"));
		return;
	}

	if (!Process32First(processSnap, &pe32))
	{
		_tprintf(_T("Process32First Error\n"));
		CloseHandle(processSnap);
		return;
	}

	do
	{
		_tprintf(_T("%25s %5d\n"), pe32.szExeFile, pe32.th32ProcessID);
	} while (Process32Next(processSnap, &pe32));

	CloseHandle(processSnap);
}

/*
	함수: void KillProcess()
	기능: 원하는 프로세스의 이름으로 프로세스를 종료
*/
void KillProcess()
{
	HANDLE processSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	if (processSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("CreateToolhelp32Snapshot (of process)"));
		return;
	}

	// 프로세스 정보를 얻기 위한 구조체 변수
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// 첫번째 프로세스 정보
	if (!Process32First(processSnap, &pe32))
	{
		_tprintf(_T("Process32First Error\n"));
		CloseHandle(processSnap);
		return;
	}

	HANDLE hProcess;	// 입력된 프로세스의 이름의 핸들을 담을 곳
	BOOL isKill = FALSE;
	do
	{
		// 입력된 프로세스의 이름과 비교해서 같다면
		if (_tcscmp(pe32.szExeFile, cmdTokenList[1]) == 0)
		{
			// 프로세스 ID를 핸들로 변환
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
			
			if (hProcess != NULL)
			{
				// 프로세스를 강제종료
				TerminateProcess(hProcess, -1);
				isKill = TRUE;
			}

			CloseHandle(hProcess);
			break;
		}
	} while (Process32Next(processSnap, &pe32));

	CloseHandle(processSnap);
	if (isKill == FALSE)
	{
		_tprintf(_T("Kill process fail.\n"));
	}
}