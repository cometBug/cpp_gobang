#include "AI.h"

void AI::init(Control* control)
{
	m_control = control;

	int size = control->getBoardSize();
	scoreMap.clear();
	for (int i = 0; i < size; i++) {
		vector<int> row;
		for (int j = 0; j < size; j++) {
			row.emplace_back(0);
		}
		scoreMap.emplace_back(row);
	}
}

void AI::go(enum chess_kind_t kind)
{	
	ChessPos pos = think(kind);

	Sleep(1000);	//假装思考

	//落子
	m_control->chessDown(&pos, kind);
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
int AI::calculateForm(const vector<int>& chessRange, enum chess_kind_t kind)
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
int AI::GetScore(const int form, enum chess_kind_t my_kind, enum chess_kind_t score_kind)
{
	if (my_kind == score_kind)
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
	else
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
int AI::GetTotalScore(const int row, const int col, enum chess_kind_t my_kind, enum chess_kind_t score_kind)
{
	vector<int> chessrange;
	int form = 0;
	int size = m_control->getBoardSize();
	int score = 0;

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
			chessrange.push_back(score_kind);
		}
		else {
			chessrange.push_back(m_control->getChessData(cur_row, cur_col));
		}
		cur_col++;
	}
	form = calculateForm(chessrange, score_kind);	//此时棋型
	score += GetScore(form, my_kind, score_kind);
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
			chessrange.push_back(score_kind);
		}
		else {
			chessrange.push_back(m_control->getChessData(cur_row, cur_col));
		}
		cur_row++;
		cur_col++;
	}
	form = calculateForm(chessrange, score_kind);	//此时棋型
	score += GetScore(form, my_kind, score_kind);
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
			chessrange.push_back(score_kind);
		}
		else {
			chessrange.push_back(m_control->getChessData(cur_row, cur_col));
		}
		cur_row++;
	}
	form = calculateForm(chessrange, score_kind);	//此时棋型
	score += GetScore(form, my_kind, score_kind);
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
			chessrange.push_back(score_kind);
		}
		else {
			chessrange.push_back(m_control->getChessData(cur_row, cur_col));
		}
		cur_row++;
		cur_col--;
	}
	form = calculateForm(chessrange, score_kind);	//此时棋型
	score += GetScore(form, my_kind, score_kind);
	chessrange.clear();

	return score;
}

//更新评分图
void AI::updateScoreMap(enum chess_kind_t kind)
{
	int score = 0;
	int size = m_control->getBoardSize();
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
			if (m_control->getChessData(row, col))
				scoreMap[row][col] = 0;
			else {
				//得到总分
				score = GetTotalScore(row, col, kind, kind) +
						GetTotalScore(row, col, kind, chess_kind_t(-kind));
				scoreMap[row][col] = score;
			}
		}
	}
}

//kind为AI的棋色
ChessPos AI::think(enum chess_kind_t kind)
{	
	vector<ChessPos>  maxPoints;
	int maxScore = 0;
	int size = m_control->getBoardSize();
	//先手走天元
	int num = 0;
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			if (m_control->getChessData(row, col) == 0) {
				num++;
			}
		}
	}
	if (num == size * size) {
		int tianyuan = static_cast<int>((size - 1) * 0.5);
		return { tianyuan,tianyuan };
	}
	//其他看评分
	updateScoreMap(kind);
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			if (m_control->getChessData(row, col) == 0) {
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
