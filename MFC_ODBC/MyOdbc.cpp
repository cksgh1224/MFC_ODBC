#include "pch.h"
#include "MyOdbc.h"


MyOdbc::MyOdbc()
{
	InitObject();
}

MyOdbc::~MyOdbc()
{
	Disconnect();
}


// 멤버변수 초기화
void MyOdbc::InitObject()
{
	mh_environment = NULL;
	mh_odbc = NULL;
	mp_owner = NULL;
	m_connect_flag = 0;
}


// 서버 연결 (성공: 1반환, 실패: 0반환)
int MyOdbc::Connect(const wchar_t* ap_dsn, const wchar_t* ap_id, const wchar_t* ap_pw, void* ap_owner)
{
	mp_owner = ap_owner;
	int result = 1;

	// ODBC 드라이버에 연결을 위한 기본 정보 설정
	SQLSetEnvAttr(NULL, SQL_ATTR_CONNECTION_POOLING, (SQLPOINTER)SQL_CP_ONE_PER_DRIVER, SQL_IS_INTEGER);

	// ODBC 기술을 사용하기 위한 환경 구성
	// 구성된 환경 정보에 대한 핸들 값은 mh_environment에 저장
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &mh_environment) != SQL_ERROR)
	{
		// 사용할 ODBC 버전 정보 설정
		SQLSetEnvAttr(mh_environment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
		SQLSetEnvAttr(mh_environment, SQL_ATTR_CP_MATCH, (SQLPOINTER)SQL_CP_RELAXED_MATCH, SQL_IS_INTEGER);


		// ODBC 함수를 사용하기 위한 정보 구성. 이 정보에 대한 핸들 값은 mh_odbc에 저장
		if (SQLAllocHandle(SQL_HANDLE_DBC, mh_environment, &mh_odbc) != SQL_ERROR)
		{
			RETCODE ret_code = SQLConnect(mh_odbc,
				// 접속할 DSN 설정
				(SQLWCHAR*)ap_dsn, SQL_NTS,
				// 접속에 사용할 ID
				(SQLWCHAR*)ap_id, SQL_NTS,
				// 접속에 사용할 Password
				(SQLWCHAR*)ap_pw, SQL_NTS);


			if (ret_code == SQL_SUCCESS || ret_code == SQL_SUCCESS_WITH_INFO)
			{
				// ODBC를 사용하여 데이터베이스 서버에 성공적으로 접속한 경우
				m_connect_flag = 1;
			}
			else
			{
				// 접속에 실패한 경우, 구성했던 메모리를 제거
				if (mh_odbc != SQL_NULL_HDBC) SQLFreeHandle(SQL_HANDLE_DBC, mh_odbc);
				if (mh_environment != SQL_NULL_HENV) SQLFreeHandle(SQL_HANDLE_ENV, mh_environment);
				result = -1; // 연결 실패
			}
		}
		else result = -2; // 연결 정보 구성 실패
	}
	else result = -3; // 시스템에 설치된 ODBC 사용 불가

	return result;
}


// 서버 연결 해제
void MyOdbc::Disconnect()
{
	if (m_connect_flag == 1) // 서버에 연결되어 있는 경우 ODBC와 관련되어 구성했던 정보 제거
	{
		if (mh_odbc != SQL_NULL_HDBC) SQLFreeHandle(SQL_HANDLE_DBC, mh_odbc);
		if (mh_environment != SQL_NULL_HENV) SQLFreeHandle(SQL_HANDLE_ENV, mh_environment);
	}
	InitObject();
}


// 명령문 실행 (insert, update, delete) (결과값을 특별하게 처리하지 않는다) (성공: 1반환)
int MyOdbc::ExecQuery(const wchar_t* ap_query)
{
	SQLHSTMT h_statement;
	int result = 0;

	// Query 문을 위한 메모리 할당
	// SQL_INVALID_HANDLE : 잘못 된 환경, 연결, 문 또는 설명자 핸들로 인해 함수가 실패
	if (SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement) == SQL_SUCCESS)
	{
		// Query 문을 실행할 때 타임 아웃 설정
		SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);

		// SQL 명령문 실행
		RETCODE ret = SQLExecDirect(h_statement, (SQLWCHAR*)(const wchar_t*)ap_query, SQL_NTS);

		// 성공적으로 완료되었는지 체크
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		{
			MessageBox(NULL, L"명령문 실행 성공!", NULL, MB_OK);
			result = 1;
		}

		// Commit. 명령 수행이 완료되었다는 것을 설정
		SQLEndTran(SQL_HANDLE_ENV, mh_environment, SQL_COMMIT);

		// Query 문을 위해 할당할 메모리 해제
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}
	else
	{
		MessageBox(NULL, L"Query 문을 위한 메모리 할당 실패!", NULL, MB_OK);
	}

	return result;
}


// 명령문 실행 (select) (검색된 데이터 처리) (성공: 1반환) 
// a_record_size : 데이터 크기, a_record_count_per_step : 데이터를 가져오는 최대 단위
int MyOdbc::ExecQuery(const wchar_t* ap_query, int a_record_size, SET_RECORD_INFO set_record_info, SQL_RESULT_RECORD sql_result_record, int a_record_count_per_step)
{
	int result = 0, index = 0; // result: 성공/실패 반환값, index: 읽은 총 데이터 개수	

	unsigned short* p_state = new unsigned short[a_record_count_per_step]; // 읽어온 데이터의 상태를 기록할 변수

	char* p_data = new char[a_record_size * a_record_count_per_step];      // 읽어온 데이터를 저장할 변수
	memset(p_data, 0, a_record_size * a_record_count_per_step);            // 데이터를 저장할 배열 초기화

	SQLHSTMT h_statement;
	RETCODE ret;


	// Query 문을 위한 메모리 할당
	if (SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement) == SQL_SUCCESS)
	{
		// 읽은 데이터의 개수를 저장할 변수
		ULONG record_num = 0;
		
		// Qoery 문을 실행할 때 타임 아웃 설정
		SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);

		// 가져온 데이터를 저장할 메모리 크기 설정
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)a_record_size, 0);

		// 데이터를 가져올때 동시성에 대한 방식 설정
		SQLSetStmtAttr(h_statement, SQL_ATTR_CONCURRENCY, (SQLPOINTER)SQL_CONCUR_ROWVER, SQL_IS_UINTEGER);

		// 검색된 데이터의 위치를 기억하는 방식 설정
		SQLSetStmtAttr(h_statement, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_KEYSET_DRIVEN, SQL_IS_UINTEGER);

		// 데이터를 가져오는 최대 단위 설정
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_NUMBER, (SQLPOINTER)a_record_count_per_step, SQL_IS_UINTEGER);

		// 읽은 데이터의 상태를 저장할 변수의 주소를 전달
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_STATUS_PTR, p_state, 0);

		// 읽은 데이터의 개수를 저장할 변수의 주소를 전달
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROWS_FETCHED_PTR, &record_num, 0);


		// 테이블에서 가져온 데이터를 속성별로 User 객체에 저장하기 위해 속성별로 저장할 메모리 위치를 설정
		(*set_record_info)(mp_owner, h_statement, p_data);
		

		// SQL 명령문 실행
		RETCODE ret = SQLExecDirect(h_statement, (SQLWCHAR*)(const wchar_t*)ap_query, SQL_NTS);
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		{
			// 결과 집합에서 지정 된 행 집합을 반환, 바인딩된 열이 있으면 해당 열에 있는 데이터를 반환
			while (SQLFetchScroll(h_statement, SQL_FETCH_NEXT, 0) != SQL_NO_DATA)
			{
				// SQLFetchScroll이 성공하면 record_num은 1이된다 (마지막에 더이상 데이터가 검색되지 않으면 0) (1보다 큰 숫자 안나옴, 전체 데이터 갯수 확인 불가..?)
				(*sql_result_record)(mp_owner, index, p_data, record_num, p_state);
				index++;
			}
			result = 1; // 명령문 실행 성공
		}
		else MessageBox(NULL, L"명령문 실행 실패", NULL, MB_OK);

		// Query 문을 위해 할당할 메모리 해제
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}


	delete[] p_data;
	delete[] p_state;
	return result;
}