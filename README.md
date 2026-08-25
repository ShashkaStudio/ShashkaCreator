# ShashkaCreator
## ShashkaCreator is open source C++ library for creating 2D games and applications on Windows. The library is written in Code::Blocks using the standard WinAPI libraries and consists of several modules. 
## Shashka Creator — это библиотека с открытым исходным кодом на C++ для создания 2D игр и приложений под Windows. Библиотека написана в Code::Blocks с использованием стандартных библиотек WinAPI и состоит из нескольких модулей.
# Кто может установить библиотеку?
Абсолютно любой человек может использовать ShashkaCreator в асболютно любых целях. Там буквально нечего скрывать: библиотека состоит из пары файлов, которые открываются даже через стандартный блокнот, а для понимания исходного кода достаточно уметь работать с **Windows.h**, материалы по которой есть в свободном доступе на сайте **MicrosoftLearn**. Так что **владейте, пользуйтесь, распоряжайтесь.**
# Как установить библиотеку?
Добавьте файлы **ShashkaCreator.cpp** и **ShashkaCreator.h** в ваш C++ проект и подключите библиотеку в основном файле как **#include "ShashkaCreator.h"**. Таже для работы вам понадобится подключить к проекту ещё 4 файла: **libgdi32.a**, **libmsimg32.a**, **libwinmm.a** и **libws2_32.a**. Эти библиотеки есть по умолчанию в каждом **Windows**, так что ничего дополнительного скачивать не придётся. **Если по какой-то причине ваш компилятор не MinGW, то я вам глубоко сочувствую и советую перейти на MinGW(а ещё лучше использовать Code::Blocks и DevC++ в качестве IDE).**
# Каковы возможности библиотеки?
ShashkaCreator использует процессор для отрисовки графики, так что свой **Hollow Knight** или **Redcon** вы конечно не напишите, а если даже и напишите, то GDI намеренно ограничит количество отрисовываемых объектов. Однако у библиотеки есть множество плюсов: вес около 40 килобайт, отсутствие зависимостей от сторонних библиотек, простота в установке, комфортная и быстрая работа на слабых устройствах. Так что ShashkaCreator отлично подходит для тех, кто хочет создать небольшую игру, разобраться в WinAPI или просто побаловаться. 
# Основной модуль - ShashkaCreator
## Окно
Основой любого windows приложения является окно. Давайте создадим такое.  

```window [имя окна] = {int style, std::string name, int x, int y, int width, int height, HWND hwnd, HDC memDC, BITMAP hBuffer, HBITMAP hOld};```  
* **style** - стиль окна. Поставьте **normal** для обычного окна и **fullscreen** для полноэкранного.
* **name** - название окна, которое будет отображаться в рамке. Лучше используйте латиницу.
* **x и y** - координаты окна на экране.
* **width и height** - размеры окна.
* **Последние четыре параметра не заполняйте.**

Перейдём к функциям для создания окна.  

```void MakeWindow(window& object);``` - создаёт окно.

Если во время работы программы вам захочется изменить размеры окна, то воспользуйтесь следующей функцией.  

```void RemakeWindow(window& object);``` - пересоздаёт окно.

Для того, чтобы программа не завершалась сразу после создания, окну необходимо реагировать на сообщения и выполнять команды.  

```void DisplayWindow(UpdateFunction userUpdate);``` - определённое количество раз в секунду вызывает пользовательскую функцию и обрабатывает сообщения окна. В пользовательской функции вы будете описывать все логические и графические действия. 

Если во время работы программы вам захочется изменить пользовательскую функцию, то воспользуйтесь следующей функцией.

```void SwitchUpdate(UpdateFunction nextUpdate);``` - устанавливает новую пользовательскую функцию, которую будет вызывать функция DisplayWindow();

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
* **x и y** - координаты примитива в окне.
* **width и height** - размеры примитива.
* **angle** - угол поворота примитива в градусах.
* **color** - цвет примитива.
* **tag** - тег примитива. При желании можете оставить пустым.
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
* ```float rad``` - **3.14159f / 180.0f**, используйте чтобы перевести значение угла **из градусов в радианы**. 

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
* **Примечание: по умолчанию библиотека ищет звуковые файлы в папке с .exe файлом вашего проекта.**

Для работы со звуками используется несколько функций.  

```void LoadSound(sound &object);``` - загружает заданный звуковой файл в программу.  
```void PlaySound(sound &object);``` - проигрывает заданный звук **один раз до конца**.  
```void PlayLoopSound(sound &object);``` - **зацикленно проигрывает** заданный звук до конца.  
```void StopSound(sound &object);``` - останавливает проигрывание заданного звука и **удаляет его**.  
```void StopAllSounds();``` - останавливает проигрывание всех звуков и **удаляет все звуки**.  
```void PauseSound(const sound &object);``` - останавливает проигрывание заданного звука и **не удаляет его**.  
```void ResumeSound(const sound &object);``` - возобновляет проигрывание заданного звука.  
```void SetSoundVolume(const sound& object, int volume);``` - устанавливает **громкость** заданного звука на заданное значение **(от 0 до 100)**.  

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
**Примечание: в данном примере используется трек Кевина Маклауда "Aces High" из альбома "Funkorama".**
## Видео
Библиотека умеет работать с видео файлами с рашсирением **.avi**. Давайте создадим видео.

```video [имя видео] = {std::string path, std::string alias, int x, int y, int width, int height, bool active};```
* **path** - путь к видео файлу. **Не используйте пробелы и не латинские буквы!**
* **alias** - название видео. Название может быть любым, но лучше используйте латиницу.
* **x и y** - координаты видео в окне.
* **width и height** - размеры видео.
* **active** - активно ли видео. При значении false видео не будет проигрываться.
* **Примечание: по умолчанию библиотека ищет видео файлы в папке с .exe файлом вашего проекта.**

Для работы с видео используется несколько простых функций.  

```void LoadVideo(window& update, video& object);``` - загружает заданное видео в заданное окно.  
```void PlayVideo(const video& object);``` - проигрывает заданное видео **один раз до конца**.  
```void CloseVideo(video& object);``` - останавливает проигрывание заданного видео и **удаляет его**.  
```void CloseAllVideos();``` - останавливает проигрывание всех видео и **удаляет все видео**.  
```void PauseVideo(const video& object);``` - останавливает прогрывание заданного видео и **не удаляет его**.  
```void ResumeVideo(const video& object);``` - возобновляет проигрывание заданного видео.  
```void SetVideoVolume(const video& object, int volume);``` - устанавливает **громкость** заданного видео на заданное значение **(от 0 до 100)**.   

**Пример полного кода:**
```
#include "ShashkaCreator.h"

window mainWindow = {normal, "Game", 0, 0, 1600, 900};

video vid = {"BeholderTrailer.avi", "Video", 500, 300, 900, 500, true};

void Update();

Start() {
    MakeWindow(mainWindow);
    DisplayWindow(Update);
}

void Update() {
    if (GetKeyDown(KeySPACE)) {
        LoadVideo(mainWindow, vid);
        PlayVideo(vid);
    }

    if (GetKeyDown(KeyESCAPE)) {
        CloseAllVideos();
    }

    BeginDraw(mainWindow);
    PaintWindow(mainWindow, white);
    EndDraw(mainWindow);
}
```
**Примечание: в данном примере используется трейлер игры "Beholder" за издательством Alawar.**
## Спрайты
Библиотека умеет работать с графическими изображениями с расширением **.bmp** и глубиной цвета **24 бита**. Давайте создадим спрайт.

```sprite [имя спрайта] = {const char* path, HBITMAP image, float x, float y, float width, float height, float angle, unsigned char alpha, COLORREF keyColor, bool active};```  
* **path** - путь к графическому изображению. **Не используйте пробелы и не латинские буквы!**
* **image** - **вам не нужно заполнять это значение.** При желании поставьте 0 или nullptr.
* **x и y** - координаты спрайта в окне.
* **width и height** - размеры спрайта.
* **angle** - угол поворота спрайта в градусах.
* **alpha** - прозрачность спрайта. Меняйте значение от 0 до 255.
* **keyColor** - код цвета, который не будет отрисовываться у спрайта. Если хотите отрисовывать спрайт полностью, то поставьте значение 0.
* **active** - активен ли спрайт. При значении false спрайт не будет отрисовываться в окне.
* **Примечание: по умолчанию библиотека ищет графические изображения в папке с .exe файлом вашего проекта.**

**Коды для keyColor:**
* **blackMask** - чёрный цвет.
* **whiteMask** - белый цвет.

Для работы со спрайтами используется несколько простых функций.  

```void LoadSprite(sprite& object);``` - загружает заданный спрайт в программу.  
```void DrawSprite(window &update, sprite &object);``` - рисует заданный спрайт в заданном окне.

**Пример полного кода:**  
```
#include "ShashkaCreator.h"

window mainWindow = {normal, "Game", 0, 0, 1600, 900};

sprite entity = {"1984Point.bmp", nullptr, 500, 500, 600, 300, 0, 255, 0, true};

void Update();

Start() {
    MakeWindow(mainWindow);
    LoadSprite(entity);
    DisplayWindow(Update);
}

void Update() {
    BeginDraw(mainWindow);
    
    PaintWindow(mainWindow, white);
    DrawSprite(mainWindow, entity);
    
    EndDraw(mainWindow);
}
```
## Курсор
Давайте создадим курсор мыши. Для комфортной работы вам хватит всего лишь одного такого объекта.

```mouse [имя курсора] = {int x, int y};```
* **x и y** - координаты курсора в окне.
* **Вам не нужно заполнять эти переменные. Они заполняются с помощью специальной функции.**

Для работы с курсором мыши используется несколько простых функций.

```mouse GetMousePosition(const window& update);``` - записывает координаты куросра мыши в заданном окне.  
```void BlockCursor();``` - блокирует кусору возможность двигаться.  
```void ReleaseCursor();``` - возвращает курсору возможность двигаться.   
```void HideCursor();``` - отключает отрисовку курсора на экране.  
```void ViewCursor();``` - включает отрисовку курсора на экране.
```bool IsCursorTouched(window& update, square& object);``` - возвращает true, если курсор касается заданного примитива в заданном окне.

**Пример полного кода:**
```
#include "ShashkaCreator.h"

window mainWindow = {normal, "Game", 0, 0, 1600, 900};

square wall = {500, 500, 100, 100, 0, red, "Wall", true};

void Update();

Start() {
    MakeWindow(mainWindow);
    DisplayWindow(Update);
}

void Update() {
    if (IsCursorTouched(mainWindow, wall)) {
        HideCursor();
    }

    else {
        ViewCursor();
    }

    BeginDraw(mainWindow);

    PaintWindow(mainWindow, white);
    DrawSquare(mainWindow, wall);

    EndDraw(mainWindow);
}
```
## Текст
Библиотека использует кодировку UTF-8, поддерживает латиницу и киррилицу, а также пользовательские шрифты c расширениями **.ttf** и **.otf**. Давайте создадим текст.

```text [имя текста] = {std::string content, int x, int y, int size, float angle, RGB color, std::string fontPath, std::string fontName, bool active};```
* **content** - содержание текста.
* **x и y** - координаты текста в окне.
* **size** - размер текста.
* **angle** - угол поворота текста в градусах.
* **color** - цвет текста.
* **fontPath** - путь к файлу с пользовательским шрифтом. **Не используйте пробелы и не латинские буквы!**
* **fontName** - точное имя пользовательского шрифта.
* **Если хотите использовать стандартный шрифт(Arial), то не заполняйте параметры fontPath и fontName.**
* **active** - активен ли текст. При значении false текст не будет отрисовываться в окне.
* **Примечание: по умолчанию библиотека ищет шрифты в папке с .exe файлом вашего проекта.**

Для работы с текстом используется несколько простых функций.

```void DrawText(window& update, text& object);``` - рисует заданный текст в заданном окне.  
```std::string toText(int variable);``` - тоже самое, что и std::to_string(); Есть также перегрузка для float, double, long long и short.

**Пример полного кода:**
```
#include "ShashkaCreator.h"

window mainWindow = {normal, "Game", 0, 0, 1600, 900};

text testText = {"Это текст ShashkaCreator!", 500, 500, 50, 0, black, "PTM55FT.ttf", "PT Mono", true};

void Update();

Start() {
    MakeWindow(mainWindow);
    DisplayWindow(Update);
}

void Update() {

    BeginDraw(mainWindow);

    PaintWindow(mainWindow, white);
    DrawText(mainWindow, testText);

    EndDraw(mainWindow);
}
```
**Примечание: в данном примере используется пользовательский шрифт PT Mono.**
## Коллизия и триггеры
Стандартный модуль библиотеки использует простой метод коллизии **AABB** для прямоугольников. Для проверки столкновения двух примитивов используется простая функция.

```bool IsCollide(square &objectA, square &objectB);``` - возвращает true, если два примитива столкнулись.

Также на основе этого метода работают триггеры. Давайте создадим такой.

```trigger [имя триггера] = {square area, std::string targetTag, TriggerAction action, bool active = true};```
* **area** - примитив, который будет телом триггера.
* **targetTag** - тег, на который будет реагировать триггер.
* **action** - функция, которую вызовет триггер после активации.
* **active** - активен ли триггер. При значении false триггер не будет работать.

Для работы с триггером используется простая функция.

```void OnTriggerEnter(trigger &triggerObject, square &targetObject);``` - выполняет действие заданного триггера, если тот сталкивается с заданным примитивом. **После выполнения действия триггер станет неактивным.**

**Пример полного кода:**
```
#include "ShashkaCreator.h"

window mainWindow = {normal, "Game", 0, 0, 1600, 900};

square player = {200, 200, 50, 25, 0, red, "Player", true};
square wall = {500, 500, 200, 200, 0, blue, "Wall", true};

float walkSpeed = 200.0f;
float rotateSpeed = 100.0f;
float radAngle;

RGB windowColor = white;

void Update();

void Paint();

trigger wallTrigger = {wall, "Player", Paint, true};

Start() {
    MakeWindow(mainWindow);
    DisplayWindow(Update);
}

void Update() {
    radAngle = player.angle * rad;

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

    OnTriggerEnter(wallTrigger, player);

    BeginDraw(mainWindow);

    PaintWindow(mainWindow, windowColor);
    DrawSquare(mainWindow, wall);
    DrawSquare(mainWindow, player);

    EndDraw(mainWindow);
}

void Paint() {
    windowColor = green;
    wall.active = false;
}
```
## Кнопки
Кнопки являются нестандартными объектами в ShashkaCreator, так как они полностью сделаны из других готовых объектов. В библиотеке есть два вида кнопок: **UIButton** и **spriteButton**. **UIButton** создаётся из обычного примитива и текста, а **spriteButton** создаётся из спрайта и текста. Давайте пройдёмся по каждому виду и создадим несколько кнопок. Да, перед созданием любой из кнопок придётся создать ещё несколько объектов.

```UIButton [имя кнопки] = {square box, RGB normalColor, RGB hoverColor, text label, bool isHovered, bool isClicked, float clickTimer};```  
* **box** - примитив, который будет телом кнопки.  
* **normalColor** - цвет кнопки в обычном состоянии.  
* **hoverColor** - цвет кнопки во время наведённого на неё курсора.
* **label** - надпись на кнопке.
* **isHovered** - наведён ли курсор на кнопку.
* **isClicked** - нажата ли кнопка.
* **clickTimer** - время с момента последнего нажатия кнопки.
* **Последние три параметра не заполняйте.**

```spriteButton [имя кнопки] = {square box, sprite normalImage, sprite hoverImage, text label, bool isHovered, bool isClicked, float clickTimer};```
* **box** - примитив, который будет телом кнопки.  
* **normalImage** - спрайт кнопки в обычном состоянии.
* **hoverImage** - спрайт кнопки во время наведённого на неё курсора.
* **label** - надпись на кнопке.
* **isHovered** - наведён ли курсор на кнопку.
* **isClicked** - нажата ли кнопка.
* **clickTimer** - время с момента последнего нажатия кнопки.
* **Последние три параметра не заполняйте.**

Чтобы кнопки могли реагировать на касания и нажатия курсора, используется несколько простых функций.

```void UpdateUIButton(window &update, UIButton &object);``` - обрабатывает взаимодействия с заданной кнопкой в заданном окне.  
```void UpdateSpriteButton(window &update, spriteButton &object);``` - обрабатывает взаимодействия с заданной кнопкой в заданном окне.  

Для отрисовки кнопок используются следующие функции.

```void DrawUIButton(window &update, UIButton &object);``` - отрисовывает заданную кнопку в заданном окне.  
```void DrawSpriteButton(window &update, spriteButton &object);``` - рисует заданную кнопку в заданном окне.  

**Пример полного кода:**
```
#include "ShashkaCreator.h"

window mainWindow = {normal, "Game", 0, 0, 1600, 900};

square box = {500, 500, 200, 50, 0, red, "Button", true};
text letter;

RGB superRed = {200, 0, 0, 255};

UIButton clickMe = {box, red, superRed, letter};

text score = {"Счёт: 0", 500, 200, 30, 0, black, "PTM55FT.ttf", "PT Mono", true};

int scoreVar;

void Update();

Start() {
    MakeWindow(mainWindow);

    clickMe.label.content = "Нажми меня!";
    clickMe.label.size = 30;
    clickMe.label.color = white;
    clickMe.label.fontPath = "PTM55FT.ttf";
    clickMe.label.fontName = "PT Mono";
    clickMe.label.active = true;

    DisplayWindow(Update);
}

void Update() {
    if (clickMe.isClicked == true) {
        scoreVar += 1;
    }

    score.content = "Счёт: " + toText(scoreVar);

    UpdateUIButton(mainWindow, clickMe);

    BeginDraw(mainWindow);

    PaintWindow(mainWindow, white);
    DrawUIButton(mainWindow, clickMe);
    DrawText(mainWindow, score);

    EndDraw(mainWindow);
}
```
**Примечание: в данном примере используется пользовательский шрифт PT Mono.**
## Поле ввода
Поле ввода также является нестандартным объектом в ShashkaCreator, так как оно полностью сделано из других готовых объектов. Давайте создадим такое. Да, перед созданием поля ввода придётся создать ещё несколько объектов.

```inputfield [имя поля ввода] = {square box, text label, std::string content, int maxChars, bool isFocused};```
* **box** - примитив, который будет телом для поля ввода.
* **label** - текст внутри поля ввода.
* **content** - символы из текста + каретка. **Этот параметр заполнять не обязательно.**
* **maxChars** - максимальное количество символов, которое можно записать в поле ввода.
* **isFouced** - взаимодействует ли сейчас пользователь с полем ввода. **Этот параметр заполнять не нужно.**

Для работы с полями ввода используется несколько простых функций.

```void UpdateInputfield(window &update, inputfield &object);``` - обрабатывает взаимодействия с заданным полем ввода в заданном окне.  
```void DrawInputfield(window &update, inputfield &object);``` - рисует заданное поле ввода в заданном окне.

**Пример полного кода:**
```
#include "ShashkaCreator.h"

window mainWindow = {normal, "Game", 0, 0, 1600, 900};

square box = {500, 500, 250, 50, 0, red, "Untagged", true};
text inputText;

inputfield testField = {box, inputText, "", 19};

void Update();

Start() {
    MakeWindow(mainWindow);

    testField.label.size = 25;
    testField.label.color = white;
    testField.label.active = true;

    DisplayWindow(Update);
}

void Update() {
    UpdateInputfield(mainWindow, testField);

    BeginDraw(mainWindow);

    PaintWindow(mainWindow, white);
    DrawInputfield(mainWindow, testField);

    EndDraw(mainWindow);
}
```
## Полоска прокрутки
Полоска прокрутки также является нестандартным объектом в ShashkaCreator, так как она полностью сделана из других готовых объектов. Давайте создадим такую. Да, перед созданием полоски прокрутки придётся создать ещё несколько объектов.

```scrollbar [имя полоски прокрутки] = {square box, square slider, int direction, bool isDragged, float filled};```
* **box** - примитив, который будет телом полоски.
* **slider** - примитив, который будет ползунком.
* **direction** - направление полоски прокрутки. Поставьте **horizontal** для горизонтального и **vertical** для вертикального.
* **isDragged** - тянет ли пользователь за ползунок.
* **filled** - на каком положении находится ползунок, изменяется от **0.0f** до **1.0f**.
* **Последние два параметра не заполняйте.**

Для работы с полоской прокрутки используется несколько простых функций.

```void UpdateScrollbar(window& update, scrollbar& object);``` - обрабатывает взаимодействия с заданной полоской прокрутки в заданном окне.  
```void DrawScrollbar(window& update, scrollbar& object);``` - рисует заданную полоску прокрутки в заданном окне.

**Пример полного кода:**
```
#include "ShashkaCreator.h"

window mainWindow = {normal, "Game", 0, 0, 1600, 900};

square body = {700, 300, 500, 50, 0, lightBlue, "Untagged", true};
square interact = {500, 300, 50, 50, 0, blue, "Untagged", true};

scrollbar testBar = {body, interact, horizontal};

text fillText = {"", 700, 100, 30, 0, black, "", "", true};

void Update();

Start() {
    MakeWindow(mainWindow);
    DisplayWindow(Update);
}

void Update() {
    UpdateScrollbar(mainWindow, testBar);

    fillText.content = toText(testBar.filled);

    BeginDraw(mainWindow);

    PaintWindow(mainWindow, white);
    DrawText(mainWindow, fillText);
    DrawScrollbar(mainWindow, testBar);

    EndDraw(mainWindow);
}
```
## Меню
У любого окна можно сделать меню(рамка в верхней части окна, на которой находятся кнопки по типу "Файл", "Правка", "Вид" и т.п.). Меню создаётся не из прямоугольников и текста, как другие UI элементы, а прямо из стандартных средств **Windows.h**, так что оно будет выглядеть немного страшно на уровне **Windows95**. Создание меню состоит из нескольких шагов: главное меню - меню - подменю. Давайте пройдёмся по каждому объекту и создадим такое.

Для начала необходимо создать объект для раммки в верхней части окна.

```headmenu [имя главного меню] = {HMENU hmn};```
* **Здесь ничего заполнять не нужно.**

Теперь создадим эту рамку в окне.

```void MakeMenu(window& update, headmenu& object);``` - создаёт заданное главное меню в заданном окне.

После создания рамки в ней можно создать само меню, а в этом меню можно создать подменю. Например: "Файл" - "Выйти".

```menu [имя меню] = {std::string content, HMENU hmn};```
* **content** - надпись на кнопке.
* **Последний параметр не заполняйте.**

```submenu [имя подменю] = {std::string content, int id};```
* **content** - надпись на кнопке.
* **id** - уникальный номер подменю. **Используйте любое целое число кроме -1.**

Теперь подменю нужно добавить в меню, а меню добавить в главное меню. Для этого используются следующие функции.

```void BeginMenu(menu& object);``` - начинает работу с заданным меню.  
```void AddMenu(menu& parent, submenu& object);``` - добавляет заданное подменю в заданное меню.  
```void SeparateMenu(menu& object);``` - добавляет разделяющую полоску в заданное меню.  
```void EndMenu(window& update, headmenu& parent, menu& object);``` - заканчивает работу с заданным меню и добавляет заданное меню в главное меню.  

Осталось только взаимдействовать с подменю. Для этого используется следующая функция.

```bool ClickMenu(const submenu& object);``` - возвращает true, если нажато заданное подменю.

**Если вы ничего не поняли или вам надоело, то всё так и должно быть.**

**Пример полного кода:**
```
#include "ShashkaCreator.h"

window mainWindow = {normal, "Game", 0, 0, 1600, 900};

headmenu hm;
menu m = {"Файл"};
submenu sm = {"Выйти", 1};

void Update();

Start() {
    MakeWindow(mainWindow);

    MakeMenu(mainWindow, hm);

    BeginMenu(m);
    AddMenu(m, sm);
    EndMenu(mainWindow, hm, m);

    DisplayWindow(Update);
}

void Update() {
    if (ClickMenu(sm)) {
        Quit();
    }

    BeginDraw(mainWindow);
    PaintWindow(mainWindow, white);
    EndDraw(mainWindow);
}
```
## Диалоговые окна
Думаю с ними знакомы все, так что разбирать их детально не придётся. Давайте создадим такое.

```message [название диалогового окна] = {std::string title, std::string content, UINT icon, UINT type, bool active};```
* **title** - имя диалогового окна.
* **content** - надпись в диалоговом окне,
* **icon** - значок диалогового окна.
* **type** - тип кнопок у диалогового окна.
* **active** - активно ли диалоговое окно. При значении false оно не будет отрисовываться в окне.

**Значки:**
* **error** - белый крест в красном круге.
* **question** - белый вопросительный знак в синем круге.
* **warning** - чёрный восклицательный знак в жёлтом треугольнике.
* **information** - белая строчная буква i в синем круге.

**Кнопки:**
* **ok** - кнопка "ОК".
* **yes_no** - кнопки "Да" и "Нет".
* **ok_cancel** - кнопки "ОК" и "Отмена".
* **yes_no_cancel** - кнопки "Да", "Нет" и "Отмена".
* **retry_cancel** - кнопки "Повторить" и "Отмена".
* **abort_retry_ignore** - кнопки "Прервать", "Повторить" и "Пропустить".
* **cancel_try_continue** - кнопки "Отмена", "Повторить попытку" и "Продолжить".

**ID кнопок:**
* **idYes** - кнопка "Да".
* **idNo** - кнопка "Нет".
* **idOk** - кнопка "ОК".
* **idCancel** - кнопка "Отмена".
* **idAbort** - кнопка "Прервать".
* **idContinue** - кнопка "Продолжить".
* **idIgnore** - кнопка "Пропустить".
* **idRetry** - кнопка "Повторить".
* **idAgain** - кнопка "Повторить попытку".

Для взаимодействия с диалоговым окном используется следующая функция.
```int ShowMessage(window &update, message &object);``` - рисует заданное диалоговое окно в заданном окне и возвращает ID нажатой кнопки.

**Пример полного кода:**
```
#include "ShashkaCreator.h"

window mainWindow = {normal, "Game", 0, 0, 1600, 900};

message msg = {"Выключение", "Завершить работу программы?", question, yes_no, true};

void Update();

Start() {
    MakeWindow(mainWindow);
    DisplayWindow(Update);
}

void Update() {
    if (ShowMessage(mainWindow, msg) == idYes) {
        Quit();
    }

    else {
        msg.active = false;
    }

    BeginDraw(mainWindow);
    PaintWindow(mainWindow, white);
    EndDraw(mainWindow);
}
```
## Завершение работы
Самое время завершить работы программы. Тут всё максимально просто.

```void Quit();``` - завершает работу программы.  
```void ForceClose();``` - завершает работу программмы.  

**Пример полного кода:**
```
#include "ShashkaCreator.h"

window mainWindow = {normal, "Game", 0, 0, 1600, 900};

message msg = {"Выключение", "Завершить работу программы?", question, yes_no, true};

void Update();

Start() {
    MakeWindow(mainWindow);
    DisplayWindow(Update);
}

void Update() {
    if (ShowMessage(mainWindow, msg) == idYes) {
        Quit();
    }

    else {
        msg.active = false;
    }

    BeginDraw(mainWindow);
    PaintWindow(mainWindow, white);
    EndDraw(mainWindow);
}
```
## Эпилог
**Если вы дошли до этого момента и всё ещё не бросили ShashkaCreator, то поздравляю - это конец.**  
Вот и всё, теперь вы ознакомились со всеми стркуктурами и функциями для того, чтобы полноценно работать в ShashkaCreator. Но закончился только стандартный модуль, а у меня есть ещё модули, которые могут сильно облегчить работу над некоторыми задачами. Но они будут позже. Также буду безумно рад, если кто-то захочет модифицировать исходный код или вообще создать отдельную библиотеку на основе ShashkaCreator.    
Хотел бы я, чтоб здесь были титры, но мы на гитхабе, а не в игре. Хотя...  
**Главный создатель: ShashkaStudio.**  
**Отдельные благодарности: сайту MicrosoftLearn за документацию по WinAPI и каналу BeginEnd за отличный курс обучающих видео по языку C.**
# Алгоритмика - ShashkaAlgorithm
## Пролог
Чтобы установить этот модуль, переместите файл **ShashkaAlgorithm.h** в папку с вашим C++ проектом и подключите библиотеку в основном файле как **#include "ShashkaAlgorithm.h"**. Для работы с этим модулем подключать основной **ShashkaCreator.h** не нужно. Сам ShashkaAlgorithm имеет свой динамический массив и функции для работы с ним(что-то по типу **vector**). Так что если вы просто подключите **vector** и **algorithm**, то разницы толком не будет.
## Буффер
