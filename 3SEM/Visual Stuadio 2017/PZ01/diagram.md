# UML Diagram - Architecture 1

## Базовая архитектура классов

```mermaid
classDiagram
    class Location {
        -int x
        -int y
        +Location(int, int)
        +~Location()
        +GetX() int
        +GetY() int
        +SetX(int)
        +SetY(int)
    }

    class Point {
        -bool visible
        +Point(int, int)
        +~Point()
        +GetVisible() bool
        +SetVisible(bool)
        +Show()*
        +Hide()*
        +MoveTo(int, int)
        +Drag(int)
    }

    class Conflict {
        -int width
        -int height
        +Conflict(int, int, int, int)
        +~Conflict()
        +GetWidth() int
        +GetHeight() int
        +SetWidth(int)
        +SetHeight(int)
    }

    class Circle {
        -int radius
        +Circle(int, int, int)
        +~Circle()
        +Show() override
        +Hide() override
        +Expand(int)
        +Reduse(int)
    }

    class BaseFlashlight {
        -int bodyWidth
        -int bodyHeight
        -int headWidth
        -int headHeight
        -bool broken
        +BaseFlashlight(int, int, int, int, int, int)
        +~BaseFlashlight()
        +GetBodyWidth() int
        +GetBodyHeight() int
        +GetHeadWidth() int
        +GetHeadHeight() int
        +IsBroken() bool
        +Break()
        +Repair()
    }

    class RectFlashlight {
        +RectFlashlight(int, int, int, int, int, int)
        +~RectFlashlight()
        +Show() override
        +Hide() override
    }

    class RoundFlashlight {
        +RoundFlashlight(int, int, int, int, int, int)
        +~RoundFlashlight()
        +Show() override
        +Hide() override
    }

    class BrokenRectFlashlight {
        +BrokenRectFlashlight(int, int, int, int, int, int)
        +~BrokenRectFlashlight()
        +Show() override
        +Hide() override
    }

    class BrokenRoundFlashlight {
        +BrokenRoundFlashlight(int, int, int, int, int, int)
        +~BrokenRoundFlashlight()
        +Show() override
        +Hide() override
    }

    class Screwdriver {
        +Screwdriver(int, int, int, int)
        +~Screwdriver()
        +Show()
        +Hide()
    }

    class Stone {
        +Stone(int, int, int, int)
        +~Stone()
        +Show()
        +Hide()
    }

    Location <|-- Point
    Point <|-- Circle
    Point <|-- Conflict
    Point <|-- BaseFlashlight
    
    Conflict <|-- Screwdriver
    Conflict <|-- Stone
    
    BaseFlashlight <|-- RectFlashlight
    BaseFlashlight <|-- RoundFlashlight
    BaseFlashlight <|-- BrokenRectFlashlight
    BaseFlashlight <|-- BrokenRoundFlashlight
    
```
