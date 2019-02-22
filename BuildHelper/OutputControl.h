#pragma once

#define DEF_OUT_RETURN_FALSE(MSG)	{ OutputControl::Instance()->PrintOut(MSG); return false; }
#define DEF_OUT(MSG)				OutputControl::Instance()->PrintOut(MSG);

class OutputControl
{
public:
	static OutputControl* Instance();

	enum EN_TYPE
	{
		EN_TYPE_NONE,
		EN_TYPE_CONSOLE,
		EN_TYPE_MSGBOX,
	};

	// default 값이 지정된 bYes를 넘길것.
	// 사용자로부터 Yes,No를 입력받아서 bYes를 채워줌.
	void ScanIn(const TCHAR* pPrompt, bool& bYes);

	// 프롬프트 출력
	void PrintOut(const TCHAR* pStr);

	// CONSOLE : 콘솔창에 프린트 됨.
	// MSGBOX : 메시지 박스가 뜬다.
	void SetType(OutputControl::EN_TYPE enType);

protected:
	OutputControl();

	EN_TYPE m_enType;
};
