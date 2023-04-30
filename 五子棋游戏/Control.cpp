#include "Control.h"

void Control::updateMessage()
{
    m_msg = getmessage(EX_MOUSE);
}

bool Control::isClick(const Coord& coord)
{
    //ȷ���������
    if (m_msg.message == WM_LBUTTONDOWN) {
        if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
            m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
            return true;
    }
    return false;
}

int Control::getclickmsg()
{
    //��ť������Ϣ
    Coord start_bt = { 400, 400, 132, 48 }; //��ʼ��Ϸ
    Coord quit_bt = { 570, 400, 132, 48 }; //�˳���Ϸ
    Coord again_bt = { 470, 450, 156, 76 }; //����һ��
    Coord menu_bt = { 280, 450, 156, 76 }; //��ʼ�˵�

    //ȷ���������    
    if (isClick(start_bt))
        return START;
    else if (isClick(quit_bt))
        return QUIT;
    else if (isClick(again_bt))
        return AGAIN;
    else if (isClick(menu_bt))
        return MENU;
    return WRONG;
}

void Control::clearinfo()
{
    m_menu = false;
    m_goon = false;
    m_over = false;
}
