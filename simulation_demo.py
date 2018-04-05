'''
The MIT License

Copyright 2018 Carlos Amelunge, Christopher Baker, Robert Thompson

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.
'''

import thorpy, pygame, re, sys

class TextRectException:
    def __init__(self, message=None):
            self.message = message

    def __str__(self):
        return self.message

pygame.init()
fpsClock = pygame.time.Clock()

res_x = 1024
res_y = 768
screen = pygame.display.set_mode((res_x, res_y))
smallfont = pygame.font.SysFont('Arial', 12)
basicfont = pygame.font.SysFont('Arial', 15)

size_x = 100
size_y = 100
nodes = []
global varset

try:
  path = sys.argv[1]
except IndexError:
  print("No Path given. Try something like this:")
  print(">> python simuation_demo.py Simulations/radioTest/")
  pygame.quit()
  sys.exit()

## colours are defined here
blue  = pygame.Color(  0,  0,255)
white = pygame.Color(255,255,255)
red   = pygame.Color(255,  0,  0)
green = pygame.Color(  0,255,  0) 

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

### a collection of regular expressions to parse what we need from
#   the log files
regex_movement = re.compile(r"([0-9]+[\.[0-9]*]*)\D*([0-9]+)\D*([0-9]+[\.[0-9]*]*):([0-9]+[\.[0-9]*]*):([0-9]+[\.[0-9]*]*)")
regex_tx = re.compile(r"([0-9]+[\.[0-9]*]*)\D*([0-9]+)\D")
regex_trimAction = re.compile(r".*? (    )([a-z]*.)([^ ].*)")

### This class draws the background image as a sprite.
class Background(pygame.sprite.Sprite):
  def __init__(self, image_file, location):
    pygame.sprite.Sprite.__init__(self)  #call Sprite initializer
    self.image = pygame.image.load(image_file)
    self.rect = self.image.get_rect()
    self.rect.left, self.rect.top = location

BackGround = Background('simulation_demo.files/background.png', [20,40])


### parse a line from the trace file and figure out what's going on in it  ###
def parseline(f):
  actioned = False
  timeStamp = ""
  line = f.readline()
### parses out the description of what's happened in the line to display   ###
#   on the screen                                                          ###
  m = regex_trimAction.search(line)
  if m:
    logEntry=m.group(3)

### sets the initial location of the nodes                                 ###    
  if "initial location" in line:
    match = regex_movement.match(line)
    actioned = True
    if match:
      node_color = blue
      node = int(match.group(2))
      (x,y,z,timeStamp,logEntry,node) = (int(float(match.group(3))), 
       int(float(match.group(4))), node_color, timeStamp,logEntry,node)
      new_node = (match.group(2),(match.group(3), match.group(4)), 
                  node_color, timeStamp,logEntry,node)
      nodes.append(new_node)

### handles if a node changes location.
  if "changed location" in line:
    match = regex_movement.match(line)
    actioned = True
    if match:
      timeStamp = match.group(1)
      node_color = blue
      node = int(match.group(2))
      (x,y,z,timeStamp,logEntry,node) = (int(float(match.group(3))), 
       int(float(match.group(4))), node_color, timeStamp,logEntry,node)
      nodes[node] = (node, (x,y), z,timeStamp,logEntry,node)

### handles if a node sends a packet.              
  if "Sending packet" in line:
    match = regex_tx.match(line)
    actioned = True
    if match:
      timeStamp = match.group(1)
      node_color = red
      node = int(match.group(2))
      tnode = nodes[node] # this is just a temporary node used for readability. 
      (x,y,z,timeStamp,logEntry,node) = (tnode[1][0], tnode[1][1], node_color,
                                    timeStamp,logEntry,node)
      nodes[node] = (node, (x,y), z,timeStamp,logEntry,node)


### handles if a node receives a packet.                                    ###              
  if "Received packet #" in line:
    match = regex_tx.match(line)
    actioned = True
    if match:
      timeStamp = match.group(1)
      node_color = green
      node = int(match.group(2))
      tnode = nodes[node] # this is just a temporary node used for readability. 
      (x,y,z,timeStamp,logEntry,node) = (tnode[1][0], tnode[1][1], node_color, 
                                    timeStamp,logEntry,node)
      nodes[node] = (node, (x,y), z,timeStamp,logEntry,node)

### reset active node colors back to default.   
  if actioned == False:
    match = regex_tx.match(line)
    if match:
      timeStamp = match.group(1)
      node_color = blue
      node = int(match.group(2))
      tnode = nodes[node] # this is just a temporary node used for readability. 
      (x,y,z,timeStamp,logEntry,node) = (tnode[1][0], tnode[1][1], node_color,
      timeStamp,logEntry,node)
      nodes[node] = (node, (x,y), z,timeStamp,logEntry,node)

  return nodes

def set_game_and_play():
    storagePath = 'simulation_demo.files/'
    testId = 'DSMAC_Attacked_'
    ## TODO
    # decide if we want to generate in real time, or play the perfect file.
    # p = subprocess.Popen(["bash", "simulation_demo.files/intAttNoDS.sh"], 
    #                     stdout=subprocess.PIPE)
    # output, err = p.communicate()
    # print("*** Simulation has run ***\n", output)
    if varset.get_value("Test One"):
        testId = 'DSMAC_Attacked_'
    elif varset.get_value("Test Two"):
        testId = 'DSMAC_NoEvil_'
    elif varset.get_value("Test Three"):
        testId = 'TMAC_Baseline_'
    elif varset.get_value("Test Four"):
        testId = 'TMAC_Broadcastattack_'
    tracefile = storagePath + testId + 'Castalia-Trace.txt'
    f = open(tracefile, "r")
    
    pwr = pygame.image.load(storagePath + testId + 'Pwr.png')
    tx  = pygame.image.load(storagePath + testId + 'Tx.png')
    textTitle = basicfont.render(testId + 'Simulation', True, 
                                (0, 0, 0), (255, 255, 255))    
    textTitlerect = textTitle.get_rect()

    while True:
      nodes = parseline(f)

      textTitlerect = pygame.draw.rect(screen,white,(500,20,50,50))
      textrectPrev = pygame.draw.rect(screen,white,(400,25,400,200))

      screen.fill(white)
      screen.blit(BackGround.image, BackGround.rect)
      screen.blit(pwr,(450,350))
      screen.blit(tx,(450,115))

    ### print nodes
      
      for node in nodes:
        x = int(int(node[1][0]) * res_x/size_x)
        y = int(int(node[1][1]) * res_y/size_y)
        node_color = pygame.Color(node[2][0],node[2][1],node[2][2])
        pygame.draw.circle(screen, node_color, (x,y), 10, 0)
        if node[2][2] == 0:
          pygame.draw.circle(screen, node_color, (x,y), 10, 0)
 
        timeStamp = node[3]
        text = basicfont.render("Time sequence:  " + timeStamp, True, 
                                (0, 0, 0), (255, 255, 255))
        textrect = text.get_rect()

        screen.blit(text, textrect)
        screen.blit(textTitle, textTitlerect)
        currentMsg = str(node[5]) + ": " + str(node[4])

        msgBuffer = [currentMsg]
        for i in msgBuffer:
            text = smallfont.render(i, True, (255, 0, 0), (255, 255, 255))
            textrectPrev.centery += 15
            screen.blit(text, textrectPrev)
    
      for event in pygame.event.get():
        if event.type == pygame.QUIT:
          pygame.quit()
          f.close()
          sys.exit()
    
        elif event.type == pygame.KEYDOWN:
          if event.key == pygame.K_ESCAPE:
            sys.exit()  
      
      pygame.display.update()
      pygame.time.wait(500)
    fpsClock.tick(30)
    return None    

METADATA_PATH = "./metadata" #create a directory to store cached data
mdm = thorpy.MetaDataManager()
mdm.read_data(METADATA_PATH)

#try loading screen size and flags (with default values indicated as arguments)
w,h,flags = mdm.get_display_data(METADATA_PATH, w=800, h=600, flags=0)
app = thorpy.Application((w,h), "DSMAC Demonstration Application", flags=flags)

thorpy.set_theme("human")
mdm.load_font_data(METADATA_PATH)

varset = thorpy.VarSet() #here we will declare options that user can set
varset.add("Test One", value=False, 
           text="DSMAC Attacked")
varset.add("Test Two", value=False, 
           text="DSMAC Normal Function")
varset.add("Test Three", value=False, 
           text="TMAC Baseline")
varset.add("Test Four", value=False, 
           text="TMAC Replay Attack")

e_options = thorpy.ParamSetterLauncher.make([varset], 
                                            "Simulation options", "Options")

e_sim1 = thorpy.make_button("Run simulation", 
                            set_game_and_play)

e_quit = thorpy.make_button("Quit", thorpy.functions.quit_menu_func)

elements = [e_options,e_sim1,e_quit]
e_background = thorpy.Background.make(elements=elements)
thorpy.store(e_background)

m_main = thorpy.Menu(e_background)
m_main.play()

app.quit()