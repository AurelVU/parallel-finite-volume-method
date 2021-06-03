import sys
import time

import imageio as imageio
import matplotlib.cm as cm
import matplotlib.pyplot as plt
import numpy as np
from lib2python.pycuda import CUDAsolver
from celluloid import Camera

T = [[0] * 30] * 30

delta = 0.1
x = y = np.arange(0, delta * 30, delta)
X, Y = np.meshgrid(x, y)

fig, ax = plt.subplots()


start_time = time.time()
# camera = Camera(fig)
solver = CUDAsolver(500)
solver.n = 30
solver.m = 30
solver = iter(solver)


for solve in solver:
    # print(np.asarray(solve))
    im = ax.imshow(np.asarray(solve), interpolation='bilinear', cmap=cm.hot,
                   origin='lower', extent=[0, len(solve), 0, len(solve)])
plt.plot()
plt.show()
    # camera.snap()
