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

//棋盘类
class Chess
{
public:

	//构造函数
	Chess(Control* control, int marginX, int marginY, int ChessSize);

	//棋盘初始化：加载棋盘的图片资源，初始化棋盘相关数据
	void init();

	//判断落子是否合法，如果点击合法就将点击位置（行，列）保存在pos中
	bool clickBoard(int row, int col, ChessPos* pos);	

	//设置、获取棋盘大小：13线，15线，17线，19线
	void setBoardSize(const int size);
	int getBoardSize();

	void updateChessMap(ChessPos* pos, enum chess_kind_t kind);

	//如果胜负已分，返回true，否则返回false
	bool isWin(enum chess_kind_t kind);

private:	

	Control* m_control;	//总控制信号
	int BoardSize;	//棋盘大小：13，15，17，19
	int margin_x;	//棋盘左侧边缘大小
	int margin_y;	//棋盘上侧边缘大小
	int ChessSize;	//棋子的大小（棋盘小方格的大小）	

};

