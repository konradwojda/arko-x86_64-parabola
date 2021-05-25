from math import atan, cos
from matplotlib import pyplot as plt
import numpy as np

a = 2.5
b = 23
c = -10


def function(x):
    return a*x**2 + b*x + c


def der_func(x):
    return 2*a*x + b


def get_next_point(point: tuple, S: float) -> tuple:
    if point == (0, 0):
        dx = S
    else:
        dx = S * cos(atan(der_func(point[0])))
    return (point[0] + dx, function(point[0] + dx))


def get_points(S):
    res = []
    x = -b / (2*a)
    res.append((x, function(x)))
    for i in range(50):
        point = res[i]
        res.append(get_next_point(point, S))
    return res


def get_x_lst(lst):
    x_lst = []
    for elem in lst:
        x_lst.append(elem[0])
    return x_lst


def get_y_lst(lst):
    y_lst = []
    for elem in lst:
        y_lst.append(elem[1])
    return y_lst


def graph(formula, x_range):
    x = np.array(x_range)
    y = formula(x)
    plt.plot(x, y)


def symetric(lst):
    res = []
    p = -b / (2*a)
    for el in reversed(lst):
        newel = (2*p-el[0], el[1])
        res.append(newel)
    return res


if __name__ == "__main__":
    pts = get_points(0.25)
    sym_pts = symetric(pts)
    new_pts = sym_pts + pts
    print(new_pts)
    x_lst = get_x_lst(new_pts)
    y_lst = get_y_lst(new_pts)
    plt.plot(x_lst, y_lst, markersize=3)
    graph(function, range(-10, 10))
    plt.show()
