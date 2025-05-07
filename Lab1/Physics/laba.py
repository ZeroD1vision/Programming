import numpy as np
import matplotlib.pyplot as plt

# Теоретическое значение магнитной постоянной
mu0_theory = 4 * np.pi * 1e-7  # Гн/м

# Обработка эксперимента с прямым проводом (Таблица 7.5)
def process_straight_wire():
    # Данные для таблицы 7.5
    r_cm = np.array([2, 3, 4, 5, 6, 7, 8, 9, 10])  # см
    r = r_cm * 0.01  # перевод в метры
    inv_r = 1 / r  # м^-1

    # Пример данных B (ЗАМЕНИТЬ НА РЕАЛЬНЫЕ ДАННЫЕ)
    B_i1 = np.array([150*10**(-6), 100*10**(-6)])  # Для i1
    B_i2 = np.array([200*10**(-6), 133*10**(-6)])  # Для i2
    B_i3 = np.array([7.5e-5, 5.4e-5, 3.9e-5, 3.0e-5, 2.4e-5, 2.1e-5, 1.8e-5, 1.65e-5, 1.5e-5])  # Для i3
    B_i4 = np.array([10.0e-5, 7.2e-5, 5.2e-5, 4.0e-5, 3.2e-5, 2.8e-5, 2.4e-5, 2.2e-5, 2.0e-5])  # Для i4

    currents = [15, 20]  # 4 значения тока
    B_data = [B_i1, B_i2]  # Каждый массив содержит 9 элементов

    k_list = []
    mu0_list = []

    plt.figure()
    for i, B in zip(currents, B_data):
        # Выбор точек A (первая) и C (последняя)
        B_A, B_C = B[0], B[-1]
        inv_r_A, inv_r_C = inv_r[0], inv_r[-1]

        # Расчет k
        k = (B_C - B_A) / (inv_r_C - inv_r_A)
        k_list.append(k)

        # Расчет μ₀
        mu0 = (2 * np.pi * k) / i
        mu0_list.append(mu0)

        # Построение графика
        plt.plot(inv_r, B, 'o', label=f'I = {i} A')
        plt.plot([inv_r_A, inv_r_C], [B_A, B_C], '--')

    plt.xlabel('1/r (м⁻¹)')
    plt.ylabel('B (Тл)')
    plt.legend()
    plt.title('Зависимость B от 1/r для прямого провода')
    plt.grid(True)
    plt.show()

    # Среднее μ₀ и погрешность
    mu0_avg = np.mean(mu0_list)
    error = abs(mu0_avg - mu0_theory) / mu0_theory * 100

    print("\nРезультаты для прямого провода:")
    print(f'Коэффициенты k: {k_list}')
    print(f'μ₀: {mu0_list}')
    print(f'Среднее μ₀: {mu0_avg:.2e} Гн/м')
    print(f'Погрешность: {error:.2f}%')

# Обработка эксперимента с круговым витком (Таблица 7.6)
def process_circular_loop():
    # Данные для таблицы 7.6
    x_cm = np.array([2, 3, 4, 5, 6, 7, 8, 9, 10])  # см
    x = x_cm * 0.01  # метры
    R = 0.1  # Пример радиуса витка (м)

    term = R**2 / ( (R**2 + x**2)**(3/2) )

    # Пример данных B (ЗАМЕНИТЬ НА РЕАЛЬНЫЕ ДАННЫЕ)
    B_i1 = np.array([151*10**(-6), 119*10**(-6)])  # Для i1
    B_i2 = np.array([201*10**(-6), 158*10**(-6)])  # Для i2
    B_i3 = np.array([])  # Для i3
    B_i4 = np.array([])  # Для i4

    currents = [15, 20]  # Ввести значения токов
    B_data = [B_i1, B_i2]

    k_list = []
    mu0_list = []

    plt.figure()
    for i, B in zip(currents, B_data):
        # Выбор точек A (первая) и C (последняя)
        B_A, B_C = B[0], B[-1]
        term_A, term_C = term[0], term[-1]

        # Расчет k
        k = (B_C - B_A) / (term_C - term_A)
        k_list.append(k)

        # Расчет μ₀
        mu0 = (2 * k) / i
        mu0_list.append(mu0)

        # Построение графика
        plt.plot(term, B, 'o', label=f'I = {i} A')
        plt.plot([term_A, term_C], [B_A, B_C], '--')

    plt.xlabel('R²/(R² + x²)^(3/2) (м⁻³)')
    plt.ylabel('B (Тл)')
    plt.legend()
    plt.title('Зависимость B от выражения для витка')
    plt.grid(True)
    plt.show()

    # Среднее μ₀ и погрешность
    mu0_avg = np.mean(mu0_list)
    error = abs(mu0_avg - mu0_theory) / mu0_theory * 100

    print("\nРезультаты для кругового витка:")
    print(f'Коэффициенты k: {k_list}')
    print(f'μ₀: {mu0_list}')
    print(f'Среднее μ₀: {mu0_avg:.2e} Гн/м')
    print(f'Погрешность: {error:.2f}%')

# Запуск обработки данных
process_straight_wire()
process_circular_loop()