#include "ChessGame.h"
#include "Chess.h"


ChessGame::ChessGame(Man* man, AI* ai, Chess* chess, Control* control)
{
	m_man = man; 
	m_ai = ai; 
	m_chess = chess;
	m_control = control;

	man->init(chess);
	ai->init(chess);
}

void ChessGame::StartMenu()
{
	//������ʼ�˵�����
	initgraph(1104, 621);

	//�ػ洰�ڣ�ʹ���ھ���
	HWND hWnd = GetHWnd();
	SetWindowPos(hWnd, nullptr, 300, 150, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	//SetWindowText(hWnd, _T("��������Ϸ"));

	//��ʾ��ʼ�˵�ͼƬ
	loadimage(0, _T("res/menu.jpg"));
	
	//���ؿ�ʼ��ť
	IMAGE start_bt;
	loadimage(&start_bt, _T("res/start_button.png"), 794/6, 350/6, true);
	putimagePNG(400, 400, &start_bt);

	//�����˳���ť
	IMAGE quit_bt;
	loadimage(&quit_bt, _T("res/quit_button.png"), 794 / 6, 350 / 6, true);
	putimagePNG(570, 400, &quit_bt);

	//��ͣ��ʾͼƬ
	//system("pause");
	
}

//�Ծ֣���ʼ��������Ϸ��
void ChessGame::play()
{
	m_control->clearinfo(); //������Ϣ��ʼ��
	m_chess->init();	    //���̳�ʼ��
	while (1) {
		//������������
		m_man->go();
		if (m_chess->checkOver()) {
			break;
		}

		//��AI����
		m_ai->go();
		if (m_chess->checkOver()) {
			break;
		}
	}
}
