#include "Chess.h"
#include "ChessGame.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

//加载png透明格式图片
void putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
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
    ////创建游戏窗口
    //initgraph(908, 903);

    ////重绘窗口，使窗口居中
    //HWND hWnd = GetHWnd();
    //SetWindowPos(hWnd, nullptr, 400, 80, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    ////显示棋盘图片
    //loadimage(0, _T("res/棋盘.jpg"));

    m_control->drawGameInterface(BoardSize);

    //播放开始提示音
    mciSendString(_T("play res/music/start.wav"), 0, 0, 0);    

    //棋盘初始化
    m_control->chessMap.clear();
    for (int i = 0; i < BoardSize; i++) {
        vector<int> row;
        for (int j = 0; j < BoardSize; j++) {
            row.emplace_back(0);
        }
        m_control->chessMap.emplace_back(row);
    }
    m_control->step = 0;//从第0步开始
    m_control->manual.clear();    
}



bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
    int col = (x - margin_x) / ChessSize;
    int row = (y - margin_y) / ChessSize;
    int leftTopPosX = margin_x + col * ChessSize;
    int leftTopPosY = margin_y + row * ChessSize;
    int offset = static_cast<int>(ChessSize * 0.4);   //点击阈值设置

    int len;
    bool selectPos = false;
    do {
        //左上角判断
        len = static_cast<int>(sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY)));
        if (len < offset) {
            pos->m_row = row;
            pos->m_col = col;
            if (m_control->chessMap[pos->m_row][pos->m_col] == 0) {
                selectPos = true;
            }
            break;
        }
        //右上角判断
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
        //左下角判断
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
        //右下角判断
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
    //只需要判断落子点周围方向
    vector<int> chessrange;
    int size = BoardSize;

    int row = m_control->m_pos.m_row;
    int col = m_control->m_pos.m_col;

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
                chessrange.push_back(m_control->getChessData(cur_row, cur_col));
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
                chessrange.push_back(m_control->getChessData(cur_row, cur_col));
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
                chessrange.push_back(m_control->getChessData(cur_row, cur_col));
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
                chessrange.push_back(m_control->getChessData(cur_row, cur_col));
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
