#pragma once
#include "pch.h"
#include "MatrixController.h"

BOOL playSnake(MatrixController &mc, std::vector<std::vector<BYTE>> &inputMatrix)
{
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
                for (int i = snake.size() - 1; i >= 1; i--)
                {
                    if (newPos == snake[i])
                    {
                        alive = false;
                        break;
                    }
                    snake[i] = snake[i - 1];
                }
                snake[0].first += 2;
                break;
            }
            case 1:
            {
                std::pair<int, int> newPos = std::pair<int, int>(snake[0].first, snake[0].second + 1);
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
                snake[0].first -= 2;
                break;
            }
            case 3:
            {
                std::pair<int, int> newPos = std::pair<int, int>(snake[0].first, snake[0].second - 1);
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
        mc.clearMatrix();
        return false;

    }
    ////////////////////////////////////////////////////////////////////END OF SNAKE SAMPLE CODE////////////////////////////////////////////
    mc.clearMatrix(); //turn LEDS off before closing program
    return true;
}

