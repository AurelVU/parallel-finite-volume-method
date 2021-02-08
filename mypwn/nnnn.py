import sys
import time

import imageio as imageio
import matplotlib.cm as cm
import matplotlib.pyplot as plt
import numpy as np
from lib2python import library
from celluloid import Camera

T = [[0] * 30] * 30


delta = 0.1
x = y = np.arange(0, delta * 30, delta)
X, Y = np.meshgrid(x, y)


fig, ax = plt.subplots()

camera = Camera(fig)
start_time = time.time()
for i in range(500):
    T = library.run_phis(T, 70, 70)


    #im = ax.imshow(T, interpolation='bilinear', cmap=cm.hot,
    #               origin='lower', extent=[0, 30, 0, 30])

    #a = plt.plot()
    camera.snap()
print("--- %s seconds ---" % (time.time() - start_time))
animation = camera.animate()
animation.save('celluloid_minimal.gif', writer = 'imagemagick')