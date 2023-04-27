#include "Man.h"

void Man::init(Control* control)
{	
	m_control = control;
}

void Man::go(enum chess_kind_t kind)
{
	ChessPos pos;

	while (1) {
		//��ȡ�������Ϣ
		//getmessage(&m_control->m_msg, EX_MOUSE);
		m_control->updateMessage();
		//cout << m_control->m_msg.x << "," << m_control->m_msg.y << endl;
		//�ж���������
		//����������
		if (m_control->clickArea(m_control->m_msg)) {
			//������Ч�Լ��
			if (m_control->clickBoard(m_control->m_msg.x, m_control->m_msg.y, &pos)) {
				//����
				m_control->chessDown(&pos, kind);
				return;
			}
			else {
				mciSendString(_T("play res/music/��Ч���.mp3"), 0, 0, 0);
			}
		}
		//���빦����
		else {
			if (m_control->getclickmsg() == RESTART) {
				m_control->m_restart = true;
				return;
			}
			if (m_control->getclickmsg() == REGRET) {
				m_control->initTipsArea();
				m_control->retractChess();
				continue;
			}
			if (m_control->getclickmsg() == TIPS) {
				m_control->renewBoard();
				m_control->initTipsArea();
				m_control->getTips(m_ai->think(kind), kind);
				continue;
			}
			if (m_control->getclickmsg() == EXCHANGE) {
				m_control->m_exchange = true;
				return;
			}
			if (m_control->getclickmsg() == MENU) {
				m_control->m_menu = true;
				return;
			}
			if (m_control->getclickmsg() == QUIT) {
				exit(0);
			}
		}
	}	
}
