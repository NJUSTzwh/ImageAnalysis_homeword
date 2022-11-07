#include "iostream"
#include"stdio.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include"math.h"
#pragma warning (disable:4996)
using namespace std;
#define u_int8_t	unsigned __int8
#define u_int		unsigned __int32


int main(int argc, char** argv)
{
	char* yuvFileName = NULL;
	char* yuceFileName = NULL;
	char* chazhiFileName = NULL;
	u_int8_t* yBuf = NULL;
	u_int8_t* yuvBuf = NULL;
	u_int8_t* yYuceBuf = NULL;
	u_int8_t* yChazhiBuf = NULL;
	u_int8_t* Buf128 = NULL;
	FILE* yuvFile = NULL;
	FILE* chazhiFile = NULL;
	FILE* yuceFile = NULL;
	u_int frameWidth = 0;
	u_int frameHeight = 0;
	int a = 0;//量化前插值
	int b = 0;//量化后值
	int c = 0;//反量化后值
	int d = 0;//+128之后值
	int x = 0;//选择4bit，8bit量化

	//读取数据yuv文件数据
	yuvFileName = argv[1];
	yuceFileName = argv[2];
	chazhiFileName = argv[3];
	frameWidth = atoi(argv[4]);
	frameHeight = atoi(argv[5]);
	yuvFile = fopen(yuvFileName, "rb");
	yuvBuf = (u_int8_t*)malloc(frameWidth * frameHeight * 3 / 2);
	yBuf = (u_int8_t*)malloc(frameWidth * frameHeight);
	yYuceBuf = (u_int8_t*)malloc(frameWidth * frameHeight);
	yChazhiBuf = (u_int8_t*)malloc(frameWidth * frameHeight);
	Buf128 = (u_int8_t*)malloc(frameWidth * frameHeight / 2);

	fread(yuvBuf, 1, frameWidth * frameHeight * 3 / 2, yuvFile);

	yuceFile = fopen(yuceFileName, "wb");
	chazhiFile = fopen(chazhiFileName, "wb");

	//给Buf128全赋值为128
	for (int i = 0; i < frameWidth * frameHeight / 2; i++)
	{
		Buf128[i] = 128;
	}

	//读取y数据
	for (int i = 0; i < frameWidth * frameHeight; i++)
	{
		yBuf[i] = yuvBuf[i];
	}

	//选择4bit，8bit量化,2bit量化，1bit量化
	while ((x != 4) && (x != 8) && (x != 2) && (x != 1) && (x != 99))
	{
		cout << "如想左向预测：选择8bit量化或者4bit量化或者2bit量化或者1bit量化，请输入“8”或“4”或“2”或“1”" << endl << "如想上向预测请输入“99”(8bit)" << endl;
		cin >> x;
		if (x == 8)
		{
			cout << "您选择了左向预测8bit量化";
		}
		else if (x == 4)
		{
			cout << "您选择了左向预测4bit量化";
		}
		else if (x == 2)
		{
			cout << "您选择了左向预测2bit量化";
		}
		else if (x == 1)
		{
			cout << "您选择了左向预测1bit量化";
		}
		if (x == 99)
		{
			cout << "您选择了上向预测";
		}
		else
		{
			cout << "您输入有误请重新输入";
		}
	}

	//左向预测8bit、4bit、2bit、1bit-计算差值与预测值
	for (int i = 0; i < frameHeight; i++)
	{
		for (int j = 0; j < frameWidth; j++)
		{
			if (x == 8)
			{
				if (j == 0)
				{
					yYuceBuf[j + i * frameWidth] = yBuf[j + i * frameWidth];
					yChazhiBuf[j + i * frameWidth] = 0;
				}
				else
				{
					a = yBuf[j + i * frameWidth] - yYuceBuf[j + i * frameWidth - 1];//计算插值
					b = a / 2;//进行量化
					c = b * 2;//进行反量化
					d = b + 128;//进行+128
					yChazhiBuf[j + i * frameWidth] = d;
					yYuceBuf[j + i * frameWidth] = yYuceBuf[j + i * frameWidth - 1] + c;
				}
			}
			if (x == 4)
			{
				if (j == 0)
				{
					yYuceBuf[j + i * frameWidth] = yBuf[j + i * frameWidth];
					yChazhiBuf[j + i * frameWidth] = 0;
				}
				else
				{
					a = yBuf[j + i * frameWidth] - yYuceBuf[j + i * frameWidth - 1];//计算插值
					b = a / 16;//进行量化
					c = b * 16;//进行反量化
					d = b + 16;//进行+64
					yChazhiBuf[j + i * frameWidth] = d * 8;
					yYuceBuf[j + i * frameWidth] = yYuceBuf[j + i * frameWidth - 1] + c;
				}
			}
			if (x == 2)
			{
				if (j == 0)
				{
					yYuceBuf[j + i * frameWidth] = yBuf[j + i * frameWidth];
					yChazhiBuf[j + i * frameWidth] = 0;
				}
				else
				{
					a = yBuf[j + i * frameWidth] - yYuceBuf[j + i * frameWidth - 1];//计算插值
					b = a / 64;//进行量化
					c = b * 64;//进行反量化
					d = b + 4;//进行+4
					yChazhiBuf[j + i * frameWidth] = d * 32;
					yYuceBuf[j + i * frameWidth] = yYuceBuf[j + i * frameWidth - 1] + c;
				}
			}
			if (x == 1)
			{
				if (j == 0)
				{
					yYuceBuf[j + i * frameWidth] = yBuf[j + i * frameWidth];
					yChazhiBuf[j + i * frameWidth] = 0;
				}
				else
				{
					a = yBuf[j + i * frameWidth] - yYuceBuf[j + i * frameWidth - 1];//计算插值
					b = a / 128;//进行量化
					c = b * 128;//进行反量化
					d = b + 2;//进行+2
					yChazhiBuf[j + i * frameWidth] = d * 64;
					yYuceBuf[j + i * frameWidth] = yYuceBuf[j + i * frameWidth - 1] + c;
				}
			}
		}
	}
	//上预测
	for (int i = 0; i < frameWidth; i++)
	{
		for (int j = 0; j < frameHeight; j++)
		{
			if (x == 99)
			{
				if (j == 0)
				{
					yYuceBuf[i] = yBuf[i];
					yChazhiBuf[i] = 0;
				}
				else
				{
					a = yBuf[i + j * frameWidth] - yYuceBuf[i + (j - 1) * frameWidth];//计算插值
					b = a / 2;//进行量化
					c = b * 2;//进行反量化
					d = b + 128;//进行+128
					yChazhiBuf[i + j * frameWidth] = d;
					yYuceBuf[i + j * frameWidth] = yYuceBuf[i + (j - 1) * frameWidth] + c;
				}
			}
		}
	}
	fwrite(yYuceBuf, frameWidth * frameHeight, 1, yuceFile);
	fwrite(yChazhiBuf, frameWidth * frameHeight, 1, chazhiFile);
	fwrite(Buf128, frameWidth * frameHeight / 2, 1, chazhiFile);
	fwrite(Buf128, frameWidth * frameHeight / 2, 1, yuceFile);
	fclose(chazhiFile);
	fclose(yuceFile);
	fclose(yuvFile);
	free(yBuf);
	free(yuvBuf);
	free(yYuceBuf);
	free(yChazhiBuf);
	free(Buf128);

	return 0;
}
