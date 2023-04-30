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
	CHESS_WHITE = -1,  //白棋
	CHESS_BLACK = 1    //黑棋
};

//棋盘类
class Chess
{
public:
		
	//构造函数
	Chess(Control* control, int BoardSize, int marginX, int marginY, int ChessSize);

	//绘制棋盘
	void drawChessBoard(const int size);

	//棋盘初始化：加载棋盘的图片资源，初始化棋盘相关数据
	void init();

	//判断落子是否合法，如果点击合法就将点击位置（行，列）保存在pos中
	bool clickBoard(int row, int col, ChessPos* pos);

	//获取上次落子位置
	ChessPos m_pos = { 0,0 };
	ChessPos chesspos();

	//在棋盘指定的位置（行，列）落子
	void chessDown(ChessPos* pos, chess_kind_t kind);

	//获取棋盘大小：13线，15线，17线，19线
	int getBoardSize();

	//获取指定位置是黑棋，还是白棋，或者空白
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	//检查棋局是否结束
	bool checkOver();	

private:
	IMAGE chessBlackImg;	//生成黑棋棋子
	IMAGE chessWhiteImg;	//生成白棋棋子

	Control* m_control;	//总控制信号
	int BoardSize;	//棋盘大小：13，15，17，19
	int margin_x;	//棋盘左侧边缘大小
	int margin_y;	//棋盘上侧边缘大小
	int ChessSize;	//棋子的大小（棋盘小方格的大小）

	//储存当前棋谱
	//chessMap[3][5]表示棋盘的第3行第5列的落子情况：0-空白，-1-白棋，1-黑棋
	vector<vector<int>> chessMap;

	//表示现在该谁下棋
	bool playerFlag;	//true-黑子落子，false-白棋落子

	void updateChessMap(ChessPos* pos);

	//如果胜负已分，返回true，否则返回false
	bool isWin();
};

