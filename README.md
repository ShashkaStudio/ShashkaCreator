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
* **name** - название окна, которое будет отображаться в рамке. Лучше всего используйте латиницу.
* **x и y** - положение окна на экране.
* **width и height** - размеры окна.
* **Последние четыре параметра не заполняйте, программа заполнит их автоматически.**

Перейдём к функциям для создания окна.  
```void MakeWindow(window& object);``` - создаёт окно.  

Для того, чтобы программа не завершалась сразу после создания, окну необходимо реагировать на сообщения.  
```void DisplayWindow(UpdateFunction userUpdate);``` - определённое количество раз в секунду вызывает пользовательскую функцию и обрабатывает сообщения окна.  

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
