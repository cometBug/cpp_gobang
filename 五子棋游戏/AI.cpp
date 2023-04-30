#include "AI.h"

void AI::init(Chess* chess)
{
	this->chess = chess;

	int size = chess->getBoardSize();
	for (int i = 0; i < size; i++) {
		vector<int> row;
		for (int j = 0; j < size; j++) {
			row.emplace_back(0);
		}
		scoreMap.emplace_back(row);
	}
}

void AI::go()
{
	ChessPos pos = think();

	Sleep(1000);	//假装思考

	//落子
	chess->chessDown(&pos, CHESS_WHITE);
}

#define LIVE5	50	//连5
#define LIVE4	40	//活4
#define DEAD4_1 39	//死4-1
#define DEAD4_2 10	//死4-2
#define LIVE3	30	//活3
#define DEAD3_1 29	//死3-1
#define DEAD3_2 9	//死3-2
#define LIVE2	20	//连2
#define DEAD2_1 19	//死2-1
#define DEAD2_2 8	//死2-2

//计算棋形
// person=1表示黑棋，person=-1表示白棋，person=0表示空白
int AI::calculateForm(const vector<int>& chessRange, const chess_kind_t kind)
{
	int form = 0;
	for (int ii = 0; ii < 5; ii++)
	{
		// 棋形判断
		if (chessRange[ii] == kind && chessRange[ii + 1] == kind &&
			chessRange[ii + 2] == kind && chessRange[ii + 3] == kind &&
			chessRange[ii + 4] == kind)
		{
			form = LIVE5; // 连5
		}
		else if (chessRange[ii] == kind && chessRange[ii + 1] == kind &&
			chessRange[ii + 2] == kind && chessRange[ii + 3] == kind)
		{
			if (ii == 0) {
				if (chessRange[ii + 4] == 0)
					form = form > DEAD4_1 ? form : DEAD4_1; // 死4-1
				else
					form = form > DEAD4_2 ? form : DEAD4_2; // 死4-2
			}
			else {
				if (chessRange[ii - 1] == 0) {
					if (chessRange[ii + 4] == 0)
						form = form > LIVE4 ? form : LIVE4; // 活4
					else
						form = form > DEAD4_1 ? form : DEAD4_1; // 死4-1
				}
				else {
					if (chessRange[ii + 4] == 0)
						form = form > DEAD4_1 ? form : DEAD4_1; // 死4-1
					else
						form = form > DEAD4_2 ? form : DEAD4_2; // 死4-2
				}
			}
		}
		else if (chessRange[ii] == kind && chessRange[ii + 1] == kind &&
			chessRange[ii + 2] == kind)
		{
			if (ii == 0) {
				if (chessRange[ii + 3] == 0)
					form = form > DEAD3_1 ? form : DEAD3_1; // 死3-1
				else
					form = form > DEAD3_2 ? form : DEAD3_2; // 死3-2
			}
			else {
				if (chessRange[ii - 1] == 0) {
					if (chessRange[ii + 3] == 0)
						form = form > LIVE3 ? form : LIVE3; // 活3
					else
						form = form > DEAD3_1 ? form : DEAD3_1; // 死3-1
				}
				else {
					if (chessRange[ii + 3] == 0)
						form = form > DEAD3_1 ? form : DEAD3_1; // 死3-1
					else
						form = form > DEAD3_2 ? form : DEAD3_2; // 死3-2
				}
			}
		}
		else if (chessRange[ii] == kind && chessRange[ii + 1] == kind) {
			if (ii == 0) {
				if (chessRange[ii + 2] == 0)
					form = form > DEAD2_1 ? form : DEAD2_1; // 死2-1
				else
					form = form > DEAD2_2 ? form : DEAD2_2; // 死2-2
			}
			else {
				if (chessRange[ii - 1] == 0) {
					if (chessRange[ii + 2] == 0)
						form = form > LIVE2 ? form : LIVE2; // 活2
					else
						form = form > DEAD2_1 ? form : DEAD2_1; // 死2-1
				}
				else {
					if (chessRange[ii + 2] == 0)
						form = form > DEAD2_1 ? form : DEAD2_1; // 死2-1
					else
						form = form > DEAD2_2 ? form : DEAD2_2; // 死2-2
				}
			}
		}
	}
	return form;
}

//评分计算
int AI::GetScore(const int form, const chess_kind_t kind)
{
	if (kind == CHESS_WHITE)
	{
		switch (form)
		{
		case LIVE5:		return 30000;
		case LIVE4:		return 10000;
		case DEAD4_1:	return 55;
		case LIVE3:		return 50;
		case DEAD3_1:	return 25;
		case LIVE2:		return 10;
		case DEAD4_2:	return 3;
		case DEAD3_2:	return 2;
		case DEAD2_2:	return 1;
		default:		return 0;
		}
	}		
	else if (kind == CHESS_BLACK)
	{
		switch (form)
		{
		case LIVE5:		return 20000;
		case LIVE4:		return 200;
		case DEAD4_1:	return 60;
		case LIVE3:		return 40;
		case DEAD3_1:	return 30;
		case LIVE2:		return 10;
		case DEAD4_2:	return 3;
		case DEAD3_2:	return 2;
		case DEAD2_2:	return 1;
		default:		return 0;
		}
	}
	return 0;
}

//计算四个方向总分
int AI::GetTotalScore(const int row, const int col, const chess_kind_t kind)
{
	vector<int> chessrange;
	int form = 0;
	int score = 0;
	int size = chess->getBoardSize();

	//四个方向
	//左->右
	int cur_row = row;
	int cur_col = col - 4;
	for (int ii = 0; ii < 9; ii++) {
		if (cur_row <  0    || cur_col <  0 ||
			cur_row >= size || cur_col >= size) {
			chessrange.push_back(2);
		}
		else if (cur_row == row && cur_col == col) {
			chessrange.push_back(kind);
		}
		else {
			chessrange.push_back(chess->getChessData(cur_row, cur_col));
		}
		cur_col++;
	}
	form = calculateForm(chessrange, kind);	//此时棋型
	score += GetScore(form, kind);
	chessrange.clear();

	//左上->右下
	cur_row = row - 4;
	cur_col = col - 4;
	for (int ii = 0; ii < 9; ii++) {
		if (cur_row < 0 || cur_col < 0 ||
			cur_row >= size || cur_col >= size) {
			chessrange.push_back(2);
		}
		else if (cur_row == row && cur_col == col) {
			chessrange.push_back(kind);
		}
		else {
			chessrange.push_back(chess->getChessData(cur_row, cur_col));
		}
		cur_row++;
		cur_col++;
	}
	form = calculateForm(chessrange, kind);
	score += GetScore(form, kind);
	chessrange.clear();

	//上->下
	cur_row = row - 4;
	cur_col = col;

	for (int ii = 0; ii < 9; ii++) {
		if (cur_row < 0 || cur_col < 0 ||
			cur_row >= size || cur_col >= size) {
			chessrange.push_back(2);
		}
		else if (cur_row == row && cur_col == col) {
			chessrange.push_back(kind);
		}
		else {
			chessrange.push_back(chess->getChessData(cur_row, cur_col));
		}
		cur_row++;
	}
	form = calculateForm(chessrange, kind);
	score += GetScore(form, kind);
	chessrange.clear();

	//右上->左下
	cur_row = row - 4;
	cur_col = col + 4;

	for (int ii = 0; ii < 9; ii++) {
		if (cur_row < 0 || cur_col < 0 ||
			cur_row >= size || cur_col >= size) {
			chessrange.push_back(2);
		}
		else if (cur_row == row && cur_col == col) {
			chessrange.push_back(kind);
		}
		else {
			chessrange.push_back(chess->getChessData(cur_row, cur_col));
		}
		cur_row++;
		cur_col--;
	}
	form = calculateForm(chessrange, kind);
	score += GetScore(form, kind);
	chessrange.clear();

	return score;
}

void AI::updateScoreMap()
{
	int score = 0;
	int size = chess->getBoardSize();
	//评分容器置零
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			scoreMap[i][j] = 0;
		}
	}
	
	//生成评分图
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			//对每个点进行评分计算
			if (chess->getChessData(row, col))
				scoreMap[row][col] = 0;
			else {
				//黑棋+白棋得分=总分
				score = GetTotalScore(row, col, CHESS_BLACK) +
						GetTotalScore(row, col, CHESS_WHITE);
				scoreMap[row][col] = score;
			}
		}
	}
}

ChessPos AI::think()
{
	updateScoreMap();

	vector<ChessPos>  maxPoints;
	int maxScore = 0;
	int size = chess->getBoardSize();
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			if (chess->getChessData(row, col) == 0) {
				if (scoreMap[row][col] > maxScore) {
					maxScore = scoreMap[row][col];
					maxPoints.clear();
					maxPoints.push_back(ChessPos(row, col));
				}
				else if (scoreMap[row][col] == maxScore) {
					maxPoints.push_back(ChessPos(row, col));
				}
			}
		}
	}

	srand((unsigned)time(NULL));//设定随机数种子，用当前时间作为种子
	int index = rand() % maxPoints.size();
	return maxPoints[index];
	
}
