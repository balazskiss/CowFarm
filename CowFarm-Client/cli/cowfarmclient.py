import sys
import time
import thread
import socket
import curses

class ServerConnection:

    def __init__(self, host, port):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect((host, port))
        self.socket.setblocking(0)
        thread.start_new_thread(self.recv, ("RecvThread",))

        self.farms = []
        self.cows = []
        self.msgs = []

    def recv(self, thread):
        while True:
            try:
                self.lastData = self.socket.recv(1024)
                self.parseData(self.lastData)
            except:
                pass

    def parseData(self, data):
        

        if data[0:8] == "farmlist":
            self.farms = data.splitlines()
            del self.farms[0]
        
        elif data[0:7] == "cowlist":
            cows = data.splitlines()
            del cows[0]
            self.cows = []
            for cow in cows:
                c = {}
                p = cow.split(': ')
                c['id'] = int(p[0])
                p = p[1].split(', ')
                c['name'] = p[0]
                c['x'] = int(p[1])
                c['y'] = int(p[2])
                c['dir'] = int(p[3])
                self.cows.append(c)
            fd.needsRender = True;

        elif data[0:8] == "msg from":
            p = data.split(': ')
            m = {}
            m["cow_id"] = int(p[0][p[0].rindex(" "):])
            m["text"] = p[1].rstrip("\n")
            m["time"] = time.time()
            self.msgs.insert(0, m)
            fd.needsRender = True;




    def send(self, msg):
        self.socket.sendall(msg+'\n')

    def setName(self, name):
        self.send("name: " + name)
        time.sleep(1)
        if self.lastData != "OK\n":
            raise ValueError

    def getFarmList(self):
        self.send("farmlist")
        time.sleep(1)
        return self.farms

    def joinFarm(self, farm):
        self.send("join: " + farm)
        time.sleep(1)

    def sendMsg(self, msg):
        self.send("msg: "+msg)

    def move(self, dx, dy):
        self.send("move: %d, %d" % (dx, dy))




class FarmDisplay:

    def __init__(self):
        self.scr = curses.initscr()
        curses.noecho()
        curses.raw()
        curses.cbreak()
        #self.scr.curses.nodelay(1)
        self.scr.keypad(1)
        self.maxY, self.maxX = self.scr.getmaxyx()
        self.needsRender = False;
        thread.start_new_thread(self.refresh, ("RefreshThread",))

    def refresh(self, thread):
        while True:
            if self.needsRender:
                try:
                    self.needsRender = False;
                    self.drawUI()
                except:
                    pass

    def drawUI(self):

        self.scr.clear()

        self.renderCows()

        self.scr.border(0)
        self.scr.hline(self.maxY-3, 1, curses.ACS_HLINE, self.maxX-2)
        if self.textinput:
            self.scr.addstr(self.maxY-2, 1, "                                ")
            self.scr.move(self.maxY-2, 1)
        else:
            self.scr.addstr(self.maxY-2, 1, "Press ENTER to type your message")

        self.scr.refresh()


    #trim print
    def trint(self, y, x, str):
        if y<1 or y>self.maxY-4:
            return

        if x<1 :
            if x+len(str) < 1:
                return
            str = str[-1*x+1:]
            x = 1

        if x+len(str)>self.maxX-1 :
            if x>self.maxX-1:
                return
            str = str[0:self.maxX-x]
            
        self.scr.addstr(y, x, str)


    def getMsgForCow(self, cow_id):
        for msg in sc.msgs:
            if msg["cow_id"] == cow_id:
                return msg
        return None

    def renderBubble(self, x, y, dir, text):
        maxWidth = 40
        words = text.split(" ")
        lines = []
        tmp = ""
        for word in words:
            
            if len(word) < maxWidth:
                if len(tmp)+len(word)+1 <= maxWidth:
                    if len(tmp) == 0:
                        tmp = word
                    else:
                        tmp += " "  + word
                else:
                    lines.append(tmp)
                    tmp = ""

            elif len(word) == maxWidth:
                if len(tmp) != 0:
                    lines.append(tmp)
                    tmp = ""
                lines.append(word)

            elif len(word) > maxWidth:
                space = maxWidth-len(tmp)-1
                if len(tmp) == 0:
                    lines.append(word[:space])
                else:
                    lines.append(tmp + " " + word[:space])
                tmp = word[space:]

        if tmp != "":
            lines.append(tmp)

        #Size of bubble
        height = len(lines)
        width = 0
        for line in lines:
            if len(line) > width:
                width = len(line)

        if dir == 0:
            x = x-12
            y = y-height-2
        elif dir == 1:
            x = x+16+12-(width+4)
            y = y-height-2


        i=0
        while i<height:
            start = "< "
            end = " >"
            if height != 1:
                if i == 0:
                    start = "/ "
                    end = " \\"
                elif i == height-1:
                    start = "\\ "
                    end = " /"
                else:
                    start = "| "
                    end = " |"
            self.trint(y+i+1, x, start)
            self.trint(y+i+1, x+2, lines[i])
            self.trint(y+i+1, x+width+2, end)
            i+=1

        self.trint(y, x+1, "_"*(width+2))
        self.trint(y+height+1, x+1, "-"*(width+2))

        if dir == 0:
            self.trint(y+height+2, x+8, "\\")
            self.trint(y+height+3, x+9, "\\")
        elif dir == 1:
            self.trint(y+height+2, x+width+4-9, "/")
            self.trint(y+height+3, x+width+4-10, "/")



    def renderCows(self):

        for cow in sc.cows:
            x = cow["x"]+1
            y = cow["y"]+1
            dir = cow["dir"]
            msg = self.getMsgForCow(cow["id"])

            if dir == 0:
                self.trint(y+0, x, "^__^")
                self.trint(y+1, x, "(oo)\\_______")
                self.trint(y+2, x, "(__)\\       )\\/\\")
                self.trint(y+3, x+4,   "||----w |")
                self.trint(y+4, x+4,   "||     ||")
                self.trint(y+5, x, cow["name"].center(16))
            elif dir == 1:
                self.trint(y+0, x+12,           "^__^")
                self.trint(y+1, x+4,    "_______/(oo)")
                self.trint(y+2, x, "/\\/(       /(__)")
                self.trint(y+3, x+3,   "| w----||")
                self.trint(y+4, x+3,   "||     ||")
                self.trint(y+5, x, cow["name"].center(16))

            if msg != None:
                self.renderBubble(x, y, dir, msg["text"])

        


    def __del__(self):
        #set back to normal
        self.scr.keypad(0)
        curses.echo()
        curses.nocbreak()
        curses.endwin()





if __name__=='__main__':

    #TODO: read parameters: --host --port --name --farm

    sc = ServerConnection("localhost", 8765)

    while True:
        try:
            name = raw_input("Enter your name: ")
            sc.setName(name)
            break
        except ValueError:
            print "Name already taken."

    farms = sc.getFarmList()
    if len(farms) != 0:
        print "Farms:"
        for farm in farms:
            print " - "+farm
    farm = raw_input("Enter farm name: ")


    fd = FarmDisplay()
    fd.textinput = False
    sc.joinFarm(farm)

    while True:

        if fd.textinput:

            curses.echo()
            curses.nocbreak()

            text = fd.scr.getstr(fd.maxY-2, 1)
            fd.scr.move(fd.maxY-2,1)

            curses.noecho()
            curses.cbreak()
            fd.textinput = False

            if text != "":
                sc.sendMsg(text)

            fd.needsRender = True

        else:
            
            char = fd.scr.getch()
            
            if char == curses.KEY_UP:
                sc.move(0, -1)
            if char == curses.KEY_DOWN:
                sc.move(0, 1)
            if char == curses.KEY_LEFT:
                sc.move(-1, 0)
            if char == curses.KEY_RIGHT:
                sc.move(1, 0)
            #ESCAPE
            elif char == 27:
                break
            #ENTER
            elif char == 10:
                fd.textinput = True
                fd.needsRender = True

    del fd