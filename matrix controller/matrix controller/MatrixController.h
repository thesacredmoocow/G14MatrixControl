#pragma once
#include "pch.h"

#define MATRIXHEIGHT 55

class MatrixController
{
private:
	WINUSB_INTERFACE_HANDLE usbHandle;
	WINUSB_SETUP_PACKET SetupPacket;

	USHORT packetLength = 640;
	unsigned char header[15] = "ASUS Tech.Inc.";
	BYTE firstByte = 0x5e;
	BYTE RequestType = 0x21;
	BYTE Request = 0x09;
	SHORT Value = 0x035e;
	BYTE Index = 0x00;
	BYTE paneHeaders[2][7] = { 0x5e, 0xc0, 0x02, 0x01, 0x00, 0x73, 0x02 ,
							   0x5e, 0xc0, 0x02, 0x74, 0x02, 0x73, 0x02 };
	BYTE firstPane[640];
	BYTE secondPane[640];
	BYTE flushPacket[640];
	std::vector<int> sizes;
	int rowIndex[MATRIXHEIGHT][2] = {
		7, 39,
		41, 73,
		76, 108,
		109,141,
		144,176,
		177,209,
		211,243,
		244,275,
		277,308,
		309,339,
		341,371,
		372,401,
		403,432,
		433,461,
		463,491,
		492,519,
		521,548,
		549,575,
		577,603,
		604,629,
		631,656,
		30,54,
		56,80,
		81,104,
		106,129,
		130,152,
		154,176,
		177,198,
		200,221,
		222,242,
		244,264,
		265,284,
		286,305,
		306,324,
		326,344,
		345,362,
		364,381,
		382,398,
		400,416,
		417,432,
		434,449,
		450,464,
		466,480,
		481,494,
		496,509,
		510,522,
		524,536,
		537,548,
		550,561,
		562,572,
		574,584,
		585,594,
		596,605,
		606,614,
		616,624,
	};
	int rowSize[MATRIXHEIGHT];
	bool hasHandle;
public:
	MatrixController();
	MatrixController(WINUSB_INTERFACE_HANDLE h);
	void setHandle(WINUSB_INTERFACE_HANDLE h);
	bool initMatrix();
	matrixStatus drawMatrix(std::vector<std::vector<BYTE>> arr);
	matrixStatus clearMatrix();
};

