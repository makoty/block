#include <windows.h>

#include <stdio.h>
#include <math.h>

#include "Bitmap.h"
#include "Graphics.h"

#include "debug.h"
#include "Fps.h"

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
#define DWIDTH  600
#define DHEIGHT 800
Graphics* graphics;

// シーン関係
#define SCENE_INVALID 0
#define SCENE_GAME    1
#define SCENE_TITLE   2

int scene, newScene;

// ゲーム本体関係
#define CHARASIZE 24

int playerX, playerY;
int ballX, ballY;

#define PLAYER_VELOCITY 5
#define BALL_VELOCITY 5

// タイトル関係
int blinkCount;

void changeScene(int _newScene);
void onSceneInit();
void onSceneGameInit();
void onSceneTitleInit();
void onSceneEnd();
//void onSceneGameEnd();
//void onSceneTitleEnd();
void onProc();
void onSceneGameProc();
void onSceneTitleProc();
void onDraw(Graphics& g);
void onSceneGameDraw(Graphics& g);
void onSceneTitleDraw(Graphics& g);
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
    }
}

void onSceneGameInit()
{
    playerX = DWIDTH / 2;
    playerY = DHEIGHT -100;
    ballX = DWIDTH / 2;
    ballY = 100;
}

void onSceneTitleInit()
{
    blinkCount = 0;
}

void onSceneEnd()
{
    switch (scene) {
    case SCENE_GAME:  /*onSceneGameEnd();*/ return;
    case SCENE_TITLE: /*onSceneTitleEnd();*/ return;
    }
}

void onProc()
{
    switch (scene) {
    case SCENE_GAME:  onSceneGameProc(); return;
    case SCENE_TITLE: onSceneTitleProc(); return;
    }
}

void onSceneGameProc()
{
    // 自キャラ移動
    if (IS_INPUT_DOWN(VK_LEFT))  playerX = (playerX - PLAYER_VELOCITY > 0) ? playerX - PLAYER_VELOCITY : 0;
    if (IS_INPUT_DOWN(VK_RIGHT)) playerX = (playerX + PLAYER_VELOCITY < DWIDTH - 1) ? playerX + PLAYER_VELOCITY : DWIDTH - 1;
    // if (IS_INPUT_DOWN(VK_UP))    playerY = (playerY - PLAYER_VELOCITY > 0) ? playerY - PLAYER_VELOCITY : 0;
    // if (IS_INPUT_DOWN(VK_DOWN))  playerY = (playerY + PLAYER_VELOCITY < DHEIGHT - 1) ? playerY + PLAYER_VELOCITY : DHEIGHT - 1;

    // ボール移動
    double dx = playerX - ballX, dy = playerY - ballY;
    double d = dx * dx + dy * dy;
    if (d > 0)
    {
        d = sqrt(d);
        ballX += (int)(dx * BALL_VELOCITY / d);
        ballY += (int)(dy * BALL_VELOCITY / d);
    }

    // 当たり判定
    int d2 = (playerX - ballX) * (playerX - ballX) + (playerY - ballY) * (playerY - ballY);
    if (d2 < ((CHARASIZE * CHARASIZE) << 2))
    {
        // 衝突
        // changeScene(SCENE_TITLE); // SCENE_GAME 再開
        // INPUT_RESET();
    }
}

void onSceneTitleProc()
{
    if (IS_INPUT_DOWN(VK_LEFT)
     || IS_INPUT_DOWN(VK_RIGHT)
     || IS_INPUT_DOWN(VK_UP)
     || IS_INPUT_DOWN(VK_DOWN)
     )
    {
        changeScene(SCENE_GAME);
    }

    blinkCount = (blinkCount + 1) % FPS;
}

void onDraw(Graphics& g)
{
    g.clear(DWIDTH, DHEIGHT);

    switch (scene) {
    case SCENE_GAME:  onSceneGameDraw(g); return;
    case SCENE_TITLE: onSceneTitleDraw(g); return;
    }
}

void onSceneGameDraw(Graphics& g)
{
    // 自キャラの描画
    HPEN pen1 = CreatePen(PS_SOLID, 2, RGB(0,255,0));
    g.setPen(pen1);
	g.drawRectangle(pen1, playerX, playerY, CHARASIZE * 5, CHARASIZE);

    // ボールの描画
    HPEN pen2 = CreatePen(PS_SOLID, 2, RGB(255,0,0));
    g.setPen(pen2);
    g.drawEllipse(ballX - CHARASIZE, ballY - CHARASIZE, ballX + CHARASIZE, ballY + CHARASIZE);

    DeleteObject(pen1);
    DeleteObject(pen2);
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
            }
            else
            {
                input[INPUT_UP] |= vk;
            }
            //TRACE("key d: %d, r: %d, u: %d\n", input[INPUT_DOWN], input[INPUT_REP], input[INPUT_UP]);
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

            //resetWindow(hwnd, hdc);
            EndPaint(hwnd , &ps);
        }
        return 0;
    case WM_DESTROY:
        //MessageBox(hwnd, TEXT("終了します"), TEXT("Block"), MB_ICONINFORMATION);
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
