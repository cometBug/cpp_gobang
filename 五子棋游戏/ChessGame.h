#pragma once
#include "AI.h"
#include "Man.h"
#include "Control.h"


class ChessGame
{
public:
	ChessGame(Man* man, Man* man1, AI* ai, Control* control);
	//ChessGame() {};

	void init();

	void play(Man* man,AI* ai);			//��->AI
	void play(AI* ai, Man* man);		//AI->��
	void play(Man* man, Man* man1);		//��->��

	//�������Ƿ����
	bool checkOver(enum chess_kind_t kind, int m_kind);
	
//���ݳ�Ա
private:	
	AI* m_ai;
	Man* m_man;
	Man* m_man1;	
	Control* m_control;
};

