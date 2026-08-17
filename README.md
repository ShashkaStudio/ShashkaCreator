# ShashkaCreator
## ShashkaCreator is open source C++ library for creating 2D games and applications on Windows. The library is written in Code::Blocks using the standard WinAPI libraries and consists of several modules. 
## Shashka Creator — это библиотека с открытым исходным кодом на C++ для создания 2D игр и приложений под Windows. Библиотека написана в Code::Blocks с использованием стандартных библиотек WinAPI и состоит из нескольких модулей.
# Как установить библиотеку?
Добавьте файлы **ShashkaCreator.cpp** и **ShashkaCreator.h** в ваш C++ проект и подключите библиотеку в основном файле как **#include "ShashkaCreator.h"**. Таже для работы вам понадобится подключить к проекту ещё 4 файла: **libgdi32.a**, **libmsimg32.a**, **libwinmm.a** и **libws2_32**.a. Эти библиотеки есть по умолчанию в каждом windows, так что ничего дополнительного скачивать не придётся. 
# Основной модуль
## Создание окна
Основой любого windows приложения является окно. Давайте создадим такое.  

```window [имя окна] = {int style, std::string name, int x, int y, int width, int height, HWND hwnd, HDC memDC, BITMAP hBuffer, HBITMAP hOld};```  
* **style** - стиль окна. Поставьте **normal** для обычного окна и **fullscreen** для полноэкранного.
* **name** - название окна, которое будет отображаться в рамке. Лучше всего используйте латиницу.
* **x и y** - положение окна на экране.
* **width и height** - размеры окна.
* **последние четыре параметра не заполняйте, программа заполнит их автоматически.**

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
