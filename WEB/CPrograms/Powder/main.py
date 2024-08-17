import os
from subprocess import call
from PIL import Image
from random import randint
from time import time_ns
from math import sqrt, sin, cos, pi
import imageio


x = 100
y = 100
frames = 50*5
dt = 0.002
physicsPerGraphic = 10
maxParticles = 20000

call_args = ["C://Users//liamc//VSCode//WEB//CPrograms//Powder//powder.exe"]
call_args.extend([f"{x}", f"{y}", f"{frames}", f"{dt}", f"{physicsPerGraphic}", f"{maxParticles}"])

# x, y, vx, vy (optional-0), expelRate, particleMass, minVel, maxVel
sources = [
    [x/2, y/2, 0, 0, 4000/physicsPerGraphic, 1, 40, 40]
]

# xStart, xEnd, yStart, yEnd
walls = [
    [x/2-10, x/2-10, y/2-10, y/2+10],
    [x/2+10, x/2+10, y/2-10, y/2+10],
    
    [x/2-10, x/2+10, y/2+10, y/2+10],
    
    [x/2-10, x/2-3, y/2-10, y/2-10],
    [x/2+3, x/2+10, y/2-10, y/2-10],
    # [x/2-10, x/2-3, y/2-11, y/2-11],
    # [x/2+3, x/2+10, y/2-11, y/2-11],
    
    [0,   0,   0, y-1],
    [x-1, x-1, 0, y-1],
    
    [0,   x-1, 0,   0],
    [0,   x-1, y-1, y-1],
]

call_args.append(f"{len(sources)}")
for t in sources:
      call_args.extend([f"{t[i]}" for i in range(len(sources[0]))])
      
call_args.append(f"{len(walls)}")
for t in walls:
      call_args.extend([f"{t[i]}" for i in range(len(walls[0]))])

time_before_program = time_ns()
print(f"------ calling program ------")
call(call_args)
print("------ program complete ------")
time_after_program = time_ns()

print(f"------ program took {time_after_program - time_before_program} ns")

path = "C:\\Users\\liamc\\VSCode\\WEB\\static\\Images\\"
filenames = [path + f"temp{i}.ppm" for i in range(frames)]
with imageio.get_writer(f'{path}powder.gif', mode='I', fps=50, loop=0) as writer:
    for filename in filenames:
        image = imageio.imread(filename)
        writer.append_data(image)
        os.remove(filename)

print("------ completely complete ------")
