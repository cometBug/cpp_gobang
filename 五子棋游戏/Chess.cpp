#include "Chess.h"
#include "ChessGame.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

//����png͸����ʽͼƬ
void putimagePNG(int x, int y, IMAGE* picture) //xΪ����ͼƬ��X���꣬yΪY����
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

Chess::Chess(Control* control, int BoardSize, int marginX, int marginY, int ChessSize)
{
    this->m_control = control;
    this->BoardSize = BoardSize;
    this->margin_x = marginX;
    this->margin_y = marginY;
    this->ChessSize = ChessSize;    
    playerFlag = CHESS_BLACK;

    for (int i = 0; i < BoardSize; i++) {
        vector<int> row;
        for (int j = 0; j < BoardSize; j++) {
            row.emplace_back(0);
        }
        chessMap.emplace_back(row);
    }
}

void Chess::drawChessBoard(const int size)
{
    //���ø��Ӵ�С
    int step = 45;

    //�������̴�С
    int graphsize = step * (size + 1);

    //��ʼ����ͼ����
    initgraph(graphsize, graphsize);
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
    line(step, step, step, size * step);
    line(step, step, size * step, step);
    line(step, size * step, size * step, size * step);
    line(size * step, step, size * step, size * step);
    //�����ߣ����Ϊ2������
    setlinestyle(PS_SOLID, 2);
    for (int i = 2; i <= 18; i++)
    {
        line(i * step, 1 * step, i * step, size * step);//������
        line(1 * step, i * step, size * step, i * step);//������
    }

    setfillcolor(RGB(0, 0, 0));//���ɫ����Ϊ��ɫ
    //��Ԫ����
    int o_x = static_cast<int>(step * (size + 1) * 0.5);
    int o_y = o_x;
    fillcircle(o_x, o_y, 5);
    //����С��ƫ��
    int setover = static_cast<int>((size + 1) * 0.5 - 4);
    //����Ԫ��С��
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            fillcircle(o_x + i * setover * step, o_y + j * setover * step, 5);
        }
    }

    //��ӡ����
    settextcolor(RGB(0, 0, 0));
    settextstyle(20, 0, _T("΢���ź�"));

    for (int i = 0; i < size; i++) {
        TCHAR c = _T('A' + i);
        outtextxy(40 + i * step, 20, c);
    }
    settextstyle(20, 0, _T("Consolas"));
    for (int i = 0; i < size; i++) {
        if (i < 9) {
            TCHAR c = _T('1' + i);
            outtextxy(25, 40 + i * step, c);
        }
        else
        {
            TCHAR s[5];
            _stprintf_s(s, _T("%d"), 1 + i);    // �����ֵ���Ƚ����ָ�ʽ�����Ϊ�ַ���
            outtextxy(20, 35 + i * step, s);
        }
    }    
}

void Chess::init()
{
    ////������Ϸ����
    //initgraph(908, 903);

    ////�ػ洰�ڣ�ʹ���ھ���
    //HWND hWnd = GetHWnd();
    //SetWindowPos(hWnd, nullptr, 400, 80, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    ////��ʾ����ͼƬ
    //loadimage(0, _T("res/����.jpg"));

    drawChessBoard(BoardSize);

    //���ſ�ʼ��ʾ��
    mciSendString(_T("play res/start.wav"), 0, 0, 0);

    //��������ͼƬ
    loadimage(&chessBlackImg, _T("res/black.png"), ChessSize, ChessSize, true);
    loadimage(&chessWhiteImg, _T("res/white.png"), ChessSize, ChessSize, true);

    //��������
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            chessMap[i][j] = 0;
        }
    }
    playerFlag = true;
}

bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
    int col = (x - margin_x) / ChessSize;
    int row = (y - margin_y) / ChessSize;
    int leftTopPosX = margin_x + col * ChessSize;
    int leftTopPosY = margin_y + row * ChessSize;
    int offset = static_cast<int>(ChessSize * 0.4);   //�����ֵ����

    int len;
    bool selectPos = false;
    do {
        //���Ͻ��ж�
        len = static_cast<int>(sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY)));
        if (len < offset) {
            pos->m_row = row;
            pos->m_col = col;
            if (chessMap[pos->m_row][pos->m_col] == 0) {
                selectPos = true;
            }
            break;
        }
        //���Ͻ��ж�
        int x2 = leftTopPosX + ChessSize;
        int y2 = leftTopPosY;
        len = static_cast<int>(sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)));
        if (len < offset) {
            pos->m_row = row;
            pos->m_col = col + 1;
            if (chessMap[pos->m_row][pos->m_col] == 0) {
                selectPos = true;
            }
            break;
        }
        //���½��ж�
        x2 = leftTopPosX;
        y2 = leftTopPosY + ChessSize;
        len = static_cast<int>(sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)));
        if (len < offset) {
            pos->m_row = row + 1;
            pos->m_col = col;
            if (chessMap[pos->m_row][pos->m_col] == 0) {
                selectPos = true;
            }
            break;
        }
        //���½��ж�
        x2 = leftTopPosX + ChessSize;
        y2 = leftTopPosY + ChessSize;
        len = static_cast<int>(sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)));
        if (len < offset) {
            pos->m_row = row + 1;
            pos->m_col = col + 1;
            if (chessMap[pos->m_row][pos->m_col] == 0) {
                selectPos = true;
            }
            break;
        }
        mciSendString(_T("play res/��Ч���.mp3"), 0, 0, 0);
    } while (0);

    return selectPos;
}

ChessPos Chess::chesspos()
{
    return ChessPos();
}

void Chess::chessDown(ChessPos* pos, chess_kind_t kind)
{
    int x = static_cast<int>(margin_x + ChessSize * pos->m_col - ChessSize * 0.5);
    int y = static_cast<int>(margin_y + ChessSize * pos->m_row - ChessSize * 0.5);

    if (kind == CHESS_WHITE) {
        putimagePNG(x, y, &chessWhiteImg);
    }
    else {
        putimagePNG(x, y, &chessBlackImg);
    }
    mciSendString(_T("play res/down7.wav"), 0, 0, 0);
    updateChessMap(pos);
}

int Chess::getBoardSize()
{
    return BoardSize;
}

int Chess::getChessData(ChessPos* pos)
{
    return chessMap[pos->m_row][pos->m_col];
}

int Chess::getChessData(int row, int col)
{
    return chessMap[row][col];
}

bool Chess::checkOver()
{  
    //���ʤ���ѷ�
    if (isWin()) {

        Sleep(1500);        
        if (playerFlag == false) {
            //�ղ�������Ǻ��壬����ʤ��
            mciSendStringW(_T("play res/����.mp3"), 0, 0, 0);
            initgraph(897, 622);
            loadimage(0, _T("res/ʤ��1.jpg"));
            HWND hWnd= GetHWnd();
            SetWindowText(hWnd, _T("ʤ��"));
        }
        else {
            //AIʤ��
            mciSendStringW(_T("play res/ʧ��.mp3"), 0, 0, 0);
            initgraph(897, 622);
            loadimage(0, _T("res/ʧ��1.jpg"));
            HWND hWnd = GetHWnd();
            SetWindowText(hWnd, _T("ʧ��"));
        }

        //��ť����
        IMAGE again_bt; //�ߴ�Ϊ313*153
        IMAGE return_bt;
        loadimage(&again_bt, _T("res/again_button.png"), 313 / 2, 153 / 2, true);
        putimagePNG(470, 450, &again_bt);
        loadimage(&return_bt, _T("res/return_button.png"), 313 / 2, 153 / 2, true);
        putimagePNG(280, 450, &return_bt);
        
        //���¿�����Ϣ
        while (1) {
            
            m_control->updateMessage();

            if (m_control->getclickmsg() == AGAIN) {
                m_control->m_goon = true;
                return true;
            }
            else if (m_control->getclickmsg() == MENU) {
                m_control->m_menu = true;
                return true;
            }
        }
        //_getch();
        //closegraph();        
    }
    return false;
}

void Chess::updateChessMap(ChessPos* pos)
{    
    this->m_pos = *pos;
    chessMap[pos->m_row][pos->m_col] = (playerFlag ? CHESS_BLACK : CHESS_WHITE);
    playerFlag = !playerFlag;   //�����ڰ׷�
}

bool Chess::isWin()
{
    //ֻ��Ҫ�ж����ӵ���Χ������ֻ��Ҫ�жϵ�ǰ����
    vector<int> chessrange;
    int size = BoardSize;

    chess_kind_t kind;
    if (playerFlag == false) kind = CHESS_BLACK;
    else kind = CHESS_WHITE;

    int row = m_pos.m_row;
    int col = m_pos.m_col;

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
                chessrange.push_back(this->getChessData(cur_row, cur_col));
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
                chessrange.push_back(this->getChessData(cur_row, cur_col));
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
                chessrange.push_back(this->getChessData(cur_row, cur_col));
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
                chessrange.push_back(this->getChessData(cur_row, cur_col));
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
