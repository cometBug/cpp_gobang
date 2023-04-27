#pragma once
#include <graphics.h>
#include <conio.h>
#include <vector>
#include <map>
#include <iostream>
#include <math.h>
#include <winuser.h>
#include "Control.h"
using namespace std;

void putimagePNG(int x, int y, IMAGE* picture);

//������
class Chess
{
public:

	//���캯��
	Chess(Control* control, int marginX, int marginY, int ChessSize);

	//���̳�ʼ�����������̵�ͼƬ��Դ����ʼ�������������
	void init();

	//�ж������Ƿ�Ϸ����������Ϸ��ͽ����λ�ã��У��У�������pos��
	bool clickBoard(int row, int col, ChessPos* pos);	

	//���á���ȡ���̴�С��13�ߣ�15�ߣ�17�ߣ�19��
	void setBoardSize(const int size);
	int getBoardSize();

	void updateChessMap(ChessPos* pos, enum chess_kind_t kind);

	//���ʤ���ѷ֣�����true�����򷵻�false
	bool isWin(enum chess_kind_t kind);

private:	

	Control* m_control;	//�ܿ����ź�
	int BoardSize;	//���̴�С��13��15��17��19
	int margin_x;	//��������Ե��С
	int margin_y;	//�����ϲ��Ե��С
	int ChessSize;	//���ӵĴ�С������С����Ĵ�С��	

};

