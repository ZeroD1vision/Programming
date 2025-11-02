# UML Diagram - Architecture 1

## Базовая архитектура классов

```mermaid
classDiagram
    direction TB

    %% Базовые классы
    class Location {
        <<Abstract>>
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

    %% Специализированные классы
    class Circle {
        -int radius
        +Expand(int)
        +Reduse(int)
    }

    class BaseFlashlight {
        <<Abstract>>
        #int bodyWidth
        #int bodyHeight
        #int headWidth
        #int headHeight
        #bool broken
        +IsBroken() bool
        +Break()
        +Repair()
    }

    %% Конкретные реализации
    class RectFlashlight {
        +Show() override
        +Hide() override
    }

    class RoundFlashlight {
        +Show() override
        +Hide() override
    }

    class BrokenFlashlight {
        -int flashlightType
        -DrawRectHead()
        -DrawRoundHead()
    }

    class Screwdriver {
        -int length
        -int width
        +GetWidth() int
        +GetLength() int
    }

    class Stone {
        -int width
        -int height
        +GetStoneWidth() int
        +GetStoneHeight() int
    }

    %% Иерархия наследования
    Location <|-- Point
    Point <|-- Circle
    Point <|-- BaseFlashlight
    Point <|-- Screwdriver
    Point <|-- Stone
    BaseFlashlight <|-- RectFlashlight
    BaseFlashlight <|-- RoundFlashlight
    BaseFlashlight <|-- BrokenFlashlight
```
