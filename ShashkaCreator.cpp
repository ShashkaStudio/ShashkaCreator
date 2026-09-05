#include "ShashkaCreator.h"

bool isRecreatingWindow = false;

int _shashkaLastClickedMenuID = -1;

window* currentWindow = nullptr;

UpdateFunction currentActiveUpdate = nullptr;

int mouseWheelDelta = 0;

wchar_t lastChar = 0;

bool keys[256] = {false};
bool prevKeys[256] = {false};

float deltaTime = 0;

float cameraX = 0.0f;
float cameraY = 0.0f;

LARGE_INTEGER qpcFrequency;
LARGE_INTEGER qpcLastTime;
bool isTimerInitialized = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_DESTROY) {
        if (isRecreatingWindow) {
            return 0;
        }

        PostQuitMessage(0);
    }

    if (msg == WM_CHAR) {
        lastChar = (wchar_t)wp;
        return 0;
    }

    if (msg == WM_MOUSEWHEEL) {
        short delta = GET_WHEEL_DELTA_WPARAM(wp);
        mouseWheelDelta = delta / WHEEL_DELTA;
        return 0;
    }

    return DefWindowProc(hwnd, msg, wp, lp);
}

void MakeWindow(window& object) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "ShashkaClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HMENU hDefaultMenu = CreateMenu();

    if (object.style == normal) {
        RECT r = { 0, 0, object.width, object.height };
        AdjustWindowRect(&r, WS_OVERLAPPED, FALSE);

        int fullWidth = r.right - r.left;
        int fullHeight = r.bottom - r.top;

        object.hwnd = CreateWindow("ShashkaClass", object.name.c_str(),
               WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE | WS_CLIPCHILDREN, object.x, object.y,
               fullWidth, fullHeight, NULL, hDefaultMenu, GetModuleHandle(NULL), NULL);
    }

    if (object.style == fullscreen) {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        object.hwnd = CreateWindow("ShashkaClass", object.name.c_str(),
               WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN, 0, 0,
               screenWidth, screenHeight, NULL, hDefaultMenu, GetModuleHandle(NULL), NULL);

        object.width = screenWidth;
        object.height = screenHeight;
    }

    HDC hdc = GetDC(object.hwnd);
    object.memDC = CreateCompatibleDC(hdc);
    object.hBuffer = CreateCompatibleBitmap(hdc, object.width, object.height);
    object.hOld = (HBITMAP)SelectObject(object.memDC, object.hBuffer);
    ReleaseDC(object.hwnd, hdc);

    currentWindow = &object;
}

void DisplayWindow(UpdateFunction userUpdate) {
    MSG msg;
    currentActiveUpdate = userUpdate;

    if (!isTimerInitialized) {
        QueryPerformanceFrequency(&qpcFrequency);
        QueryPerformanceCounter(&qpcLastTime);
        isTimerInitialized = true;
    }

    timeBeginPeriod(1);

    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;

            if (msg.message == WM_COMMAND) {
                _shashkaLastClickedMenuID = LOWORD(msg.wParam);
            }

            if (msg.message == WM_CHAR) {
                lastChar = (wchar_t)msg.wParam;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        else {
            LARGE_INTEGER qpcCurrentTime;
            QueryPerformanceCounter(&qpcCurrentTime);

            deltaTime = (float)(qpcCurrentTime.QuadPart - qpcLastTime.QuadPart) / (float)qpcFrequency.QuadPart;
            qpcLastTime = qpcCurrentTime;

            if (deltaTime > 0.1f) deltaTime = 0.016f;
            if (deltaTime < 0.0001f) deltaTime = 0.0001f;

            for (int i = 0; i < 256; i++) {
                prevKeys[i] = keys[i];

                keys[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
            }

            if (currentActiveUpdate != nullptr) {
                currentActiveUpdate();
            }

            mouseWheelDelta = 0;

            Sleep(1);
        }
    }

    timeEndPeriod(1);
}

void DrawSquare(window& update, square& object) {
    if (object.active == false) return;

    float radAngle = object.angle * 3.14159f / 180.0f;
    float s_sin = sinf(radAngle);
    float s_cos = cosf(radAngle);

    float hw = object.width / 2.0f;
    float hh = object.height / 2.0f;

    POINT pts[4];
    float corners[4][2] = { {-hw, -hh}, {hw, -hh}, {hw, hh}, {-hw, hh} };

    float screenX = object.x - cameraX;
    float screenY = object.y - cameraY;

    for (int i = 0; i < 4; i++) {
        pts[i].x = (long)(screenX + (corners[i][0] * s_cos - corners[i][1] * s_sin));
        pts[i].y = (long)(screenY + (corners[i][0] * s_sin + corners[i][1] * s_cos));
    }

    if (object.color.alpha == 255) {
        HBRUSH brush = CreateSolidBrush(RGB(object.color.red, object.color.green, object.color.blue));
        HBRUSH oldBrush = (HBRUSH)SelectObject(update.memDC, brush);

        Polygon(update.memDC, pts, 4);

        SelectObject(update.memDC, oldBrush);
        DeleteObject(brush);
    }

    else {
        HDC tempDC = CreateCompatibleDC(update.memDC);
        HBITMAP tempBitmap = CreateCompatibleBitmap(update.memDC, update.width, update.height);
        HBITMAP oldBmp = (HBITMAP)SelectObject(tempDC, tempBitmap);

        BitBlt(tempDC, 0, 0, update.width, update.height, update.memDC, 0, 0, SRCCOPY);

        HBRUSH brush = CreateSolidBrush(RGB(object.color.red, object.color.green, object.color.blue));
        HBRUSH oldBrush = (HBRUSH)SelectObject(tempDC, brush);

        Polygon(tempDC, pts, 4);

        BLENDFUNCTION blend = { 0 };
        blend.BlendOp = AC_SRC_OVER;
        blend.SourceConstantAlpha = object.color.alpha;

        AlphaBlend(
            update.memDC, 0, 0, update.width, update.height,
            tempDC, 0, 0, update.width, update.height,
            blend
        );

        SelectObject(tempDC, oldBrush);
        DeleteObject(brush);
        SelectObject(tempDC, oldBmp);
        DeleteObject(tempBitmap);
        DeleteDC(tempDC);
    }
}

void PaintWindow(window& object, RGB color) {
    RECT rect = {0, 0, object.width, object.height};
    HBRUSH brush = CreateSolidBrush(RGB(color.red, color.green, color.blue));
    FillRect(object.memDC, &rect, brush);
    DeleteObject(brush);
}

void BeginDraw(window& object) {
    RECT r = {0, 0, object.width, object.height};
    HBRUSH br = (HBRUSH)GetStockObject(BLACK_BRUSH);
    FillRect(object.memDC, &r, br);
}

void EndDraw(window& object) {
    HDC hdc = GetDC(object.hwnd);
    BitBlt(hdc, 0, 0, object.width, object.height, object.memDC, 0, 0, SRCCOPY);
    ReleaseDC(object.hwnd, hdc);
}

bool GetKey(int KeyCode) {
    return keys[KeyCode];
}

bool GetKeyDown(int KeyCode) {
    return keys[KeyCode] && !prevKeys[KeyCode];
}

bool GetKeyUp(int KeyCode) {
    return !keys[KeyCode] && prevKeys[KeyCode];
}

void LoadSprite(sprite& object) {
    object.image = (HBITMAP)LoadImage(NULL, object.path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}

void LoadSound(sound &object) {
    std::string closeCmd = "close " + object.alias;
    mciSendStringA(closeCmd.c_str(), NULL, 0, NULL);

    std::string openCmd = "open \"" + object.path + "\" type mpegvideo alias " + object.alias;
    MCIERROR err = mciSendStringA(openCmd.c_str(), NULL, 0, NULL);

    if (err != 0) {
        return;
    }
}

void PlaySound(sound &object) {
    if (object.path.empty() || object.alias.empty()) return;
    std::string cmd = "play " + object.alias + " from 0";
    mciSendStringA(cmd.c_str(), NULL, 0, NULL);
}

void PlayLoopSound(sound &object) {
    if (object.path.empty() || object.alias.empty()) return;
    std::string cmd = "play " + object.alias + " repeat";
    mciSendStringA(cmd.c_str(), NULL, 0, NULL);
}

void StopSound(sound &object) {
    if (object.path.empty() || object.alias.empty()) return;
    std::string cmd = "stop " + object.alias;
    mciSendStringA(cmd.c_str(), NULL, 0, NULL);
}

void StopAllSounds() {
    mciSendStringA("stop all", NULL, 0, NULL);
    mciSendStringA("close all", NULL, 0, NULL);
}

void PauseSound(const sound &object) {
    if (object.path.empty() || object.alias.empty()) return;
    std::string cmd = "pause " + object.alias;
    mciSendStringA(cmd.c_str(), NULL, 0, NULL);
}

void ResumeSound(const sound &object) {
    if (object.path.empty() || object.alias.empty()) return;
    std::string cmd = "resume " + object.alias;
    mciSendStringA(cmd.c_str(), NULL, 0, NULL);
}

void DrawText(window& update, text& object) {
    if (object.active == false) return;

    std::wstring finalFontName = L"Arial";

    if (!object.fontPath.empty()) {
        if (AddFontResourceExA(object.fontPath.c_str(), FR_PRIVATE, 0) != 0) {
            if (!object.fontName.empty()) {
                int fontNameLen = MultiByteToWideChar(CP_UTF8, 0, object.fontName.c_str(), (int)object.fontName.length(), NULL, 0);

                if (fontNameLen > 0) {
                    finalFontName.resize(fontNameLen);
                    MultiByteToWideChar(CP_UTF8, 0, object.fontName.c_str(), (int)object.fontName.length(), &finalFontName[0], fontNameLen);
                }
            }
        }
    }

    int winAngle = (int)(object.angle * 10.0f);

    int screenTextX = object.x - (int)cameraX;
    int screenTextY = object.y - (int)cameraY;

    int len = MultiByteToWideChar(CP_UTF8, 0, object.content.c_str(), (int)object.content.length(), NULL, 0);
    if (len <= 0) return;

    std::wstring wideText(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, object.content.c_str(), (int)object.content.length(), &wideText[0], len);

    if (object.color.alpha == 255) {
        HFONT hFont = CreateFontW(object.size, 0, winAngle, winAngle, FW_BOLD, FALSE, FALSE, FALSE,
                                  RUSSIAN_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
                                  CLEARTYPE_QUALITY, VARIABLE_PITCH, finalFontName.c_str());
        HFONT hOldFont = (HFONT)SelectObject(update.memDC, hFont);

        SetTextColor(update.memDC, RGB(object.color.red, object.color.green, object.color.blue));
        SetBkMode(update.memDC, TRANSPARENT);

        TextOutW(update.memDC, screenTextX, screenTextY, wideText.c_str(), (int)wideText.length());

        SelectObject(update.memDC, hOldFont);
        DeleteObject(hFont);
    }

    else {
        HDC tempDC = CreateCompatibleDC(update.memDC);
        HBITMAP tempBitmap = CreateCompatibleBitmap(update.memDC, update.width, update.height);
        HBITMAP oldBmp = (HBITMAP)SelectObject(tempDC, tempBitmap);

        BitBlt(tempDC, 0, 0, update.width, update.height, update.memDC, 0, 0, SRCCOPY);

        HFONT hFont = CreateFontW(object.size, 0, winAngle, winAngle, FW_BOLD, FALSE, FALSE, FALSE,
                                  RUSSIAN_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
                                  CLEARTYPE_QUALITY, VARIABLE_PITCH, finalFontName.c_str());
        HFONT hOldFont = (HFONT)SelectObject(tempDC, hFont);

        SetTextColor(tempDC, RGB(object.color.red, object.color.green, object.color.blue));
        SetBkMode(tempDC, TRANSPARENT);

        TextOutW(tempDC, screenTextX, screenTextY, wideText.c_str(), (int)wideText.length());

        BLENDFUNCTION blend = { 0 };
        blend.BlendOp = AC_SRC_OVER;
        blend.SourceConstantAlpha = object.color.alpha;

        AlphaBlend(update.memDC, 0, 0, update.width, update.height, tempDC, 0, 0, update.width, update.height, blend);

        SelectObject(tempDC, hOldFont);
        DeleteObject(hFont);
        SelectObject(tempDC, oldBmp);
        DeleteObject(tempBitmap);
        DeleteDC(tempDC);
    }

    RemoveFontResourceExA(object.fontPath.c_str(), FR_PRIVATE, 0);
}

void DrawSprite(window &update, sprite &object) {
    if (object.active == false || !object.image) return;

    BITMAP bm;
    GetObject(object.image, sizeof(bm), &bm);

    HDC hdcMem = CreateCompatibleDC(update.memDC);
    HBITMAP hOldMemBmp = (HBITMAP)SelectObject(hdcMem, object.image);

    HBITMAP hMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
    HDC hdcMask = CreateCompatibleDC(update.memDC);
    HBITMAP hOldMaskBmp = (HBITMAP)SelectObject(hdcMask, hMask);

    SetBkColor(hdcMem, object.keyColor);
    BitBlt(hdcMask, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
    BitBlt(hdcMask, 0, 0, bm.bmWidth, bm.bmHeight, hdcMask, 0, 0, DSTINVERT);

    float radAngle = object.angle * rad;
    float cosA = cosf(radAngle);
    float sinA = sinf(radAngle);

    float hw = object.width / 2.0f;
    float hh = object.height / 2.0f;

    float screenX = object.x - cameraX;
    float screenY = object.y - cameraY;

    POINT pt[3];
    pt[0].x = (long)(screenX + (-hw * cosA - (-hh) * sinA));
    pt[0].y = (long)(screenY + (-hw * sinA + (-hh) * cosA));
    pt[1].x = (long)(screenX + (hw * cosA - (-hh) * sinA));
    pt[1].y = (long)(screenY + (hw * sinA + (-hh) * cosA));
    pt[2].x = (long)(screenX + (-hw * cosA - hh * sinA));
    pt[2].y = (long)(screenY + (-hw * sinA + hh * cosA));

   if (object.alpha == 255) {
        PlgBlt(update.memDC, pt, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hMask, 0, 0);
    }

    else {
        HDC tempDC = CreateCompatibleDC(update.memDC);

        int scrW = GetDeviceCaps(update.memDC, HORZRES);
        int scrH = GetDeviceCaps(update.memDC, VERTRES);

        HBITMAP tempBitmap = CreateCompatibleBitmap(update.memDC, scrW, scrH);
        HBITMAP hOldTempBmp = (HBITMAP)SelectObject(tempDC, tempBitmap);

        BitBlt(tempDC, 0, 0, scrW, scrH, update.memDC, 0, 0, SRCCOPY);

        PlgBlt(tempDC, pt, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hMask, 0, 0);

        BLENDFUNCTION blend = { 0 };
        blend.BlendOp = AC_SRC_OVER;
        blend.SourceConstantAlpha = object.alpha;

        AlphaBlend(update.memDC, 0, 0, scrW, scrH, tempDC, 0, 0, scrW, scrH, blend);

        SelectObject(tempDC, hOldTempBmp);
        DeleteObject(tempBitmap);
        DeleteDC(tempDC);
    }

    SelectObject(hdcMem, hOldMemBmp);
    SelectObject(hdcMask, hOldMaskBmp);
    DeleteDC(hdcMem);
    DeleteDC(hdcMask);
    DeleteObject(hMask);
}

std::string toText(long long variable) {
    return std::to_string(variable);
}

std::string toText(int variable) {
    return std::to_string(variable);
}

std::string toText(float variable) {
    return std::to_string(variable);
}

std::string toText(double variable) {
    return std::to_string(variable);
}

std::string toText(short variable) {
    return std::to_string(variable);
}

bool IsCollide(square &objectA, square &objectB) {
    if (objectA.active == false || objectB.active == false) return false;

    float aLeft = objectA.x - (objectA.width / 2.0f);
    float aTop  = objectA.y - (objectA.height / 2.0f);

    float bLeft = objectB.x - (objectB.width / 2.0f);
    float bTop  = objectB.y - (objectB.height / 2.0f);

    return (aLeft < bLeft + objectB.width &&
            aLeft + objectA.width > bLeft &&
            aTop < bTop + objectB.height &&
            aTop + objectA.height > bTop);
}

void OnTriggerEnter(trigger &triggerObject, square &targetObject) {
    if (triggerObject.area.active == false) return;
    if (!triggerObject.active) return;

    if (IsCollide(triggerObject.area, targetObject) && targetObject.tag == triggerObject.targetTag) {
        triggerObject.action();
        triggerObject.active = false;
    }
}

void SwitchUpdate(UpdateFunction nextUpdate) {
    currentActiveUpdate = nextUpdate;
}

bool IsCursorTouched(window& update, square& object) {
    if (object.active == false) return false;

    POINT m;
    GetCursorPos(&m);
    ScreenToClient(update.hwnd, &m);

    float left   = object.x - (object.width / 2.0f);
    float right  = object.x + (object.width / 2.0f);
    float top    = object.y - (object.height / 2.0f);
    float bottom = object.y + (object.height / 2.0f);

    if (m.x >= left && m.x <= right && m.y >= top && m.y <= bottom) {
        return true;
    }

    return false;
}

void UpdateUIButton(window &update, UIButton &object) {
    if (!object.box.active) return;

    if (object.clickTimer > 0) {
        object.clickTimer -= deltaTime;
        object.isClicked = false;
        return;
    }

    if (IsCursorTouched(update, object.box)) {
        object.isHovered = true;
        object.box.color = object.hoverColor;

        if (keys[1]) {
            object.isClicked = true;
            object.clickTimer = 0.1f;
        }
    }

    else {
        object.isHovered = false;
        object.isClicked = false;
        object.box.color = object.normalColor;
    }
}

void UpdateSpriteButton(window &update, spriteButton &object) {
    if (!object.box.active) return;

    if (object.clickTimer > 0) {
        object.clickTimer -= deltaTime;
        object.isClicked = false;
        return;
    }

    if (IsCursorTouched(update, object.box)) {
        object.isHovered = true;
        if (keys[1]) {
            object.isClicked = true;
            object.clickTimer = 0.1f;
        }
    }

    else {
        object.isHovered = false;
        object.isClicked = false;
    }
}

void DrawUIButton(window &update, UIButton &object) {
    if (!object.box.active) return;

    object.label.x = object.box.x - (object.box.width / 2.0f) + 10;
    object.label.y = object.box.y - (object.label.size / 2.0f);

    DrawSquare(update, object.box);
    DrawText(update, object.label);
}

void DrawSpriteButton(window &update, spriteButton &object) {
    if (!object.box.active) return;

    sprite* currentImage;

    if (object.isHovered == true) {
        currentImage = &object.hoverImage;
    }

    else {
        currentImage = &object.normalImage;
    }

    currentImage->x = object.box.x;
    currentImage->y = object.box.y;
    currentImage->width = object.box.width;
    currentImage->height = object.box.height;

    DrawSprite(update, *currentImage);

    object.label.x = object.box.x - (object.box.width / 2.0f) + 10;
    object.label.y = object.box.y - (object.label.size / 2.0f);

    DrawText(update, object.label);
}

int ShowMessage(window &update, message &object) {
    if (object.active == false) {
        return 39217;
    }

    int contentLen = MultiByteToWideChar(CP_UTF8, 0, object.content.c_str(), (int)object.content.length(), NULL, 0);
    std::wstring wideContent(contentLen, L'\0');

    if (contentLen > 0) {
        MultiByteToWideChar(CP_UTF8, 0, object.content.c_str(), (int)object.content.length(), &wideContent[0], contentLen);
    }

    int titleLen = MultiByteToWideChar(CP_UTF8, 0, object.title.c_str(), (int)object.title.length(), NULL, 0);
    std::wstring wideTitle(titleLen, L'\0');

    if (titleLen > 0) {
        MultiByteToWideChar(CP_UTF8, 0, object.title.c_str(), (int)object.title.length(), &wideTitle[0], titleLen);
    }

    return MessageBoxW(update.hwnd, wideContent.c_str(), wideTitle.c_str(), object.icon | object.type);
}

void UpdateInputfield(window &update, inputfield &object) {
    if (!object.box.active) return;

    if (GetKeyDown(LeftMouse)) {
        object.isFocused = IsCursorTouched(update, object.box);
    }

    if (object.isFocused && lastChar != 0) {
        if (lastChar == L'\b') {
            if (!object.content.empty()) {
                while (!object.content.empty()) {
                    unsigned char c = object.content.back();
                    object.content.pop_back();
                    if ((c & 0xC0) != 0x80) break;
                }
            }
        }

        else if (lastChar >= 32) {
            if (object.content.length() < (size_t)object.maxChars * 2) {
                if (lastChar <= 255 && (lastChar >= 192 || lastChar == 184 || lastChar == 168)) {
                    wchar_t wch = 0;
                    MultiByteToWideChar(1251, 0, (char*)&lastChar, 1, &wch, 1);

                    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wch, 1, NULL, 0, NULL, NULL);

                    if (size_needed > 0) {
                        std::string utf8Char(size_needed, 0);
                        WideCharToMultiByte(CP_UTF8, 0, &wch, 1, &utf8Char[0], size_needed, NULL, NULL);
                        object.content += utf8Char;
                    }
                }

                else {
                    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &lastChar, 1, NULL, 0, NULL, NULL);

                    if (size_needed > 0) {
                        std::string utf8Char(size_needed, 0);
                        WideCharToMultiByte(CP_UTF8, 0, &lastChar, 1, &utf8Char[0], size_needed, NULL, NULL);
                        object.content += utf8Char;
                    }
                }
            }
        }

        lastChar = 0;
    }

    object.label.content = object.content;

    if (object.isFocused && (int)(GetTickCount() / 500) % 2 == 0) {
        object.label.content += "|";
    }
}

void DrawInputfield(window &update, inputfield &object) {
    if (!object.box.active) return;

    DrawSquare(update, object.box);

    object.label.x = object.box.x - (object.box.width / 2.0f) + 10;
    object.label.y = object.box.y - (object.label.size / 2.0f);

    DrawText(update, object.label);
}

void Quit() {
    PostQuitMessage(0);
}

void ForceClose() {
    ExitProcess(0);
}

void LoadVideo(window& update, video& object) {
    if (object.path.empty() || object.alias.empty()) return;

    std::string cmd = "open \"" + object.path + "\" type mpegvideo alias " + object.alias +
                      " parent " + std::to_string((uintptr_t)update.hwnd) + " style child";

    if (mciSendStringA(cmd.c_str(), NULL, 0, NULL) != 0) return;

    std::string posCmd = "put " + object.alias + " window at " +
                         std::to_string(object.x) + " " + std::to_string(object.y) + " " +
                         std::to_string(object.width) + " " + std::to_string(object.height);

    mciSendStringA(posCmd.c_str(), NULL, 0, NULL);
    return;
}

void PlayVideo(const video& object) {
    if (!object.active) return;
    std::string cmd = "play " + object.alias;
    mciSendStringA(cmd.c_str(), NULL, 0, NULL);
}

void CloseVideo(video& object) {
    if (!object.active) return;
    std::string cmd = "close " + object.alias;
    mciSendStringA(cmd.c_str(), NULL, 0, NULL);
}

void CloseAllVideos() {
    mciSendStringA("close all", NULL, 0, NULL);
}

void PauseVideo(const video& object) {
    if (!object.active) return;
    std::string cmd = "pause " + object.alias;
    mciSendStringA(cmd.c_str(), NULL, 0, NULL);
}

void ResumeVideo(const video& object) {
    if (!object.active) return;
    std::string cmd = "resume " + object.alias;
    mciSendStringA(cmd.c_str(), NULL, 0, NULL);
}


void UpdateScrollbar(window& update, scrollbar& object) {
    if (object.box.active == false || object.slider.active == false) return;

    if (IsCursorTouched(update, object.slider) && keys[1]) {
        object.isDragged = true;
    }

    if (!keys[1]) {
        object.isDragged = false;
    }

    if (object.direction == horizontal) {
        POINT p;
        int mouseX;

        GetCursorPos(&p);
        ScreenToClient(update.hwnd, &p);

        mouseX = p.x;

        int worldMouseX = mouseX + (int)cameraX;

        if (object.isDragged) {
            int newX = worldMouseX;

            int bodyLeftEdge = object.box.x - (object.box.width / 2);
            int bodyRightEdge = object.box.x + (object.box.width / 2);

            int minX = bodyLeftEdge + (object.slider.width / 2);
            int maxX = bodyRightEdge - (object.slider.width / 2);

            if (newX < minX) newX = minX;
            if (newX > maxX) newX = maxX;

            object.slider.x = newX;
        }

        float maxPath = (float)(object.box.width - object.slider.width);

        if (maxPath > 0) {
            float startX = (float)(object.box.x - (object.box.width / 2) + (object.slider.width / 2));

            float currentPath = (float)(object.slider.x - startX);

            object.filled = currentPath / maxPath;
        }
    }

    else if (object.direction == vertical) {
        POINT p;
        int mouseY;

        GetCursorPos(&p);
        ScreenToClient(update.hwnd, &p);

        mouseY = p.y;

        int worldMouseY = mouseY + (int)cameraY;

        if (object.isDragged) {
            int newY = worldMouseY;

            int bodyTopEdge = object.box.y - (object.box.height / 2);
            int bodyDownEdge = object.box.y + (object.box.height / 2);

            int minY = bodyTopEdge + (object.slider.height / 2);
            int maxY = bodyDownEdge - (object.slider.height / 2);

            if (newY < minY) newY = minY;
            if (newY > maxY) newY = maxY;

            object.slider.y = newY;
        }

        float maxPath = (float)(object.box.height - object.slider.height);

        if (maxPath > 0) {
            float startY = (float)(object.box.y - (object.box.height / 2) + (object.slider.height / 2));

            float currentPath = (float)(object.slider.y - startY);

            object.filled = currentPath / maxPath;
        }
    }

    else {
        return;
    }
}

void DrawScrollbar(window& update, scrollbar& object) {
    if (object.box.active == false || object.slider.active == false) return;

    DrawSquare(update, object.box);
    DrawSquare(update, object.slider);
}

mouse GetMousePosition(const window& update) {
    POINT cp;
    GetCursorPos(&cp);
    ScreenToClient(update.hwnd, &cp);

    mouse pos;
    pos.x = cp.x;
    pos.y = cp.y;
    return pos;
}

void SetVideoVolume(const video& object, int volume) {
    if (!object.active) return;

    if (volume < 0) volume = 0;
    if (volume > 1000) volume = 1000;

    std::string cmd = "setaudio " + object.alias + " volume to " + std::to_string(volume);
    mciSendStringA(cmd.c_str(), NULL, 0, NULL);
}

void SetSoundVolume(const sound& object, int volume) {
    if (volume < 0) volume = 0;
    if (volume > 1000) volume = 1000;

    std::string cmd = "setaudio " + object.alias + " volume to " + std::to_string(volume);
    mciSendStringA(cmd.c_str(), NULL, 0, NULL);
}

std::wstring UTF8ToUTF16(const std::string& utf8Str) {
    if (utf8Str.empty()) return L"";

    int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, &utf8Str[0], (int)utf8Str.size(), NULL, 0);
    std::wstring utf16Str(sizeNeeded, 0);
    MultiByteToWideChar(CP_UTF8, 0, &utf8Str[0], (int)utf8Str.size(), &utf16Str[0], sizeNeeded);

    return utf16Str;
}

void MakeMenu(window& update, headmenu& object) {
    object.hmn = CreateMenu();
    SetMenu(update.hwnd, object.hmn);
}

void BeginMenu(menu& object) {
    object.hmn = CreatePopupMenu();
}

void AddMenu(menu& parent, submenu& object) {
    std::wstring wideContent = UTF8ToUTF16(object.content);
    AppendMenuW(parent.hmn, MF_STRING, object.id, wideContent.c_str());
}

void SeparateMenu(menu& object) {
    AppendMenuW(object.hmn, MF_SEPARATOR, 0, NULL);
}

void EndMenu(window& update, headmenu& parent, menu& object) {
    std::wstring wideContent = UTF8ToUTF16(object.content);
    AppendMenuW(parent.hmn, MF_POPUP, (UINT_PTR)object.hmn, wideContent.c_str());
    SetMenu(update.hwnd, parent.hmn);
    DrawMenuBar(update.hwnd);
}

bool ClickMenu(const submenu& object) {
    if (_shashkaLastClickedMenuID == object.id) {
        _shashkaLastClickedMenuID = -1;
        return true;
    }
    return false;
}

void BlockCursor() {
    POINT currentPos;
    GetCursorPos(&currentPos);

    RECT lockRect;
    lockRect.left   = currentPos.x;
    lockRect.top    = currentPos.y;
    lockRect.right  = currentPos.x + 1;
    lockRect.bottom = currentPos.y + 1;

    ClipCursor(&lockRect);
}

void ReleaseCursor() {
    ClipCursor(NULL);
}

void HideCursor() {
    ShowCursor(FALSE);
}

void ViewCursor() {
    ShowCursor(TRUE);
}

void RemakeWindow(window& object) {
    SetWindowPos(object.hwnd, NULL, object.x, object.y, object.width, object.height, SWP_NOMOVE | SWP_NOZORDER);
}

int GetMouseWheel() {
    if (mouseWheelDelta > 0) {
        return UpMouse;
    }

    else if (mouseWheelDelta < 0) {
        return DownMouse;
    }

    else {
        return NormalMouse;
    }
}
