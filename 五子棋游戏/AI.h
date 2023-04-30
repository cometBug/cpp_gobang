#pragma once
#include "Chess.h"

class AI
{
public:
	void init(Chess* chess);
	void go();

private:
	Chess* chess;
	vector<vector<int>> scoreMap;

private:
	int GetScore(const int from, const chess_kind_t kind);
	int calculateForm(const vector<int>& chessRange, const chess_kind_t kind);
	int GetTotalScore(const int row, const int col, const chess_kind_t kind);
	void updateScoreMap();
	ChessPos think();
};

