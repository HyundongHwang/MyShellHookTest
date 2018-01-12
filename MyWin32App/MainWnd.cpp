#include "stdafx.h"
#include "MainWnd.h"



MainWnd* MainWnd::s_pCurrent = NULL;

MainWnd::MainWnd()
{
    s_pCurrent = this;
}


MainWnd::~MainWnd()
{
}


LRESULT MainWnd::_On_WM_CREATE(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    USES_CONVERSION;

    auto x = 0;
    auto y = 0;
    auto w = 500;
    auto h = 30;

    m_arrLabelFuncSimple.Add(CSTRING_FUNC_SIMPLE_PAIR(L"test 00", MainWnd::_OnClickBtn_test_00));
    m_arrLabelFuncSimple.Add(CSTRING_FUNC_SIMPLE_PAIR(L"test 01", MainWnd::_OnClickBtn_test_01));
    m_arrLabelFuncSimple.Add(CSTRING_FUNC_SIMPLE_PAIR(L"test 02", MainWnd::_OnClickBtn_test_02));


    for (auto i = 0; i < m_arrLabelFuncSimple.GetCount(); i++)
    {
        auto pair = m_arrLabelFuncSimple[i];
        auto strBtnName = pair.strLabel;
        auto nBtnId = i;
        CWindow wndBtn;
        wndBtn.Create(L"Button", this->m_hWnd, CRect(x, y, x + w, y + h), strBtnName, WS_CHILD | WS_VISIBLE | BS_LEFT, NULL, nBtnId);
        y += h;
    }

    m_edLog.Create(L"Edit", this->m_hWnd, CRect(0, 0, 0, 0), L"log", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_WANTRETURN, NULL, 1001);
    return 0;
}

LRESULT MainWnd::_On_WM_DESTROY(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    ::PostQuitMessage(0);
    return 0;
}

LRESULT MainWnd::_On_WM_SIZE(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    CRect rcClient;
    ::GetClientRect(m_hWnd, &rcClient);
    m_edLog.MoveWindow(0, rcClient.Height() / 2, rcClient.Width(), rcClient.Height() / 2);
    return 0;
}

int MainWnd::_GetValidByteCountFromCharPtr(char* pSz)
{
    byte* pBuf = (byte*)pSz;
    int lengthStr = ::strlen(pSz);

    for (auto i = 0; i < lengthStr * 3; i++)
    {
        if (pBuf[i] == '\0')
            return i + 1;
    }

    return -1;
}

void MainWnd::_WriteLog(LPCSTR szLog)
{
    USES_CONVERSION;

    CString strEd;
    m_edLog.GetWindowText(strEd);
    strEd += L"\r\n";
    strEd += A2T(szLog);
    m_edLog.SetWindowText(strEd);
}

void MainWnd::_ReadFile(LPCWSTR wFileName, LPSTR szFileContent, int nFileContent)
{
    USES_CONVERSION;
    wchar_t wJsonFilePath[MAX_PATH] = { 0, };
    ::GetModuleFileName(NULL, wJsonFilePath, MAX_PATH);
    ::PathRemoveFileSpec(wJsonFilePath);
    ::PathAppend(wJsonFilePath, wFileName);
    CAtlFile file;
    file.Create(wJsonFilePath, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);
    file.Read(szFileContent, nFileContent);
    file.Close();
}

LRESULT MainWnd::_On_ID_BTN(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    auto nBtnId = wID;
    auto pair = m_arrLabelFuncSimple[nBtnId];
    pair.pFuncSimple(this);
    return 0;
}





//Creating a Child Process with Redirected Input and Output
//https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms682499(v=vs.85).aspx

HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;






#define BUFSIZE 4096

void MainWnd::_OnClickBtn_test_00(MainWnd* pThis)
{
    USES_CONVERSION;
    SECURITY_ATTRIBUTES saAttr;

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    BOOL bSuccess = FALSE;
    bSuccess = ::CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0);
    bSuccess = ::SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0);

    wchar_t szCmdline[] = L"C:\\Users\\hhd20\\AppData\\Local\\Android\\Sdk\\platform-tools\\adb.exe devices";
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;

    ::ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    ::ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = g_hChildStd_OUT_Wr;
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    bSuccess = ::CreateProcess(NULL,
        szCmdline,     // command line 
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        0,             // creation flags 
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION

    ::CloseHandle(piProcInfo.hProcess);
    ::CloseHandle(piProcInfo.hThread);

    DWORD dwRead, dwWritten;
    char chBuf[BUFSIZE];
    HANDLE hParentStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    CStringA strStdOut;

    for (;;)
    {
        ::ZeroMemory(chBuf, BUFSIZE);
        bSuccess = ::ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);

        if (!bSuccess || dwRead == 0) 
            break;

        chBuf[dwRead] = '\0';
        strStdOut += chBuf;
        bSuccess = ::WriteFile(hParentStdOut, chBuf, dwRead, &dwWritten, NULL);
        
        if (!bSuccess) 
            break;
    }

    pThis->_WriteLog(strStdOut);
}

void MainWnd::_OnClickBtn_test_01(MainWnd* pThis)
{
    ::MessageBox(NULL, L"test 01", L"MyWin32App", MB_OK);
}

void MainWnd::_OnClickBtn_test_02(MainWnd* pThis)
{
    ::MessageBox(NULL, L"test 02", L"MyWin32App", MB_OK);
}