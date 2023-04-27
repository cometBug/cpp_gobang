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
	CHESS_WHITE = -1,  //白棋
	CHESS_BLACK = 1    //黑棋
};

struct ChessPos {
	int row;
	int col;
	enum chess_kind_t kind;
	ChessPos(int r = 0, int c = 0, enum chess_kind_t k = CHESS_BLACK) :row(r), col(c), kind(k) {}
};

enum game_mod_t {
	MAN_to_AI = 1,	 //人->机
	AI_to_MAN = 2,   //机->人
	MAN_to_MAN = 3,	 //人->人
};

enum game_process_t {
	GAME_NOTICE = 0, //公告，信息
	START_MENU = 1,	 //开始
	MOD_MENU = 2,	 //棋盘选择
	PLAY_GAME = 3,   //游戏
	GAME_OVER = 4,	 //结算
};

enum mouse_click_t {
	WRONG = 0,		//无效点击	
	QUIT = 2,		//退出
	MENU = 3,		//开始菜单
	AGAIN = 4,		//再试一次
	MAN2AI = 8,		//人机对弈
	MAN2MAN = 9,    //好友对弈
	MAN2NET = 10,	//网络对弈
	ABOUTGAME = 11, //关于游戏
	//READ = 12,      //已读消息

	MOD_13 = 13,	//13线棋盘
	MOD_15 = 15,	//15线棋盘
	MOD_17 = 17,	//17线棋盘
	MOD_19 = 19,	//19线棋盘

	RESTART = 20,	//重新开始
	REGRET = 21,	//悔棋
	TIPS = 22,		//提示
	EXCHANGE = 23	//黑白交换
};

//贴图坐标信息
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
	//构造函数
	//Control(ExMessage msg) : m_msg(msg), m_menu(false), m_goon(false), m_quit(false) { };
	Control();

	//加载PNG透明图片
	void putimagePNG(int x, int y, IMAGE* picture);	
	
	//棋盘初始化
	void init();
	
	//开始界面渲染
	void drawMainMenu();
	//模式选择界面渲染
	void drawModMenu();
	//模式选择
	void selectMod();
	//游戏界面渲染
	void drawGameInterface();
	//初始化游戏界面坐标
	void initGameButton();
	//结束界面
	void drawGameOver(int m_kind, enum chess_kind_t win_kind);
	//网络对弈（暂未开放）
	void drawGameNotice();
	//关于游戏
	void drawGameInfo();
	void readInfo();

	//更新鼠标消息
	void updateMessage();
	//获取上次落子位置
	ChessPos m_pos;
	ChessPos chesspos();
	//鼠标点击确认范围，返回true,否则返回false
	bool isClick(const Coord& coord);
	//判断鼠标点击区域，在棋盘区返回true，在功能区返回false	
	bool clickArea(const ExMessage& msg);
	//判断落子是否合法，如果点击合法就将点击位置（行，列）保存在pos中
	bool clickBoard(int row, int col, ChessPos* pos);	
	
	//获取指定位置是黑棋，还是白棋，或者空白
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);
	//在棋盘指定的位置（行，列）落子
	void chessDown(ChessPos* pos, enum chess_kind_t kind);
	void chessDown(ChessPos pos, enum chess_kind_t kind);
	void renewChess(ChessPos pos);
	void updateChessMap(ChessPos* pos, enum chess_kind_t kind);
	void updateChessMap(ChessPos pos, enum chess_kind_t kind);
	
	//悔棋
	void retractChess();
	//刷新棋盘
	void renewBoard();
	//刷新文字提示区
	void initTipsArea();
	void getTips(ChessPos pos, enum chess_kind_t kind);
	
	//返回点击的按钮
	int getclickmsg();
	//重置控制信息
	void clearinfo(); 

	//如果胜负已分，返回true，否则返回false
	bool isWin(enum chess_kind_t kind);

public:
	game_mod_t GameMod;	//游戏类型
	game_process_t Gameproc;//游戏进程

	ExMessage m_msg;	//鼠标信息
	bool m_menu;		//玩家希望返回主菜单
	bool m_goon;		//玩家希望继续游戏
	bool m_quit;		//玩家希望结束游戏
	bool m_restart;		//玩家希望重新开始
	bool m_exchange;	//玩家希望交换黑白
	bool m_regret;		//玩家希望悔棋

	int ChessSize;
	int BoardSize;
	//设置棋盘大小：13线，15线，17线，19线
	void setBoardSize(const int size);
	//获取棋盘大小：13线，15线，17线，19线
	int getBoardSize();	

	//chess_kind_t man_chess;
	//步数
	int step;
	//棋谱记录//chessManual[1]表示棋盘的第2步落在ChessPos.row,ChessPos.col的位置，棋色是ChessPos.kind
	vector<ChessPos> ChessManual;
	//储存当前棋子分布//chessMap[3][5]表示棋盘的第3行第5列的落子情况：0-空白，-1-白棋，1-黑棋
	vector<vector<int>> ChessMap;

private:
	//贴图资源
	IMAGE ChessBlackImg;	//生成黑棋棋子
	IMAGE ChessWhiteImg;	//生成白棋棋子
	IMAGE bg_menu, bg_victory, bg_defeat;
	IMAGE bt_man2ai, bt_man2man, bt_man2net,
		bt_about, bt_quit, bt_again, bt_menu;
	//开始菜单
	Coord bt_man2ai_menu;	//人机对弈
	Coord bt_man2man_menu;	//好友对弈
	Coord bt_man2net_menu;	//网络对弈
	Coord bt_about_menu;	//关于游戏
	Coord bt_quit_menu;		//退出游戏
	//结算界面
	Coord bt_again_result;	//再试一次
	Coord bt_menu_result;	//开始菜单
	Coord bt_quit_result;	//退出游戏
	//模式选择界面
	Coord bt_13_mod; //13线
	Coord bt_15_mod; //15线
	Coord bt_17_mod; //17线
	Coord bt_19_mod; //19线	
	//游戏界面
	Coord bt_restart_play;	 //重新开始	
	Coord bt_regret_play;	 //悔棋
	Coord bt_tips_play;		 //提示
	Coord bt_exchange_play;	 //黑白交换
	Coord bt_menu_play;		 //开始菜单
	Coord bt_quit_play;		 //退出游戏
	//公告界面
	Coord bt_menu_notice;	 //回主菜单

};
