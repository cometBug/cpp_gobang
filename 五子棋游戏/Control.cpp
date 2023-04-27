#include "Control.h"

Control::Control()
{
    BoardSize = 13;
    ChessSize = 45;
    step = 0;
    //��������ͼƬ
    loadimage(&ChessBlackImg, _T("res/black.png"), ChessSize, ChessSize, true);
    loadimage(&ChessWhiteImg, _T("res/white.png"), ChessSize, ChessSize, true);
    //������ͼ
    loadimage(&bg_menu, _T("res/menu.jpg"), 0, 0, true);
    loadimage(&bg_victory, _T("res/ʤ��1.jpg"), 0, 0, true);
    loadimage(&bg_defeat, _T("res/ʧ��1.jpg"), 0, 0, true);
    //���ذ�ť
    loadimage(&bt_man2ai, _T("res/button/�˻�����.png"), BT_WIDTH, BT_HEIGHT, true);
    loadimage(&bt_man2man, _T("res/button/���Ѷ���.png"), BT_WIDTH, BT_HEIGHT, true);
    loadimage(&bt_man2net, _T("res/button/�������.png"), BT_WIDTH, BT_HEIGHT, true);
    loadimage(&bt_about, _T("res/button/������Ϸ.png"), BT_WIDTH, BT_HEIGHT, true);
    loadimage(&bt_quit, _T("res/button/�˳���Ϸ.png"), BT_WIDTH, BT_HEIGHT, true);
    loadimage(&bt_again, _T("res/button/����һ��.png"), BT_WIDTH, BT_HEIGHT, true);
    loadimage(&bt_menu, _T("res/button/��ʼ�˵�.png"), BT_WIDTH, BT_HEIGHT, true);
    //��ť������Ϣ
    //��ʼ�˵�
    bt_man2ai_menu = { 950, 140, BT_WIDTH, BT_HEIGHT };   //�˻�����
    bt_man2man_menu = { 950, 200, BT_WIDTH, BT_HEIGHT };  //���Ѷ���
    bt_man2net_menu = { 950, 260, BT_WIDTH, BT_HEIGHT };  //�������
    bt_about_menu = { 950, 320, BT_WIDTH, BT_HEIGHT };    //������Ϸ
    bt_quit_menu = { 950, 380, BT_WIDTH, BT_HEIGHT };     //�˳���Ϸ
    //�������
    bt_menu_result = { 175, 500, BT_WIDTH, BT_HEIGHT };   //��ʼ�˵�
    bt_again_result = { 385, 500, BT_WIDTH, BT_HEIGHT };  //����һ��
    bt_quit_result = { 595, 500, BT_WIDTH, BT_HEIGHT };   //�˳���Ϸ
    //ģʽѡ�����
    bt_13_mod = { 175, 210, 200, 50 };   //13��
    bt_15_mod = { 725, 210, 200, 50 };   //15��
    bt_17_mod = { 175, 410, 200, 50 };   //17��
    bt_19_mod = { 725, 410, 200, 50 };   //19��
    //�������
    bt_menu_notice = { 725, 410, 200, 50 }; //�����˵�
    
    m_pos = { 0,0,CHESS_BLACK };

    //������Ϣ��ʼ��
    m_menu = false;
    m_goon = false;
    m_quit = false;
    m_restart = false;
    m_exchange = false;
    m_regret = false;    
}

void Control::putimagePNG(int x, int y, IMAGE* picture) //xΪ����ͼƬ��X���꣬yΪY����
{
    // ������ʼ��
    DWORD* dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
    DWORD* draw = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
    int picture_width = picture->getwidth(); //��ȡpicture�Ŀ�ȣ�EASYX�Դ�
    int picture_height = picture->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
    int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
    int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
    int dstX = 0;    //���Դ������صĽǱ�

    // ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
            int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
            int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
            int sg = ((src[srcX] & 0xff00) >> 8);   //G
            int sb = src[srcX] & 0xff;              //B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
            {
                dstX = (ix + x) + (iy + y) * graphWidth; //���Դ������صĽǱ�
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
                    | (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
            }
        }
    }
}

void Control::init()
{

    drawGameInterface();
    initGameButton();
    clearinfo();
    //���̳�ʼ��
    ChessMap.clear();
    for (int i = 0; i < BoardSize; i++) {
        vector<int> row;
        for (int j = 0; j < BoardSize; j++) {
            row.emplace_back(0);
        }
        ChessMap.emplace_back(row);
    }
    step = 0;//�ӵ�0����ʼ
    ChessManual.clear();
}

void Control::drawMainMenu()
{    
    Gameproc = START_MENU;

    //������ʼ�˵�����
    initgraph(bg_menu.getwidth(), bg_menu.getheight());

    //�ػ洰�ڣ�ʹ���ھ���
    HWND hWnd = GetHWnd();
    SetWindowPos(hWnd, nullptr, 300, 150, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    //SetWindowText(hWnd, _T("��������Ϸ"));   

    //��ʾ��ʼ�˵�	
    putimage(0, 0, &bg_menu);
    putimagePNG(bt_man2ai_menu.x, bt_man2ai_menu.y, &bt_man2ai);	//�˻�����
    putimagePNG(bt_man2man_menu.x, bt_man2man_menu.y, &bt_man2man);	//���Ѷ���
    putimagePNG(bt_man2net_menu.x, bt_man2net_menu.y, &bt_man2net);	//�������
    putimagePNG(bt_about_menu.x, bt_about_menu.y, &bt_about);		//������Ϸ
    putimagePNG(bt_quit_menu.x, bt_quit_menu.y, &bt_quit);			//�˳���Ϸ

    //��ͣ��ʾͼƬ
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

    //���ñ���ɫΪľɫ
    setbkcolor(RGB(222, 174, 110));
    //�ñ���ɫ�����Ļ
    cleardevice();
    //�������ֱ���ɫ͸��
    setbkmode(TRANSPARENT);

    //�����ߣ����Ϊ4������
    setlinecolor(RGB(167, 90, 36));//����ɫ
    setlinestyle(PS_SOLID, 4);
    rectangle(edge, edge, w - edge, h - edge);

    //��ӡ����
    settextcolor(RGB(255, 255, 255));
    settextstyle(50, 0, _T("΢���ź�"));

    TCHAR s[] = _T("��ѡ�����̴�С");
    outtextxy(w / 2 - 130, 80, s);

    //��ť����
    setfillcolor(RGB(53, 26, 7));//���ɫ
    //ͼ������
    int o_x = w / 2;//550
    int o_y = h / 2;//310
    int setover = (o_x - 200) / 2;//175
    fillroundrect(o_x - setover - 200, o_y - 100, o_x - setover, o_y - 100 + 50, 20, 20);//175��210��375��260
    fillroundrect(o_x + setover, o_y - 100, o_x + setover + 200, o_y - 100 + 50, 20, 20);//725��210��925��260
    fillroundrect(o_x - setover - 200, o_y + 100, o_x - setover, o_y + 100 + 50, 20, 20);//175��410��375��460
    fillroundrect(o_x + setover, o_y + 100, o_x + setover + 200, o_y + 100 + 50, 20, 20);//725��410��925��460	

    ////����ƫ������
    int text_seto_x = 60;
    int text_seto_y = -2;
    //�������
    TCHAR t0[] = _T("13��");
    TCHAR t1[] = _T("15��");
    TCHAR t2[] = _T("17��");
    TCHAR t3[] = _T("19��");

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
    //���ſ�ʼ��ʾ��
    mciSendString(_T("play res/music/start.wav"), 0, 0, 0);

    //���ø��Ӵ�С
    int grid = ChessSize;
    int size = BoardSize;

    //�������̴�С    
    int graphsize = grid * (size + 1);

    //��ʼ����ͼ����
    initgraph(graphsize + 200, graphsize/*, EX_SHOWCONSOLE*/);
    //���ػ�ͼ����λ��
    HWND hWnd = GetHWnd();
    SetWindowPos(hWnd, nullptr, 300, 70, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    //���ñ���ɫΪľɫ
    setbkcolor(RGB(222, 174, 110));
    //�ñ���ɫ�����Ļ
    cleardevice();

    setlinecolor(RGB(0, 0, 0));//����ɫ����Ϊ��ɫ
    //�����ߣ����Ϊ4������
    setlinestyle(PS_SOLID, 4);
    rectangle(grid, grid, size * grid, size * grid);
    //�����ߣ����Ϊ2������
    setlinestyle(PS_SOLID, 2);
    for (int i = 2; i <= size - 1; i++)
    {
        line(i * grid, 1 * grid, i * grid, size * grid);//������
        line(1 * grid, i * grid, size * grid, i * grid);//������
    }

    setfillcolor(RGB(0, 0, 0));//���ɫ����Ϊ��ɫ
    //��Ԫ����
    int o_x = grid * (size + 1) * 0.5;
    int o_y = o_x;
    fillcircle(o_x, o_y, 5);
    //����С��ƫ��
    int setover = (size + 1) * 0.5 - 4;
    //����Ԫ��С��
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            fillcircle(o_x + i * setover * grid, o_y + j * setover * grid, 5);
        }
    }

    //��ӡ����
    settextcolor(RGB(0, 0, 0));
    setbkmode(TRANSPARENT);
    settextstyle(20, 0, _T("΢���ź�"));

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
            _stprintf_s(s, _T("%d"), 1 + i);    // �����ֵ���Ƚ����ָ�ʽ�����Ϊ�ַ���
            outtextxy(20, 35 + i * grid, s);
        }
    }

    //���ƿ��ư�ť,����������200
    int c_x = (size + 1) * grid; //���������϶���
    int c_y = 0;
    setlinecolor(RGB(167, 90, 36));//����ɫ
    line(c_x, 0, c_x, c_x);//������

    //��ť����
    setfillcolor(RGB(53, 26, 7));//���ɫ    
    for (int ii = 0; ii < 6; ii++)
        fillroundrect(c_x + 40, c_y + 50 + ii * 50, c_x + 160, c_y + 80 + ii * 50, 20, 20);

    //������ָ�ʽ
    settextcolor(RGB(255, 255, 255));
    settextstyle(30, 0, _T("΢���ź�"));

    //����ƫ������
    int text_seto_x = 15;
    int text_seto_y = -2;
    //�������
    TCHAR t0[] = _T("���¿�ʼ");
    TCHAR t1[] = _T("  ��  ��");
    TCHAR t2[] = _T("  ��  ʾ");
    TCHAR t3[] = _T("�ڰ׽���");
    TCHAR t4[] = _T("��ʼ�˵�");
    TCHAR t5[] = _T("�˳���Ϸ");

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
    bt_restart_play  = { c_x, c_y + 0 * gap, 120, 30 };  //���¿�ʼ
    bt_regret_play   = { c_x, c_y + 1 * gap, 120, 30 };  //����
    bt_tips_play     = { c_x, c_y + 2 * gap, 120, 30 };  //��ʾ
    bt_exchange_play = { c_x, c_y + 3 * gap, 120, 30 };  //�ڰ׽���
    bt_menu_play     = { c_x, c_y + 4 * gap, 120, 30 };	//��ʼ�˵�
    bt_quit_play     = { c_x, c_y + 5 * gap, 120, 30 };  //�˳���Ϸ
}

void Control::drawGameOver(int m_kind, enum chess_kind_t win_kind)
{
    Gameproc = GAME_OVER;
    {
        if ((m_kind == MAN_to_AI && win_kind == CHESS_BLACK) ||
            (m_kind == AI_to_MAN && win_kind == CHESS_WHITE)) {
            mciSendStringW(_T("play res/music/����.mp3"), 0, 0, 0);
            initgraph(bg_victory.getwidth(), bg_victory.getheight());
            putimage(0, 0, &bg_victory);
            HWND hWnd = GetHWnd();
            SetWindowText(hWnd, _T("��Ӯ�ˣ�"));
        }
        else if ((m_kind == MAN_to_AI && win_kind == CHESS_WHITE) ||
            (m_kind == AI_to_MAN && win_kind == CHESS_BLACK)) {
            mciSendStringW(_T("play res/music/ʧ��.mp3"), 0, 0, 0);
            initgraph(bg_defeat.getwidth(), bg_defeat.getheight());
            putimage(0, 0, &bg_defeat);
            HWND hWnd = GetHWnd();
            SetWindowText(hWnd, _T("ʧ���ˣ�"));
        }
        else {
            if (win_kind == CHESS_WHITE) {
                mciSendStringW(_T("play res/music/����.mp3"), 0, 0, 0);
                initgraph(bg_victory.getwidth(), bg_victory.getheight());
                putimage(0, 0, &bg_victory);
                HWND hWnd = GetHWnd();
                SetWindowText(hWnd, _T("�׷�ʤ����"));
            }
            else {
                mciSendStringW(_T("play res/music/����.mp3"), 0, 0, 0);
                initgraph(bg_victory.getwidth(), bg_victory.getheight());
                putimage(0, 0, &bg_victory);
                HWND hWnd = GetHWnd();
                SetWindowText(hWnd, _T("�ڷ�ʤ����"));
            }
        }
        //��ť����
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

    //���ñ���ɫΪľɫ
    setbkcolor(RGB(222, 174, 110));
    //�ñ���ɫ�����Ļ
    cleardevice();
    //�������ֱ���ɫ͸��
    setbkmode(TRANSPARENT);

    //�����ߣ����Ϊ4������
    setlinecolor(RGB(167, 90, 36));//����ɫ
    setlinestyle(PS_SOLID, 4);
    rectangle(edge, edge, w - edge, h - edge);

    //��ӡ����
    settextcolor(RGB(255, 255, 255));
    settextstyle(50, 0, _T("΢���ź�"));

    TCHAR s[] = _T("��ǰģʽ�����з�����δ����");
    outtextxy(w / 2 - 250, 80, s);

    //��ť����
    setfillcolor(RGB(53, 26, 7));//���ɫ
    //ͼ������
    int o_x = w / 2;//550
    int o_y = h / 2;//310
    int setover = (o_x - 200) / 2;//175
    
    fillroundrect(o_x + setover, o_y + 100, o_x + setover + 200, o_y + 100 + 50, 20, 20);//725��410��925��460	

    ////����ƫ������
    int text_seto_x = 20;
    int text_seto_y = -2;
    //�������    
    TCHAR t[] = _T("�����˵�");
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

    //���ñ���ɫΪľɫ
    setbkcolor(RGB(222, 174, 110));
    //�ñ���ɫ�����Ļ
    cleardevice();
    //�������ֱ���ɫ͸��
    setbkmode(TRANSPARENT);

    //�����ߣ����Ϊ4������
    setlinecolor(RGB(167, 90, 36));//����ɫ
    setlinestyle(PS_SOLID, 4);
    rectangle(edge, edge, w - edge, h - edge);

    //��ӡ����
    settextcolor(RGB(255, 255, 255));
    settextstyle(50, 0, _T("΢���ź�"));
    TCHAR s[] = _T("��������Ϸ");
    outtextxy(w / 2 - 100, 80, s);

    //��ӡ����
    settextstyle(30, 0, _T("΢���ź�"));
    TCHAR s0[] = _T("�汾��5.0");
    TCHAR s1[] = _T("���ߣ�cometBug");
    TCHAR s2[] = _T("����Ϸ��C++����������easyxͼ�ο⣬�زľ����Ի�������");
    TCHAR s3[] = _T("����ѧϰ�������Ͻ�������ҵ��;������24Сʱ��ɾ����");
    TCHAR s4[] = _T("��Ŀ��Դ��ַ��https://github.com/cometBug/cpp_gobang");
    outtextxy(100, 150, s0);
    outtextxy(100, 200, s1);
    outtextxy(100, 250, s2);
    outtextxy(100, 300, s3);
    outtextxy(100, 350, s4);

    //��ť����
    setfillcolor(RGB(53, 26, 7));//���ɫ
    //ͼ������
    int o_x = w / 2;//550
    int o_y = h / 2;//310
    int setover = (o_x - 200) / 2;//175

    fillroundrect(o_x + setover, o_y + 100, o_x + setover + 200, o_y + 100 + 50, 20, 20);//725��410��925��460	

    ////����ƫ������
    int text_seto_x = 20;
    int text_seto_y = -2;
    //�������
    settextstyle(50, 0, _T("΢���ź�"));
    TCHAR t[] = _T("�����˵�");
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
    //ȷ���������
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
    int offset = static_cast<int>(ChessSize * 0.4);   //�����ֵ����
    int len;
    bool selectPos = false;
    do {
        //���Ͻ��ж�
        len = static_cast<int>(sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY)));
        if (len < offset) {
            pos->row = row;
            pos->col = col;
            if (ChessMap[pos->row][pos->col] == 0) {
                selectPos = true;
            }
            break;
        }
        //���Ͻ��ж�
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
        //���½��ж�
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
        //���½��ж�
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
        //�޸�����        
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
        settextstyle(25, 0, _T("΢���ź�"));
        TCHAR t[] = _T("�Ѿ����ܻ����ˣ�");
        outtextxy(bt_quit_play.x-10, bt_quit_play.y + 50, t);
    }
}

void Control::renewBoard()
{
    //���ø��Ӵ�С
    int grid = ChessSize;
    int graphsize = grid * (BoardSize + 1);

    //�������ɫΪľɫ
    setfillcolor(RGB(222, 174, 110));
    //���ޱ߿��������
    solidrectangle(10, 10, graphsize - 10, graphsize - 10);

    setlinecolor(RGB(0, 0, 0));//����ɫ����Ϊ��ɫ
    //�����ߣ����Ϊ4������
    setlinestyle(PS_SOLID, 4);
    rectangle(grid, grid, BoardSize * grid, BoardSize * grid);
    //�����ߣ����Ϊ2������
    setlinestyle(PS_SOLID, 2);
    for (int i = 2; i <= BoardSize - 1; i++)
    {
        line(i * grid, 1 * grid, i * grid, BoardSize * grid);//������
        line(1 * grid, i * grid, BoardSize * grid, i * grid);//������
    }

    setfillcolor(RGB(0, 0, 0));//���ɫ����Ϊ��ɫ
    //��Ԫ����
    int o_x = grid * (BoardSize + 1) * 0.5;
    int o_y = o_x;
    fillcircle(o_x, o_y, 5);
    //����С��ƫ��
    int setover = (BoardSize + 1) * 0.5 - 4;
    //����Ԫ��С��
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            fillcircle(o_x + i * setover * grid, o_y + j * setover * grid, 5);
        }
    }
    //��ӡ����
    settextcolor(RGB(0, 0, 0));
    setbkmode(TRANSPARENT);
    settextstyle(20, 0, _T("΢���ź�"));

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
            _stprintf_s(s, _T("%d"), 1 + i);    // �����ֵ���Ƚ����ָ�ʽ�����Ϊ�ַ���
            outtextxy(20, 35 + i * grid, s);
        }
    }
    //��������    
    for (int ii = 0; ii < step; ii++)
        renewChess(ChessManual[ii]);
}

void Control::initTipsArea()
{
    setbkmode(OPAQUE);
    setfillcolor(RGB(222, 174, 110));
    //���ޱ߿��������
    solidrectangle(bt_quit_play.x - 20, bt_quit_play.y + 40,
        ChessSize * (BoardSize + 1) + 200, bt_quit_play.y + 100);
}

void Control::getTips(ChessPos pos, enum chess_kind_t kind)
{
    setbkmode(TRANSPARENT);
    settextcolor(RGB(255, 255, 255));
    settextstyle(25, 0, _T("΢���ź�"));
   
    TCHAR s[20];
    _stprintf_s(s, _T("������%d, %cλ�ã�"), pos.row + 1, pos.col + 'A');
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
    //���ص��Ч��
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
    //ֻ��Ҫ�ж����ӵ���Χ����
    vector<int> chessrange;
    int size = BoardSize;

    int row = m_pos.row;
    int col = m_pos.col;

    int cur_row;
    int cur_col;
    do {
        //�ĸ�����
        //��->��
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
            // ��5�ж�
            if (chessrange[ii] == kind && chessrange[ii + 1] == kind &&
                chessrange[ii + 2] == kind && chessrange[ii + 3] == kind &&
                chessrange[ii + 4] == kind)
                return true;
        }
        //����->����
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
            // ��5�ж�
            if (chessrange[ii] == kind && chessrange[ii + 1] == kind &&
                chessrange[ii + 2] == kind && chessrange[ii + 3] == kind &&
                chessrange[ii + 4] == kind)
                return true;
        }
        //��->��
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
            // ��5�ж�
            if (chessrange[ii] == kind && chessrange[ii + 1] == kind &&
                chessrange[ii + 2] == kind && chessrange[ii + 3] == kind &&
                chessrange[ii + 4] == kind)
                return true;
        }
        //����->����
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
            // ��5�ж�
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

