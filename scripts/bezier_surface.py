import numpy as np
import matplotlib.pyplot as plt
# 定义贝塞尔基函数
def bezier_basis(t):
    return [(1 - t) ** 3, 3 * t * (1 - t) ** 2, 3 * t ** 2 * (1 - t), t ** 3]

# 计算曲面点的函数
def bezier_surface(control_points, u, v):
    B_u = bezier_basis(u)
    B_v = bezier_basis(v)
    point = np.zeros(3)
    for i in range(4):
        for j in range(4):
            point += B_u[i] * B_v[j] * control_points[i][j]
    return point


# 控制点
control_points = np.array([
    [[0, 0, 0], [1, 0, 2], [2, 0, 2], [3, 0, 0]],
    [[0, 1, 2], [1, 1, 4], [2, 1, 4], [3, 1, 2]],
    [[0, 2, 2], [1, 2, 4], [2, 2, 4], [3, 2, 2]],
    [[0, 3, 0], [1, 3, 2], [2, 3, 2], [3, 3, 0]]
])

# 绘制原始曲面
u_values = np.linspace(0, 1, 30)
v_values = np.linspace(0, 1, 30)
original_surface_points = np.array([[bezier_surface(control_points, u, v) for v in v_values] for u in u_values])
x_original = original_surface_points[:, :, 0]
y_original = original_surface_points[:, :, 1]
z_original = original_surface_points[:, :, 2]

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.plot_surface(x_original, y_original, z_original, rstride=1, cstride=1, cmap='viridis', edgecolor='none', alpha=0.6)

# 绘制原始控制点
control_points_flat = control_points.reshape(-1, 3)
ax.scatter(control_points_flat[:, 0], control_points_flat[:, 1], control_points_flat[:, 2], color='red',
           label='Original Control Points')

for i, (px, py, pz) in enumerate(control_points_flat):
    ax.text(px, py, pz, f'P{i}', fontsize=12, ha='right')

ax.set_title('Original Cubic Bézier Surface')
ax.legend()


# 计算16个型值点
u_values = np.linspace(0, 1, 4)
v_values = np.linspace(0, 1, 4)
surface_points = np.array([[bezier_surface(control_points, u, v) for v in v_values] for u in u_values])
x = surface_points[:, :, 0].flatten()
y = surface_points[:, :, 1].flatten()
z = surface_points[:, :, 2].flatten()

# 构造线性方程组
A = []
for i in range(4):
    for j in range(4):
        u = u_values[i]
        v = v_values[j]
        basis = bezier_basis(u)
        A_row = np.kron(basis, bezier_basis(v))
        A.append(A_row)

A = np.array(A) #[16x16]
b = np.column_stack((x, y, z)) #[16,3]

# 求解线性方程组
control_points_reconstructed = np.linalg.lstsq(A, b, rcond=None)[0]  #[16,3]
control_points_reconstructed = control_points_reconstructed.reshape(4, 4, 3)

# 绘制重构的控制点
control_points_reconstructed_flat = control_points_reconstructed.reshape(-1, 3)
ax.scatter(control_points_reconstructed_flat[:, 0], control_points_reconstructed_flat[:, 1], control_points_reconstructed_flat[:, 2],
           color='blue', label='Reconstructed Control Points')

for i, (px, py, pz) in enumerate(control_points_reconstructed_flat):
    ax.text(px, py, pz, f'P{i}', fontsize=12, ha='right')

ax.set_title('Cubic Bezier Surfaces')
ax.legend()

plt.show()
