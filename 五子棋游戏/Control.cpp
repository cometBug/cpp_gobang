#include "Control.h"

Control::Control()
{
    BoardSize = 13;
    ChessSize = 45;
    step = 0;
    //加载棋子图片
    loadimage(&ChessBlackImg, _T("res/black.png"), ChessSize, ChessSize, true);
    loadimage(&ChessWhiteImg, _T("res/white.png"), ChessSize, ChessSize, true);
    //加载贴图
    loadimage(&bg_menu, _T("res/menu.jpg"), 0, 0, true);
    loadimage(&bg_victory, _T("res/胜利1.jpg"), 0, 0, true);
    loadimage(&bg_defeat, _T("res/失败1.jpg"), 0, 0, true);
    //加载按钮
    loadimage(&bt_man2ai, _T("res/button/人机对弈.png"), BT_WIDTH, BT_HEIGHT, true);
    loadimage(&bt_man2man, _T("res/button/好友对弈.png"), BT_WIDTH, BT_HEIGHT, true);
    loadimage(&bt_man2net, _T("res/button/网络对弈.png"), BT_WIDTH, BT_HEIGHT, true);
    loadimage(&bt_about, _T("res/button/关于游戏.png"), BT_WIDTH, BT_HEIGHT, true);
    loadimage(&bt_quit, _T("res/button/退出游戏.png"), BT_WIDTH, BT_HEIGHT, true);
    loadimage(&bt_again, _T("res/button/再试一次.png"), BT_WIDTH, BT_HEIGHT, true);
    loadimage(&bt_menu, _T("res/button/开始菜单.png"), BT_WIDTH, BT_HEIGHT, true);
    //按钮坐标信息
    //开始菜单
    bt_man2ai_menu = { 950, 140, BT_WIDTH, BT_HEIGHT };   //人机对弈
    bt_man2man_menu = { 950, 200, BT_WIDTH, BT_HEIGHT };  //好友对弈
    bt_man2net_menu = { 950, 260, BT_WIDTH, BT_HEIGHT };  //网络对弈
    bt_about_menu = { 950, 320, BT_WIDTH, BT_HEIGHT };    //关于游戏
    bt_quit_menu = { 950, 380, BT_WIDTH, BT_HEIGHT };     //退出游戏
    //结算界面
    bt_menu_result = { 175, 500, BT_WIDTH, BT_HEIGHT };   //开始菜单
    bt_again_result = { 385, 500, BT_WIDTH, BT_HEIGHT };  //再试一次
    bt_quit_result = { 595, 500, BT_WIDTH, BT_HEIGHT };   //退出游戏
    //模式选择界面
    bt_13_mod = { 175, 210, 200, 50 };   //13线
    bt_15_mod = { 725, 210, 200, 50 };   //15线
    bt_17_mod = { 175, 410, 200, 50 };   //17线
    bt_19_mod = { 725, 410, 200, 50 };   //19线
    //公告界面
    bt_menu_notice = { 725, 410, 200, 50 }; //回主菜单
    
    m_pos = { 0,0,CHESS_BLACK };

    //控制信息初始化
    m_menu = false;
    m_goon = false;
    m_quit = false;
    m_restart = false;
    m_exchange = false;
    m_regret = false;    
}

void Control::putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
    // 变量初始化
    DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
    DWORD* draw = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
    int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
    int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
    int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
    int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
    int dstX = 0;    //在显存里像素的角标

    // 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width; //在显存里像素的角标
            int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
            int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
            int sg = ((src[srcX] & 0xff00) >> 8);   //G
            int sb = src[srcX] & 0xff;              //B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
            {
                dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
                    | (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
            }
        }
    }
}

void Control::init()
{

    drawGameInterface();
    initGameButton();
    clearinfo();
    //棋盘初始化
    ChessMap.clear();
    for (int i = 0; i < BoardSize; i++) {
        vector<int> row;
        for (int j = 0; j < BoardSize; j++) {
            row.emplace_back(0);
        }
        ChessMap.emplace_back(row);
    }
    step = 0;//从第0步开始
    ChessManual.clear();
}

void Control::drawMainMenu()
{    
    Gameproc = START_MENU;

    //创建开始菜单窗口
    initgraph(bg_menu.getwidth(), bg_menu.getheight());

    //重绘窗口，使窗口居中
    HWND hWnd = GetHWnd();
    SetWindowPos(hWnd, nullptr, 300, 150, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    //SetWindowText(hWnd, _T("五子棋游戏"));   

    //显示开始菜单	
    putimage(0, 0, &bg_menu);
    putimagePNG(bt_man2ai_menu.x, bt_man2ai_menu.y, &bt_man2ai);	//人机对弈
    putimagePNG(bt_man2man_menu.x, bt_man2man_menu.y, &bt_man2man);	//好友对弈
    putimagePNG(bt_man2net_menu.x, bt_man2net_menu.y, &bt_man2net);	//网络对弈
    putimagePNG(bt_about_menu.x, bt_about_menu.y, &bt_about);		//关于游戏
    putimagePNG(bt_quit_menu.x, bt_quit_menu.y, &bt_quit);			//退出游戏

    //暂停显示图片
    //system("pause");
}

void Control::drawModMenu()
{
    Gameproc = MOD_MENU;

    int w = 1100;
    int h = 620;
    int edge = 50;
    initgraph(w, h);
    HWND hWnd = GetHWnd();
    SetWindowPos(hWnd, nullptr, 300, 150, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    //设置背景色为木色
    setbkcolor(RGB(222, 174, 110));
    //用背景色清空屏幕
    cleardevice();
    //设置文字背景色透明
    setbkmode(TRANSPARENT);

    //画框线，宽度为4个像素
    setlinecolor(RGB(167, 90, 36));//线条色
    setlinestyle(PS_SOLID, 4);
    rectangle(edge, edge, w - edge, h - edge);

    //打印标题
    settextcolor(RGB(255, 255, 255));
    settextstyle(50, 0, _T("微软雅黑"));

    TCHAR s[] = _T("请选择棋盘大小");
    outtextxy(w / 2 - 130, 80, s);

    //按钮绘制
    setfillcolor(RGB(53, 26, 7));//填充色
    //图形中心
    int o_x = w / 2;//550
    int o_y = h / 2;//310
    int setover = (o_x - 200) / 2;//175
    fillroundrect(o_x - setover - 200, o_y - 100, o_x - setover, o_y - 100 + 50, 20, 20);//175，210，375，260
    fillroundrect(o_x + setover, o_y - 100, o_x + setover + 200, o_y - 100 + 50, 20, 20);//725，210，925，260
    fillroundrect(o_x - setover - 200, o_y + 100, o_x - setover, o_y + 100 + 50, 20, 20);//175，410，375，460
    fillroundrect(o_x + setover, o_y + 100, o_x + setover + 200, o_y + 100 + 50, 20, 20);//725，410，925，460	

    ////文字偏距修正
    int text_seto_x = 60;
    int text_seto_y = -2;
    //填充文字
    TCHAR t0[] = _T("13线");
    TCHAR t1[] = _T("15线");
    TCHAR t2[] = _T("17线");
    TCHAR t3[] = _T("19线");

    outtextxy(o_x - setover - 200 + text_seto_x, o_y - 100 + text_seto_y, t0);
    outtextxy(o_x + setover + text_seto_x, o_y - 100 + text_seto_y, t1);
    outtextxy(o_x - setover - 200 + text_seto_x, o_y + 100 + text_seto_y, t2);
    outtextxy(o_x + setover + text_seto_x, o_y + 100 + text_seto_y, t3);
}

void Control::selectMod()
{
    drawModMenu();
    while (1) {
        updateMessage();
        switch (getclickmsg())
        {
        case MOD_13:setBoardSize(13); return;
        case MOD_15:setBoardSize(15); return;
        case MOD_17:setBoardSize(17); return;
        case MOD_19:setBoardSize(19); return;
        default: break;
        }
    }
}

void Control::drawGameInterface()
{
    Gameproc = PLAY_GAME;
    //播放开始提示音
    mciSendString(_T("play res/music/start.wav"), 0, 0, 0);

    //设置格子大小
    int grid = ChessSize;
    int size = BoardSize;

    //设置棋盘大小    
    int graphsize = grid * (size + 1);

    //初始化绘图窗口
    initgraph(graphsize + 200, graphsize/*, EX_SHOWCONSOLE*/);
    //重载绘图窗口位置
    HWND hWnd = GetHWnd();
    SetWindowPos(hWnd, nullptr, 300, 70, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    //设置背景色为木色
    setbkcolor(RGB(222, 174, 110));
    //用背景色清空屏幕
    cleardevice();

    setlinecolor(RGB(0, 0, 0));//线条色设置为黑色
    //画框线，宽度为4个像素
    setlinestyle(PS_SOLID, 4);
    rectangle(grid, grid, size * grid, size * grid);
    //画格线，宽度为2个像素
    setlinestyle(PS_SOLID, 2);
    for (int i = 2; i <= size - 1; i++)
    {
        line(i * grid, 1 * grid, i * grid, size * grid);//画竖线
        line(1 * grid, i * grid, size * grid, i * grid);//画横线
    }

    setfillcolor(RGB(0, 0, 0));//填充色设置为黑色
    //天元坐标
    int o_x = grid * (size + 1) * 0.5;
    int o_y = o_x;
    fillcircle(o_x, o_y, 5);
    //计算小星偏距
    int setover = (size + 1) * 0.5 - 4;
    //画天元和小星
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            fillcircle(o_x + i * setover * grid, o_y + j * setover * grid, 5);
        }
    }

    //打印坐标
    settextcolor(RGB(0, 0, 0));
    setbkmode(TRANSPARENT);
    settextstyle(20, 0, _T("微软雅黑"));

    for (int i = 0; i < size; i++) {
        TCHAR c = _T('A' + i);
        outtextxy(40 + i * grid, 20, c);
    }
    settextstyle(20, 0, _T("Consolas"));
    for (int i = 0; i < size; i++) {
        if (i < 9) {
            TCHAR c = _T('1' + i);
            outtextxy(25, 40 + i * grid, c);
        }
        else
        {
            TCHAR s[5];
            _stprintf_s(s, _T("%d"), 1 + i);    // 输出数值，先将数字格式化输出为字符串
            outtextxy(20, 35 + i * grid, s);
        }
    }

    //绘制控制按钮,控制区域宽度200
    int c_x = (size + 1) * grid; //控制区左上顶点
    int c_y = 0;
    setlinecolor(RGB(167, 90, 36));//线条色
    line(c_x, 0, c_x, c_x);//画竖线

    //按钮绘制
    setfillcolor(RGB(53, 26, 7));//填充色    
    for (int ii = 0; ii < 6; ii++)
        fillroundrect(c_x + 40, c_y + 50 + ii * 50, c_x + 160, c_y + 80 + ii * 50, 20, 20);

    //填充文字格式
    settextcolor(RGB(255, 255, 255));
    settextstyle(30, 0, _T("微软雅黑"));

    //文字偏距修正
    int text_seto_x = 15;
    int text_seto_y = -2;
    //填充文字
    TCHAR t0[] = _T("重新开始");
    TCHAR t1[] = _T("  悔  棋");
    TCHAR t2[] = _T("  提  示");
    TCHAR t3[] = _T("黑白交换");
    TCHAR t4[] = _T("开始菜单");
    TCHAR t5[] = _T("退出游戏");

    int ii = 0;
    outtextxy(c_x + 40 + text_seto_x, c_y + 50 + text_seto_y + ii++ * 50, t0);
    outtextxy(c_x + 40 + text_seto_x, c_y + 50 + text_seto_y + ii++ * 50, t1);
    outtextxy(c_x + 40 + text_seto_x, c_y + 50 + text_seto_y + ii++ * 50, t2);
    outtextxy(c_x + 40 + text_seto_x, c_y + 50 + text_seto_y + ii++ * 50, t3);
    outtextxy(c_x + 40 + text_seto_x, c_y + 50 + text_seto_y + ii++ * 50, t4);
    outtextxy(c_x + 40 + text_seto_x, c_y + 50 + text_seto_y + ii++ * 50, t5);
}

void Control::initGameButton()
{    
    int c_x = (BoardSize + 1) * ChessSize + 40;
    int c_y = 50; int gap = 50; int ii = 0;
    bt_restart_play  = { c_x, c_y + 0 * gap, 120, 30 };  //重新开始
    bt_regret_play   = { c_x, c_y + 1 * gap, 120, 30 };  //悔棋
    bt_tips_play     = { c_x, c_y + 2 * gap, 120, 30 };  //提示
    bt_exchange_play = { c_x, c_y + 3 * gap, 120, 30 };  //黑白交换
    bt_menu_play     = { c_x, c_y + 4 * gap, 120, 30 };	//开始菜单
    bt_quit_play     = { c_x, c_y + 5 * gap, 120, 30 };  //退出游戏
}

void Control::drawGameOver(int m_kind, enum chess_kind_t win_kind)
{
    Gameproc = GAME_OVER;
    {
        if ((m_kind == MAN_to_AI && win_kind == CHESS_BLACK) ||
            (m_kind == AI_to_MAN && win_kind == CHESS_WHITE)) {
            mciSendStringW(_T("play res/music/不错.mp3"), 0, 0, 0);
            initgraph(bg_victory.getwidth(), bg_victory.getheight());
            putimage(0, 0, &bg_victory);
            HWND hWnd = GetHWnd();
            SetWindowText(hWnd, _T("你赢了！"));
        }
        else if ((m_kind == MAN_to_AI && win_kind == CHESS_WHITE) ||
            (m_kind == AI_to_MAN && win_kind == CHESS_BLACK)) {
            mciSendStringW(_T("play res/music/失败.mp3"), 0, 0, 0);
            initgraph(bg_defeat.getwidth(), bg_defeat.getheight());
            putimage(0, 0, &bg_defeat);
            HWND hWnd = GetHWnd();
            SetWindowText(hWnd, _T("失败了！"));
        }
        else {
            if (win_kind == CHESS_WHITE) {
                mciSendStringW(_T("play res/music/不错.mp3"), 0, 0, 0);
                initgraph(bg_victory.getwidth(), bg_victory.getheight());
                putimage(0, 0, &bg_victory);
                HWND hWnd = GetHWnd();
                SetWindowText(hWnd, _T("白方胜利！"));
            }
            else {
                mciSendStringW(_T("play res/music/不错.mp3"), 0, 0, 0);
                initgraph(bg_victory.getwidth(), bg_victory.getheight());
                putimage(0, 0, &bg_victory);
                HWND hWnd = GetHWnd();
                SetWindowText(hWnd, _T("黑方胜利！"));
            }
        }
        //按钮加载
        putimagePNG(bt_menu_result.x, bt_menu_result.y, &bt_menu);
        putimagePNG(bt_again_result.x, bt_again_result.y, &bt_again);
        putimagePNG(bt_quit_result.x, bt_quit_result.y, &bt_quit);
    }
}

void Control::drawGameNotice()
{
    Gameproc = GAME_NOTICE;

    int w = 1100;
    int h = 620;
    int edge = 50;
    initgraph(w, h);
    HWND hWnd = GetHWnd();
    SetWindowPos(hWnd, nullptr, 300, 150, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    //设置背景色为木色
    setbkcolor(RGB(222, 174, 110));
    //用背景色清空屏幕
    cleardevice();
    //设置文字背景色透明
    setbkmode(TRANSPARENT);

    //画框线，宽度为4个像素
    setlinecolor(RGB(167, 90, 36));//线条色
    setlinestyle(PS_SOLID, 4);
    rectangle(edge, edge, w - edge, h - edge);

    //打印标题
    settextcolor(RGB(255, 255, 255));
    settextstyle(50, 0, _T("微软雅黑"));

    TCHAR s[] = _T("当前模式正在研发，暂未开放");
    outtextxy(w / 2 - 250, 80, s);

    //按钮绘制
    setfillcolor(RGB(53, 26, 7));//填充色
    //图形中心
    int o_x = w / 2;//550
    int o_y = h / 2;//310
    int setover = (o_x - 200) / 2;//175
    
    fillroundrect(o_x + setover, o_y + 100, o_x + setover + 200, o_y + 100 + 50, 20, 20);//725，410，925，460	

    ////文字偏距修正
    int text_seto_x = 20;
    int text_seto_y = -2;
    //填充文字    
    TCHAR t[] = _T("回主菜单");
    outtextxy(o_x + setover + text_seto_x, o_y + 100 + text_seto_y, t);
}

void Control::drawGameInfo()
{
    Gameproc = GAME_NOTICE;

    int w = 1100;
    int h = 620;
    int edge = 50;
    initgraph(w, h);
    HWND hWnd = GetHWnd();
    SetWindowPos(hWnd, nullptr, 300, 150, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    //设置背景色为木色
    setbkcolor(RGB(222, 174, 110));
    //用背景色清空屏幕
    cleardevice();
    //设置文字背景色透明
    setbkmode(TRANSPARENT);

    //画框线，宽度为4个像素
    setlinecolor(RGB(167, 90, 36));//线条色
    setlinestyle(PS_SOLID, 4);
    rectangle(edge, edge, w - edge, h - edge);

    //打印标题
    settextcolor(RGB(255, 255, 255));
    settextstyle(50, 0, _T("微软雅黑"));
    TCHAR s[] = _T("五子棋游戏");
    outtextxy(w / 2 - 100, 80, s);

    //打印内容
    settextstyle(30, 0, _T("微软雅黑"));
    TCHAR s0[] = _T("版本：5.0");
    TCHAR s1[] = _T("作者：cometBug");
    TCHAR s2[] = _T("本游戏由C++开发，采用easyx图形库，素材均来自互联网，");
    TCHAR s3[] = _T("仅供学习交流，严禁用于商业用途，请于24小时内删除！");
    TCHAR s4[] = _T("项目开源地址：https://github.com/cometBug/cpp_gobang");
    outtextxy(100, 150, s0);
    outtextxy(100, 200, s1);
    outtextxy(100, 250, s2);
    outtextxy(100, 300, s3);
    outtextxy(100, 350, s4);

    //按钮绘制
    setfillcolor(RGB(53, 26, 7));//填充色
    //图形中心
    int o_x = w / 2;//550
    int o_y = h / 2;//310
    int setover = (o_x - 200) / 2;//175

    fillroundrect(o_x + setover, o_y + 100, o_x + setover + 200, o_y + 100 + 50, 20, 20);//725，410，925，460	

    ////文字偏距修正
    int text_seto_x = 20;
    int text_seto_y = -2;
    //填充文字
    settextstyle(50, 0, _T("微软雅黑"));
    TCHAR t[] = _T("回主菜单");
    outtextxy(o_x + setover + text_seto_x, o_y + 100 + text_seto_y, t);
}

void Control::readInfo()
{
    while (1) {
        updateMessage();
        if (getclickmsg() == MENU) {
            m_menu = true;
            return;
        }
    }
}

void Control::updateMessage()
{
    getmessage(&m_msg, EX_MOUSE);
}

ChessPos Control::chesspos()
{
    return m_pos;
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

bool Control::clickArea(const ExMessage& msg)
{
    int maxsize = (BoardSize + 1) * ChessSize;
    if (msg.message == WM_LBUTTONDOWN &&
        msg.x < maxsize && msg.y < maxsize) {
        return true;
    }
    return false;
}

bool Control::clickBoard(int x, int y, ChessPos* pos)
{
    int col = static_cast<int>(x / ChessSize - 1);
    int row = static_cast<int>(y / ChessSize - 1);
    int leftTopPosX = (1 + col) * ChessSize;
    int leftTopPosY = (1 + row) * ChessSize;
    int offset = static_cast<int>(ChessSize * 0.4);   //点击阈值设置
    int len;
    bool selectPos = false;
    do {
        //左上角判断
        len = static_cast<int>(sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY)));
        if (len < offset) {
            pos->row = row;
            pos->col = col;
            if (ChessMap[pos->row][pos->col] == 0) {
                selectPos = true;
            }
            break;
        }
        //右上角判断
        int x2 = leftTopPosX + ChessSize;
        int y2 = leftTopPosY;
        len = static_cast<int>(sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)));
        if (len < offset) {
            pos->row = row;
            pos->col = col + 1;
            if (ChessMap[pos->row][pos->col] == 0) {
                selectPos = true;
            }
            break;
        }
        //左下角判断
        x2 = leftTopPosX;
        y2 = leftTopPosY + ChessSize;
        len = static_cast<int>(sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)));
        if (len < offset) {
            pos->row = row + 1;
            pos->col = col;
            if (ChessMap[pos->row][pos->col] == 0) {
                selectPos = true;
            }
            break;
        }
        //右下角判断
        x2 = leftTopPosX + ChessSize;
        y2 = leftTopPosY + ChessSize;
        len = static_cast<int>(sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)));
        if (len < offset) {
            pos->row = row + 1;
            pos->col = col + 1;
            if (ChessMap[pos->row][pos->col] == 0) {
                selectPos = true;
            }
            break;
        }
    } while (0);

    return selectPos;
}

int Control::getChessData(ChessPos* pos)
{
    return ChessMap[pos->row][pos->col];
}

int Control::getChessData(int row, int col)
{
    return ChessMap[row][col];
}

void Control::chessDown(ChessPos* pos, enum chess_kind_t kind)
{
    int x = static_cast<int>(ChessSize * pos->col + ChessSize * 0.5);
    int y = static_cast<int>(ChessSize * pos->row + ChessSize * 0.5);

    if (kind == CHESS_WHITE) {
        putimagePNG(x, y, &ChessWhiteImg);
    }
    else {
        putimagePNG(x, y, &ChessBlackImg);
    }
    mciSendString(_T("play res/music/down7.wav"), 0, 0, 0);    

    updateChessMap(pos, kind);
}

void Control::chessDown(ChessPos pos, enum chess_kind_t kind)
{
    int x = static_cast<int>(ChessSize * pos.col + ChessSize * 0.5);
    int y = static_cast<int>(ChessSize * pos.row + ChessSize * 0.5);

    if (kind == CHESS_WHITE) {
        putimagePNG(x, y, &ChessWhiteImg);
    }
    else {
        putimagePNG(x, y, &ChessBlackImg);
    }
    mciSendString(_T("play res/music/down7.wav"), 0, 0, 0);

    updateChessMap(pos, kind);
}

void Control::renewChess(ChessPos pos)
{
    int x = static_cast<int>(ChessSize * pos.col + ChessSize * 0.5);
    int y = static_cast<int>(ChessSize * pos.row + ChessSize * 0.5);
    if (pos.kind == CHESS_WHITE) 
        putimagePNG(x, y, &ChessWhiteImg);
    else 
        putimagePNG(x, y, &ChessBlackImg);
}

void Control::updateChessMap(ChessPos* pos, enum chess_kind_t kind)
{
    m_pos = *pos;
    ChessMap[pos->row][pos->col] = int(kind);

    step++;
    ChessManual.emplace_back(pos->row, pos->col, kind);

    //cout << step << "," << pos->row << "," << pos->col << endl;
}

void Control::updateChessMap(ChessPos pos, enum chess_kind_t kind) {

    ChessMap[pos.row][pos.col] = int(kind);

    step++;
    ChessManual.emplace_back(pos.row, pos.col, kind);

    //cout << step << "," << pos.row << "," << pos.col << endl;
}

void Control::retractChess()
{
    if (step > 1) {
        //修改棋谱        
        ChessMap[ChessManual[step - 1].row][ChessManual[step - 1].col] = 0;
        ChessMap[ChessManual[step - 2].row][ChessManual[step - 2].col] = 0;
        ChessManual.pop_back();
        ChessManual.pop_back();
        step -= 2;
        renewBoard();
    }
    else {
        setbkmode(TRANSPARENT);
        settextcolor(RGB(255, 255, 255));
        settextstyle(25, 0, _T("微软雅黑"));
        TCHAR t[] = _T("已经不能悔棋了！");
        outtextxy(bt_quit_play.x-10, bt_quit_play.y + 50, t);
    }
}

void Control::renewBoard()
{
    //设置格子大小
    int grid = ChessSize;
    int graphsize = grid * (BoardSize + 1);

    //设置填充色为木色
    setfillcolor(RGB(222, 174, 110));
    //画无边框的填充矩形
    solidrectangle(10, 10, graphsize - 10, graphsize - 10);

    setlinecolor(RGB(0, 0, 0));//线条色设置为黑色
    //画框线，宽度为4个像素
    setlinestyle(PS_SOLID, 4);
    rectangle(grid, grid, BoardSize * grid, BoardSize * grid);
    //画格线，宽度为2个像素
    setlinestyle(PS_SOLID, 2);
    for (int i = 2; i <= BoardSize - 1; i++)
    {
        line(i * grid, 1 * grid, i * grid, BoardSize * grid);//画竖线
        line(1 * grid, i * grid, BoardSize * grid, i * grid);//画横线
    }

    setfillcolor(RGB(0, 0, 0));//填充色设置为黑色
    //天元坐标
    int o_x = grid * (BoardSize + 1) * 0.5;
    int o_y = o_x;
    fillcircle(o_x, o_y, 5);
    //计算小星偏距
    int setover = (BoardSize + 1) * 0.5 - 4;
    //画天元和小星
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            fillcircle(o_x + i * setover * grid, o_y + j * setover * grid, 5);
        }
    }
    //打印坐标
    settextcolor(RGB(0, 0, 0));
    setbkmode(TRANSPARENT);
    settextstyle(20, 0, _T("微软雅黑"));

    for (int i = 0; i < BoardSize; i++) {
        TCHAR c = _T('A' + i);
        outtextxy(40 + i * grid, 20, c);
    }
    settextstyle(20, 0, _T("Consolas"));
    for (int i = 0; i < BoardSize; i++) {
        if (i < 9) {
            TCHAR c = _T('1' + i);
            outtextxy(25, 40 + i * grid, c);
        }
        else
        {
            TCHAR s[5];
            _stprintf_s(s, _T("%d"), 1 + i);    // 输出数值，先将数字格式化输出为字符串
            outtextxy(20, 35 + i * grid, s);
        }
    }
    //重新走棋    
    for (int ii = 0; ii < step; ii++)
        renewChess(ChessManual[ii]);
}

void Control::initTipsArea()
{
    setbkmode(OPAQUE);
    setfillcolor(RGB(222, 174, 110));
    //画无边框的填充矩形
    solidrectangle(bt_quit_play.x - 20, bt_quit_play.y + 40,
        ChessSize * (BoardSize + 1) + 200, bt_quit_play.y + 100);
}

void Control::getTips(ChessPos pos, enum chess_kind_t kind)
{
    setbkmode(TRANSPARENT);
    settextcolor(RGB(255, 255, 255));
    settextstyle(25, 0, _T("微软雅黑"));
   
    TCHAR s[20];
    _stprintf_s(s, _T("建议走%d, %c位置！"), pos.row + 1, pos.col + 'A');
    outtextxy(bt_quit_play.x - 10, bt_quit_play.y + 50, s);

    int x = static_cast<int>(ChessSize * (pos.col + 1));
    int y = static_cast<int>(ChessSize * (pos.row + 1));
    setlinestyle(PS_DASH);
    if (kind == CHESS_WHITE) {
        setlinecolor(WHITE);
        circle(x, y, ChessSize * 0.5 - 4);
    }
    else {
        setlinecolor(BLACK);
        circle(x, y, ChessSize * 0.5 - 4);
    }
}

int Control::getclickmsg()
{
    //返回点击效果
    Coord coord;
    if (m_msg.message == WM_LBUTTONDOWN)
    {
        if (Gameproc == START_MENU)
        {
            coord = bt_man2ai_menu;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return MAN2AI;
            coord = bt_man2man_menu;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return MAN2MAN;
            coord = bt_man2net_menu;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return MAN2NET;
            coord = bt_about_menu;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return ABOUTGAME;
            coord = bt_quit_menu;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return QUIT;           
        }
        if (Gameproc == MOD_MENU)
        {
            coord = bt_13_mod;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return MOD_13;
            coord = bt_15_mod;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return MOD_15;
            coord = bt_17_mod;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return MOD_17;
            coord = bt_19_mod;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return MOD_19;
        }
        if (Gameproc == PLAY_GAME)
        {
            coord = bt_restart_play;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return RESTART;
            coord = bt_regret_play;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return REGRET;
            coord = bt_tips_play;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return TIPS;
            coord = bt_exchange_play;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return EXCHANGE;
            coord = bt_menu_play;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return MENU;
            coord = bt_quit_play;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return QUIT;
        }
        if (Gameproc == GAME_OVER)
        {
            coord = bt_again_result;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return AGAIN;
            coord = bt_menu_result;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return MENU;
            coord = bt_quit_result;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return QUIT;
        }
        if (Gameproc == GAME_NOTICE)
        {
            coord = bt_menu_notice;
            if (m_msg.x > coord.x && m_msg.x < coord.x + coord.width &&
                m_msg.y > coord.y && m_msg.y < coord.y + coord.height)
                return MENU;
        }

    }
    return WRONG;
}

void Control::clearinfo()
{
    m_menu = false;
    m_goon = false;
    m_quit = false;
    m_restart = false;
    m_exchange = false;
}

bool Control::isWin(enum chess_kind_t kind)
{
    //只需要判断落子点周围方向
    vector<int> chessrange;
    int size = BoardSize;

    int row = m_pos.row;
    int col = m_pos.col;

    int cur_row;
    int cur_col;
    do {
        //四个方向
        //左->右
        cur_row = row;
        cur_col = col - 4;
        chessrange.clear();
        for (int ii = 0; ii < 9; ii++) {
            if (cur_row < 0 || cur_col < 0 ||
                cur_row >= size || cur_col >= size) {
                chessrange.push_back(0);
            }
            else {
                chessrange.push_back(getChessData(cur_row, cur_col));
            }
            cur_col++;
        }
        for (int ii = 0; ii < 5; ii++) {
            // 连5判断
            if (chessrange[ii] == kind && chessrange[ii + 1] == kind &&
                chessrange[ii + 2] == kind && chessrange[ii + 3] == kind &&
                chessrange[ii + 4] == kind)
                return true;
        }
        //左上->右下
        cur_row = row - 4;
        cur_col = col - 4;
        chessrange.clear();
        for (int ii = 0; ii < 9; ii++) {
            if (cur_row < 0 || cur_col < 0 ||
                cur_row >= size || cur_col >= size) {
                chessrange.push_back(0);
            }
            else {
                chessrange.push_back(getChessData(cur_row, cur_col));
            }
            cur_row++;
            cur_col++;
        }
        for (int ii = 0; ii < 5; ii++) {
            // 连5判断
            if (chessrange[ii] == kind && chessrange[ii + 1] == kind &&
                chessrange[ii + 2] == kind && chessrange[ii + 3] == kind &&
                chessrange[ii + 4] == kind)
                return true;
        }
        //上->下
        cur_row = row - 4;
        cur_col = col;
        chessrange.clear();
        for (int ii = 0; ii < 9; ii++) {
            if (cur_row < 0 || cur_col < 0 ||
                cur_row >= size || cur_col >= size) {
                chessrange.push_back(0);
            }
            else {
                chessrange.push_back(getChessData(cur_row, cur_col));
            }
            cur_row++;
        }
        for (int ii = 0; ii < 5; ii++) {
            // 连5判断
            if (chessrange[ii] == kind && chessrange[ii + 1] == kind &&
                chessrange[ii + 2] == kind && chessrange[ii + 3] == kind &&
                chessrange[ii + 4] == kind)
                return true;
        }
        //右上->左下
        cur_row = row - 4;
        cur_col = col + 4;
        chessrange.clear();
        for (int ii = 0; ii < 9; ii++) {
            if (cur_row < 0 || cur_col < 0 ||
                cur_row >= size || cur_col >= size) {
                chessrange.push_back(0);
            }
            else {
                chessrange.push_back(getChessData(cur_row, cur_col));
            }
            cur_row++;
            cur_col--;
        }
        for (int ii = 0; ii < 5; ii++) {
            // 连5判断
            if (chessrange[ii] == kind && chessrange[ii + 1] == kind &&
                chessrange[ii + 2] == kind && chessrange[ii + 3] == kind &&
                chessrange[ii + 4] == kind)
                return true;
        }
    } while (0);

    return false;
    //return true;
}

void Control::setBoardSize(const int size)
{
    BoardSize = size;
}

int Control::getBoardSize()
{
    return BoardSize;
}

