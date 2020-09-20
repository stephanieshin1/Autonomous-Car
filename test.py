import tkinter as tk
import serial

# we will be given...
# (x,y) coordinates of car every interval
# (x,y) of left sensor when there is a drastic change

def main():
    # setting up the window
    window = tk.Tk()
    window.title('Environment Map')
    canvas = tk.Canvas(window, width=750, height=750, bg="white")
    # 4x4 pixels = 1 cm^3
    canvas.pack()

    # setting up variables
    mapping = True

    # set up serial port
    # COM = ""
    # BAUD = ""
    # SerialPort = serial.Serial(COM, BAUD, timeout=1)

    # variable initialization
    car_x_start = 750
    car_y_start = 50
    car_x_end = 100
    car_y_end = 50

    obst_x_start = 0
    obst_y_start = 0
    obst_x_end = 0
    obst_y_end = 0

    obstLine = True
    carLine = True
    turned = False
    verticalFirst = True

    side1 = 0
    side2 = 0
    IncomingData = True

    # while loop for actual mapping portion while car is moving
    while (mapping):
        # IncomingData = SerialPort.readline()

        if (IncomingData):
            # print((IncomingData).decode('utf-8')) ??
            # !!!! convert it to integers x and y

            if obstLine: # start point of new obstacle
                obst_x_start = int(input("obst_x_start: ")) # TEMP CODE
                obst_y_start = int(input("obst_y_start: ")) # TEMP CODE
                obstLine = False # line has been started
            else: # end point of new obstacle
                obst_x_end = int(input("obst_x_end: ")) # TEMP CODE
                obst_y_end = int(input("obst_y_end: ")) # TEMP CODE
                canvas.create_line(obst_x_start, obst_y_start, obst_x_end, obst_y_end) # draw the line w/ start and end points
                obstLine = True # line has finished
                if turned == False: # if we have not turned a corner, only 1 side of the obstacle is drawn
                    # figure out if the x or the y was changing and calculate the length of side
                    if obst_x_start == obst_x_end: # figure out if the x or the y was changing
                        side1 = (obst_y_start - obst_y_end)
                        verticalFirst = True
                    else:
                        side1 = (obst_x_start - obst_x_end)
                        verticalFirst = False
                    turned = True # tell the obstacle we have turned
                else: # if we have turned, we have drawn 2 of the sides of the obstacle
                    if obst_x_start == obst_x_end:  # figure out if the x or the y was changing and calculate the length of side
                        side2 = (obst_y_start - obst_y_end)
                    else:
                        side2 = (obst_x_start - obst_x_end)
                    # drawing the rest of the box since we have 2 of the sides of the box
                    if verticalFirst: # if we went up/down then to the side
                        canvas.create_line(obst_x_end, obst_y_end, obst_x_end, obst_y_end + side1)
                        canvas.create_line(obst_x_end, obst_y_end + side1, obst_x_end + side2, obst_y_end + side1)
                    else: # if we went to the side then up/down
                        canvas.create_line(obst_x_end, obst_y_end, obst_x_end + side1, obst_y_end)
                        canvas.create_line(obst_x_end + side1, obst_y_end, obst_x_end + side1, obst_y_end + side2)

                    turned = False # reset variable for next obstacle

            if carLine:
                # update car start
                carLine = False
            else:
                # update car end
                # canvas.create_line(car_x_start, car_y_start, car_x_end, car_y_end)
                carLine = True

        else: # if there is no incoming data...
            pass
            # elif the car is finished
            # CODE TO END WHILE LOOP

        # TEST CODE DELETE LATER
        user = input("Still collecting data? ")
        if user != "yes":
            return window
        # END OF TEST CODE BLOCK

window = main()
window.mainloop()

