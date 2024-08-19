import os
from subprocess import call
from PIL import Image
from random import randint, random
import random as r
from time import time_ns
from math import sqrt, sin, cos, pi
import imageio

import json
import sys

# print(sys.argv)
# data = json.loads(sys.argv[1])

# x = int(data['x'])
# y = int(data['y'])
# frames = int(data['frames'])  # 50*7
# dt = float(data['dt'])  # 0.00005
# physicsPerGraphic = int(data['physicsPerGraphic'])  # 600
# effectRadius = float(data['universalConstant'])


x = 200
y = 200
frames = 400
dt = 0.04
physicsPerGraphic = 1
effectRadius = 15

gravx = 0
gravy = 0

call_args = ["C://Users//liamc//VSCode//PhysicsWebsite//WEB//CPrograms//Fluid//fluid.exe"]
call_args.extend([f"{x}", f"{y}", f"{frames}", f"{dt}", f"{physicsPerGraphic}", f"{effectRadius}", f"{gravx}", f"{gravy}"])

# mass, x, y, vx, vy

num_particles = 2000
min_mass = 1
max_mass = 1
min_x = 10
max_x = x - 10
min_y = 10
max_y = y - 10

min_vx = -1
max_vx = 1
min_vy = -1
max_vy = 1


# min_x = x/2 - 4
# max_x = x/2 + 4

# min_y = y/2 - 4
# max_y = y/2 + 4


call_args.extend([f"{num_particles}", 
                  f"{min_mass}", f"{max_mass}", 
                  f"{min_x}", f"{max_x}", 
                  f"{min_y}", f"{max_y}",
                  f"{min_vx}", f"{max_vx}",
                  f"{min_vy}", f"{max_vy}"])

time_before_program = time_ns()
print(f"------ calling program ------")
call(call_args)
print("------ program complete ------")
time_after_program = time_ns()

print(f"------ program took {time_after_program - time_before_program} ns")

path = "C:\\Users\\liamc\\VSCode\\PhysicsWebsite\\WEB\\static\\Images\\"
filenames = [path + f"temp{i}.ppm" for i in range(frames)]
with imageio.get_writer(f'{path}fluid.gif', mode='I', fps=50, loop=0) as writer:
    for filename in filenames:
        image = imageio.imread(filename)
        writer.append_data(image)
        os.remove(filename)

print("------ completely complete ------")
