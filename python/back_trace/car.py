from vpython import *

class Car:
    def __init__(self, position):
        self.car = compound([
            cylinder(pos=position + vector(-0.5, 0, 0), axis=vector(0, 0, 0.1), radius=0.2, color=color.red),
            cylinder(pos=position + vector(0.5, 0, 0), axis=vector(0, 0, 0.1), radius=0.2, color=color.red)
        ])

        

    def move(self, dx):
        self.car.pos.x += dx