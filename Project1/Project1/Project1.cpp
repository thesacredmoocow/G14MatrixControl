// Project1.cpp : Defines the entry point for the application.
//



#pragma once
#include "pch.h"
#include "framework.h"
#include "Project1.h"
#define MAX_LOADSTRING 100

TCHAR greeting[] = _T("Hello, Windows desktop!");

MatrixController mc;
std::vector<std::vector<BYTE>> inputMatrix;

BOOL playSnake(MatrixController& mc, std::vector<std::vector<BYTE>>& inputMatrix);

NOTIFYICONDATA nidApp;
HMENU hPopMenu;
TCHAR szApplicationToolTip[MAX_LOADSTRING];	    // the main window class name
BOOL bDisable = FALSE;							// keep application state
BOOL bShowWindow = FALSE;
BOOL lightsOn = false;
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    DEVICE_DATA           deviceData;
    HRESULT               hr;
    USB_DEVICE_DESCRIPTOR deviceDesc;
    BOOL                  bResult;
    BOOL                  noDevice;
    ULONG                 lengthReceived;

    hr = OpenDevice(&deviceData, &noDevice);
    if (FAILED(hr)) {

        if (noDevice) {

            wprintf(L"Device not connected or driver not installed\n");

        }
        else {

            wprintf(L"Failed looking for device, HRESULT 0x%x\n", hr);
        }
        system("pause");
        return 0;
    }

    bResult = WinUsb_GetDescriptor(deviceData.WinusbHandle,
        USB_DEVICE_DESCRIPTOR_TYPE,
        0,
        0,
        (PBYTE)&deviceDesc,
        sizeof(deviceDesc),
        &lengthReceived);

    if (FALSE == bResult || lengthReceived != sizeof(deviceDesc)) {

        wprintf(L"Error among LastError %d or lengthReceived %d\n",
            FALSE == bResult ? GetLastError() : 0,
            lengthReceived);
        CloseDevice(&deviceData);
        return 0;
    }

    mc.setHandle(deviceData.WinusbHandle);
    for (int i = 0; i < MATRIXHEIGHT; i++)
    {
        std::vector<BYTE> temp;
        for (int j = 0; j < arrSize[i]; j++)
        {
            temp.push_back(0xff);
        }
        inputMatrix.push_back(temp);
    }
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   HICON hMainIcon;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_HIDE);
   UpdateWindow(hWnd);
   hMainIcon = LoadIcon(hInstance, (LPCTSTR)MAKEINTRESOURCE(IDI_SYSTRAYDEMO));

   nidApp.cbSize = sizeof(NOTIFYICONDATA); // sizeof the struct in bytes 
   nidApp.hWnd = (HWND)hWnd;              //handle of the window which will process this app. messages 
   nidApp.uID = IDI_SYSTRAYDEMO;           //ID of the icon that willl appear in the system tray 
   nidApp.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; //ORing of all the flags 
   nidApp.hIcon = hMainIcon; // handle of the Icon to be displayed, obtained from LoadIcon 
   nidApp.uCallbackMessage = WM_USER_SHELLICON;
   LoadString(hInstance, IDS_APPTOOLTIP, nidApp.szTip, MAX_LOADSTRING);
   Shell_NotifyIcon(NIM_ADD, &nidApp);
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    POINT lpClickPoint;
    switch (message)
    {
    case WM_USER_SHELLICON:
        // systray msg callback 
        switch (LOWORD(lParam))
        {
        case WM_RBUTTONDOWN:
            UINT uFlag = MF_BYPOSITION | MF_STRING;
            GetCursorPos(&lpClickPoint);
            hPopMenu = CreatePopupMenu();
            InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_ABOUT, _T("About"));
            if (bDisable == TRUE)
            {
                uFlag |= MF_GRAYED;
            }
            InsertMenu(hPopMenu, 0xFFFFFFFF, uFlag, IDM_TEST2, _T("Test 2")); // Test 2
            InsertMenu(hPopMenu, 0xFFFFFFFF, uFlag, IDM_TEST1, _T("Test 1")); // Test 1				
            InsertMenu(hPopMenu, 0xFFFFFFFF, MF_SEPARATOR, IDM_SEP, _T("SEP"));
            if (bDisable == TRUE)
            {
                InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_ENABLE, _T("Enable"));
            }
            else
            {
                InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_DISABLE, _T("Disable"));
            }
            if (bShowWindow)
            {
                InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_SHOWWINDOW, _T("Show Window"));
            }
            else
            {
                InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_HIDEWINDOW, _T("Hide Window"));
            }
            if (lightsOn)
            {
                InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_MATRIX_P1, _T("Disable Lighting"));
            }
            else
            {
                InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_MATRIX_P2, _T("Enable Lighting"));
            }
            InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_MATRIX_SNAKE, _T("Play Snake"));
            InsertMenu(hPopMenu, 0xFFFFFFFF, MF_SEPARATOR, IDM_SEP, _T("SEP"));
            InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_EXIT, _T("Exit"));

            SetForegroundWindow(hWnd);
            TrackPopupMenu(hPopMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_BOTTOMALIGN, lpClickPoint.x, lpClickPoint.y, 0, hWnd, NULL);
            return TRUE;

        }
        break;
    case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_TEST1:
            MessageBox(NULL, _T("This is a test for menu Test 1"), _T("Test 1"), MB_OK);
            break;
        case IDM_TEST2:
            MessageBox(NULL, _T("This is a test for menu Test 2"), _T("Test 2"), MB_OK);
            break;
        case IDM_DISABLE:
            bDisable = TRUE;
            break;
        case IDM_ENABLE:
            bDisable = FALSE;
            break;
        case IDM_SHOWWINDOW:
            ShowWindow(hWnd, SW_SHOW);
            UpdateWindow(hWnd);
            bShowWindow = false;
            break;
        case IDM_HIDEWINDOW:
            ShowWindow(hWnd, SW_HIDE);
            UpdateWindow(hWnd);
            bShowWindow = true;
            break;
        case IDM_MATRIX_P1:
            mc.clearMatrix();
            lightsOn = false;
            break;
        case IDM_MATRIX_P2:
            mc.drawMatrix(inputMatrix);
            lightsOn = true;
            break;
        case IDM_MATRIX_SNAKE:
            playSnake(mc, inputMatrix);
            break;
        case IDM_EXIT:
            Shell_NotifyIcon(NIM_DELETE, &nidApp);
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    /*case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            TextOut(hdc,
                5, 5,
                greeting, _tcslen(greeting));
            EndPaint(hWnd, &ps);
            mc.drawMatrix(inputMatrix);
        }
        break;*/
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
