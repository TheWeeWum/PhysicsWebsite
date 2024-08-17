from numpy import sin, cos, pi, sqrt

def cirleMaker(numPoints: int, xCenter, yCenter, radius, mass, springConst, equib, vx, vy, offset):
    points = []
    springs = []
    
    # closest = radius*sqrt(pow(sin(2*pi/numPoints * 1), 2) + pow(1 - cos(2*pi/numPoints * 1), 2))
    closest = 2*(1 - cos(2*pi/numPoints))
    for i in range(numPoints):
        points.append([mass, xCenter + radius*sin(2*pi/numPoints * i), yCenter + radius*cos(2*pi/numPoints * i), vx, vy, numPoints-1])
        
        for j in range(i+1, numPoints):
            dist = radius * sqrt((pow(sin(2*pi/numPoints * i) - sin(2*pi/numPoints * j), 2) + pow(cos(2*pi/numPoints * i) - cos(2*pi/numPoints * j), 2)))
            springs.append([dist*equib, springConst/pow(dist/closest, 0.5), i+offset, j+offset])
        
        # j = int((i + 1) % numPoints)
        # print(i, j, numPoints)
        # dist = closest
        # springs.append([dist*equib, springConst, i+offset, j+offset])
        
        # j = int((i + int(numPoints)/2) % numPoints)
        # print(i, j, numPoints)
        # dist = radius * sqrt((pow(sin(2*pi/numPoints * i) - sin(2*pi/numPoints * j), 2) + pow(cos(2*pi/numPoints * i) - cos(2*pi/numPoints * j), 2)))
        # springs.append([dist*equib, springConst/pow(dist/closest, 0.5), i+offset, j+offset])
    
    return points, springs


def circleWithMiddle(numPoints, xCenter, yCenter, radius, mass, springConst, equib, vx, vy, offset):
    points = []
    springs = []
    
    closest = 2*(1 - cos(2*pi/numPoints))
    for i in range(numPoints):
        points.append([mass, xCenter + radius*sin(2*pi/numPoints * i), yCenter + radius*cos(2*pi/numPoints * i), vx, vy, numPoints])
        
        for j in range(i+1, numPoints):
            dist = radius * sqrt((pow(sin(2*pi/numPoints * i) - sin(2*pi/numPoints * j), 2) + pow(cos(2*pi/numPoints * i) - cos(2*pi/numPoints * j), 2)))
            springs.append([dist*equib, springConst/pow(dist/closest, 0.5), i+offset, j+offset])
    
    points.append([mass, xCenter, yCenter, vx, vy, numPoints])
    for i in range(numPoints):
        springs.append([radius*equib, springConst/pow(radius/closest, 0.5), numPoints+offset, i+offset])
    
    return points, springs
    