# Спецификация проекта Real Engine

*Это ооочень ранняя версия спецификации, которая будет нормально дописываться. Предложения по спеке будут прослушанны и могут быть приняты.*

Real engine - движок для 2д игр. Он предоставляет интерфейсы для рисовки спрайтовой графики, GUI, регистрации объектов в физическом движке, а также предоставляет возможность автоматической загрузки игровых объектов. Все объекты и функции движка находятся в namespace re 

Пользователю также предоставляется интерфейс класса, от которого нужно отнаследоваться и переопределять методы для написания клиентского приложения.
```C
class IBaseApp{
public:
    virtual void setup()    = 0;
    virtual void update()   = 0;
    virtual void display()  = 0;

    // Event handlers
    virtual void on_key_pressed(Key key) {}
    virtual void on_key_released(Key key) {}
    virtual void on_button_pressed(int button) {}
    virtual void on_mouse_move(int mouseX, int mouseY) {}
};
typedef std::shared_ptr<IBaseApp> IBaseAppPtr;
```

Также предоставляются две функции
```C
void init_graphics(GraphicalBackend backend_type, int window_width, int window_height);
void run_base_app(IBaseApp* base_app_ptr);
```
init_graphics инициализирует графический бекэнд. 

run_base_app запускает цикл приложения. 

## Графический backend
Графический компонент предоставляет пользователю функции для отрисовки разных графических примитивов, заливки заднего фона и отрисовки изображений. Технологический инструмент - openGL. Интерфейс функций, предоставляемых пользователю, находятся в файле include/RealEngine/graphic.h

Должны поддерживаться функции (todo: добавить их описания):
```C
void background     (Color color);
void draw_rectangle (int x0, int y0, int w, int h, Color color);
void draw_line      (int x0, int y0, int x1, int y1, Color color);
void draw_circle    (int x0, int y0, int r, Color color);
void draw_image     (int x0, int y0, ImagePtr im);
void set_fps        (uint fps);
void translate      (int x, int y);
void scale          (float x, float y);
void rotate         (float angle);
```

## Анимация
Работа с изображениями ведется через класс Image, который в свою очередь использует библиотеку stb_image (для считывания изображения из файла). 

Анимация осуществляется классом Animation. Спрайтовая анимация заключается в последовательном изменении картинки, поэтому этот класс должен содержать в себе пачку изображений, и функцию которая будет выдавать указатель на текущий кадр.


## XML parser
Нужно уметь парсить xml. Пример xml файла
```xml
<recipe name="хлеб" preptime="5min" cooktime="180min">
   <title>
      Простой хлеб
   </title>
   <composition>
      <ingredient amount="3" unit="стакан">Мука</ingredient>
      <ingredient amount="0.25" unit="грамм">Дрожжи</ingredient>
      <ingredient amount="1.5" unit="стакан">Тёплая вода</ingredient>
   </composition>
   <instructions>
     <step>
        Смешать все ингредиенты и тщательно замесить. 
     </step>
     <step>
        Закрыть тканью и оставить на один час в тёплом помещении. 
     </step>
     <!-- 
        <step>
           Почитать вчерашнюю газету. 
        </step>
         - это сомнительный шаг...
      -->
     <step>
        Замесить ещё раз, положить на противень и поставить в духовку.
     </step>
   </instructions>
</recipe>
```

## TMUX file parser
TMUX - файлы редактора карт Tiled, его нужно уметь парсить. Формат файла xml. Основные данные в этом файле:
- путь к тайлсету и параметры его разбиения
- списки номеров тайлов в карте
- списки объектов карты, их имена и координаты

Пример файла tsx (содержит информацию о тайлсете)
```xml
<?xml version="1.0" encoding="UTF-8"?>
<tileset name="Tile" tilewidth="16" tileheight="16" tilecount="32" columns="8">
 <image source="Tile.png" width="128" height="64"/>
</tileset>
```

Пример файла tmx (собственно уровень)
```xml
<?xml version="1.0" encoding="UTF-8"?>
<map version="1.0" orientation="orthogonal" renderorder="right-down" width="120" height="40" tilewidth="16" tileheight="16" nextobjectid="6">
 <tileset firstgid="1" source="test_tile.tsx"/>
 <layer name="Слой тайлов 1" width="120" height="40">
  <data encoding="csv">
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8 (и ещё много данных в формате csv)

</data>
 </layer>
 <objectgroup name="object_layer">
  <object id="1" name="plat" x="0" y="560" width="1920" height="78"/>
  <object id="2" name="plat" x="194" y="480" width="160" height="46"/>
  <object id="3" name="plat" x="418" y="384" width="254" height="50"/>
  <object id="4" name="player" x="505.333" y="518.667" width="36" height="36">
   <ellipse/>
  </object>
 </objectgroup>
</map>
```


## Менеджер ресурсов
Этот модуль должен по xml файлу загружать различные ресурсы. Пример подобного конфигурационного файла (гипотетического)
```xml
<resourses>
    <image> 
        <im id="mario_sprite_sheet" file="../example/mario/mario.png"/>
    </image>
    <sprites>
        <sprite id="mario_sprite" source_id="mario_sprite_sheet" speed="0.05">
            <sp x="4" y="9" w="17" h="21"/>
            <sp x="37" y="9" w="17" h="21"/>
        </sprite>
    </sprites>
</resourses>
```

## Физический движок

Бекендов физического движка будет несколько, поэтому интерфейс физического мира будет изменен (скорее всего). Но идея такая. Мы создаем физический мир, с помощью фабрик создаем объекты и регистрируем их в нашем мире. В этом мире наши объекты будут изменяться соответственно его законам. 

Если сохранить указатель на физический объект, то к нему можно прикладывать силы (через интерфейс) или задавать скорость напрямую. 

```C
class PhysicalObject{
public:
    Point2f position;
    Point2f velocity, correction_velocity;
    double mass;
    PhysicShapePtr shape;

    static constexpr double DEFAULT_MASS_VALUE = 1.0;
public:
    void apply_force(Point2f force);
    void refresh_force();
    void set_velosity(Point2f vel);
    void add_velosity(Point2f vel);
```


```C
class PhysicalWorld{
    void RegisterObject(PhysicalObjectPtr obj);
    void RegisterCallback(std::function<void(ManifoldPtr)> callback); // callback, который вызывается при столкновении
    void update(int microseconds);
    void update(); // если не заданно явно, то dt считается внутри
    void set_gravity(Point2f gravity);
    void set_gravity(float gx, float gy);
    Point2f get_current_gravity();

    void debug_display();
```

Также должны быть фабрики объектов для всех типов объектов.

То есть, условно говоря, для создания объекта должен быть подобный код. 

```C
re::PhysicalWorld phys_world;

phys_world.RegisterCallback(std::bind(&MainApp::collision, this, std::placeholders::_1));

re::PhysicalObjectPtr obj = re::create_circle_object(re::Point2f(player.x0, player.y0), re::Point2f(0, 0), 10, 0.5);

phys_world.RegisterObject(obj);
phys_world.set_gravity(0, 9.8);
```

Далее можно применять силу.

```
obj->apply_force(5, 0);
```

При столкновении объектов должен вызываться callback (если задан). В колбек передается экземпляр ManifoldPtr, в котором содержатся указатели на объекты, которые столкнулись, вектор нормали столкновения и "степень столкновения".
```C
struct Manifold{
    PhysicalObjectPtr a, b;
    float penetration;
    Point2f normal;
};
typedef std::shared_ptr<Manifold> ManifoldPtr;
```

##

