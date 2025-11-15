# Лабораторная работа: Динамический полиморфизм

## Lab2/1:

#### Что сделано?
1. Иерархия с сломанными - наслдениками соответствующих целых

2. В main.cpp заменили BrokenFlashlight на создание BrokenRectFlashlight и BrokenRoundFlashlight.

3. Функция IsCollision работает с новыми классами (она должна работать, так как они наследуются от BaseFlashlight).

4. В коде main.cpp, в секции управления фонариками, мы проверяем столкновения и меняем указатели currentRect и currentRound на соответствующие сломанные или целые версии.


## Lab2/2:
#### Что сделано?
1. ```currentRect``` и ```currentRound``` — это указатели на ```BaseFlashlight*```.
Вызовы вроде 
```cpp
currentRect->IsCollision(rect_conflicts[i])
```
```cpp
currentRect->Show(), currentRect->Hide()
``` 
— это виртуальные функции. В зависимости от того, на какой объект указывает ```currentRect``` (```RectFlashlight``` или ```BrokenRectFlashlight```), вызывается соответствующая реализация ```IsCollision``` (например, для целого фонарика столкновение с камнем ломает его, а для сломанного — нет).
   - Без полиморфизма нам пришлось бы писать отдельный код для каждого типа фонарика. С ним — один универсальный цикл.

**Структура массивов для прямоугольного фонарика (rect)**
(Аналогично для круглого — `round_from_states`, `round_conflicts`, `round_to_states`.)

- **rect_from_states** (массив указателей на `BaseFlashlight*`):
  - `&rectFlashlight, &brokenRect, &brokenRect`
  - Это **исходные состояния**. Для каждого индекса `i` указывает, из какого состояния мы можем перейти.

- **rect_conflicts** (массив указателей на `Conflict*`):
  - `&stone, &screwdriver, &screwdriver`
  - Это **объекты, с которыми проверяется столкновение**. Для каждого перехода — свой "триггер" (камень или отвёртка).

- **rect_to_states** (массив указателей на `BaseFlashlight*`):
  - `&brokenRect, &rectFlashlight, &brokenRect`
  - Это **целевое состояние**. Куда переходим, если условие выполнено.

Константы:
- `TRANSITIONS_COUNT = 2` — первые 2 перехода требуют **наличия столкновения**.
- `NOT_TRANSITIONS_COUNT = 1` — последний переход требует **отсутствия столкновения**.

По шагам:

- **i = 0**:
  - `currentRect == rect_from_states[0]` → `currentRect == &rectFlashlight` (да, если мы в целом состоянии).
  - `collision = currentRect->IsCollision(&stone)` → виртуальный вызов: проверяет, столкнулся ли фонарик с камнем.
  - `valid_transition = (0 < 2 && collision)` → `collision` должен быть `true` (столкновение обязательно).
  - Если да: `currentRect = &brokenRect` (переход в сломанное состояние).
  - **Результат**: Целый фонарик ломает камень → становится сломанным.

- **i = 1**:
  - `currentRect == rect_from_states[1]` → `currentRect == &brokenRect` (теперь мы в сломанном состоянии).
  - `collision = currentRect->IsCollision(&screwdriver)` → виртуальный вызов: проверяет столкновение сломанного фонарика с отвёрткой.
  - `valid_transition = (1 < 2 && collision)` → `collision` должен быть `true`.
  - Если да: `currentRect = &rectFlashlight` (ремонт с помощью отвёртки).
  - **Результат**: Сломанный фонарик + отвёртка → становится целым.

- **i = 2**:
  - `currentRect == rect_from_states[2]` → `currentRect == &brokenRect`.
  - `collision = currentRect->IsCollision(&screwdriver)` → снова проверка с отвёрткой.
  - `valid_transition = (2 >= 2 && !collision)` → `collision` должен быть `false` (нет столкновения).
  - Если да: `currentRect = &brokenRect` (остаётся сломанным, если отвёртка далеко).
  - **Результат**: Если сломанного фонарика отодвинуть от отвёртки, он остаётся сломанным (без ремонта).