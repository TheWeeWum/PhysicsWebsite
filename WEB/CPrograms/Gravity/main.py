import os
from subprocess import call
from PIL import Image
from random import randint
from time import time_ns
from math import sqrt, sin, cos, pi

import json
import sys

print(sys.argv)
data = json.loads(sys.argv[1])

x = int(data['x'])  # 400
y = int(data['y'])  # 400
linesPer = int(data['lines'])  # 6
numMass = int(data['numMass'])  # 6
color = int(data['color'])

objects = []
for i in range(numMass):
      temp = []
      temp.append(int(data[f'x{i+1}']))
      temp.append(int(data[f'y{i+1}']))
      temp.append(int(data[f'mass{i+1}']))
      objects.append(temp[:])

# x, y, mass
if "hidden":
      pass
      # random objecs
      # numobjects = 8
      # maxobjmagnitude = 30
      # negative_allowed = -1
      # objects = []
      # for _ in range(numobjects):
      #       objects.append([randint(0, x), randint(0, x), randint(-maxobjmagnitude * negative_allowed, maxobjmagnitude)])

      # dipole
      # objects = [
      #       [x/4, y/2, 50],
      #       [3*x/4, y/2, -50],
      # ]

      # "helium"
      # pos = 2
      # neg = 2
      # scale = 10

      # objects = [
      #       [x/2, y/2, pos * scale]
      # ]
      # num_elec_cloud = 30
      # A = sqrt(x*x+y*y) / 4
      # for i in range(num_elec_cloud):
      #       objects.append([x/2 + A*sin(2*pi*i/num_elec_cloud), y/2 + A*cos(2*pi*i/num_elec_cloud), -neg * scale/num_elec_cloud])

      # single planet
      # objects = [
      #       [x/2, y/2, 20]
      # ]

      # objects = [
      #       [x/2, y/2, 50],
      #       [x/3, y/2, 2],
      #       [2*x/3, 3*y/4, 3]
      # ]

      # earth and moon
      # objects = [
      #       [x/2, y/2, 30.01357398],
      #       [x/1.5, y/2, 0.3670001508]
      # ]

      # sun-system
      # objects = [
      #       [0, y/2, 1000/100],
      #       [0.00980292564 * x, y/2, 0.0001659041778*10],
      #       [0.01831911147 * x, y/2, 0.002448343472*10],
      #       [0.02532845727 * x, y/2, 0.003001357398*10],
      #       [0.03860219423 * x, y/2, 0.0003227590368*10],
      #       [0.1318061764 * x, y/2, 0.9542003921*1],
      #       [0.242448869 * x, y/2, 0.2855562817*1],
      #       [0.4854056617 * x, y/2, 0.04363782615*1],
      #       [0.7644250305 * x, y/2, 0.05127947313*1],
      #       [1 * x, y/2, 0.000006535619124*10]
      # ]


# ########################### trajectory section ############################
# [
#   [xStart, yStart, vx, vy, sign]
# ]
universal_constant = 40
trajectories = []
# trajectories = [
#       [x/2, y/3+y/15, 7.971, 0, 1]
# ]


call_args = ["C://Users//liamc//VSCode//WEB//CPrograms//Gravity//gravsim.exe"]
call_args.extend([f"{x}", f"{y}", f"{linesPer}", f"{color}", f"{len(objects)}"])
for obj in objects:
      call_args.extend([f"{obj[0]}", f"{obj[1]}", f"{obj[2]}"])

call_args.extend([f"{universal_constant}", f"{len(trajectories)}"])
for trajectory in trajectories:
      call_args.extend([f"{trajectory[0]}", f"{trajectory[1]}", f"{trajectory[2]}", f"{trajectory[3]}", f"{trajectory[4]}"])

time_before_program = time_ns()
print("------ calling program ------")
call(call_args)
print("------ program complete ------")
time_after_program = time_ns()

print(f"------ program took {time_after_program - time_before_program} ns")

im = Image.open("static/Images/gravsim.ppm")
print("------ saving images ------")
im.save("static/Images/gravsim.png")
time_after_image_convert = time_ns()
print(f"------ saving image took {time_after_image_convert - time_after_program} ns")


os.remove("static/Images/gravsim.ppm")
print("------ completely complete ------")
