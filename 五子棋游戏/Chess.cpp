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
    //设置格子大小
    int step = 45;

    //设置棋盘大小
    int graphsize = step * (size + 1);

    //初始化绘图窗口
    initgraph(graphsize, graphsize);
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
    line(step, step, step, size * step);
    line(step, step, size * step, step);
    line(step, size * step, size * step, size * step);
    line(size * step, step, size * step, size * step);
    //画格线，宽度为2个像素
    setlinestyle(PS_SOLID, 2);
    for (int i = 2; i <= 18; i++)
    {
        line(i * step, 1 * step, i * step, size * step);//画竖线
        line(1 * step, i * step, size * step, i * step);//画横线
    }

    setfillcolor(RGB(0, 0, 0));//填充色设置为黑色
    //天元坐标
    int o_x = static_cast<int>(step * (size + 1) * 0.5);
    int o_y = o_x;
    fillcircle(o_x, o_y, 5);
    //计算小星偏距
    int setover = static_cast<int>((size + 1) * 0.5 - 4);
    //画天元和小星
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            fillcircle(o_x + i * setover * step, o_y + j * setover * step, 5);
        }
    }

    //打印坐标
    settextcolor(RGB(0, 0, 0));
    settextstyle(20, 0, _T("微软雅黑"));

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
            _stprintf_s(s, _T("%d"), 1 + i);    // 输出数值，先将数字格式化输出为字符串
            outtextxy(20, 35 + i * step, s);
        }
    }    
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

    drawChessBoard(BoardSize);

    //播放开始提示音
    mciSendString(_T("play res/start.wav"), 0, 0, 0);

    //加载棋子图片
    loadimage(&chessBlackImg, _T("res/black.png"), ChessSize, ChessSize, true);
    loadimage(&chessWhiteImg, _T("res/white.png"), ChessSize, ChessSize, true);

    //棋盘清理
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
    int offset = static_cast<int>(ChessSize * 0.4);   //点击阈值设置

    int len;
    bool selectPos = false;
    do {
        //左上角判断
        len = static_cast<int>(sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY)));
        if (len < offset) {
            pos->m_row = row;
            pos->m_col = col;
            if (chessMap[pos->m_row][pos->m_col] == 0) {
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
            if (chessMap[pos->m_row][pos->m_col] == 0) {
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
            if (chessMap[pos->m_row][pos->m_col] == 0) {
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
            if (chessMap[pos->m_row][pos->m_col] == 0) {
                selectPos = true;
            }
            break;
        }
        mciSendString(_T("play res/无效点击.mp3"), 0, 0, 0);
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
    //如果胜负已分
    if (isWin()) {

        Sleep(1500);        
        if (playerFlag == false) {
            //刚才走棋的是黑棋，棋手胜利
            mciSendStringW(_T("play res/不错.mp3"), 0, 0, 0);
            initgraph(897, 622);
            loadimage(0, _T("res/胜利1.jpg"));
            HWND hWnd= GetHWnd();
            SetWindowText(hWnd, _T("胜利"));
        }
        else {
            //AI胜利
            mciSendStringW(_T("play res/失败.mp3"), 0, 0, 0);
            initgraph(897, 622);
            loadimage(0, _T("res/失败1.jpg"));
            HWND hWnd = GetHWnd();
            SetWindowText(hWnd, _T("失败"));
        }

        //按钮加载
        IMAGE again_bt; //尺寸为313*153
        IMAGE return_bt;
        loadimage(&again_bt, _T("res/again_button.png"), 313 / 2, 153 / 2, true);
        putimagePNG(470, 450, &again_bt);
        loadimage(&return_bt, _T("res/return_button.png"), 313 / 2, 153 / 2, true);
        putimagePNG(280, 450, &return_bt);
        
        //更新控制信息
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
    playerFlag = !playerFlag;   //交换黑白方
}

bool Chess::isWin()
{
    //只需要判断落子点周围方向，且只需要判断当前棋手
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
                chessrange.push_back(this->getChessData(cur_row, cur_col));
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
                chessrange.push_back(this->getChessData(cur_row, cur_col));
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
                chessrange.push_back(this->getChessData(cur_row, cur_col));
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
                chessrange.push_back(this->getChessData(cur_row, cur_col));
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
