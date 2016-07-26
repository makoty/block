#include <windows.h>
#include <random>

#include <stdio.h>
#include <math.h>
#include <vector>
#include <iterator>
#include <time.h>
#include <iostream>
#include <sstream>

#include "Const.h"
#include "Bitmap.h"
#include "Graphics.h"

#include "debug.h"
#include "Fps.h"

#include "Bar.h"
#include "Ball.h"
#include "Block.h"

#include "Collision.h"

// FPS管理関係
#define FPS 60

// キー入力関係
#define INPUT_DOWN 0
#define INPUT_UP   1
#define INPUT_REP  2

unsigned char input[3]; // 入力状態監視(4方向キーしか使わないので8bitあれば十分)

#define IS_INPUT_DOWN(vk) (input[INPUT_DOWN] & (0x1 << (vk - VK_LEFT)))
#define IS_INPUT_UP(vk)   (input[INPUT_UP]   & (0x1 << (vk - VK_LEFT)))
#define IS_INPUT_REP(vk)  (input[INPUT_REP]  & (0x1 << (vk - VK_LEFT)))

#define INPUT_RESET() (input[INPUT_DOWN] = input[INPUT_UP] = 0)

// ディスプレイ関係
Graphics* graphics;

// シーン関係
#define SCENE_INVALID 0
#define SCENE_GAME    1
#define SCENE_TITLE   2
#define SCENE_CLEAR   3

int scene, newScene;

// ゲーム本体関係
std::vector<Ball*>  balls;
std::vector<Block*> blocks;

#define BLOCK_START_X     60
#define BLOCK_START_Y    100
#define BLOCK_COL_AMOUNT  5
#define BLOCK_ROW_AMOUNT  10

// 当たり判定省略用
short BLOCK_MAX_POS_Y = 0;

// 得点
short totalPoint;

// タイトル関係
int blinkCount;

#define IS_RESTART_ABLE -1
#define IS_RESTART_WAIT  0
#define IS_RESTART       1

bool isStart  = false;
int isRestart = IS_RESTART_ABLE;


void changeScene(int _newScene);
void onSceneInit();
void onSceneGameInit();
void onSceneTitleInit();
void onSceneClearInit();
void onSceneEnd();
void objectInit();
//void onSceneGameEnd();
//void onSceneTitleEnd();
void onProc();
void onSceneGameProc();
void onSceneTitleProc();
void onSceneClearProc();
void onDraw(Graphics& g);
void onSceneGameDraw(Graphics& g);
void onSceneTitleDraw(Graphics& g);
void onSceneClearDraw(Graphics& g);
void resetWindow(HWND hwnd, HDC hdc);

void changeScene(int _newScene)
{
    newScene = _newScene;
}

void onSceneInit()
{
    switch (scene) {
    case SCENE_GAME:  onSceneGameInit(); return;
    case SCENE_TITLE: onSceneTitleInit(); return;
    case SCENE_CLEAR: onSceneClearInit(); return;
    }
}

void onSceneGameInit()
{
    objectInit();

    totalPoint = 0;
    isStart = false;

    // プレイヤーの初期位置セット
    Bar& bar = Bar::getInstance();
    bar.setPos(DWIDTH / 2, DHEIGHT -100);

    // ブロック初期位置セット
	short w = (DWIDTH - (BLOCK_START_X * 2)) / BLOCK_COL_AMOUNT;
	short h = (DHEIGHT / 4) / BLOCK_ROW_AMOUNT;
	for (int i = 0; i < BLOCK_ROW_AMOUNT; i++) {
		for (int j = 0; j < BLOCK_COL_AMOUNT; j++) {
			Block *block = new Block;
			short x = w * j;
			short y = h * i;
			block->setPos(BLOCK_START_X + x, BLOCK_START_Y + y);
			block->setSize(w, h);
			blocks.push_back(block);

            // 最終の下線分位置を保持
            if (i == BLOCK_ROW_AMOUNT - 1 && j == BLOCK_COL_AMOUNT - 1) {
                BLOCK_MAX_POS_Y = BLOCK_START_Y + y + h;
            }
		}
	}
}

void objectInit() {
    for (Ball* ball : balls) {
        delete ball;
    }
    balls.clear();

    for (Block* block : blocks) {
        delete block;
    }
    blocks.clear();
}

void onSceneTitleInit()
{
    blinkCount = 0;
}

void onSceneClearInit()
{
    blinkCount = 0;
}

void onSceneEnd()
{
    switch (scene) {
    case SCENE_GAME:  /*onSceneGameEnd();*/ return;
    case SCENE_TITLE: /*onSceneTitleEnd();*/ return;
    case SCENE_CLEAR: /*onSceneClearEnd();*/ return;
    }
}

void onProc()
{
    switch (scene) {
    case SCENE_GAME:  onSceneGameProc(); return;
    case SCENE_TITLE: onSceneTitleProc(); return;
    case SCENE_CLEAR: onSceneClearProc(); return;
    }
}

void onSceneGameProc()
{
    std::random_device rnd;
    Bar& bar = Bar::getInstance();

    // 自キャラ移動
    short vX = bar.getVX();
    short vY = bar.getVY();

    if (IS_INPUT_DOWN(VK_LEFT))  bar.addPos(-1 * vX, 0);
    if (IS_INPUT_DOWN(VK_RIGHT)) bar.addPos(vX, 0);
    if (! isStart && balls.size() == 0 && IS_INPUT_DOWN(VK_UP)) {
        // ボールの初期位置セット
	    Ball *ball = new Ball;
	    ball->setPos(bar.getPosX() + bar.getWidth() / 2, bar.getPosY() - ball->getRadius());
	    ball->setColor(0);
        ball->setVec(ball->getVX() * rnd() % 10, -5);
	    balls.push_back(ball);
        isStart = true;
    }

	std::vector<Ball*> margeBalls;
	bool ballAddFlg = false;

    SYSTEMTIME st;
    GetSystemTime(&st);
    char szTime[25] = { 0 };

	for (auto ball : balls) {
		// ボール移動
        if (ball->getVisible()) ball->move();

		// 当たり判定
		float ox = ball->getPosX();
		float oy = ball->getPosY();
		float r  = ball->getRadius();
		float ax, ay, vx, vy;

		if (ball->getVY() > 0) {
			// ball & bar 上から
			ax = bar.getPosX();
			ay = bar.getPosY();
			vx = bar.getPosX() + bar.getWidth();
			vy = bar.getPosY();

			if (checkCollision(ox, oy, r, ax, ay, vx, vy)) {
				ball->changeVecY();

				bool recycle = false;

                if (ball->getVX() == 0) {
                    ball->setVec(rnd() % 10, ball->getVY());
                }

				for (auto b : balls) {
					if (b->getVisible()) continue;
					b->setPos(ox, oy + r);
					b->setVisible();
					b->setRandomColor();
					b->setVec(ball->getVX() * rnd() % 10, ball->getVY());
					recycle = true;
					break;
				}
				if (!recycle) {
					Ball *newBall = new Ball;
					newBall->setPos(ox, oy + r);
					newBall->setRandomColor();
					newBall->setVec(ball->getVX() * rnd() % 10, ball->getVY());
					margeBalls.push_back(newBall);
					ballAddFlg = true;
				}
			}

			// ball & block 上から
            if (ball->getPosY() < BLOCK_MAX_POS_Y) {
			    for (auto block : blocks) {
				    if (! block->getVisible()) continue;

				    ax = block->getPosX();
				    ay = block->getPosY();
				    vx = block->getPosX() + block->getWidth();
				    vy = block->getPosY();

				    if (checkCollision(ox, oy, r, ax, ay, vx, vy)) {
					    ball->changeVecY();
					    block->unVisible();

                        break;
				    }
			    }
            }

			// ball & wall 上から
			if (ball->getVisible() && checkCollision(ox, oy, r, 0, DHEIGHT, DWIDTH, DHEIGHT)) {
                totalPoint -= ball->getPoint();
				ball->unVisible();
			}
		} else if (ball->getVY() < 0) {
			// ball & wall 下から
			if (checkCollision(ox, oy, r, 0, 0, DWIDTH, 0)) {
				ball->changeVecY();
			}

			// ball & block 下から
            if (ball->getPosY() <= BLOCK_MAX_POS_Y + ball->getRadius()) {
			    for (auto block : blocks) {
				    if (! block->getVisible()) continue;

				    ax = block->getPosX();
				    ay = block->getPosY() + block->getHeight();
				    vx = block->getPosX() + block->getWidth();
				    vy = block->getPosY() + block->getHeight();

				    if (checkCollision(ox, oy, r, ax, ay, vx, vy)) {
					    ball->changeVecY();
					    block->unVisible();

                        break;
				    }
			    }
            }
		}

		if (ball->getVX() > 0) {
			// ball & wall 左から
			if (checkCollision(ox, oy, r, DWIDTH, 0, DWIDTH, DHEIGHT)) {
				ball->changeVecX();
			}
			// ball & block 左から
			for (auto block : blocks) {
				if (! block->getVisible()) continue;

				ax = block->getPosX();
				ay = block->getPosY();
				vx = block->getPosX();
				vy = block->getPosY() + block->getHeight();

				if (checkCollision(ox, oy, r, ax, ay, vx, vy)) {
					ball->changeVecX();
					block->unVisible();

                    break;
				}
			}
		} else if (ball->getVX() < 0) {
			// ball & wall 右から
			if (checkCollision(ox, oy, r, 0, 0, 0, DHEIGHT)) {
				ball->changeVecX();
			}
			// ball & block 右から
			for (auto block : blocks) {
				if (! block->getVisible()) continue;

				ax = block->getPosX() + block->getWidth();
				ay = block->getPosY();
				vx = block->getPosX() + block->getWidth();
				vy = block->getPosY() + block->getHeight();

				if (checkCollision(ox, oy, r, ax, ay, vx, vy)) {
					ball->changeVecX();
					block->unVisible();

                    break;
				}
			}
		}
    }

	if (ballAddFlg) {
		std::copy(margeBalls.begin(), margeBalls.end(), std::back_inserter(balls));
	}

    short changeMode = SCENE_INVALID;
    
    // クリア判定
    for (auto block : blocks) {
        if (! block->getVisible()) {
            changeMode = SCENE_CLEAR;
        } else {
            changeMode = SCENE_INVALID;
            break;
        }
    }
    if (changeMode == SCENE_INVALID) {
        // ゲームオーバー判定
        for (auto ball : balls) {
            if (! ball->getVisible()) {
                changeMode = SCENE_TITLE;
            } else {
                changeMode = SCENE_INVALID;
                break;
            }
        }
    }

    // シーン切替
    if (changeMode != SCENE_INVALID) {
        changeScene(changeMode);
    }

}

void onSceneTitleProc()
{
    if ((IS_INPUT_DOWN(VK_LEFT)
     || IS_INPUT_DOWN(VK_RIGHT)
     || IS_INPUT_DOWN(VK_UP)
     || IS_INPUT_DOWN(VK_DOWN)
     ) && isRestart == IS_RESTART_ABLE)
    {
        changeScene(SCENE_GAME);
    }

    blinkCount = (blinkCount + 1) % FPS;
}

void onSceneClearProc()
{
    if (IS_INPUT_DOWN(VK_LEFT)
     || IS_INPUT_DOWN(VK_RIGHT)
     || IS_INPUT_DOWN(VK_UP)
     || IS_INPUT_DOWN(VK_DOWN)
     )
    {
        isRestart = IS_RESTART;
        newScene = SCENE_INVALID;
        changeScene(SCENE_TITLE);
    }

    blinkCount = (blinkCount + 1) % FPS;
}

void onDraw(Graphics& g)
{
    g.clear(DWIDTH, DHEIGHT);

    switch (scene) {
    case SCENE_GAME:  onSceneGameDraw(g); return;
    case SCENE_TITLE: onSceneTitleDraw(g); return;
    case SCENE_CLEAR: onSceneClearDraw(g); return;
    }
}

void onSceneGameDraw(Graphics& g)
{
    // 自キャラの描画
    Bar& bar = Bar::getInstance();
    HPEN penBar = CreatePen(PS_SOLID, 2, RGB(0,255,0));
    g.setPen(penBar);
	g.drawRectangle(penBar, bar.getPosX(), bar.getPosY(), bar.getWidth(), bar.getHeight());

    // ボールの描画
    HPEN penBall = NULL;
    
    totalPoint = 0;
    for (auto ball : balls) {
		if (! ball->getVisible()) continue;

        penBall = CreatePen(PS_SOLID, 2, ball->getColor());

        g.setPen(penBall);
        g.drawEllipse(ball->getPosX() - ball->getRadius(), 
                      ball->getPosY() - ball->getRadius(), 
                      ball->getPosX() + ball->getRadius(),
                      ball->getPosY() + ball->getRadius());

        totalPoint += ball->getPoint();
    }
    if (penBall != NULL) DeleteObject(penBall);

	// ブロックの描画
	HPEN penBlock = CreatePen(PS_SOLID, 1, RGB(0,255,0));;
	for (auto block : blocks) {
		if (! block->getVisible()) continue;

		g.setPen(penBlock);
		short x,y,w,h;
		x = block->getPosX();
		y = block->getPosY();
		w = block->getWidth();
		h = block->getHeight();
		g.drawRectangle(penBlock, block->getPosX(), block->getPosY(), block->getWidth(), block->getHeight());
	}



    DeleteObject(penBar);
}

void onSceneTitleDraw(Graphics& g)
{
    if (blinkCount < FPS / 2)
    {
        HFONT font = CreateFont(40, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
                SHIFTJIS_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH | FF_SWISS, NULL);
        g.setFont(font);

        g.setTextColor(128, 128, 255);
        g.setTextAlign(TA_BASELINE | TA_CENTER | TA_NOUPDATECP);

        g.drawString(DWIDTH / 2, DHEIGHT / 2, "Push cursor key");

        g.setFont(NULL);
        DeleteObject(font);
    }
}

void onSceneClearDraw(Graphics& g)
{
    if (blinkCount < FPS / 2)
    {
        HFONT font = CreateFont(40, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
                SHIFTJIS_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH | FF_SWISS, NULL);
        g.setFont(font);

        g.setTextColor(128, 128, 255);
        g.setTextAlign(TA_BASELINE | TA_CENTER | TA_NOUPDATECP);

        std::stringstream ss;
        ss << totalPoint;
        std::string score = ss.str();

        g.drawString(DWIDTH / 2, DHEIGHT / 2, score.c_str());

        g.setFont(NULL);
        DeleteObject(font);
    }
}

void resetWindow(HWND hwnd, HDC hdc)
{
    // 入力テスト
    char testBuf[5];
    sprintf(testBuf, "%c%c%c%c", IS_INPUT_DOWN(VK_LEFT)?'<':' ', IS_INPUT_DOWN(VK_UP)?'^':' ', IS_INPUT_DOWN(VK_DOWN)?'v':' ', IS_INPUT_DOWN(VK_RIGHT)?'>':' ');
    TextOutA(hdc, 10, 10, testBuf, strlen(testBuf));
    sprintf(testBuf, "%c%c%c%c", IS_INPUT_REP(VK_LEFT)?'<':' ', IS_INPUT_REP(VK_UP)?'^':' ', IS_INPUT_REP(VK_DOWN)?'v':' ', IS_INPUT_REP(VK_RIGHT)?'>':' ');
    TextOutA(hdc, 10, 30, testBuf, strlen(testBuf));
    //TextOut(hdc, 10, 10, TEXT("test"), lstrlen(TEXT("test")));
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_KEYDOWN:
    case WM_KEYUP:
        {
            //   38
            //37 40 39
            //TRACE("key %c: %ld, rep: %ld, auto: %ld\n", (lp >> 31) ? 'u' : 'd', wp, lp & 0xffff, (lp >> 30) & 1);
            int vk = wp - VK_LEFT;
            if (vk < 0 || vk > 3) return 0;
            vk = 0x1 << vk;
            if (msg == WM_KEYDOWN)
            {
                input[INPUT_REP] |= vk; // リピート入力
                if (!(lp & 0x40000000)) // オートリピート
                {
                    input[INPUT_DOWN] |= vk;
                }

                isRestart = IS_RESTART_ABLE;
            }
            else
            {
                input[INPUT_UP] |= vk;
                isRestart = IS_RESTART_WAIT;
            }
            // TRACE("key d: %d, r: %d, u: %d\n", input[INPUT_DOWN], input[INPUT_REP], input[INPUT_UP]);
        }
        return 0;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd , &ps);

            if (graphics)
            {
                Graphics g(hdc);
                RECT rc;
                GetClientRect(hwnd , &rc);
                int w = rc.right - rc.left, h = rc.bottom - rc.top;
                if (w == DWIDTH && h == DHEIGHT && 0)
                {
                    g.copy(*graphics, 0, 0, w, h, 0, 0);
                }
                else
                {
                    g.copy(*graphics, 0, 0, w, h, 0, 0, DWIDTH, DHEIGHT);
                }
            }

            std::wstringstream ss;
            ss << totalPoint;
            std::wstring score = ss.str();

            SetTextColor(hdc, RGB(255,255,255));
            TextOut(hdc, 20, DHEIGHT - 40, TEXT("SCORE:"), lstrlen(TEXT("SCORE:")));
            TextOut(hdc, 100, DHEIGHT - 40, score.c_str(), lstrlen(score.c_str()));

            //resetWindow(hwnd, hdc);
            EndPaint(hwnd , &ps);
            ReleaseDC(hwnd, hdc);
        }
        return 0;
    case WM_DESTROY:
        //MessageBox(hwnd, TEXT("終了します"), TEXT("Block"), MB_ICONINFORMATION);
		for (auto ball : balls) {
			delete ball;
		}
		for (auto block : blocks) {
			delete block;
		}

        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // メインウィンドウ作成
    WNDCLASS winc;
    winc.style         = CS_HREDRAW | CS_VREDRAW;
    winc.lpfnWndProc   = wndProc;
    winc.cbClsExtra    =
    winc.cbWndExtra    = 0;
    winc.hInstance     = hInstance;
    winc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    winc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    winc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    winc.lpszMenuName  = NULL;
    winc.lpszClassName = TEXT("Block");
    if (!RegisterClass(&winc)) return 0;

    HWND hwnd = CreateWindow(
            TEXT("Block"), TEXT("Block"),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, DWIDTH, DHEIGHT, NULL, NULL,
            hInstance, NULL
        );
    if (!hwnd) return 0;

    // 画面バッファの作成
    HDC hdc = GetDC(hwnd);
    if (!hdc) return 0;
    Bitmap bitmap(hdc, DWIDTH, DHEIGHT);
    Graphics g(bitmap);
    graphics = &g;
    ReleaseDC(hwnd, hdc);

    // サイズ調整
    RECT rc;
    GetClientRect(hwnd, &rc);
    SetWindowPos(hwnd, NULL, 0, 0, DWIDTH + DWIDTH - (rc.right - rc.left),
                                   DHEIGHT + DHEIGHT - (rc.bottom - rc.top), SWP_NOMOVE | SWP_NOZORDER);
    ShowWindow(hwnd, SW_SHOW);

    // メインルーチン開始
    MSG msg;
    Fps fps(FPS);

    // 最初のシーン
    newScene = SCENE_INVALID;
    changeScene(SCENE_TITLE);

    fps.reset();

    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!GetMessage(&msg, NULL, 0, 0)) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // シーン変更
            if (newScene)
            {
                onSceneEnd();
                scene = newScene;
                newScene = SCENE_INVALID;
                onSceneInit();
            }

            // メインルーチン
            onProc();

            // キーアップを反映
            input[INPUT_DOWN] &= ~input[INPUT_UP];
            input[INPUT_REP] = input[INPUT_UP] = 0;

            // 描画ルーチン
            onDraw(g);

            // 画面更新
            InvalidateRect(hwnd, NULL, FALSE);

            fps.sleep();
        }
    }

    g.dispose();
    bitmap.dispose();

    return msg.wParam;;
}
