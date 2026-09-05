#ifndef SHASHKACREATOR_H
#define SHASHKACREATOR_H

#include <windows.h>
#include <objbase.h>
#include <math.h>
#include <mmsystem.h>
#include <string>
#include <clocale>

#define Start int main

#define LeftMouse 0x01
#define RightMouse 0x02

#define UpMouse 0x011
#define DownMouse 0x021
#define NormalMouse 0x031

#define KeyW 0x57
#define KeyA 0x41
#define KeyS 0x53
#define KeyD 0x44

#define KeyC 0x43
#define KeyZ 0x5A
#define KeyX 0x58
#define KeyV 0x56
#define KeyB 0x42
#define KeyN 0x4E
#define KeyM 0x4D

#define KeyT 0x54
#define KeyY 0x59
#define KeyU 0x55
#define KeyI 0x49
#define KeyO 0x4F
#define KeyP 0x50

#define KeyG 0x47
#define KeyH 0x48
#define KeyJ 0x4A
#define KeyK 0x4B
#define KeyL 0x4C

#define KeyOEM 0xC0

#define KeyUP 0x26
#define KeyDOWN 0x28
#define KeyLEFT 0x25
#define KeyRIGHT 0x27

#define KeySPACE 0x20
#define KeyESCAPE 0x1B
#define KeyENTER  0x0D
#define KeyLSHIFT 0xA0
#define KeyRSHIFT 0xA1
#define KeyLCONTROL 0xA2
#define KeyTAB 0x09
#define KeyLAlt 0xA4
#define KeyRAlt 0xA5

#define KeyE 0x45
#define KeyF 0x46
#define KeyQ 0x51
#define KeyR 0x52

#define Key0 0x30
#define Key1 0x31
#define Key2 0x32
#define Key3 0x33
#define Key4 0x34
#define Key5 0x35
#define Key6 0x36
#define Key7 0x37
#define Key8 0x38
#define Key9 0x39

#define blackMask 0x00000000
#define whiteMask 0x00FFFFFF

#define ok MB_OK
#define yes_no MB_YESNO
#define ok_cancel MB_OKCANCEL
#define yes_no_cancel MB_YESNOCANCEL
#define retry_cancel MB_RETRYCANCEL
#define help MB_HELP
#define abort_retry_ignore MB_ABORTRETRYIGNORE
#define cancel_try_continue MB_CANCELTRYCONTINUE

#define error MB_ICONERROR
#define question MB_ICONQUESTION
#define warning MB_ICONWARNING
#define information MB_ICONINFORMATION

#define idYes 6
#define idNo 7
#define idOk 1
#define idCancel 2
#define idAbort 3
#define idContinue 11
#define idIgnore 5
#define idRetry 4
#define idAgain 10

#define horizontal 0
#define vertical 1

#define fullscreen 0
#define normal 1

typedef void (*UpdateFunction)();
typedef void (*TriggerAction)();

extern float deltaTime;

extern float cameraX;
extern float cameraY;

extern UpdateFunction currentActiveUpdate;

struct RGB {
    int red, green, blue;
    unsigned char alpha = 255;
};

struct window {
    int style;
    std::string name;
    int x, y;
    int width, height;
    HWND hwnd;
    HDC memDC;
    HBITMAP hBuffer;
    HBITMAP hOld;
};

struct square {
    float x, y;
    float width, height;
    float angle;
    RGB color;
    std::string tag;
    bool active;
};

struct sprite {
    const char* path;
    HBITMAP image;
    float x, y;
    float width, height;
    float angle;
    unsigned char alpha = 255;
    COLORREF keyColor;
    bool active;
};

struct text {
    std::string content;
    int x;
    int y;
    int size;
    float angle;
    RGB color;
    std::string fontPath;
    std::string fontName;
    bool active;
};

struct sound {
    std::string path;
    std::string alias;
};

struct trigger {
    square area;
    std::string targetTag;
    TriggerAction action;
    bool active = true;
};

struct UIButton {
    square box;
    RGB normalColor;
    RGB hoverColor;
    text label;
    bool isHovered;
    bool isClicked;
    float clickTimer = 0;
};

struct spriteButton {
    square box;
    sprite normalImage;
    sprite hoverImage;
    text label;
    bool isHovered;
    bool isClicked;
    float clickTimer = 0;
};

struct message {
    std::string title;
    std::string content;
    UINT icon;
    UINT type;
    bool active;
};

struct inputfield {
    square& box;
    text& label;
    std::string content;
    int maxChars = 20;
    bool isFocused = false;
};

struct video {
    std::string path;
    std::string alias;
    int x, y;
    int width, height;
    bool active;
};

struct scrollbar {
    square& box;
    square& slider;
    int direction;
    bool isDragged;
    float filled = 0.0f;
};

struct mouse {
    int x, y;
};

struct headmenu {
    HMENU hmn;
};

struct menu {
    std::string content;
    HMENU hmn;
};

struct submenu {
    std::string content;
    int id;
};

const float rad = 3.14159f / 180.0f;

const RGB red = {255, 0, 0, 255};
const RGB green = {0, 255, 0, 255};
const RGB blue = {0, 0, 255, 255};
const RGB white = {255, 255, 255, 255};
const RGB purple = {255, 0, 255, 255};
const RGB yellow = {255, 255, 0, 255};
const RGB lightBlue = {0, 255, 255, 255};
const RGB black = {0, 0, 0, 255};

void MakeWindow(window& object);

void DisplayWindow(UpdateFunction userUpdate);

void PaintWindow(window& object, RGB color);

void DrawSquare(window& update, square& object);

void BeginDraw(window& object);

void EndDraw(window& object);

bool GetKey(int KeyCode);

bool GetKeyDown(int KeyCode);

bool GetKeyUp(int KeyCode);

void LoadSprite(sprite& object);

void DrawSprite(window &update, sprite &object);

void DrawText(window& update, text& object);

void LoadSound(sound &object);

void PlaySound(sound &object);

void PlayLoopSound(sound &object);

void StopSound(sound &object);

void StopAllSounds();

std::string toText(long long variable);

std::string toText(int variable);

std::string toText(double variable);

std::string toText(float variable);

std::string toText(short variable);

bool IsCollide(square &objectA, square &objectB);

void OnTriggerEnter(trigger &triggerObject, square &targetObject);

void SwitchUpdate(UpdateFunction nextUpdate);

void UpdateUIButton(window &update, UIButton &object);

void DrawUIButton(window &update, UIButton &object);

void UpdateSpriteButton(window &update, spriteButton &object);

void DrawSpriteButton(window &update, spriteButton &object);

int ShowMessage(window &update, message &object);

bool IsCursorTouched(window& update, square& object);

void UpdateInputfield(window &update, inputfield &object);

void DrawInputfield(window &update, inputfield &object);

void Quit();

void ForceClose();

void LoadVideo(window& update, video& object);

void PlayVideo(const video& object);

void CloseVideo(video& object);

void CloseAllVideos();

void UpdateScrollbar(window& update, scrollbar& object);

void DrawScrollbar(window& update, scrollbar& object);

mouse GetMousePosition(const window& update);

void PauseVideo(const video& object);

void ResumeVideo(const video& object);

void PauseSound(const sound &object);

void ResumeSound(const sound &object);

void SetVideoVolume(const video& object, int volume);

void SetSoundVolume(const sound& object, int volume);

std::wstring UTF8ToUTF16(const std::string& utf8Str);

void MakeMenu(window& update, headmenu& object);

void BeginMenu(menu& object);

void AddMenu(menu& parent, submenu& object);

void EndMenu(window& update, headmenu& parent, menu& object);

bool ClickMenu(const submenu& object);

void SeparateMenu(menu& object);

void BlockCursor();

void ReleaseCursor();

void HideCursor();

void ViewCursor();

void RemakeWindow(window& object);

int GetMouseWheel();

#endif // SHASHKACREATOR_H
