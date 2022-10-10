import numpy as np

class Player:
    def __init__(self):         
        self.grid = np.zeros((10,10))
        self.hit_grid = np.zeros((10,10)) #on enemy
        self.ships_placed = np.array([1, -1, -1, -1, -1, -1, 1])
        self.ship_current_overlapped = False
        self.is_ready = False
        self.state = False
