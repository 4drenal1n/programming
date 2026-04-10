"""
Лабораторная работа N 1
Численное интегрирование функции f(x) = 8 + 2x - x^2 на [-2, 4]
"""

a = -2  # нижний предел
b = 4   # верхний предел
n_values = [10, 1000, 10000]

for n in n_values:
    h = (b - a) / n

    print("Количество разбиений:", n)

    # Левые прямоугольники
    s_left = 0
    for i in range(n):
        x = a + i * h
        s_left = s_left + (8 + 2 * x - x ** 2)
    s_left = s_left * h
    print("  Левые прямоугольники =", s_left)

    # Правые прямоугольники
    s_right = 0
    for i in range(1, n + 1):
        x = a + i * h
        s_right = s_right + (8 + 2 * x - x ** 2)
    s_right = s_right * h
    print("  Правые прямоугольники =", s_right)

    # Метод трапеций
    s_trap = (8 + 2 * a - a ** 2) / 2
    for i in range(1, n):
        x = a + i * h
        s_trap = s_trap + (8 + 2 * x - x ** 2)
    s_trap = s_trap + (8 + 2 * b - b ** 2) / 2
    s_trap = s_trap * h
    print("  Трапеции =", s_trap)

    # Метод парабол
    s_por = (8 + 2 * a - a ** 2) + (8 + 2 * b - b ** 2)
    for i in range(1, n):
        x = a + i * h
        if i % 2 != 0:
            s_por = s_por + 4 * (8 + 2 * x - x ** 2)
        else:
            s_por = s_por + 2 * (8 + 2 * x - x ** 2)
    s_por = s_por * h / 3
    print("  Параболы =", s_por)

print("Точное значение: 36")
