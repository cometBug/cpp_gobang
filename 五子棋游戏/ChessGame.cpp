#include "ChessGame.h"

ChessGame::ChessGame(Man* man, Man* man1, AI* ai, Control* control)
{
	m_ai = ai;
	m_man = man;
	m_man1 = man1;	
	m_control = control;

	ai->init(control);
	man->init(control);
	man1->init(control);
}

void ChessGame::init()
{
	m_control->init();
	m_ai->init(m_control);
	m_man->init(m_control);
	m_man1->init(m_control);
}

//�Ծ֣���ʼ��������Ϸ��
void ChessGame::play(Man* man, AI* ai)
{
RESTART:
	init();	//��ʼ��
	while (1)
	{
		//������������
		man->go(CHESS_BLACK);
		m_control->renewBoard();
		m_control->initTipsArea();
		if (m_control->m_menu || m_control->m_exchange)
			return;
		if (m_control->m_restart)
			goto RESTART;
		if (checkOver(CHESS_BLACK, MAN_to_AI))
			break;
		//��AI����
		ai->go(CHESS_WHITE);
		if (checkOver(CHESS_WHITE, MAN_to_AI))
			break;
	}
}


//AI����
//void ChessGame::play(Man* man, AI* ai)
//{
//RESTART:
//	m_chess->init();	//���̳�ʼ��
//	m_control->clearinfo(); //������Ϣ��ʼ��
//	while (1)
//	{
//		//������������
//		ai->go(CHESS_BLACK);
//		if (checkOver(CHESS_WHITE, AI_to_MAN))
//			break;
//		//��AI����
//		ai->go(CHESS_WHITE);
//		if (checkOver(CHESS_WHITE, MAN_to_AI))
//			break;
//	}
//}

void ChessGame::play(AI* ai,Man* man)
{	
	RESTART:
	init();	//��ʼ��
	while (1) {
		//����AI����
		m_ai->go(CHESS_BLACK);
		if (checkOver(CHESS_BLACK, AI_to_MAN))
			break;		
		//����������
		m_man->go(CHESS_WHITE);
		m_control->renewBoard();
		m_control->initTipsArea();
		if (m_control->m_menu || m_control->m_exchange)
			return;
		if (m_control->m_restart)
			goto RESTART;
		if (checkOver(CHESS_WHITE, AI_to_MAN))
			break;
	}
}

void ChessGame::play(Man* man, Man* man1)
{	
	RESTART:
	init();	//��ʼ��
	while (1) {
		//������������
		man->go(CHESS_BLACK);
		m_control->renewBoard();
		m_control->initTipsArea();
		if (m_control->m_menu || m_control->m_exchange)
			return;
		if (m_control->m_restart)
			goto RESTART;
		if (checkOver(CHESS_BLACK, MAN_to_MAN))
			break;
		//������1����
		man1->go(CHESS_WHITE);
		m_control->renewBoard();
		m_control->initTipsArea();
		if (m_control->m_menu || m_control->m_exchange)
			return;
		if (m_control->m_restart)
			goto RESTART;
		if (checkOver(CHESS_WHITE, MAN_to_MAN))
			break;
	}
}

bool ChessGame::checkOver(enum chess_kind_t kind, int m_kind)
{

	//���ʤ���ѷ�
	if (m_control->isWin(kind)) {

		Sleep(1500);		
		if (m_kind == MAN_to_AI) {
			//����man����ʤ
			if (kind == CHESS_BLACK)
				m_control->drawGameOver(m_kind, CHESS_BLACK);
			else
				m_control->drawGameOver(m_kind, CHESS_WHITE);
		}
		else if (m_kind == AI_to_MAN) {
			//����man����ʤ
			if (kind == CHESS_WHITE)
				m_control->drawGameOver(m_kind, CHESS_WHITE);
			else
				m_control->drawGameOver(m_kind, CHESS_BLACK);
		}
		else {
			if (kind == CHESS_BLACK)
				m_control->drawGameOver(m_kind, CHESS_BLACK);
			else
				m_control->drawGameOver(m_kind, CHESS_WHITE);
		}
		while (1) {
			//���¿�����Ϣ
			m_control->updateMessage();
			if (m_control->getclickmsg() == AGAIN) {
				m_control->m_goon = true;
				return true;
			}
			else if (m_control->getclickmsg() == MENU) {
				m_control->m_menu = true;
				return true;
			}
			else if (m_control->getclickmsg() == QUIT) {
				m_control->m_quit = true;
				return true;
			}
		}
		//_getch();
		//closegraph();
	}
	return false;
}
