import os
from subprocess import call
from PIL import Image
from random import randint
from time import time_ns
from math import sqrt, sin, cos, pi
import imageio
from shapeGenerator import *

import json
import sys

print(sys.argv)
data = json.loads(sys.argv[1])


x = int(data['x'])
y = int(data['y'])
physicsPerGraphic = int(data['physicsPerGraphic'])  # 600
frames = int(data['frames'])  # 50*7
dt = float(data['dt'])  # 0.00005
damping = float(data['damping'])  # 0.00001

gravx = float(data['gravx'])  # 0
gravy = float(data['gravy'])  # 9


call_args = ["C://Users//liamc//VSCode//PhysicsWebsite//WEB//CPrograms//Bounce//bounce.exe"]
call_args.extend([f"{x}", f"{y}", f"{frames}", f"{dt}", f"{gravx}", f"{gravy}", f"{physicsPerGraphic}", f"{damping}"])

points = []
springs = []

# points, springs = cirleMaker(
#       15, 
#       sqrt(2)*x/2, 2*y/3, 
#       10, 1, 66, 1, 
#       50, -30, offset=0)
# points2, springs2 = cirleMaker(
#       10, 
#       2*x/3, y/3, 
#       10, 1, 2000, 1, 
#       -50, 30, offset=len(points))
# points.extend(points2)
# springs.extend(springs2)
for i in range(0, int(data['numCircles'])):
      pointsTemp, springsTemp = cirleMaker(
            int(data[f'C_numPoints{i+1}']),
            int(data[f'C_xCenter{i+1}']),
            int(data[f'C_yCenter{i+1}']),
            int(data[f'C_radius{i+1}']),
            int(data[f'C_mass{i+1}']),
            int(data[f'C_springConstant{i+1}']),
            int(data[f'C_equilib{i+1}']),
            int(data[f'C_vx{i+1}']),
            int(data[f'C_vy{i+1}']),
            offset=len(points),
      )
      points.extend(pointsTemp)
      springs.extend(springsTemp)
      

# mass, x, y, vx, vy, numConnections
# points = [
#     [1, x/2 - 10, y/2 - 10, 0, 0, 3],
#     [1, x/2 - 10, y/2 + 10, 0, 0, 3],
#     [1, x/2 + 10, y/2 - 10, 0, 0, 3],
#     [1, x/2 + 10, y/2 + 10, 0, 0, 3],
# ]
# equilib, springConst, indecies of first and second connection points
# springs = [
#     [20, 4, 1, 0],
#     [20, 4, 1, 3],
#     [20, 4, 2, 3],
#     [20, 4, 0, 2],
#     [20*sqrt(2), 4, 0, 3]
#     [20*sqrt(2), 4, 1, 2]
# ]
for i in range(0, int(data['numPoints'])):
      points.append([
            int(data[f'P_mass{i+1}']),
            int(data[f'P_x{i+1}']),
            int(data[f'P_y{i+1}']),
            int(data[f'P_vx{i+1}']),
            int(data[f'P_vy{i+1}']),
            int(data[f'P_numConnections{i+1}']),
      ])

for i in range(0, int(data['numSprings'])):
      springs.append([
            int(data[f'S_equilib{i+1}']),
            int(data[f'P_springConst{i+1}']),
            int(data[f'P_index1{i+1}']),
            int(data[f'P_index2{i+1}']),
      ])


call_args.append(f"{len(points)}")
for p in points:
      call_args.extend([f"{p[i]}" for i in range(len(points[0]))])

call_args.append(f"{len(springs)}")
for s in springs:
      call_args.extend([f"{s[i]}" for i in range(len(springs[0]))])

time_before_program = time_ns()
print(f"------ calling program ------")
call(call_args)
print("------ program complete ------")
time_after_program = time_ns()

print(f"------ program took {time_after_program - time_before_program} ns")

path = "C:\\Users\\liamc\\VSCode\\PhysicsWebsite\\WEB\\static\\Images\\"
filenames = [path + f"temp{i}.ppm" for i in range(frames)]
with imageio.get_writer(f'{path}bounce.gif', mode='I', fps=50, loop=0) as writer:
    for filename in filenames:
        image = imageio.imread(filename)
        writer.append_data(image)
        os.remove(filename)

print("------ completely complete ------")
