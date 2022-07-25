import socket
import threading
import pickle
import numpy as np
import shared_plr

sock = socket.socket()
host = "127.0.0.1"
print(host)
port = 8888

print("Server started...")

sock.bind((host, port))
sock.listen(2)

print("Waiting for players...")

player1sock, addr1 = sock.accept()
print("Player1 connected...")

player2sock, addr2 = sock.accept()
print("Player2 connected...")

print("Both players connected, starting in 3s...")

player_confirm = [0,0]
player1 = shared_plr.Player()
player2 = shared_plr.Player()

while not player_confirm[0] and not player_confirm[1]:
    msg = player1sock.recv(8192)
    if msg != "":
        player1 = pickle.loads(msg)
        player_confirm[0] = True
    msg = player2sock.recv(8198)
    if msg != "":
        player2 = pickle.loads(msg)
        player_confirm[1] = True

#From now: Send turn signal, switch screens based on that

print(player1, player2)

match = True
#Player state True means it's the players turn to shoot
player1.state = True
player2.state = False
while match:
    #Send player states
    print("Sending dumps")

    packet = pickle.dumps(player1)
    player1sock.send(packet)
    packet = pickle.dumps(player2)
    player2sock.send(packet)

    print(player1.grid)
    print(player2.grid)

    if player1.state:
        print("Player 1: ", player1.state, player2.state)
        #Wait for response from shooter
        packet = player1sock.recv(256)
        shot = pickle.loads(packet)
        x, y = shot[0], shot[1]
        print(x,y,player1.grid[x][y])
        if player2.grid[x][y] > 0 and player2.grid[x][y] < 10:
            print("Player 1 hit")
            #Means it was a hit
            player2.grid[x][y] = 128
            player1.hit_grid[x][y] = 1
            player1.state = True
            player2.state = False
        else:
            print("Player 1 missed")
            #Means it was a miss
            player2.grid[x][y] = -128
            player1.hit_grid[x][y] = -1
            player1.state = False
            player2.state = True

    elif player2.state:
        packet = player2sock.recv(256)
        shot = pickle.loads(packet)
        x, y = shot[0], shot[1]
        print(x,y,player1.grid[x][y])
        if player1.grid[x][y] > 0 and player1.grid[x][y] < 10:
            #Means it's a hit
            print("Player 2 hit")
            player1.grid[x][y] = 128
            player2.hit_grid[x][y] = 1
            player2.state = True
            player1.state = False
        else:
            #Means it missed
            print("Player 2 missed")
            player1.grid[x][y] = -128
            player2.hit_grid[x][y] = -1
            player1.state = True
            player2.state = False

    #Send player states
    print("Sending dumps")
    packet = pickle.dumps(player1)
    player1sock.send(packet)
    packet = pickle.dumps(player2)
    player2sock.send(packet)



        


