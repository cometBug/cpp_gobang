#include "Man.h"
#include "Control.h"
#include <iostream>


void Man::init(Chess* chess)
{
	m_chess = chess;
}

void Man::go()
{
	ChessPos pos;
	
	while (1) {
		//获取鼠标点击消息
		m_control->updateMessage();

		//通过棋盘对象，调用判断落子是否有效，以及落子功能
		if (m_control->m_msg.message == WM_LBUTTONDOWN &&
			m_chess->clickBoard(m_control->m_msg.x, m_control->m_msg.y, &pos)) {
			break;
		}
	}
	//cout << pos.m_row <<"," << pos.m_col << endl;

	//落子
	m_chess->chessDown(&pos, CHESS_BLACK);
}
