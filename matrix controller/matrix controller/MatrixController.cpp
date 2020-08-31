#include "MatrixController.h"

MatrixController::MatrixController()
{
	hasHandle = false;
	usbHandle = NULL;
	ZeroMemory(flushPacket, sizeof(BYTE) * 640);
	flushPacket[0] = firstByte;
	flushPacket[1] = 0xc0;
	flushPacket[2] = 0x03;
	ZeroMemory(firstPane, sizeof(BYTE) * 640);
	ZeroMemory(secondPane, sizeof(BYTE) * 640);
	ZeroMemory(&SetupPacket, sizeof(WINUSB_SETUP_PACKET));
	SetupPacket.RequestType = RequestType;
	SetupPacket.Request = Request;
	SetupPacket.Value = Value;
	SetupPacket.Index = Index;
	SetupPacket.Length = packetLength;

	for (int i = 0; i < MATRIXHEIGHT; i++)
	{
		rowSize[i] = rowIndex[i][1] - rowIndex[i][0] + 1;
	}
}

MatrixController::MatrixController(WINUSB_INTERFACE_HANDLE h)
{
	usbHandle = h;
	hasHandle = true;
	ZeroMemory(flushPacket, sizeof(BYTE) * 640);
	flushPacket[0] = firstByte;
	flushPacket[1] = 0xc0;
	flushPacket[2] = 0x03;
	ZeroMemory(firstPane, sizeof(BYTE) * 640);
	ZeroMemory(secondPane, sizeof(BYTE) * 640);
	ZeroMemory(&SetupPacket, sizeof(WINUSB_SETUP_PACKET));
	SetupPacket.RequestType = RequestType;
	SetupPacket.Request = Request;
	SetupPacket.Value = Value;
	SetupPacket.Index = Index;
	SetupPacket.Length = packetLength;

	for (int i = 0; i < MATRIXHEIGHT; i++)
	{
		rowSize[i] = rowIndex[i][1] - rowIndex[i][0] + 1;
		std::cout << rowSize[i] << ",";
	}
}

void MatrixController::setHandle(WINUSB_INTERFACE_HANDLE h)
{
	usbHandle = h;
	hasHandle = true;
}

bool MatrixController::initMatrix()
{
	if (!hasHandle)
	{
		return false;
	}

	ULONG cbSent = 0;
	BYTE packet[640];
	ZeroMemory(packet, sizeof(BYTE) * 640);
	packet[0] = firstByte;
	for (int i = 0; i < 14; i++)
	{
		packet[i + 1] = header[i];
	}

	if (!WinUsb_ControlTransfer(usbHandle, SetupPacket, packet, 640, &cbSent, 0) || cbSent != 640)
	{
		return false;
	}
	return true;
}

matrixStatus MatrixController::drawMatrix(std::vector<std::vector<BYTE>> arr)
{
	ZeroMemory(firstPane, sizeof(BYTE) * 640);
	ZeroMemory(secondPane, sizeof(BYTE) * 640);
	for (int i = 0; i < 7; i++)
	{
		firstPane[i] = paneHeaders[0][i];
		secondPane[i] = paneHeaders[1][i];
	}
	if (!hasHandle)
	{
		return matrixStatus::MATRIX_NO_HANDLE;
	}
	if (arr.size() != MATRIXHEIGHT)
	{
		return matrixStatus::MATRIX_VECTOR_INPUT_ERROR;
	}
	else
	{
		for (int i = 0; i < MATRIXHEIGHT; i++)
		{
			if (arr[i].size() != rowSize[i])
			{
				return matrixStatus::MATRIX_VECTOR_INPUT_ERROR;
			}
			else
			{
				if (i < 20)
				{
					if (i == 0)
					{
						for (int j = 0; j < arr[i].size()-1; j++)
						{
							firstPane[rowIndex[i][1] - j] = arr[i][int(j)+1];
						}
					}
					else
					{
						for (int j = 0; j < arr[i].size(); j++)
						{
							firstPane[rowIndex[i][1] - j] = arr[i][j];
						}
					}
				}
				else if (i > 20)
				{
					for (int j = 0; j < arr[i].size(); j++)
					{
						secondPane[rowIndex[i][1] - j] = arr[i][j];
					}
				}
				else if (i == 20)
				{
					for (int j = 0; j < 23; j++)
					{
						secondPane[29 - j] = arr[i][j];
					}
					for (int j = 23; j < 26; j++)
					{
						firstPane[rowIndex[i][1] - j] = arr[i][j];
					}
				}
			}
		}
	}
	ULONG bytesSent = 0;
	if (!WinUsb_ControlTransfer(usbHandle, SetupPacket, firstPane, 640, &bytesSent, 0) || bytesSent != 640)
	{
		return matrixStatus::MATRIX_DRAW_ERROR;
	}
	if (!WinUsb_ControlTransfer(usbHandle, SetupPacket, secondPane, 640, &bytesSent, 0) || bytesSent != 640)
	{
		return matrixStatus::MATRIX_DRAW_ERROR;
	}
	if (!WinUsb_ControlTransfer(usbHandle, SetupPacket, flushPacket, 640, &bytesSent, 0) || bytesSent != 640)
	{
		return matrixStatus::MATRIX_DRAW_ERROR;
	}
	return matrixStatus::MATRIX_DRAW_SUCCESSFUL;
}

matrixStatus MatrixController::clearMatrix()
{
	int arrSize[55] = { 33,33,33,33,33,33,33,32,32,31,31,30,30,29,29,28,28,27,27,26,26,25,25,24,24,23,23,22,22,21,21,20,20,19,19,18,18,17,17,16,16,15,15,14,14,13,13,12,12,11,11,10,10,9,9 };
	std::vector<std::vector<BYTE>> inputMatrix;
	for (int i = 0; i < MATRIXHEIGHT; i++)
	{
		std::vector<BYTE> temp;
		for (int j = 0; j < arrSize[i]; j++)
		{
			temp.push_back(0x00);
		}
		inputMatrix.push_back(temp);
	}
	return drawMatrix(inputMatrix);
}