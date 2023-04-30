#pragma once
#include <graphics.h>
#include <conio.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <winuser.h>
using namespace std;

enum mouse_click_t {
	WRONG = 0,   //无效点击
	START = 1,   //开始
	QUIT = 2,    //退出
	MENU = 3,    //开始菜单
	AGAIN = 4	 //再试一次
};

//贴图坐标信息
struct Coord {
	int x; 
	int y;
	int width;
	int height;
};

class Control
{
public:

	//构造函数
	Control(ExMessage msg) : m_msg(msg), m_menu(false), m_goon(false), m_over(false) { };
	Control() { };

	//更新鼠标消息
	void updateMessage();

	//鼠标点击确认范围，返回true,否则返回false
	bool isClick(const Coord& coord);

	//获取点击效果
	int getclickmsg();

	ExMessage m_msg; //鼠标信息
	bool m_menu;	 //玩家希望返回主菜单
	bool m_goon;	 //玩家希望继续游戏
	bool m_over;	 //玩家希望结束游戏

	void clearinfo(); //重置控制信息

};

