git #!/usr/bin/env python simulation_demo.py

# The MIT License (MIT)

# Copyright (c) 2018 Carlos Amelunge, Christopher Baker, Robert Thompson

# Based upon visualize_castalia.py code (c) 2014 Simon Hook

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

import re
import pygame, sys
from pygame.locals import *

pygame.init()
fpsClock = pygame.time.Clock()

res_x = 1024
res_y = 768
screen = pygame.display.set_mode((res_x, res_y))

size_x = 100
size_y = 100
nodes = []

try:
    path = sys.argv[1]
except IndexError:
    print("No Path given. Try something like this:")
    print(">> python simuation_demo.py Simulations/radioTest/")
    pygame.quit()
    sys.exit()

## colour are defined here
blue  = pygame.Color(  0,  0,255)
white = pygame.Color(255,255,255)
red   = pygame.Color(255,  0,  0)

### Read omnetpp.ini file to get field size ###
inifile = path + 'omnetpp.ini'
ini = open(inifile, 'r')
regex = re.compile(r"(\d+)")
for line in ini:
    if "SN.field_x" in line:
        match = regex.search(line)
        size_x = int(match.group(1))
    elif "SN.field_y" in line:
        match = regex.search(line)
        size_y = int(match.group(1))
ini.close()


### Open Tracefile ###
tracefile = path + 'Castalia-Trace.txt'
f = open(tracefile, "r")
regex_movement = re.compile(r"([0-9]+[\.[0-9]*]*)\D*([0-9]+)\D*([0-9]+[\.[0-9]*]*):([0-9]+[\.[0-9]*]*):([0-9]+[\.[0-9]*]*)")
regex_tx = re.compile(r"([0-9]+[\.[0-9]*]*)\D*([0-9]+)\D")

### This class draws the background image as a sprite.
class Background(pygame.sprite.Sprite):
    def __init__(self, image_file, location):
        pygame.sprite.Sprite.__init__(self)  #call Sprite initializer
        self.image = pygame.image.load(image_file)
        self.rect = self.image.get_rect()
        self.rect.left, self.rect.top = location

BackGround = Background('simulation_demo.files/background.png', [20,40])

### parse a line from the trace file and check for mobility ###
def parseline(f):
    line = f.readline()
    if "initial location" in line:
        match = regex_movement.match(line)
        if match:
            # print (match.group(2)) ## DEBUG statement, refactor out.
            # print (str(match.group(1)) + ", " + str(match.group(2)) + ", " + str(match.group(3)) + ":" + str(match.group(4)) + ":" + str(match.group(5)))
            node_color = blue
            node = int(match.group(2))
            (x,y,z) = (int(float(match.group(3))), int(float(match.group(4))), node_color)
            new_node = (match.group(2),(match.group(3), match.group(4)), node_color)
            nodes.append(new_node)
# handles if a node changes location.
    if "changed location" in line:
          match = regex_movement.match(line)
          if match:
              node_color = blue
              node = int(match.group(2))
              (x,y,z) = (int(float(match.group(3))), int(float(match.group(4))), node_color)
              nodes[node] = (node, (x,y), z)

# handles if a node sends a packet.              
    if "Sending packet" in line:
          match = regex_tx.match(line)
          if match:
              node_color = red
              node = int(match.group(2))
              nodes[node] = nodes[node]

    return nodes


### Main loop of "Game" ###
while True:
    nodes = parseline(f)
    screen.fill(white)
    screen.blit(BackGround.image, BackGround.rect)

    # print nodes

    for node in nodes:
        x = int(int(node[1][0]) * res_x/size_x)
        y = int(int(node[1][1]) * res_y/size_y)
        node_color = pygame.Color(node[2][0],node[2][1],node[2][2])
        pygame.draw.circle(screen, node_color, (x,y), 10, 0)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            f.close()
            sys.exit()

        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                pygame.event.post(pygame.event.Event(pygame.QUIT))

    pygame.display.update()
fpsClock.tick(30)