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


// ������� �ʱ�ȭ
void MyOdbc::InitObject()
{
	mh_environment = NULL;
	mh_odbc = NULL;
	mp_owner = NULL;
	m_connect_flag = 0;
}


// ���� ���� (����: 1��ȯ, ����: 0��ȯ)
int MyOdbc::Connect(const wchar_t* ap_dsn, const wchar_t* ap_id, const wchar_t* ap_pw, void* ap_owner)
{
	mp_owner = ap_owner;
	int result = 1;

	// ODBC ����̹��� ������ ���� �⺻ ���� ����
	SQLSetEnvAttr(NULL, SQL_ATTR_CONNECTION_POOLING, (SQLPOINTER)SQL_CP_ONE_PER_DRIVER, SQL_IS_INTEGER);

	// ODBC ����� ����ϱ� ���� ȯ�� ����
	// ������ ȯ�� ������ ���� �ڵ� ���� mh_environment�� ����
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &mh_environment) != SQL_ERROR)
	{
		// ����� ODBC ���� ���� ����
		SQLSetEnvAttr(mh_environment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
		SQLSetEnvAttr(mh_environment, SQL_ATTR_CP_MATCH, (SQLPOINTER)SQL_CP_RELAXED_MATCH, SQL_IS_INTEGER);


		// ODBC �Լ��� ����ϱ� ���� ���� ����. �� ������ ���� �ڵ� ���� mh_odbc�� ����
		if (SQLAllocHandle(SQL_HANDLE_DBC, mh_environment, &mh_odbc) != SQL_ERROR)
		{
			RETCODE ret_code = SQLConnect(mh_odbc,
				// ������ DSN ����
				(SQLWCHAR*)ap_dsn, SQL_NTS,
				// ���ӿ� ����� ID
				(SQLWCHAR*)ap_id, SQL_NTS,
				// ���ӿ� ����� Password
				(SQLWCHAR*)ap_pw, SQL_NTS);


			if (ret_code == SQL_SUCCESS || ret_code == SQL_SUCCESS_WITH_INFO)
			{
				// ODBC�� ����Ͽ� �����ͺ��̽� ������ ���������� ������ ���
				m_connect_flag = 1;
			}
			else
			{
				// ���ӿ� ������ ���, �����ߴ� �޸𸮸� ����
				if (mh_odbc != SQL_NULL_HDBC) SQLFreeHandle(SQL_HANDLE_DBC, mh_odbc);
				if (mh_environment != SQL_NULL_HENV) SQLFreeHandle(SQL_HANDLE_ENV, mh_environment);
				result = -1; // ���� ����
			}
		}
		else result = -2; // ���� ���� ���� ����
	}
	else result = -3; // �ý��ۿ� ��ġ�� ODBC ��� �Ұ�

	return result;
}


// ���� ���� ����
void MyOdbc::Disconnect()
{
	if (m_connect_flag == 1) // ������ ����Ǿ� �ִ� ��� ODBC�� ���õǾ� �����ߴ� ���� ����
	{
		if (mh_odbc != SQL_NULL_HDBC) SQLFreeHandle(SQL_HANDLE_DBC, mh_odbc);
		if (mh_environment != SQL_NULL_HENV) SQLFreeHandle(SQL_HANDLE_ENV, mh_environment);
	}
	InitObject();
}


// ��ɹ� ���� (insert, update, delete) (������� Ư���ϰ� ó������ �ʴ´�) (����: 1��ȯ)
int MyOdbc::ExecQuery(const wchar_t* ap_query)
{
	SQLHSTMT h_statement;
	int result = 0;

	// Query ���� ���� �޸� �Ҵ�
	// SQL_INVALID_HANDLE : �߸� �� ȯ��, ����, �� �Ǵ� ������ �ڵ�� ���� �Լ��� ����
	if (SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement) == SQL_SUCCESS)
	{
		// Query ���� ������ �� Ÿ�� �ƿ� ����
		SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);

		// SQL ��ɹ� ����
		RETCODE ret = SQLExecDirect(h_statement, (SQLWCHAR*)(const wchar_t*)ap_query, SQL_NTS);

		// ���������� �Ϸ�Ǿ����� üũ
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		{
			MessageBox(NULL, L"��ɹ� ���� ����!", NULL, MB_OK);
			result = 1;
		}

		// Commit. ��� ������ �Ϸ�Ǿ��ٴ� ���� ����
		SQLEndTran(SQL_HANDLE_ENV, mh_environment, SQL_COMMIT);

		// Query ���� ���� �Ҵ��� �޸� ����
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}
	else
	{
		MessageBox(NULL, L"Query ���� ���� �޸� �Ҵ� ����!", NULL, MB_OK);
	}

	return result;
}


// ��ɹ� ���� (select) (�˻��� ������ ó��) (����: 1��ȯ) 
// a_record_size : ������ ũ��, a_record_count_per_step : �����͸� �������� �ִ� ����
int MyOdbc::ExecQuery(const wchar_t* ap_query, int a_record_size, SET_RECORD_INFO set_record_info, SQL_RESULT_RECORD sql_result_record, int a_record_count_per_step)
{
	int result = 0, index = 0; // result: ����/���� ��ȯ��, index: ���� �� ������ ����	

	unsigned short* p_state = new unsigned short[a_record_count_per_step]; // �о�� �������� ���¸� ����� ����

	char* p_data = new char[a_record_size * a_record_count_per_step];      // �о�� �����͸� ������ ����
	memset(p_data, 0, a_record_size * a_record_count_per_step);            // �����͸� ������ �迭 �ʱ�ȭ

	SQLHSTMT h_statement;
	RETCODE ret;


	// Query ���� ���� �޸� �Ҵ�
	if (SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement) == SQL_SUCCESS)
	{
		// ���� �������� ������ ������ ����
		ULONG record_num = 0;
		
		// Qoery ���� ������ �� Ÿ�� �ƿ� ����
		SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);

		// ������ �����͸� ������ �޸� ũ�� ����
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)a_record_size, 0);

		// �����͸� �����ö� ���ü��� ���� ��� ����
		SQLSetStmtAttr(h_statement, SQL_ATTR_CONCURRENCY, (SQLPOINTER)SQL_CONCUR_ROWVER, SQL_IS_UINTEGER);

		// �˻��� �������� ��ġ�� ����ϴ� ��� ����
		SQLSetStmtAttr(h_statement, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_KEYSET_DRIVEN, SQL_IS_UINTEGER);

		// �����͸� �������� �ִ� ���� ����
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_NUMBER, (SQLPOINTER)a_record_count_per_step, SQL_IS_UINTEGER);

		// ���� �������� ���¸� ������ ������ �ּҸ� ����
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_STATUS_PTR, p_state, 0);

		// ���� �������� ������ ������ ������ �ּҸ� ����
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROWS_FETCHED_PTR, &record_num, 0);


		// ���̺��� ������ �����͸� �Ӽ����� User ��ü�� �����ϱ� ���� �Ӽ����� ������ �޸� ��ġ�� ����
		(*set_record_info)(mp_owner, h_statement, p_data);
		

		// SQL ��ɹ� ����
		RETCODE ret = SQLExecDirect(h_statement, (SQLWCHAR*)(const wchar_t*)ap_query, SQL_NTS);
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		{
			// ��� ���տ��� ���� �� �� ������ ��ȯ, ���ε��� ���� ������ �ش� ���� �ִ� �����͸� ��ȯ
			while (SQLFetchScroll(h_statement, SQL_FETCH_NEXT, 0) != SQL_NO_DATA)
			{
				// SQLFetchScroll�� �����ϸ� record_num�� 1�̵ȴ� (�������� ���̻� �����Ͱ� �˻����� ������ 0) (1���� ū ���� �ȳ���, ��ü ������ ���� Ȯ�� �Ұ�..?)
				(*sql_result_record)(mp_owner, index, p_data, record_num, p_state);
				index++;
			}
			result = 1; // ��ɹ� ���� ����
		}
		else MessageBox(NULL, L"��ɹ� ���� ����", NULL, MB_OK);

		// Query ���� ���� �Ҵ��� �޸� ����
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}


	delete[] p_data;
	delete[] p_state;
	return result;
}