#include "ChessGame.h"

int main()
{	
	Control control;
	AI ai(&control);
	Man man(&ai, &control);
	Man man1(&ai, &control);
	ChessGame game(&man, &man1, &ai, &control);

	int msg = 0;
	while (1)
	{
	MENU://��ʼ�˵�
		control.clearinfo();
		control.drawMainMenu();
		while(1)
		{
			control.updateMessage();
			msg = control.getclickmsg();

			//����˳���Ϸ��ť
			if (msg == QUIT)
				return 0;

			//����˻����İ�ť
			if (msg == MAN2AI)
			{
				{
					control.selectMod();
				PLAY_MAN2AI:
					control.clearinfo();
					game.play(&man, &ai);
					if (control.m_menu == true)
						goto MENU;
					else if (control.m_goon == true)
						goto PLAY_MAN2AI;
					else if (control.m_exchange == true)
						goto PLAY_AI2MAN;
					else if (control.m_quit == true)
						return 0;
				}
				{
				PLAY_AI2MAN:
					control.clearinfo();
					game.play(&ai, &man);
					if (control.m_menu == true)
						goto MENU;
					else if (control.m_goon == true)
						goto PLAY_AI2MAN;
					else if (control.m_exchange == true)
						goto PLAY_MAN2AI;
					else if (control.m_quit == true)
						return 0;
				}
			}
			//������Ѷ��İ�ť
			if (msg == MAN2MAN)
			{
				control.selectMod();
			PLAY_MAN2MAN:
				control.clearinfo();
				game.play(&man, &man1);
				if (control.m_menu == true)
					goto MENU;
				else if (control.m_goon == true)
					goto PLAY_MAN2MAN;
				else if (control.m_quit == true)
					return 0;
			}
			//���������İ�ť
			if (msg == MAN2NET)
			{
				//��ʾ��δ����
				control.clearinfo();
				control.drawGameNotice();
				control.readInfo();
				if (control.m_menu == true)
					goto MENU;
			}
			//���������Ϸ��ť
			if (msg == ABOUTGAME)
			{
				control.clearinfo();
				control.drawGameInfo();
				control.readInfo();
				if (control.m_menu == true)
					goto MENU;
			}
		}
	}	
	return 0;
}