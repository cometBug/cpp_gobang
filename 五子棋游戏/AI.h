#pragma once
#include "Control.h"

class Man;
class AI
{	
	friend Man;
public:	
	AI(Control* control) :m_control(control) {};
	void init(Control* control);
	void go(enum chess_kind_t kind);

private:
	Control* m_control;
	vector<vector<int>> scoreMap;

private:
	int GetScore(const int form, enum chess_kind_t my_kind, enum chess_kind_t score_kind);
	int calculateForm(const vector<int>& chessRange, enum chess_kind_t kind);
	int GetTotalScore(const int row, const int col, enum chess_kind_t my_kind, enum chess_kind_t score_kind);
	void updateScoreMap(enum chess_kind_t kind);
	ChessPos think(enum chess_kind_t kind);
};

