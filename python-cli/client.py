import socket
import os
import winsound
import keyboard
import colorama
import getpass
import threading
import pynput
import numpy as np
import time
import pickle
import shared_plr

host = "127.0.0.1"
port = 8888
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((host,port))

colorama.init()
current_ship = -1
current_ship_prev_pos = np.zeros((10,10))
hit_coord = [-1, -1]
hit_sent = False


player = shared_plr.Player()
ships = np.zeros((6,10,10))


ships[1] = np.array([[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 1, 1, 1, 1, 1, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                        ])

ships[2] = np.array([[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 2, 2, 2, 2, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                        ])

ships[3] = np.array([[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 3, 3, 3, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                        ])

ships[4] = np.array([[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 4, 4, 4, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                        ])

ships[5] = np.array([[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 5, 5, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ],
                        ])


def set_cursor_pos(x, y):
    print("\033[%d;%dH" % (x, y), end="")

def is_cell(x,y):
    y = y - 12
    if (y % 6 == 0) or (x % 3 == 0):
        return False

    return True

def get_cell(x,y):
    if not is_cell(x,y):
        return (-1, -1)
    return (y // 6, x // 3)

def get_pos_from_cell(cx,cy):
    return (cx * 3+2, cy * 6 + 12)

def fill_cell(cx,cy, color):
    a, b = get_pos_from_cell(cx, cy)
    set_cursor_pos(a, b)
    if color == "white":
        print(colorama.Fore.WHITE + "█████", end="")
        set_cursor_pos(a+1, b)
        print(colorama.Fore.WHITE + "█████" + colorama.Fore.RESET, end="")
    elif color == "red":
        print(colorama.Fore.RED + "█████", end="")
        set_cursor_pos(a+1, b)
        print(colorama.Fore.RED + "█████" + colorama.Fore.RESET, end="")
    elif color == "green":
        print(colorama.Fore.GREEN + "█████", end="")
        set_cursor_pos(a+1, b)
        print(colorama.Fore.GREEN + "█████" + colorama.Fore.RESET, end="")
    elif color == "black":
        print(colorama.Fore.BLACK + "█████", end="")
        set_cursor_pos(a+1, b)
        print(colorama.Fore.BLACK + "█████" + colorama.Fore.RESET, end="")

def clear_cell(cx,cy):
    a, b = get_pos_from_cell(cx, cy)
    set_cursor_pos(a, b)
    print("     ", end="")
    set_cursor_pos(a+1, b)
    print("     ", end="")


def can_ship_move(ship, direction):
    if ship == 0:
        return False

    ship_matrix = ships[ship]
    if direction == "left":
        for i in range(0, 10):
            for j in range(0, 10):
                if (ship_matrix[i][j] != 0) and (j == 0):
                    return False
    
    elif direction == "right":
        for i in range(0, 10):
            for j in range(0, 10):
                if (ship_matrix[i][j] != 0) and (j == 9):
                    return False

    elif direction == "up":
        for i in range(0, 10):
            for j in range(0, 10):
                if (ship_matrix[i][j] != 0) and (i == 0):
                    return False
    
    elif direction == "down":
        for i in range(0, 10):
            for j in range(0, 10):
                if (ship_matrix[i][j] != 0) and (i == 9):
                    return False
    return True

def ship_move(ship, direction):
    if ship == 0: 
        return

    if not can_ship_move(ship, direction):
        return False
    
    ship_matrix = ships[ship]
    
    new_matrix = np.zeros((10,10))

    if direction == "left":
        for i in range(0, 10):
            for j in range(0, 10):
                if ship_matrix[i][j] != 0:
                    new_matrix[i][j-1] = ship

    if direction == "right":
        for i in range(0, 10):
            for j in range(0, 10):
                if ship_matrix[i][j] != 0:
                    new_matrix[i][j+1] = ship

    if direction == "up":
        for i in range(0, 10):
            for j in range(0, 10):
                if ship_matrix[i][j] != 0:
                    new_matrix[i-1][j] = ship

    if direction == "down":
        for i in range(0, 10):
            for j in range(0, 10):
                if ship_matrix[i][j] != 0:
                    new_matrix[i+1][j] = ship

    ships[ship] = new_matrix

def ship_rotate(ship):
    if ship == 0:
        return

    ships[ship] = np.rot90(ships[ship])

def is_ship_overlap(ship1, ship2):
    ship_matrix1 = ships[ship1]
    ship_matrix2 = ships[ship2]
    for i in range(0,10):
        for j in range(0,10):
            if (ship_matrix1[i][j] != 0) and (ship_matrix2[i][j] !=0):
                return True
    return False

def grid_init():
    set_cursor_pos(1, 1)
    print("          ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          │     │     │     │     │     │     │     │     │     │     │")
    print("          └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘")

def grid_print_edit():
    player.ship_current_overlapped = False
    for i in range(1,6):
        if i != current_ship and player.ships_placed[i] != -1:
            if is_ship_overlap(current_ship, i):
                player.ship_current_overlapped = True
            for j in range(0,10):
                for k in range(0,10):
                    if ships[i][j][k] != 0:
                        fill_cell(j, k, "white")

    if current_ship != -1:
        for i in range(0,10):
            for j in range(0,10):
                if ships[current_ship][i][j] != 0:
                    if player.ship_current_overlapped:
                        fill_cell(i, j, "red")
                    else:
                        fill_cell(i, j, "green")


def listen_for_ship_edit(key):
    global current_ship
    global current_ship_prev_pos
    for i in range(0,10):
        for j in range(0,10):
            if current_ship_prev_pos[i][j] != 0:
                clear_cell(i, j)

    try:
        if key.char in ("1", "2", "3", "4", "5"):
            if player.ship_current_overlapped and key.char != str(current_ship):
                player.ships_placed[current_ship] = -1
            else:
                player.ships_placed[current_ship] = 1
            current_ship = int(key.char)
            set_cursor_pos(16, 84)
            print("    ", end="")
            for i in range (1,6):
                if i == current_ship:
                    print(colorama.Fore.GREEN + str(i) + "   " + colorama.Fore.RESET, end="")
                elif player.ships_placed[i] == -1:
                    print(colorama.Fore.BLACK + str(i) + "   " + colorama.Fore.RESET, end="")
                else:
                    print(colorama.Fore.WHITE + str(i) + "   " + colorama.Fore.RESET, end="")

        if key.char == "r":
            ship_rotate(current_ship)
        
        if key.char == "t":
            print(player.ships_placed)

    except AttributeError:
        if key in (pynput.keyboard.Key.up, pynput.keyboard.Key.down, pynput.keyboard.Key.left, pynput.keyboard.Key.right):
            if key == pynput.keyboard.Key.up:
                ship_move(current_ship, "up")
            if key == pynput.keyboard.Key.down:
                ship_move(current_ship, "down")
            if key == pynput.keyboard.Key.left:
                ship_move(current_ship, "left")
            if key == pynput.keyboard.Key.right:
                ship_move(current_ship, "right")
        if key == pynput.keyboard.Key.enter:
            neg = 0
            for i in range(1,6):
                if player.ships_placed[i] < 0:
                    neg += 1
            if neg == 1 and player.ship_current_overlapped == False:
                player.is_ready = True
                return False
            else:
                set_cursor_pos(18, 84)
                print("Not all ships placed!", end="")

    current_ship_prev_pos = ships[current_ship]
    grid_print_edit()
    

def grid_shot(key):
    global hit_sent
    global hit_coord
    hit_sent = False
    if key in (pynput.keyboard.Key.up, pynput.keyboard.Key.down, pynput.keyboard.Key.left, pynput.keyboard.Key.right):
        x, y = hit_coord[0], hit_coord[1]

        if key == pynput.keyboard.Key.left:
            if y != 0:
                y -= 1
        if key == pynput.keyboard.Key.right:
            if y != 9:
                y += 1
        if key == pynput.keyboard.Key.up:
            if x != 0:
                x -= 1
        if key == pynput.keyboard.Key.down:
            if x != 9:
                x += 1
        #If something was overwritten, write it back 
        if player.hit_grid[hit_coord[0]][hit_coord[1]] == 1:
            fill_cell(hit_coord[0], hit_coord[1], "red")
        elif player.hit_grid[hit_coord[0]][hit_coord[1]] == -1:
            fill_cell(hit_coord[0], hit_coord[1], "black")
        else:
            clear_cell(hit_coord[0], hit_coord[1])

        hit_coord[0], hit_coord[1] = x, y
        fill_cell(x, y, "green")
    elif key == pynput.keyboard.Key.enter:
        hit_sent = True
        return False


'''
def get_hit_coord(key):
    global hit_sent
    global hit_coord
    hit_sent = False
    if key in (pynput.keyboard.Key.up, pynput.keyboard.Key.down, pynput.keyboard.Key.left, pynput.keyboard.Key.right):
        if player.hit_grid[hit_coord[0]][hit_coord[1]] == 0:
            clear_cell(hit_coord[0], hit_coord[1])
        elif player.hit_grid[hit_coord[0]][hit_coord[1]] == -1:
            fill_cell(hit_coord[0], hit_coord[1], "white")
        else:
            fill_cell(hit_coord[0], hit_coord[1], "red")
        if key == pynput.keyboard.Key.left:
            if hit_coord[1] != 0:
                hit_coord[1] -= 1
        if key == pynput.keyboard.Key.right:
            if hit_coord[1] != 9:
                hit_coord[1] += 1
        if key == pynput.keyboard.Key.up:
            if hit_coord[0] != 0:
                hit_coord[0] -= 1
        if key == pynput.keyboard.Key.down:
            if hit_coord[0] != 9:
                hit_coord[0] += 1
        fill_cell(hit_coord[0], hit_coord[1], "green")
    elif key == pynput.keyboard.Key.enter:
        hit_sent = True
        return False
        
'''

os.system("cls")
set_cursor_pos(1,1)
#Shot missed winsound.Beep(500, 1000)
#Shot landed winsound.Beep(1000, 1000)


grid_init()


#Can be used to get input w/o typing to stdout   
#password = getpass.getpass(prompt="")
set_cursor_pos(15, 84)
print("Select ship to place/move", end="")
set_cursor_pos(16, 84)
print(colorama.Fore.BLACK + "    1   2   3   4   5" + colorama.Fore.RESET, end="")


listener = pynput.keyboard.Listener(on_release=listen_for_ship_edit)
listener.start()
set_cursor_pos(get_pos_from_cell(8, 9)[0], get_pos_from_cell(8, 9)[1])

while not player.is_ready:
    continue    

listener.stop()

for i in range(1,6):
    for j in range(0,10):
        for k in range(0,10):
            if ships[i][j][k] != 0:
                player.grid[j][k] = ships[i][j][k]

data_stream = pickle.dumps(player)
sock.send(data_stream)

hit_coord[0], hit_coord[1] = 0, 0
while True:
    packet = sock.recv(4096)
    player = pickle.loads(packet)

    if player.state:
        #Show hit grid as is
        grid_init()
        for i in range(1,10):
            for j in range(1,10):
                if player.hit_grid[i][j] == 1:
                    fill_cell(i, j, "red")
                elif player.hit_grid[i][j] == -1:
                    fill_cell(i, j, "black")
                else:
                    clear_cell(i, j)
        
        #Show hit and choose where to send
        listener = pynput.keyboard.Listener(on_release=grid_shot)
        listener.start()
        hit_sent = False
        while not hit_sent:
            continue
        listener.stop()

        #Send coords
        packet = pickle.dumps(hit_coord)
        sock.send(packet)
    else:
        grid_init()
        for i in range(1,10):
            for j in range(1,10):
                if player.grid[i][j] in range(1,10):
                    fill_cell(i, j, "white")
                elif player.grid[i][j] > 10:
                    fill_cell(i, j, "red")
                elif player.grid[i][j] < 0:
                    fill_cell(i, j, "black")

    packet = sock.recv(4096)
    player = pickle.loads(packet)



