import thorpy, pygame, re, sys

pygame.init()
fpsClock = pygame.time.Clock()

res_x = 1024
res_y = 768
screen = pygame.display.set_mode((res_x, res_y))

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

#define a font surface 
spamSurface = pygame.font.SysFont('Arial', 20)

## colour are defined here
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
  actioned = False
  line = f.readline()
  if "initial location" in line:
    match = regex_movement.match(line)
    actioned = True
    if match:
      node_color = blue
      node = int(match.group(2))
      (x,y,z) = (int(float(match.group(3))), int(float(match.group(4))), node_color)
      new_node = (match.group(2),(match.group(3), match.group(4)), node_color)
      nodes.append(new_node)

### handles if a node changes location.
  if "changed location" in line:
    match = regex_movement.match(line)
    actioned = True
    if match:
      node_color = blue
      node = int(match.group(2))
      (x,y,z) = (int(float(match.group(3))), int(float(match.group(4))), node_color)
      nodes[node] = (node, (x,y), z)

### handles if a node sends a packet.              
  if "Sending packet" in line:
    match = regex_tx.match(line)
    actioned = True
    if match:
      node_color = red
      node = int(match.group(2))
      tnode = nodes[node] # this is just a temporary node used for readability. 
      (x,y,z) = (tnode[1][0], tnode[1][1], node_color)
      nodes[node] = (node, (x,y), z)

### handles if a node sends a packet.              
  if "Received packet #" in line:
    match = regex_tx.match(line)
    actioned = True
    if match:
      node_color = green
      node = int(match.group(2))
      tnode = nodes[node] # this is just a temporary node used for readability. 
      (x,y,z) = (tnode[1][0], tnode[1][1], node_color)
      nodes[node] = (node, (x,y), z)

### reset active node colors back to default.   
  if actioned == False:
    match = regex_tx.match(line)
    if match:
      node_color = blue
      node = int(match.group(2))
      tnode = nodes[node] # this is just a temporary node used for readability. 
      (x,y,z) = (tnode[1][0], tnode[1][1], node_color)
      nodes[node] = (node, (x,y), z)

  return nodes

def set_game_and_play():
    ## TODO
    # fix this if possible later. There is some sort of path thing happening
    # p = subprocess.Popen(["bash", "simulation_demo.files/intAttNoDS.sh"], 
    #                     stdout=subprocess.PIPE)
    # output, err = p.communicate()
    # print("*** Simulation has run ***\n", output)
    if varset.get_value("Test One"):
        print("Running test 1\n")
    elif varset.get_value("Test Two"):
        print("Running test 2\n")
    elif varset.get_value("Test Three"):
        print("Running test 3\n")
    elif varset.get_value("Test Four"):
        print("Running test 4\n")
        
    while True:
      nodes = parseline(f)
      screen.fill(white)
      screen.blit(BackGround.image, BackGround.rect)
    
    ### print nodes
      
      for node in nodes:
        x = int(int(node[1][0]) * res_x/size_x)
        y = int(int(node[1][1]) * res_y/size_y)
        node_color = pygame.Color(node[2][0],node[2][1],node[2][2])
        pygame.draw.circle(screen, node_color, (x,y), 10, 0)
        if node[2][2] == 0:
          pygame.draw.circle(screen, node_color, (x,y), 10, 0)
          
      for event in pygame.event.get():
        if event.type == pygame.QUIT:
          pygame.quit()
          f.close()
          sys.exit()
    
        elif event.type == pygame.KEYDOWN:
          if event.key == pygame.K_ESCAPE:
            sys.exit()

      #then, in your infinite cycle...   
      eggsPixels = spamSurface.render(str("hello"), True, (0, 0, 0))
      screen.blit(eggsPixels, (50, 50))    

      pygame.display.update()
      pygame.time.wait(100)

    fpsClock.tick(30)
    return None    

METADATA_PATH = "./metadata" #create a file for storing data on application
mdm = thorpy.MetaDataManager()
mdm.read_data(METADATA_PATH)
#try loading screen size and flags (with default values indicated as arguments)
w,h,flags = mdm.get_display_data(METADATA_PATH, w=800, h=600, flags=0)
app = thorpy.Application((w,h), "DSMAC Demonstration Application", flags=flags)

thorpy.set_theme("human")
mdm.load_font_data(METADATA_PATH)

varset = thorpy.VarSet() #here we will declare options that user can set
varset.add("Test One", value=False, text="Unauthenticated broadcast (no DSMAC)")
varset.add("Test Two", value=False, text="Intelligent replay attack (no DSMAC)")
varset.add("Test Three", value=False, text="Unauthenticated broadcast (with DSMAC)")
varset.add("Test Four", value=False, text="Intelligent replay attack (with DSMAC)")

e_options = thorpy.ParamSetterLauncher.make([varset], "Simulation options", "Options")

e_sim1 = thorpy.make_button("Run simulation", 
                            set_game_and_play)

e_quit = thorpy.make_button("Quit", thorpy.functions.quit_menu_func)

elements = [e_options,e_sim1,e_quit]
#elements = [e_sim1,e_sim2,e_sim3,e_sim4,e_quit]
e_background = thorpy.Background.make(elements=elements)
thorpy.store(e_background)

m_main = thorpy.Menu(e_background)
m_main.play()

app.quit()


