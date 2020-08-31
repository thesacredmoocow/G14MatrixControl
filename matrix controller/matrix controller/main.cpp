//#include "pch.h"
#pragma warning( disable : 4267)

#include <stdio.h>
#include "MatrixController.h"

LONG __cdecl
_tmain(
    LONG     Argc,
    LPTSTR * Argv
    )
/*++

Routine description:

    Sample program that communicates with a USB device using WinUSB

--*/
{
    
    //////////////////////////////////////////////////////MICROSOFT DRIVER CODE////////////////////////////////////////////////////
    DEVICE_DATA           deviceData;
    HRESULT               hr;
    USB_DEVICE_DESCRIPTOR deviceDesc;
    BOOL                  bResult;
    BOOL                  noDevice;
    ULONG                 lengthReceived;

    UNREFERENCED_PARAMETER(Argc);
    UNREFERENCED_PARAMETER(Argv);

    hr = OpenDevice(&deviceData, &noDevice);
    if (FAILED(hr)) {

        if (noDevice) {

            wprintf(L"Device not connected or driver not installed\n");

        } else {

            wprintf(L"Failed looking for device, HRESULT 0x%x\n", hr);
        }
        system("pause");
        return 0;
    }

    bResult = WinUsb_GetDescriptor(deviceData.WinusbHandle,
                                   USB_DEVICE_DESCRIPTOR_TYPE,
                                   0,
                                   0,
                                   (PBYTE) &deviceDesc,
                                   sizeof(deviceDesc),
                                   &lengthReceived);

    if (FALSE == bResult || lengthReceived != sizeof(deviceDesc)) {

        wprintf(L"Error among LastError %d or lengthReceived %d\n",
                FALSE == bResult ? GetLastError() : 0,
                lengthReceived);
        CloseDevice(&deviceData);
        return 0;
    }
    
    //////////////////////////////////////////////////////CODE FOR MANIPULATING ANIME MATRIX///////////////////////////////////////////

    
    const int arrSize[55] = { 33,33,33,33,33,33,33,32,32,31,31,30,30,29,29,28,28,27,27,26,26,25,25,24,24,23,23,22,22,21,21,20,20,19,19,18,18,17,17,16,16,15,15,14,14,13,13,12,12,11,11,10,10,9,9 }; //initializing an array with hardcoded widths of each row
    std::vector<std::vector<BYTE>> inputMatrix; //creating a 2d vector to store the led matrix [0][0] is top right corner, [55][0] is bottom right corner, [0][33] is top left corner
    for (int i = 0; i < MATRIXHEIGHT; i++)
    {
        std::vector<BYTE> temp;
        for (int j = 0; j < arrSize[i]; j++)
        {
            temp.push_back(0x00);
        }
        inputMatrix.push_back(temp);
    }

    MatrixController mc(deviceData.WinusbHandle); //create MatrixController and assign the winUSB interface handle
    mc.initMatrix(); //send out first initialization packet to aniMe Matrix
    mc.drawMatrix(inputMatrix); //draws Matrix using data from input vector and returns a matrixStatus value (see matrixStatus.h)
    
    //////////////////////////////SAMPLE SNAKE GAME//////////////////////////////////////////////////////////////////////////////////
    srand(unsigned int(time(0)));
    int inverseSpeed = 14;
    std::vector<std::pair<int, int>>snake;
    snake.push_back(std::pair<int, int>(0, 3));
    snake.push_back(std::pair<int, int>(0, 2));
    snake.push_back(std::pair<int, int>(0, 1));
    int snakeDir = 1; //0 = up, increases clockwise
    bool alive = true;
    int movementCounter = 0;
    bool changed = false;
    std::pair<int, int> goal;
    do
    {
        goal.first = rand() % MATRIXHEIGHT;
    } while (goal.first % 2);
    goal.second = rand() % arrSize[goal.first];

    while (alive)
    {
        if (GetAsyncKeyState(VK_ESCAPE))
        {
            break;
        }
        else if (GetAsyncKeyState(VK_UP) && snakeDir != 2 && !changed)
        {
            snakeDir = 0;
            changed = true;
        }
        else if (GetAsyncKeyState(VK_LEFT) && snakeDir != 1 && !changed)
        {
            snakeDir = 3;
            changed = true;
        }
        else if (GetAsyncKeyState(VK_RIGHT) && snakeDir != 3 && !changed)
        {
            snakeDir = 1;
            changed = true;
        }
        else if (GetAsyncKeyState(VK_DOWN) && snakeDir != 0 && !changed)
        {
            snakeDir = 2;
            changed = true;
        }
        for (int i = 0; i < snake.size(); i++)
        {
            std::cout << snake[i].first << "," << snake[i].second << "   ";
        }
        std::cout << std::endl;
        movementCounter++;
        if (movementCounter >= inverseSpeed)
        {
            changed = false;
            switch (snakeDir)
            {
                case 0:
                {
                    std::pair<int, int> newPos = std::pair<int, int>(snake[0].first + 2, snake[0].second);
                    if (newPos == goal)
                    {
                        snake.insert(snake.begin(), newPos);
                        do
                        {
                            goal.first = rand() % MATRIXHEIGHT;
                        } while (goal.first % 2);
                        goal.second = rand() % arrSize[goal.first];
                        break;
                    }
                    for (int i = snake.size()-1; i >=1; i--)
                    {
                        if (newPos == snake[i])
                        {
                            alive = false;
                            break;
                        }
                        snake[i] = snake[i - 1];
                    }
                    snake[0].first+=2;
                    break;
                }
                case 1:
                {
                    std::pair<int, int> newPos = std::pair<int, int>(snake[0].first, snake[0].second+1);
                    if (newPos == goal)
                    {
                        snake.insert(snake.begin(), newPos);
                        do
                        {
                            goal.first = rand() % MATRIXHEIGHT;
                        } while (goal.first % 2);
                        goal.second = rand() % arrSize[goal.first];
                        break;
                    }
                    for (int i = snake.size() - 1; i >= 1; i--)
                    {
                        if (newPos == snake[i])
                        {
                            alive = false;
                            break;
                        }
                        snake[i] = snake[i - 1];
                    }
                    snake[0].second++;
                    break;
                }
                case 2:
                {
                    std::pair<int, int> newPos = std::pair<int, int>(snake[0].first - 2, snake[0].second);
                    if (newPos == goal)
                    {
                        snake.insert(snake.begin(), newPos);
                        do
                        {
                            goal.first = rand() % MATRIXHEIGHT;
                        } while (goal.first % 2);
                        goal.second = rand() % arrSize[goal.first];
                        break;
                    }
                    for (int i = snake.size() - 1; i >= 1; i--)
                    {
                        if (newPos == snake[i])
                        {
                            alive = false;
                            break;
                        }
                        snake[i] = snake[i - 1];
                    }
                    snake[0].first-=2;
                    break;
                }
                case 3:
                {
                    std::pair<int, int> newPos = std::pair<int, int>(snake[0].first, snake[0].second-1);
                    if (newPos == goal)
                    {
                        snake.insert(snake.begin(), newPos);
                        do
                        {
                            goal.first = rand() % MATRIXHEIGHT;
                        } while (goal.first % 2);
                        goal.second = rand() % arrSize[goal.first];
                        break;
                    }
                    for (int i = snake.size() - 1; i >= 1; i--)
                    {
                        if (newPos == snake[i])
                        {
                            alive = false;
                            break;
                        }
                        snake[i] = snake[i - 1];
                    }
                    snake[0].second--;
                    break;
                }
            }
            if (snake[0].first < 0 || snake[0].first >= MATRIXHEIGHT || snake[0].second < 0 || snake[0].second >= arrSize[snake[0].first])
            {
                alive = false;
                break;
            }
            for (int i = 0; i < MATRIXHEIGHT; i++)
            {
                for (int j = 0; j < inputMatrix[i].size(); j++)
                {
                    inputMatrix[i][j] = 0x00;
                }
            }
            for (int i = 0; i < snake.size(); i++)
            {
                inputMatrix[snake[i].first][snake[i].second] = 0xff;
            }
            inputMatrix[goal.first][goal.second] = 0xff;
            mc.drawMatrix(inputMatrix);
            movementCounter = 0;
        }
        
        Sleep(10);
    }
    if (!alive)
    {
        for (int i = 0; i < MATRIXHEIGHT; i++)
        {
            for (int j = 0; j < inputMatrix[i].size(); j++)
            {
                inputMatrix[i][j] = 0xff;
            }
        }
        mc.drawMatrix(inputMatrix);
        Sleep(400);
        for (int i = 0; i < MATRIXHEIGHT; i++)
        {
            for (int j = 0; j < inputMatrix[i].size(); j++)
            {
                inputMatrix[i][j] = 0x00;
            }
        }
        mc.drawMatrix(inputMatrix);
    }
    ////////////////////////////////////////////////////////////////////END OF SNAKE SAMPLE CODE////////////////////////////////////////////
    system("pause");
    mc.clearMatrix(); //turn LEDS off before closing program
    CloseDevice(&deviceData);
    return 0;
}
