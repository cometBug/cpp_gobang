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

	Sleep(1000);	//��װ˼��

	//����
	chess->chessDown(&pos, CHESS_WHITE);
}

#define LIVE5	50	//��5
#define LIVE4	40	//��4
#define DEAD4_1 39	//��4-1
#define DEAD4_2 10	//��4-2
#define LIVE3	30	//��3
#define DEAD3_1 29	//��3-1
#define DEAD3_2 9	//��3-2
#define LIVE2	20	//��2
#define DEAD2_1 19	//��2-1
#define DEAD2_2 8	//��2-2

//��������
// person=1��ʾ���壬person=-1��ʾ���壬person=0��ʾ�հ�
int AI::calculateForm(const vector<int>& chessRange, const chess_kind_t kind)
{
	int form = 0;
	for (int ii = 0; ii < 5; ii++)
	{
		// �����ж�
		if (chessRange[ii] == kind && chessRange[ii + 1] == kind &&
			chessRange[ii + 2] == kind && chessRange[ii + 3] == kind &&
			chessRange[ii + 4] == kind)
		{
			form = LIVE5; // ��5
		}
		else if (chessRange[ii] == kind && chessRange[ii + 1] == kind &&
			chessRange[ii + 2] == kind && chessRange[ii + 3] == kind)
		{
			if (ii == 0) {
				if (chessRange[ii + 4] == 0)
					form = form > DEAD4_1 ? form : DEAD4_1; // ��4-1
				else
					form = form > DEAD4_2 ? form : DEAD4_2; // ��4-2
			}
			else {
				if (chessRange[ii - 1] == 0) {
					if (chessRange[ii + 4] == 0)
						form = form > LIVE4 ? form : LIVE4; // ��4
					else
						form = form > DEAD4_1 ? form : DEAD4_1; // ��4-1
				}
				else {
					if (chessRange[ii + 4] == 0)
						form = form > DEAD4_1 ? form : DEAD4_1; // ��4-1
					else
						form = form > DEAD4_2 ? form : DEAD4_2; // ��4-2
				}
			}
		}
		else if (chessRange[ii] == kind && chessRange[ii + 1] == kind &&
			chessRange[ii + 2] == kind)
		{
			if (ii == 0) {
				if (chessRange[ii + 3] == 0)
					form = form > DEAD3_1 ? form : DEAD3_1; // ��3-1
				else
					form = form > DEAD3_2 ? form : DEAD3_2; // ��3-2
			}
			else {
				if (chessRange[ii - 1] == 0) {
					if (chessRange[ii + 3] == 0)
						form = form > LIVE3 ? form : LIVE3; // ��3
					else
						form = form > DEAD3_1 ? form : DEAD3_1; // ��3-1
				}
				else {
					if (chessRange[ii + 3] == 0)
						form = form > DEAD3_1 ? form : DEAD3_1; // ��3-1
					else
						form = form > DEAD3_2 ? form : DEAD3_2; // ��3-2
				}
			}
		}
		else if (chessRange[ii] == kind && chessRange[ii + 1] == kind) {
			if (ii == 0) {
				if (chessRange[ii + 2] == 0)
					form = form > DEAD2_1 ? form : DEAD2_1; // ��2-1
				else
					form = form > DEAD2_2 ? form : DEAD2_2; // ��2-2
			}
			else {
				if (chessRange[ii - 1] == 0) {
					if (chessRange[ii + 2] == 0)
						form = form > LIVE2 ? form : LIVE2; // ��2
					else
						form = form > DEAD2_1 ? form : DEAD2_1; // ��2-1
				}
				else {
					if (chessRange[ii + 2] == 0)
						form = form > DEAD2_1 ? form : DEAD2_1; // ��2-1
					else
						form = form > DEAD2_2 ? form : DEAD2_2; // ��2-2
				}
			}
		}
	}
	return form;
}

//���ּ���
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

//�����ĸ������ܷ�
int AI::GetTotalScore(const int row, const int col, const chess_kind_t kind)
{
	vector<int> chessrange;
	int form = 0;
	int score = 0;
	int size = chess->getBoardSize();

	//�ĸ�����
	//��->��
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
	form = calculateForm(chessrange, kind);	//��ʱ����
	score += GetScore(form, kind);
	chessrange.clear();

	//����->����
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

	//��->��
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

	//����->����
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
	//������������
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			scoreMap[i][j] = 0;
		}
	}
	
	//��������ͼ
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			//��ÿ����������ּ���
			if (chess->getChessData(row, col))
				scoreMap[row][col] = 0;
			else {
				//����+����÷�=�ܷ�
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

	srand((unsigned)time(NULL));//�趨��������ӣ��õ�ǰʱ����Ϊ����
	int index = rand() % maxPoints.size();
	return maxPoints[index];
	
}
