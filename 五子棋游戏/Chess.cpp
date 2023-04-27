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

Chess::Chess(Control* control, int marginX, int marginY, int ChessSize)
{
    this->m_control = control;
    this->BoardSize = BoardSize;
    this->margin_x = marginX;
    this->margin_y = marginY;
    this->ChessSize = ChessSize;    
   
    control->ChessSize = ChessSize;
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

    m_control->drawGameInterface(BoardSize);

    //���ſ�ʼ��ʾ��
    mciSendString(_T("play res/music/start.wav"), 0, 0, 0);    

    //���̳�ʼ��
    m_control->chessMap.clear();
    for (int i = 0; i < BoardSize; i++) {
        vector<int> row;
        for (int j = 0; j < BoardSize; j++) {
            row.emplace_back(0);
        }
        m_control->chessMap.emplace_back(row);
    }
    m_control->step = 0;//�ӵ�0����ʼ
    m_control->manual.clear();    
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
            if (m_control->chessMap[pos->m_row][pos->m_col] == 0) {
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
            if (m_control->chessMap[pos->m_row][pos->m_col] == 0) {
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
            if (m_control->chessMap[pos->m_row][pos->m_col] == 0) {
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
            if (m_control->chessMap[pos->m_row][pos->m_col] == 0) {
                selectPos = true;
            }
            break;
        }        
    } while (0);

    return selectPos;
}

void Chess::setBoardSize(const int size)
{
    BoardSize = size;
}

int Chess::getBoardSize()
{
    return BoardSize;
}



bool Chess::isWin(enum chess_kind_t kind)
{
    //ֻ��Ҫ�ж����ӵ���Χ����
    vector<int> chessrange;
    int size = BoardSize;

    int row = m_control->m_pos.m_row;
    int col = m_control->m_pos.m_col;

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
                chessrange.push_back(m_control->getChessData(cur_row, cur_col));
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
                chessrange.push_back(m_control->getChessData(cur_row, cur_col));
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
                chessrange.push_back(m_control->getChessData(cur_row, cur_col));
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
                chessrange.push_back(m_control->getChessData(cur_row, cur_col));
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
