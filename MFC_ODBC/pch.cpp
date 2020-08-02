// pch.cpp: 미리 컴파일된 헤더에 해당하는 소스 파일

#include "pch.h"

// 미리 컴파일된 헤더를 사용하는 경우 컴파일이 성공하려면 이 소스 파일이 필요합니다.




// Query의 결과에 해당하는 데이터를 User 객체와 연결하기 위한 코드 구성
void SetRecordInfo(void* ap_owner, HSTMT ah_statement, void* ap_data)
{
	User* p = (User*)ap_data;

	// 테이블에서 가져온 데이터를 속성별로 raw_data 변수에 저장하기 위해 속성별로 저장할 메모리 위치를 설정 (데이터 버퍼를 결과 집합의 열에 바인딩)
	SQLBindCol(ah_statement, 1, SQL_WCHAR, p->GetID(), sizeof(wchar_t) * 20, NULL);
	SQLBindCol(ah_statement, 2, SQL_WCHAR, p->GetPW(), sizeof(wchar_t) * 20, NULL);
	SQLBindCol(ah_statement, 3, SQL_WCHAR, p->GetName(), sizeof(wchar_t) * 20, NULL);
}


// SQL 명령문에 의해서 가져온 데이터들을 자신이 원하는 형태로 변환하거나 저장하는 작업
// ap_owner: 윈도우 핸들, ap_data: 읽은 데이터, a_step_index: 0부터 시작해서 데이터를 하나 읽을때마다 1씩 증가
// a_count: 읽은 데이터 개수, ap_state: 읽어온 데이터의 상태, option: 검색 조건
int ResultRecord(void* ap_owner, int a_step_index, void* ap_data, ULONG a_count, unsigned short* ap_state)
{
	int result = 0; // 성공: 1반환
	

	CString str;
	User* p = (User*)ap_data;

	for (ULONG i = 0; i < a_count; i++)
	{
		if (ap_state[i] != SQL_ROW_DELETED && ap_state[i] != SQL_ROW_ERROR)
		{
			str.Format(L"ID: %s\nPW:%s\nName:%s", p->GetID(), p->GetPW(), p->GetName());
			MessageBox(NULL, str, L"사용자", MB_OK);
			result = 1;
		}
		p++;
		ap_state++;
	}


	int option = -1;
	// 전체 사용자 검색
	if (option == 0)
	{
		for (ULONG i = 0; i < a_count; i++)
		{
			if (ap_state[i] != SQL_ROW_DELETED && ap_state[i] != SQL_ROW_ERROR)
			{
				str.Format(L"ID: %s\nPW:%s\nName:%s", p->GetID(), p->GetPW(), p->GetName());
				MessageBox(NULL, str, NULL, MB_OK);
				result = 1;
			}
			p++;
			ap_state++;
		}
	}

	// 로그인 체크
	else if (option == 1)
	{
		if (ap_state[0] != SQL_ROW_DELETED && ap_state[0] != SQL_ROW_ERROR)
		{
			MessageBox(NULL, L"로그인 성공!", NULL, MB_OK);
			result = 1;
		}
		else
		{
			MessageBox(NULL, L"로그인 실패!", NULL, MB_OK);
		}
	}

	// 아이디 찾기
	else if (option == 2)
	{
		if (ap_state[0] != SQL_ROW_DELETED && ap_state[0] != SQL_ROW_ERROR)
		{
			if (ap_state[0] != SQL_ROW_DELETED && ap_state[0] != SQL_ROW_ERROR)
			{
				str.Format(L"ID: %s", p->GetID());
				MessageBox(NULL, str, NULL, MB_OK);
				result = 1;
			}
			else
			{
				MessageBox(NULL, L"입력하신 정보가 일치하지 않습니다!", NULL, MB_OK);
			}
		}
	}

	// 비밀번호 찾기
	else if (option == 3)
	{
		if (ap_state[0] != SQL_ROW_DELETED && ap_state[0] != SQL_ROW_ERROR)
		{
			if (ap_state[0] != SQL_ROW_DELETED && ap_state[0] != SQL_ROW_ERROR)
			{
				str.Format(L"PW: %s", p->GetPW());
				MessageBox(NULL, str, NULL, MB_OK);
				result = 1;
			}
			else
			{
				MessageBox(NULL, L"입력하신 정보가 일치하지 않습니다!", NULL, MB_OK);
			}
		}
	}

	return result;
}
