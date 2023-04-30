#pragma once
#include "Man.h"
#include "AI.h"
#include "Chess.h"
#include "Control.h"

class ChessGame
{
public:
	ChessGame(Man* man, AI* ai, Chess* chess, Control* control);

	void StartMenu();	//开始菜单
	void play();	//开始对局
	
//数据成员
private:
	Man* m_man;
	AI* m_ai;
	Chess* m_chess;
	Control* m_control;
};

