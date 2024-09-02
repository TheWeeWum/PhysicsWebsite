import os
from subprocess import call
from time import time_ns
import imageio

import json
import sys
import platform

data = json.loads(sys.argv[1])

x = int(data['x'])
y = int(data['y'])
frames = int(data['frames'])  # 50*7
physicsPerGraphic = int(data['physicsPerGraphic'])  # 600
dt = float(data['dt'])  # 0.00005
effectRadius = float(data['effectRadius'])
gravx = float(data['gravx'])
gravy = float(data['gravy'])

# x = 8*16
# y = 8*16
# frames = 400
# dt = 0.005
# physicsPerGraphic = 1
# effectRadius = 12
# gravx = 0
# gravy = 0

if platform.system() == "Linux":
    generalPath = "var/www/PhysicsWebsite/WEB/"
else: 
    generalPath = "C:/Users/liamc/VSCode/PhysicsWebsite/WEB/"

imagePath = generalPath + "static/Images/"
if platform.system() == "Linux":
    call_args = [generalPath + "CPrograms/Fluid/fluid", imagePath + 'temp']
else:
    call_args = [generalPath + "CPrograms/Fluid/fluid.exe", imagePath + 'temp']

call_args.extend([f"{x}", f"{y}", f"{frames}", f"{dt}", f"{physicsPerGraphic}", f"{effectRadius}", f"{gravx}", f"{gravy}"])

# mass, x, y, vx, vy

num_particles = int(data['numParticles'])

min_mass = float(data['minMass'])
max_mass = float(data['maxMass'])

min_x = float(data['minX'])
max_x = float(data['maxX'])
min_y = float(data['minY'])
max_y = float(data['maxY'])

min_vx = float(data['minVX'])
max_vx = float(data['maxVX'])

min_vy = float(data['minVY'])
max_vy = float(data['maxVY'])

pressureView = int(data['pressureView'])
particleView = int(data['particleView'])

pressureConstant = float(data['pressureConstant'])
viscosity = float(data['viscosity'])

colorMode = int(data['colorMode'])

# num_particles = 4500
# min_mass = 1
# max_mass = 1
# min_x = 5
# max_x = x - 5
# min_y = 5
# max_y = y - 5

# min_vx = 0
# max_vx = 0
# min_vy = 0
# max_vy = 0


# min_x = x/2 - 2
# max_x = x/2 + 2

# min_y = y/2 - 2
# max_y = y/2 + 2


call_args.extend([f"{num_particles}", 
                  f"{min_mass}", f"{max_mass}", 
                  f"{min_x}", f"{max_x}", 
                  f"{min_y}", f"{max_y}",
                  f"{min_vx}", f"{max_vx}",
                  f"{min_vy}", f"{max_vy}",
                  f"{pressureView}", f"{particleView}",
                  f"{pressureConstant}", f"{viscosity}",
                  f"{colorMode}"])

print(call_args)
time_before_program = time_ns()
print(f"------ calling program ------")
call(call_args)
print("------ program complete ------")
time_after_program = time_ns()

print(f"------ program took {time_after_program - time_before_program} ns")

filenames = [imagePath + f"temp{i}.ppm" for i in range(frames)]
with imageio.get_writer(f'{imagePath}fluid.gif', mode='I', fps=50, loop=0) as writer:
    for filename in filenames:
        image = imageio.imread(filename)
        writer.append_data(image)
        os.remove(filename)

print("------ completely complete ------")
