/*
* 24.1.7
* CommandPrmpt_One.cpp
* 프로그램 설명: 명령 프롬포트의 골격
* 주목할 사항
* 1. 명령어를 추가할 때 변경되는 부분
* 2. 명령어의 대소문자를 구분하지 않기 위해서 고려된 부분
* 3. 명령어 EXIT가 입력되었을 때 프로그램 종료방식
*/

/*
* 24.1.8
* CommandPrompt_Two.cpp
* 수정: 자식 프로세스 생성 코드 추가 + 추가 기능 수행
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>	//한글 입력을 위한 헤더파일
#include <Windows.h>

#define STR_LEN	256
#define CMD_TOKEN_NUM	10

//전역변수 문자열 선언
TCHAR ERROR_CMD[] = _T("'%s'은(는) 실행할 수 있는 프로그램이 아닙니다. \n");

//함수 선언
int CmdProcessing(void);
TCHAR* StrLower(TCHAR*);


int _tmain(int argc, TCHAR* argv[])
{

	//유니코드 기반
	//한글 입력을 가능하게 하기 위해
	_tsetlocale(LC_ALL, _T("Korean"));

	DWORD isExit;
	while (1)
	{
		isExit = CmdProcessing();
		if (isExit == TRUE)
		{
			_fputts(_T("명령어 처리를 종료합니다. \n"), stdout);
			break;
		}
	}
	return 0;
}

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(",\t\n");	//?? 토큰 분리를 위한 구분자를 뜻함

/*
* 함수: TCHAR int CmdProcessing(void)
* 기능: 명령어를 입력 받아서 해당 명령어에 지정되어 있는 기능을 수행
* exit가 입력되면 TRUE를 반환하고, 프로그램의 종료로 이어진다.
* 반환값: 명령어
*/
int CmdProcessing(void)
{
	_fputts(_T("Best command prompt>> "), stdout);
	_getts_s(cmdString);	//_getts()함수 대신 사용

	/*
		프로세스 생성을 위한 두 개의 구조체 초기화
	*/
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi = { 0, };
	//구조체 변수의 크기 설정, 미래에 호환성을 위한 것
	si.cb = sizeof(si);


	/*
	* char* strtok(char* s1, char* s2)
	* 1. s1문자열에서 s2구분자를 통해 토큰을 추출
	* 2. 토큰: 특정 구분자로 분리되는 최소 문장 요소
	* 3. 반환값으로 문자열의 포인터 or NULL
	* 4. 하나의 토큰만을 추출
	*/
	TCHAR* token = _tcstok(cmdString, seps);
	int tokenNum = 0;

	//토큰으로 분리하는 과정
	while (token != NULL)
	{
	
		// token에 NULL을 넣어서 무한루프를 방지?????????
		// StrLower함수를 통해 대소문자의 구분을 없앰
		_tcscpy(cmdTokenList[tokenNum++], StrLower(token));
		token = _tcstok(NULL, seps);
	}

	if (!_tcscmp(cmdTokenList[0], _T("exit")))
	{
		return TRUE;	//프로그램 종료를 위한 반환값
	}
	else if (!_tcscmp(cmdTokenList[0], _T("추가 되는 명령어 1")))
	{
		//내가 직접 추가할 명령어의 코드 영역
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