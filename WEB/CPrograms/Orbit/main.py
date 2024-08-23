import os
from subprocess import call
from PIL import Image
from random import randint
from time import time_ns
from math import sqrt, sin, cos, pi
import imageio

import json
import sys

print(sys.argv)
data = json.loads(sys.argv[1])

x = int(data['x'])
y = int(data['y'])
physicsPerGraphic = int(data['physicsPerGraphic'])  # 600
frames = int(data['frames'])  # 50*7
dt = float(data['dt'])  # 0.00005
universalConstant = float(data['universalConstant'])

call_args = ["C://Users//liamc//VSCode//PhysicsWebsite//WEB//CPrograms//Orbit//orbit.exe"]
call_args.extend([f"{x}", f"{y}", f"{frames}", f"{dt}", f"{physicsPerGraphic}", f"{universalConstant}"])

# mass, x, y, vx, vy
trajectories = []
# trajectories.append([5, x/2, y/2, 0, 0])

for i in range(0, int(data['numMasses'])):
      trajectories.append([
            int(data[f'mass{i+1}']),
            int(data[f'x{i+1}']),
            int(data[f'y{i+1}']),
            int(data[f'vx{i+1}']),
            int(data[f'vy{i+1}']),
      ])

call_args.append(f"{len(trajectories)}")
for t in trajectories:
      call_args.extend([f"{t[i]}" for i in range(len(trajectories[0]))])

time_before_program = time_ns()
print(f"------ calling program ------")
call(call_args)
print("------ program complete ------")
time_after_program = time_ns()

print(f"------ program took {time_after_program - time_before_program} ns")

path = "C:\\Users\\liamc\\VSCode\\physicsWebsite\\WEB\\static\\Images\\"
filenames = [path + f"temp{i}.ppm" for i in range(frames)]
with imageio.get_writer(f'{path}orbit.gif', mode='I', fps=50, loop=0) as writer:
    for filename in filenames:
        image = imageio.imread(filename)
        writer.append_data(image)
        os.remove(filename)

print("------ completely complete ------")
