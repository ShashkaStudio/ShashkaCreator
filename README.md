# ShashkaCreator
## ShashkaCreator is open source C++ library for creating 2D games and applications on Windows. The library is written in Code::Blocks using the standard WinAPI libraries and consists of several modules. 
## Shashka Creator — это библиотека с открытым исходным кодом на C++ для создания 2D игр и приложений под Windows. Библиотека написана в Code::Blocks с использованием стандартных библиотек WinAPI и состоит из нескольких модулей.
# Как установить библиотеку?
Добавьте файлы **ShashkaCreator.cpp** и **ShashkaCreator.h** в ваш C++ проект и подключите библиотеку в основном файле как **#include "ShashkaCreator.h"**. Таже для работы вам понадобится подключить к проекту ещё 4 файла: **libgdi32.a**, **libmsimg32.a**, **libwinmm.a** и **libws2_32**.a. Эти библиотеки есть по умолчанию в каждом windows, так что ничего дополнительного скачивать не придётся. 
# Каковы возможности библиотеки?
ShashkaCreator использует процессор для отрисовки графики, так что свой Hollow Knight или Redcon вы конечно не напишите, а если даже и напишите, то GDI намеренно ограничит количество отрисовываемых объектов. Однако у библиотеки есть множество плюсов: вес около 40 килобайт, отсутствие зависимостей от сторонних библиотек, простота в установке, комфортная работа на слабых устройствах. Так что ShashkaCreator отлично подходит для тех, кто хочет создать небольшую игру, разобраться в WinAPI или просто побаловаться. 
# Основной модуль - ShashkaCreator
## Создание окна
Основой любого windows приложения является окно. Давайте создадим такое.  

```window [имя окна] = {int style, std::string name, int x, int y, int width, int height, HWND hwnd, HDC memDC, BITMAP hBuffer, HBITMAP hOld};```  
* **style** - стиль окна. Поставьте **normal** для обычного окна и **fullscreen** для полноэкранного.
* **name** - название окна, которое будет отображаться в рамке. Лучше используйте латиницу.
* **x и y** - положение окна на экране.
* **width и height** - размеры окна.
* **Последние четыре параметра не заполняйте, программа заполнит их автоматически.**

Перейдём к функциям для создания окна.  
```void MakeWindow(window& object);``` - создаёт окно.

Если во время работы программы вам захочется изменить размеры окна, то воспользуйтесь следующей функцией.  
```void RemakeWindow(window& object);``` - пересоздаёт окно.

Для того, чтобы программа не завершалась сразу после создания, окну необходимо реагировать на сообщения и выполнять команды.  
```void DisplayWindow(UpdateFunction userUpdate);``` - определённое количество раз в секунду вызывает пользовательскую функцию и обрабатывает сообщения окна. В пользовательской функции вы будете описывать все логические и графические действия. 

**Пример полного кода:**  
```
#include "ShashkaCreator.h"

window mainWindow = {normal, "Game", 0, 0, 1600, 900};

void Update();

Start() {
    MakeWindow(mainWindow);
    DisplayWindow(Update);
}

void Update() {

}
```
**Примечание: Start() это тоже самое, что и int main().**
## Рисование в окне
Функции для вызова отрисовки всегда должны быть в вашей функции, которую вы указали в **DisplayWindow();**   
Рисование любых объектов всегда должно быть между двумя функциями.

```void BeginDraw(window& object);``` - начинает отрисовку в заданном окне.  
```void EndDraw(window& object);``` - заканчивает отрисовку в заданном окне.

У каждого объекта для рисования есть свой цвет. 

```RGB [имя цвета] = {int red, int green, int blue, unsigned char alpha};```  
* **red** - преобладание красного в цвете. 
* **green** - преобладание зелёного в цвете.
* **blue** - преобладание синего в цвете.
* **alpha** - прозрачность цвета.
* **У всех параметров меняйте значения от 0 до 255.**

В библиотеке уже есть несколько готовых цветов: **red**, **green**, **blue**, **white**, **black**, **yellow**, **purple**, **lightBlue**.

```void PaintWindow(window& object, RGB color);``` - красит фон заданного окна в заданный цвет.  
**Примечаниее: фон окна нельзя сделать прозрачным.**

**Пример полного кода**:
```
#include "ShashkaCreator.h"

window mainWindow = {normal, "Game", 0, 0, 1600, 900};

void Update();

Start() {
    MakeWindow(mainWindow);
    DisplayWindow(Update);
}

void Update() {
    BeginDraw(mainWindow);
    PaintWindow(mainWindow, white);
    EndDraw(mainWindow);
}
```
## Отрисовка примитивов
Основой любых графических элементов в ShashkaCreator являются примитивы, то есть прямоугольники. Давайте создадим такой.

```square [имя примитива] = {float x, float y, float width, float height, float angle, RGB color, std::string tag, bool active};```
* **x и y** - положение примитива в окне.
* **width и height** - размеры примитива.
* **angle** - угол поворота примитива. **Если хотите перевести значение в радианы, то домножьте его на ```rad```.**
* **color** - цвет примитива.
* **tag** - тэг примитива. При желании можете оставить пустым.
* **active** - активен ли примитив. При значении false примитив не будет отрисовываться в окне.

После создания примитива его можно нарисовать в окне.

```void DrawSquare(window& update, square& object);``` - рисует заданный примитив в заданном окне.

**Пример полного кода:**  
```
#include "ShashkaCreator.h"

window mainWindow = {normal, "Game", 0, 0, 1600, 900};

square player = {500, 500, 50, 50, 0, red, "Player", true};

void Update();

Start() {
    MakeWindow(mainWindow);
    DisplayWindow(Update);
}

void Update() {
    BeginDraw(mainWindow);

    PaintWindow(mainWindow, white);
    DrawSquare(mainWindow, player);

    EndDraw(mainWindow);
}
```
## Нажатие клавиш
Чтобы отслеживать нажатие клавиш клавиатуры или кнопок мыши, используется несколько простых функций.

```bool GetKeyDown(int KeyCode);``` - возвращает true, если заданная клавиша была **нажата один раз**.  
```bool GetKey(int KeyCode);``` - возвращает true, если заданная клавиша **зажата в данный момент**.  
```bool GetKeyUp(int KeyCode);``` - возвращает true, если заданная клавиша **была отпущена после зажатия**.  

Список кодов клавиш достаточно простой.
* Коды клавиш букв и цифр сделаны по образцу - **Key[название]**. Например ```KeyW``` или ```Key1```.
* У кнопок мыши всего два кода - ```LeftMouse``` и ```RightMouse```.
* Коды для стрелочек сделаны по образцу - **Key[направление стрелочки заглавными буквами]**. Например ```KeyLEFT``` или ```KeyUP```.
* Остальные коды сделаны по образцу - **Key[название клавиши заглавными буквами]**. Например ```KeyLSHIFT``` или ```KeySPACE```.

**Пример полного кода:**
```
#include "ShashkaCreator.h"

window mainWindow = {normal, "Game", 0, 0, 1600, 900};

square player = {500, 500, 50, 50, 0, red, "Player", true};

void Update();

Start() {
    MakeWindow(mainWindow);
    DisplayWindow(Update);
}

void Update() {
    if (GetKey(KeySPACE)) {
        player.color = blue;
    }

    if (GetKeyUp(KeySPACE)) {
        player.color = red;
    }

    BeginDraw(mainWindow);

    PaintWindow(mainWindow, white);
    DrawSquare(mainWindow, player);

    EndDraw(mainWindow);
}
```
## Глобальные переменные
В ShashkaCreator есть несколько важных глобальных переменных, которые необходимы для качественной работы библиотеки.

* ```float deltaTime``` - время, которое прошло с момента отрисовки предыдущего кадра. **Эта переменная заполняется автоматически.**  
* ```float cameraX``` - координаты смещения области отрисовки внутри окна по оси X(по умолчанию 0.0). **Вы можете изменять эту переменную.**  
* ```float cameraY``` - координаты смещения области отрисовки внутри окна по оси Y(по умолчанию 0.0). **Вы можете изменять эту переменную.**

**Примечание: в WinAPI координаты внутри окна считаются от верхнего левого угла. Ось X направлена вправо, ось Y направлена вниз.**

**Пример полного кода**:
```
#include "ShashkaCreator.h"

window mainWindow = {normal, "Game", 0, 0, 1600, 900};

square player = {200, 200, 50, 25, 0, red, "Player", true};
square wall = {500, 500, 200, 200, 0, blue, "Wall", true};

float walkSpeed = 200.0f;
float rotateSpeed = 100.0f;
float radAngle;

void Update();

Start() {
    MakeWindow(mainWindow);
    DisplayWindow(Update);
}

void Update() {
    radAngle = player.angle * rad;

    if (player.x >= 0) cameraX = player.x / 2;
    if (player.y >= 0) cameraY = player.y / 2;
    if (player.x < 0) cameraX = player.x * 1.1;
    if (player.y < 0) cameraY = player.y * 1.1;

    if (GetKey(KeyLEFT)) player.angle -= rotateSpeed * deltaTime;
    if (GetKey(KeyRIGHT)) player.angle += rotateSpeed * deltaTime;

    if (GetKey(KeyW)) {
        player.x -= walkSpeed * cos(radAngle) * deltaTime;
        player.y -= walkSpeed * sin(radAngle) * deltaTime;
    }

    if (GetKey(KeyS)) {
        player.x += walkSpeed * cos(radAngle) * deltaTime;
        player.y += walkSpeed * sin(radAngle) * deltaTime;
    }

    if (GetKey(KeyD)) {
        player.x += walkSpeed * sin(radAngle) * deltaTime;
        player.y -= walkSpeed * cos(radAngle) * deltaTime;
    }

    if (GetKey(KeyA)) {
        player.x -= walkSpeed * sin(radAngle) * deltaTime;
        player.y += walkSpeed * cos(radAngle) * deltaTime;
    }

    BeginDraw(mainWindow);

    PaintWindow(mainWindow, white);
    DrawSquare(mainWindow, wall);
    DrawSquare(mainWindow, player);

    EndDraw(mainWindow);
}
```
## Звуки
Библиотека умееет работать со звуковыми файлами c расширениями **.wav** и **.mp3**. Давайте созадим звуковой объект.

```sound [имя звука] = {std::string path, std::string alias};```
* **path** - путь к звуковому файлу. **Не используйте пробелы и не латинские буквы!**
* **alias** - название звука. Название может быть любым, но лучше используйте латиницу.
* **Примечание: по умолчанию библиотека ищет звуковые файла в папке с .exe файлом.**

Для работы со звуками используется несколько функций.  

```void LoadSound(sound &object);``` - **загружает** заданный звуковой файл в программу.  
```void PlaySound(sound &object);``` - **проигрывает** заданный звук один раз **до конца**.  
```void PlayLoopSound(sound &object);``` - **зацикленно проигрывает** заданный звук до конца.  
```void StopSound(sound &object);``` - **останавливает** проигрывание заданного звука и **удаляет его**.  
```void StopAllSounds();``` - **останавливает** проигрывание всех звуков и **удаляет все звуки**.  
```void PauseSound(const sound &object);``` - ставит проигрывание заданного звука на **паузу**, то есть останавливает проигрывание и **не удаляет звук**.  
```void ResumeSound(const sound &object);``` - **возобновляет проигрывание** заданного звука, если до этого он был на паузе.  
```void SetSoundVolume(const sound& object, int volume);``` - устанавливает **громкость** заданного звука на заданное значение**(от 0 до 100)**.  

**Пример полного кода:**
```
#include "ShashkaCreator.h"

window mainWindow = {normal, "Game", 0, 0, 1600, 900};

sound music = {"AcesHigh.mp3", "Track1"};

void Update();

Start() {
    MakeWindow(mainWindow);
    DisplayWindow(Update);
}

void Update() {
    if (GetKeyDown(KeySPACE))  {
        LoadSound(music);
        PlayLoopSound(music);
    }

    if (GetKeyDown(KeyESCAPE)) {
        StopAllSounds();
    }

    BeginDraw(mainWindow);
    PaintWindow(mainWindow, white);
    EndDraw(mainWindow);
}
```
**Примечание: в данном примере я использовал трек Кевина Маклауда "Aces High" из альбома "Funkorama".**
