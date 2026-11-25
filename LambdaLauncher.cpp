#ifdef UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
#pragma comment (lib, "comctl32")
#include <iostream>
#include <future>

#include <windows.h>
#include <CommCtrl.h>
#include "API.h"
#include <Uxtheme.h>
#include <thread>
#define CheckBox_SexAddon 1
#define Lambda_Launch 2
std::map<bool, std::string> Correlate;
#define Scroll_Bar 3
HINSTANCE hInstance;

HFONT hfont = CreateFont(-14, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI Variable ");
const UINT Width = 800;
const UINT Height = 600;

HWND g_Scroll = NULL;
HWND GameLaunchButton = NULL;
HWND Object = NULL;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    INITCOMMONCONTROLSEX icex = { sizeof(icex), ICC_STANDARD_CLASSES|ICC_WIN95_CLASSES};
    InitCommonControlsEx(&icex);
	const wchar_t CLASS_NAME[] = L"Lambda Launcher";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);
    HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"Lambda Launcher", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
    
  
    ShowWindow(hwnd, nCmdShow);

    SendMessage(GameLaunchButton, WM_SETFONT, (WPARAM)hfont, TRUE);
    MSG msg = {};
    API::API_FUNCTIONS::Get_Addons();
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE: {
        int i = 0;
        g_Scroll = CreateWindowEx(0, L"SCROLLBAR", L"Scrollbar", WS_VISIBLE | WS_CHILD | SBS_VERT, Width - 20, 0, 20, Height, hwnd, (HMENU)(UINT_PTR)Scroll_Bar, hInstance, NULL);

        GameLaunchButton = CreateWindowEx(0, L"BUTTON", L"Launch game", WS_CHILD | WS_VISIBLE, 100, 30, 100, 30, hwnd, (HMENU)(UINT_PTR)Lambda_Launch, hInstance, NULL);

        for (const auto& [key, value] : API::API_FUNCTIONS::Get_Addons()) {
            i++;
            HMENU Checkbox = (HMENU)(INT_PTR)(1000 + i);

            HWND Object = CreateWindowExA(0, "BUTTON", key.c_str(), WS_CHILD | BS_AUTOCHECKBOX | WS_VISIBLE, 100, (30 + 10) * (i + 1), 180, 30, hwnd, Checkbox, hInstance, NULL);
            SetPropA(Object, key.c_str(), HANDLE(NULL));
            SendMessage(Object, WM_SETFONT, (WPARAM)hfont, TRUE);
            if (value == 0) {
                SendMessage(Object, BM_SETCHECK, (WPARAM)BST_UNCHECKED, TRUE);
            }
            else {
                SendMessage(Object, BM_SETCHECK, (WPARAM)BST_CHECKED, TRUE);
            }

        }
        break;
    };
    case WM_COMMAND: {
        WORD control_id = LOWORD(wParam);
        HWND hW = HWND(lParam);
        if (control_id >= 1000) {
            char name[1000];
            GetWindowTextA(hW, name, 1000);
            LRESULT Checkboxy = SendMessageA(hW, BM_GETCHECK, 0, 0);
            switch (Checkboxy) {
            case BST_CHECKED: {
                API::API_FUNCTIONS::Set_Addons(name, 1);
                break;

            };
            case BST_UNCHECKED: {
                API::API_FUNCTIONS::Set_Addons(name, 0);
                break;

            };
            };
        }
        else if (control_id == 2)
        {
            API::API_FUNCTIONS::Save_Addon_Change();
            auto feature = std::async(std::launch::async, []() {
                ShellExecuteA(NULL, "open", R"(steam://rungameid/270370)", "-game lambdawars", NULL, SW_SHOWNORMAL);
                std::this_thread::sleep_for(std::chrono::seconds(2));
                }); //UX TIMEOUT
            PostQuitMessage(0);
            return 0;
        }
        break;
        
    };

    case WM_DESTROY: {
        PostQuitMessage(0);
            return 0;
        };
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);


        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));



        EndPaint(hwnd, &ps);
        return 0;
    };

    case WM_CTLCOLORSTATIC: {
        HDC hdc = (HDC)wParam;
        SetBkMode(hdc, TRANSPARENT);
        return (LRESULT)GetStockObject(COLOR_WINDOW + 1);
    };

    case WM_SIZE: {
        UINT Width = LOWORD(lParam);
        UINT Height = HIWORD(lParam);
        MoveWindow(g_Scroll, Width - 20, 0, 20, Height, SWP_NOACTIVATE | SWP_NOZORDER);
        MoveWindow(GameLaunchButton, Width  - 200, Height - 80, 100, 40, SWP_NOACTIVATE | SWP_NOZORDER);
        MoveWindow(Object, Width / 20, Height / 20, 100, 40, SWP_NOACTIVATE | SWP_NOZORDER);
    };
     return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
                           
}