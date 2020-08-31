// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <winusb.h>
#include <usb.h>
#include <cstdlib>
#include "device.h"
#include <iostream>
#include <synchapi.h>
#include<vector>
#include "matrixStatus.h"
#include <time.h>
#include <shellapi.h>

#define IDC_MYICON                      2
#define IDD_SYSTRAYDEMO_DIALOG          102
#define IDS_APP_TITLE                   103
#define IDD_ABOUTBOX                    103
#define IDM_ABOUT                       104
#define IDS_APPTOOLTIP                  104
#define IDM_EXIT                        105
#define IDI_SYSTRAYDEMO                 107
#define IDI_SMALL                       108
#define IDC_SYSTRAYDEMO                 109
#define IDR_MAINFRAME                   128
#define ID_S_                           32771
#define ID_S_EXIT                       32772
#define ID_SYSTRAYMENU_                 32773
#define ID_SYSTRAYMENU_ENABLE           32774
#define ID_SYSTRAYMENU_DISABLE          32775
#define ID_SYSTRAYMENU_TEST1            32776
#define ID_SYSTRAYMENU_TEST2            32777
#define IDM_DISABLE                     32778
#define IDM_ENABLE                      32779
#define IDM_TEST2                       32780
#define IDM_TEST1                       32781
#define ID_SYSTRAYMENU_SEP              32782
#define IDM_SEP                         32783
#define IDM_SHOWWINDOW					32784
#define IDM_HIDEWINDOW					32785
#define IDM_MATRIX_P1					32786
#define IDM_MATRIX_P2					32787
#define IDM_MATRIX_SNAKE				32788
#define IDC_STATIC                      -1
#define	WM_USER_SHELLICON WM_USER + 1

const int arrSize[55] = { 33,33,33,33,33,33,33,32,32,31,31,30,30,29,29,28,28,27,27,26,26,25,25,24,24,23,23,22,22,21,21,20,20,19,19,18,18,17,17,16,16,15,15,14,14,13,13,12,12,11,11,10,10,9,9 }; //initializing an array with hardcoded widths of each row
#endif //PCH_H
