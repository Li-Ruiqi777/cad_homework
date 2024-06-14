import matplotlib.pyplot as plt
import numpy as np

def get_px(t):
    return (1 - t) ** 2 * P0[0] + 2 * (1 - t) * t * P1[0] + t ** 2 * P2[0]

def get_py(t):
    return (1 - t) ** 2 * P0[1] + 2 * (1 - t) * t * P1[1] + t ** 2 * P2[1]

def get_B_row(t):
    return [(1-t)**2, 2*t*(1-t), t**2]

# 定义控制点
P0 = [0, 0]
P1 = [1, 2]
P2 = [2, 0]

# 定义参数t的取值范围
t = np.linspace(0, 1, 100)
# t = np.array([0,0.5,1])

# 型值点
t_v = [0,0.5,1]
R = []
B = []
for i in t_v:
    R.append([get_px(i),get_py(i)])
    B.append(get_B_row(i))

R = np.array(R)
print("R = ")
print(R)

B = np.array(B)
print("B = ")
print(B)

B_inv = np.linalg.inv(B)
P = B_inv @ R

print("P = ")
print(P)

# 计算贝塞尔曲线的点
x = get_px(t)
y = get_py(t)

plt.plot(x, y, label='二次贝塞尔曲线')# 绘制原曲线

plt.scatter([P0[0], P1[0], P2[0]], [P0[1], P1[1], P2[1]], color='red')# 绘制控制点
plt.scatter([P[0][0], P[1][0], P[2][0]], [P[0][1], P[1][1], P[2][1]], color='blue')# 绘制反算的控制点

plt.show()# 显示图形