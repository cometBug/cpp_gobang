#include <iostream>
#include "ChessGame.h"


int main()
{			
	Control control;
	AI ai;
	Man man(&control);
	Chess chess(&control, 13, 45, 45, 45);
	ChessGame game(&man, &ai, &chess, &control);

	int msg = 0;
	while(1)
	{
	MENU://��ʼ�˵�
		game.StartMenu();
		while(1)
		{						
			control.updateMessage();
			msg = control.getclickmsg();

			//����˳���Ϸ��ť
			if (msg == QUIT)
				return 0;

			//�����ʼ��Ϸ��ť
			else if (msg == START) {
			PLAY:
				game.play();
				if (control.m_menu == true) {
					goto MENU;
				}
				else if (control.m_goon == true) {
					goto PLAY;
				}
			}
		}
	}
	return 0;
}