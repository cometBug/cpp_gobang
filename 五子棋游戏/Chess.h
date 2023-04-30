#pragma once
#include <graphics.h>
#include <conio.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <winuser.h>
#include "Control.h"
using namespace std;

void putimagePNG(int x, int y, IMAGE* picture);

struct ChessPos {
	int m_row;
	int m_col;

	ChessPos(int r = 0, int c = 0) :m_row(r), m_col(c) {}
};

enum chess_kind_t {
	CHESS_WHITE = -1,  //����
	CHESS_BLACK = 1    //����
};

//������
class Chess
{
public:
		
	//���캯��
	Chess(Control* control, int BoardSize, int marginX, int marginY, int ChessSize);

	//��������
	void drawChessBoard(const int size);

	//���̳�ʼ�����������̵�ͼƬ��Դ����ʼ�������������
	void init();

	//�ж������Ƿ�Ϸ����������Ϸ��ͽ����λ�ã��У��У�������pos��
	bool clickBoard(int row, int col, ChessPos* pos);

	//��ȡ�ϴ�����λ��
	ChessPos m_pos = { 0,0 };
	ChessPos chesspos();

	//������ָ����λ�ã��У��У�����
	void chessDown(ChessPos* pos, chess_kind_t kind);

	//��ȡ���̴�С��13�ߣ�15�ߣ�17�ߣ�19��
	int getBoardSize();

	//��ȡָ��λ���Ǻ��壬���ǰ��壬���߿հ�
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	//�������Ƿ����
	bool checkOver();	

private:
	IMAGE chessBlackImg;	//���ɺ�������
	IMAGE chessWhiteImg;	//���ɰ�������

	Control* m_control;	//�ܿ����ź�
	int BoardSize;	//���̴�С��13��15��17��19
	int margin_x;	//��������Ե��С
	int margin_y;	//�����ϲ��Ե��С
	int ChessSize;	//���ӵĴ�С������С����Ĵ�С��

	//���浱ǰ����
	//chessMap[3][5]��ʾ���̵ĵ�3�е�5�е����������0-�հף�-1-���壬1-����
	vector<vector<int>> chessMap;

	//��ʾ���ڸ�˭����
	bool playerFlag;	//true-�������ӣ�false-��������

	void updateChessMap(ChessPos* pos);

	//���ʤ���ѷ֣�����true�����򷵻�false
	bool isWin();
};

