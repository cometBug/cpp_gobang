#pragma once
#include "Man.h"
#include "AI.h"
#include "Chess.h"
#include "Control.h"

class ChessGame
{
public:
	ChessGame(Man* man, AI* ai, Chess* chess, Control* control);

	void StartMenu();	//��ʼ�˵�
	void play();	//��ʼ�Ծ�
	
//���ݳ�Ա
private:
	Man* m_man;
	AI* m_ai;
	Chess* m_chess;
	Control* m_control;
};

