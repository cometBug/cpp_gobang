#pragma once
#include "Control.h"
#include "AI.h"


class Man
{
public:
	Man(AI* ai, Control* control) :m_ai(ai), m_control(control) {};
	void init(Control* control);
	void go(enum chess_kind_t kind);	//kindÂä×Ó
private:	
	AI* m_ai;
	Control* m_control;
};

