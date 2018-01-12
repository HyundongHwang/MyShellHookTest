#pragma once
#include "MyWin32App.h"


class MainWnd;
typedef void(*LP_FUNC_SIMPLE)(MainWnd* pThis);

struct CSTRING_FUNC_SIMPLE_PAIR
{
    CSTRING_FUNC_SIMPLE_PAIR(CString strLabelNew, LP_FUNC_SIMPLE pFuncSimpleNew)
    {
        strLabel = strLabelNew;
        pFuncSimple = pFuncSimpleNew;
    }

    CString strLabel;
    LP_FUNC_SIMPLE pFuncSimple;
};

class MainWnd
    : public CWindowImpl<MainWnd>
{
public:
    const int ID_EDIT_LOG = 2000;

    MainWnd();
    virtual ~MainWnd();

    DECLARE_WND_CLASS(L"MainWnd")

    BEGIN_MSG_MAP(MainWnd)
        MESSAGE_HANDLER(WM_CREATE, _On_WM_CREATE)
        MESSAGE_HANDLER(WM_DESTROY, _On_WM_DESTROY)
        MESSAGE_HANDLER(WM_SIZE, _On_WM_SIZE)
        COMMAND_CODE_HANDLER(BN_CLICKED, _On_ID_BTN)
    END_MSG_MAP()

    static MainWnd* s_pCurrent;

private:
    static int _Coconut_OnPush(LPCSTR szRequest, LPSTR szResponse, int nResponseLength);
    static int _GetValidByteCountFromCharPtr(char* pSz);

    void _WriteLog(LPCSTR szLog);
    void _ReadFile(LPCWSTR wFileName, LPSTR szFileContent, int nFileContent);

    LRESULT _On_WM_CREATE(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT _On_WM_DESTROY(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT _On_WM_SIZE(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT _On_ID_BTN(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

    static void _OnClickBtn_test_00(MainWnd* pThis);
    static void _OnClickBtn_test_01(MainWnd* pThis);
    static void _OnClickBtn_test_02(MainWnd* pThis);

    CWindow m_edLog;

    CAtlArray<CSTRING_FUNC_SIMPLE_PAIR> m_arrLabelFuncSimple;
};