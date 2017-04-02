class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

PolygonVar = []

raw_input()
n_vertices = int(raw_input())

for i in range(n_vertices):
    x, y = map(int, raw_input().split())
    p = Point(x, y)
    PolygonVar.append(p)

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import Polygon

points = []
for point in PolygonVar:
    points.append([point.x, point.y])
points = np.array(points)

fig_2_1 = matplotlib.pyplot.figure()
ax = fig_2_1.add_subplot(121)
ax.scatter(points[...,0], points[...,1], color='black', s=200, label = 'vertices')
polygon = Polygon(points, alpha = 0.5)
ax.add_patch(polygon)
ax.set_title('A Polygon', fontsize=18)
fig_2_1.set_size_inches(17,8.5)
fig_2_1.savefig('polygon')
