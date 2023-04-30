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
	//创建开始菜单窗口
	initgraph(1104, 621);

	//重绘窗口，使窗口居中
	HWND hWnd = GetHWnd();
	SetWindowPos(hWnd, nullptr, 300, 150, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	//SetWindowText(hWnd, _T("五子棋游戏"));

	//显示开始菜单图片
	loadimage(0, _T("res/menu.jpg"));
	
	//加载开始按钮
	IMAGE start_bt;
	loadimage(&start_bt, _T("res/start_button.png"), 794/6, 350/6, true);
	putimagePNG(400, 400, &start_bt);

	//加载退出按钮
	IMAGE quit_bt;
	loadimage(&quit_bt, _T("res/quit_button.png"), 794 / 6, 350 / 6, true);
	putimagePNG(570, 400, &quit_bt);

	//暂停显示图片
	//system("pause");
	
}

//对局（开始五子棋游戏）
void ChessGame::play()
{
	m_control->clearinfo(); //控制信息初始化
	m_chess->init();	    //棋盘初始化
	while (1) {
		//先由棋手走棋
		m_man->go();
		if (m_chess->checkOver()) {
			break;
		}

		//由AI走棋
		m_ai->go();
		if (m_chess->checkOver()) {
			break;
		}
	}
}
