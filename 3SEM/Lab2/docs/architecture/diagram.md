### Файл: `docs/architecture/uml_diagram_2.md`

````markdown
# UML Diagram - Architecture 2

## Улучшенная архитектура с классом Conflict

```mermaid
classDiagram
    %% Базовые классы
    class Location {
        -int x
        -int y
        +GetX() int
        +GetY() int
        +SetX(int)
        +SetY(int)
    }

    class Point {
        -bool visible
        +Show()*
        +Hide()*
        +MoveTo(int, int)
        +Drag(int)
    }

    class Conflict {
        -int width
        -int height
        +GetWidth() int
        +GetHeight() int
        +SetWidth(int)
        +SetHeight(int)
    }

    class BaseFlashlight {
        -int bodyWidth
        -int bodyHeight
        -int headWidth
        -int headHeight
        -bool broken
        +GetBodyWidth() int
        +GetBodyHeight() int
        +GetHeadWidth() int
        +GetHeadHeight() int
        +IsBroken() bool
        +Break()
        +Repair()
    }

    %% Простые геометрические фигуры
    class Circle {
        -int radius
        +Show() override
        +Hide() override
        +Expand(int)
        +Reduse(int)
    }

    %% Прямоугольные фонарики
    class RectFlashlight {
        +Show() override
        +Hide() override
    }

    class BrokenRectFlashlight {
        +Show() override
        +Hide() override
    }

    %% Круглые фонарики
    class RoundFlashlight {
        +Show() override
        +Hide() override
    }

    class BrokenRoundFlashlight {
        +Show() override
        +Hide() override
    }

    %% Конфликтные объекты
    class Screwdriver {
        +Show()
        +Hide()
    }

    class Stone {
        +Show()
        +Hide()
    }

    %% Иерархия наследования
    Location <|-- Point
    Point <|-- Circle
    Point <|-- Conflict
    Point <|-- BaseFlashlight
    
    Conflict <|-- Screwdriver
    Conflict <|-- Stone
    
    BaseFlashlight <|-- RectFlashlight
    BaseFlashlight <|-- RoundFlashlight
    
    RectFlashlight <|-- BrokenRectFlashlight
    RoundFlashlight <|-- BrokenRoundFlashlight

    %% Группировка для наглядности
    note for BaseFlashlight "Базовый класс фонариков"
    note for RectFlashlight "Прямоугольные фонарики"
    note for RoundFlashlight "Круглые фонарики"
    ```
