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

	void play(Man* man,AI* ai);			//人->AI
	void play(AI* ai, Man* man);		//AI->人
	void play(Man* man, Man* man1);		//人->人

	//检查棋局是否结束
	bool checkOver(enum chess_kind_t kind, int m_kind);
	
//数据成员
private:	
	AI* m_ai;
	Man* m_man;
	Man* m_man1;	
	Control* m_control;
};

