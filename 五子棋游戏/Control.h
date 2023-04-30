#pragma once
#include <graphics.h>
#include <conio.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <winuser.h>
using namespace std;

enum mouse_click_t {
	WRONG = 0,   //��Ч���
	START = 1,   //��ʼ
	QUIT = 2,    //�˳�
	MENU = 3,    //��ʼ�˵�
	AGAIN = 4	 //����һ��
};

//��ͼ������Ϣ
struct Coord {
	int x; 
	int y;
	int width;
	int height;
};

class Control
{
public:

	//���캯��
	Control(ExMessage msg) : m_msg(msg), m_menu(false), m_goon(false), m_over(false) { };
	Control() { };

	//���������Ϣ
	void updateMessage();

	//�����ȷ�Ϸ�Χ������true,���򷵻�false
	bool isClick(const Coord& coord);

	//��ȡ���Ч��
	int getclickmsg();

	ExMessage m_msg; //�����Ϣ
	bool m_menu;	 //���ϣ���������˵�
	bool m_goon;	 //���ϣ��������Ϸ
	bool m_over;	 //���ϣ��������Ϸ

	void clearinfo(); //���ÿ�����Ϣ

};

