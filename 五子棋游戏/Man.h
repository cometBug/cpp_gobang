#pragma once
#include "Chess.h"


class Man
{
public:
	Man(Control* control) :m_control(control) {};
	void init(Chess* chess);
	void go();
private:
	Chess* m_chess;
	Control* m_control;
};

