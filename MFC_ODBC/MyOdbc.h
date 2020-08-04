#pragma once

#ifndef _MYODBC_H_
#define _MYODBC_H_


// ODBC API를 사용하기 위한 헤더 파일 추가
#include <odbcinst.h>
#include <sqlext.h>

// ODBC API를 사용하기 위한 라이브러리 추가
#pragma comment(lib, "odbc32.lib")


// 라이브러리
//#ifdef _DEBUG
//	#pragma comment(lib, "DSH_TWAPI_ODBC.lib")
//#else
//	#pragma comment(lib, "RST_TWAPI_ODBC.lib")
//#endif



// typedef 함수 포인터 : 반환형(*함수이름)(매개변수)
// SET_RECORD_INFO    : Query의 결과에 해당하는 데이터를 담을 메모리(사용자 정의 객체)와 연결하기 위한 코드 구성
// SQL_RESULT_RECORD  : SQL 명령문에 의해서 가져온 데이터들을 자신이 원하는 형태로 변환하거나 저장,처리하는 작업
// 실제로 호출되는 함수는 실제로 작업을 하는(MyOdbc를 사용하는) cpp 파일에 선언해서 사용
typedef void(*SET_RECORD_INFO)(void* ap_owner, HSTMT ah_statement, void* ap_data);
typedef int(*SQL_RESULT_RECORD)(void* ap_owner, int a_step_index, void* ap_data, ULONG a_count, unsigned short* ap_state, int option);



class MyOdbc
{
private:
	SQLHANDLE mh_environment;  // ODBC 기술을 사용하기 위한 환경 정보
	SQLHDBC mh_odbc;           // ODBC 함수를 사용하기 위한 정보
	int m_connect_flag;        // 서버와 연결 여부를 저장할 변수 (1:연결)
	void* mp_owner;            // 윈도우 핸들을 저장할 변수

public:
	MyOdbc();
	virtual ~MyOdbc();

	inline char IsConnected() { return m_connect_flag; }
	inline SQLHANDLE GetEnvHandle() { return mh_environment; }
	inline SQLHDBC GetOdbcHandle() { return mh_odbc; }


	// 멤버변수 초기화
	void InitObject();

	// 서버 연결 (성공: 1반환)
	int Connect(const wchar_t* ap_dsn, const wchar_t* ap_id, const wchar_t* ap_pw, void* ap_owner = 0);

	// 서버 연결 해제
	void Disconnect();

	// 명령문 실행 (insert, update, delete) (결과값을 특별하게 처리하지 않는다) (성공: 1반환)
	int ExecQuery(const wchar_t* ap_query);

	// 명령문 실행 (select) (검색된 데이터 처리) (성공: 1반환)
	// a_record_size : sizeof(데이터), a_record_count_per_step : 데이터를 가져오는 최대 단위
	// set_record_info, sql_result_record : typedef 함수 포인터
	// option: 0-전체 데이터 검색, 1-로그인, 2-아이디 찾기, 3-비밀번호 찾기, 4-아이디 중복확인
	int ExecQuery(const wchar_t* ap_query, int a_record_size, SET_RECORD_INFO set_record_info, SQL_RESULT_RECORD sql_result_record, 
		int option, int a_record_count_per_step = 100);
	
};



#endif