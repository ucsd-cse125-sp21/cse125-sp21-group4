import json
import numpy as np
file = open("assets\layout\map.json", "r")

mapJson = json.load(file)
objects = mapJson["layers"][1]["objects"]

serverCSV = open("assets/layout/map_server.csv", "w")
graphicsCSV = open("assets/layout/map_client.csv", "w")
ratio = 16

# Tile IDs
SPACE_ID  = 0
OBST_ID   = 1
HUNTER_HP_ID  = 2
HUNTER_ARMOR_ID   = 3
MONSTER_HP_ID   = 4
MONSTER_EVOLVE_ID   = 5
BEAC_ID   = 6

MAP_WIDTH = 500
MAP_HEIGHT = 600

grid = np.zeros((MAP_WIDTH, MAP_HEIGHT))


for object in objects:
    # print(object)
    objectX = int(object["x"] / ratio)
    objectY = int(object["y"] / ratio)
    objectWidth = int(object["width"] / ratio)
    objectHeight = int(object["height"] / ratio)
    rotation = int(object["rotation"])
    if rotation != 0 and rotation != 180 and rotation != -180 and rotation != 360 and rotation != -360:
        print(rotation)
    if rotation == 180 or rotation == -180:
        objectX -= objectWidth
        objectY -= objectHeight

    # only want to draw obstacles in the beginning, will draw objectives when server tells the client
    if object["type"] == "obstacle":
        graphicsCSV.write("{},{},{},{},{}\n".format(object["name"], objectX, objectY, objectWidth, objectHeight))

    tileID = SPACE_ID

    type = object["type"]
    if type == "obstacle":
        tileID = OBST_ID
    elif type == "hunter_healing":
        tileID = HUNTER_HP_ID
    elif type == "hunter_armor":
        tileID = HUNTER_ARMOR_ID
    elif type == "monster_healing":
        tileID = MONSTER_HP_ID
    elif type == "monster_evolve":
        tileID = MONSTER_EVOLVE_ID
    elif type == "beacon":
        tileID = BEAC_ID


    # for each object type logically tell us its place on the grid
    x_count = 0
    for i in range(objectWidth):
        gridX = objectX + x_count

        y_count = 0
        for i in range(objectHeight):
            gridY = objectY + y_count
            if gridX < MAP_WIDTH and gridY < MAP_HEIGHT:
                grid[gridX][gridY] = tileID
            y_count += 1
        x_count += 1
    x_count = 0

for y in range (MAP_HEIGHT):
    for x in range (MAP_WIDTH):
        if x == 499:
            serverCSV.write(str(int(grid[x][y])))
        else:
            serverCSV.write(str(int(grid[x][y])) + ",")
    serverCSV.write("\n")
