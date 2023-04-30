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
		//��ȡ�������Ϣ
		m_control->updateMessage();

		//ͨ�����̶��󣬵����ж������Ƿ���Ч���Լ����ӹ���
		if (m_control->m_msg.message == WM_LBUTTONDOWN &&
			m_chess->clickBoard(m_control->m_msg.x, m_control->m_msg.y, &pos)) {
			break;
		}
	}
	//cout << pos.m_row <<"," << pos.m_col << endl;

	//����
	m_chess->chessDown(&pos, CHESS_BLACK);
}
