from car import car


class Scene:
    def __init__(self):
        self.canvas = canvas(width=800, height=600)
        self.road = box(pos=vector(0, 0, 0), size=vector(10, 0.1, 1), color=color.gray(0.5))
        self.cars = []

    def add_car(self, car):
        self.cars.append(car)
