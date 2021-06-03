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

for bifFuckingN in range(50, 10000, 25):
    start_time = time.time()
    # camera = Camera(fig)
    solver = CUDAsolver(500)
    solver.n = bifFuckingN
    solver.m = bifFuckingN
    solver = iter(solver)


    for solve in solver:
        pass
        #im = ax.imshow(solve, interpolation='bilinear', cmap=cm.hot,
        #               origin='lower', extent=[0, len(solve), 0, len(solve)])
        # a = plt.plot()
        # camera.snap()
    print(f"{(time.time() - start_time)},", end=" ")
    # print("--- %s seconds ---" % (time.time() - start_time))
    # animation = camera.animate()
    # animation.save('celluloid_minimal.gif', writer='imagemagick')
