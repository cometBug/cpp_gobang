#include "Control.h"

void Control::updateMessage()
{
    m_msg = getmessage(EX_MOUSE);
}

bool Control::isClick(const Coord& coord)
{
    //确定点击区域
    if (m_msg.message == WM_LBUTTONDOWN) {
        if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
            m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
            return true;
    }
    return false;
}

int Control::getclickmsg()
{
    //按钮坐标信息
    Coord start_bt = { 400, 400, 132, 48 }; //开始游戏
    Coord quit_bt = { 570, 400, 132, 48 }; //退出游戏
    Coord again_bt = { 470, 450, 156, 76 }; //再试一次
    Coord menu_bt = { 280, 450, 156, 76 }; //开始菜单

    //确定点击区域    
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
