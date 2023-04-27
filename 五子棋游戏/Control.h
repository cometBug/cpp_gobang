#pragma once
#include <graphics.h>
#include <conio.h>
#include <vector>
#include <map>
#include <iostream>
#include <math.h>
#include <winuser.h>
#include <mmsystem.h>
using namespace std;
#pragma comment(lib,"winmm.lib")


#define BT_WIDTH 146
#define BT_HEIGHT 56

enum chess_kind_t {
	CHESS_WHITE = -1,  //����
	CHESS_BLACK = 1    //����
};

struct ChessPos {
	int row;
	int col;
	enum chess_kind_t kind;
	ChessPos(int r = 0, int c = 0, enum chess_kind_t k = CHESS_BLACK) :row(r), col(c), kind(k) {}
};

enum game_mod_t {
	MAN_to_AI = 1,	 //��->��
	AI_to_MAN = 2,   //��->��
	MAN_to_MAN = 3,	 //��->��
};

enum game_process_t {
	GAME_NOTICE = 0, //���棬��Ϣ
	START_MENU = 1,	 //��ʼ
	MOD_MENU = 2,	 //����ѡ��
	PLAY_GAME = 3,   //��Ϸ
	GAME_OVER = 4,	 //����
};

enum mouse_click_t {
	WRONG = 0,		//��Ч���	
	QUIT = 2,		//�˳�
	MENU = 3,		//��ʼ�˵�
	AGAIN = 4,		//����һ��
	MAN2AI = 8,		//�˻�����
	MAN2MAN = 9,    //���Ѷ���
	MAN2NET = 10,	//�������
	ABOUTGAME = 11, //������Ϸ
	//READ = 12,      //�Ѷ���Ϣ

	MOD_13 = 13,	//13������
	MOD_15 = 15,	//15������
	MOD_17 = 17,	//17������
	MOD_19 = 19,	//19������

	RESTART = 20,	//���¿�ʼ
	REGRET = 21,	//����
	TIPS = 22,		//��ʾ
	EXCHANGE = 23	//�ڰ׽���
};

//��ͼ������Ϣ
struct Coord {
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
};

class Control
{	
	friend class ChessGame;
public:
	//���캯��
	//Control(ExMessage msg) : m_msg(msg), m_menu(false), m_goon(false), m_quit(false) { };
	Control();

	//����PNG͸��ͼƬ
	void putimagePNG(int x, int y, IMAGE* picture);	
	
	//���̳�ʼ��
	void init();
	
	//��ʼ������Ⱦ
	void drawMainMenu();
	//ģʽѡ�������Ⱦ
	void drawModMenu();
	//ģʽѡ��
	void selectMod();
	//��Ϸ������Ⱦ
	void drawGameInterface();
	//��ʼ����Ϸ��������
	void initGameButton();
	//��������
	void drawGameOver(int m_kind, enum chess_kind_t win_kind);
	//������ģ���δ���ţ�
	void drawGameNotice();
	//������Ϸ
	void drawGameInfo();
	void readInfo();

	//���������Ϣ
	void updateMessage();
	//��ȡ�ϴ�����λ��
	ChessPos m_pos;
	ChessPos chesspos();
	//�����ȷ�Ϸ�Χ������true,���򷵻�false
	bool isClick(const Coord& coord);
	//�ж����������������������true���ڹ���������false	
	bool clickArea(const ExMessage& msg);
	//�ж������Ƿ�Ϸ����������Ϸ��ͽ����λ�ã��У��У�������pos��
	bool clickBoard(int row, int col, ChessPos* pos);	
	
	//��ȡָ��λ���Ǻ��壬���ǰ��壬���߿հ�
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);
	//������ָ����λ�ã��У��У�����
	void chessDown(ChessPos* pos, enum chess_kind_t kind);
	void chessDown(ChessPos pos, enum chess_kind_t kind);
	void renewChess(ChessPos pos);
	void updateChessMap(ChessPos* pos, enum chess_kind_t kind);
	void updateChessMap(ChessPos pos, enum chess_kind_t kind);
	
	//����
	void retractChess();
	//ˢ������
	void renewBoard();
	//ˢ��������ʾ��
	void initTipsArea();
	void getTips(ChessPos pos, enum chess_kind_t kind);
	
	//���ص���İ�ť
	int getclickmsg();
	//���ÿ�����Ϣ
	void clearinfo(); 

	//���ʤ���ѷ֣�����true�����򷵻�false
	bool isWin(enum chess_kind_t kind);

public:
	game_mod_t GameMod;	//��Ϸ����
	game_process_t Gameproc;//��Ϸ����

	ExMessage m_msg;	//�����Ϣ
	bool m_menu;		//���ϣ���������˵�
	bool m_goon;		//���ϣ��������Ϸ
	bool m_quit;		//���ϣ��������Ϸ
	bool m_restart;		//���ϣ�����¿�ʼ
	bool m_exchange;	//���ϣ�������ڰ�
	bool m_regret;		//���ϣ������

	int ChessSize;
	int BoardSize;
	//�������̴�С��13�ߣ�15�ߣ�17�ߣ�19��
	void setBoardSize(const int size);
	//��ȡ���̴�С��13�ߣ�15�ߣ�17�ߣ�19��
	int getBoardSize();	

	//chess_kind_t man_chess;
	//����
	int step;
	//���׼�¼//chessManual[1]��ʾ���̵ĵ�2������ChessPos.row,ChessPos.col��λ�ã���ɫ��ChessPos.kind
	vector<ChessPos> ChessManual;
	//���浱ǰ���ӷֲ�//chessMap[3][5]��ʾ���̵ĵ�3�е�5�е����������0-�հף�-1-���壬1-����
	vector<vector<int>> ChessMap;

private:
	//��ͼ��Դ
	IMAGE ChessBlackImg;	//���ɺ�������
	IMAGE ChessWhiteImg;	//���ɰ�������
	IMAGE bg_menu, bg_victory, bg_defeat;
	IMAGE bt_man2ai, bt_man2man, bt_man2net,
		bt_about, bt_quit, bt_again, bt_menu;
	//��ʼ�˵�
	Coord bt_man2ai_menu;	//�˻�����
	Coord bt_man2man_menu;	//���Ѷ���
	Coord bt_man2net_menu;	//�������
	Coord bt_about_menu;	//������Ϸ
	Coord bt_quit_menu;		//�˳���Ϸ
	//�������
	Coord bt_again_result;	//����һ��
	Coord bt_menu_result;	//��ʼ�˵�
	Coord bt_quit_result;	//�˳���Ϸ
	//ģʽѡ�����
	Coord bt_13_mod; //13��
	Coord bt_15_mod; //15��
	Coord bt_17_mod; //17��
	Coord bt_19_mod; //19��	
	//��Ϸ����
	Coord bt_restart_play;	 //���¿�ʼ	
	Coord bt_regret_play;	 //����
	Coord bt_tips_play;		 //��ʾ
	Coord bt_exchange_play;	 //�ڰ׽���
	Coord bt_menu_play;		 //��ʼ�˵�
	Coord bt_quit_play;		 //�˳���Ϸ
	//�������
	Coord bt_menu_notice;	 //�����˵�

};
