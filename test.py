import tkinter as tk
import serial

# we will be given...
# (x,y) coordinates of car every interval
# (x,y) of left sensor when there is a drastic change

# ** ??? **
# CONVERTING ARDUINO DATA INTO PYTHON

def main():
    # setting up the window
    window = tk.Tk()
    window.title('Environment Map')
    canvas = tk.Canvas(window, width=750, height=750, bg="white")
    canvas.pack()

    canvas.create_rectangle(5, 5, 750, 750, outline="black", width=5) # drawing the border

    # setting up variables
    mapping = True
    # s = serial.Serial('/dev/cu.HC-05-DevB')
    # if s.isOpen() == False:
        # print("uh oh!")
    # else:
        # print("yay!")


    # variable initialization
    car = None
    car_x = 650
    car_y = 100

    obst_x_start = 0
    obst_y_start = 0
    obst_x_end = 0
    obst_y_end = 0

    obstLine = True
    turned = False
    verticalFirst = True

    side1 = 0
    side2 = 0
    IncomingData = True

    # while loop for actual mapping portion while car is moving
    while (mapping):
        if (IncomingData):
            if obstLine: # start point of new obstacle
                obst_x_start = int(input("obst_x_start: ")) # TEMP CODE
                obst_y_start = int(input("obst_y_start: ")) # TEMP CODE
                obstLine = False # line has been started
            else: # end point of new obstacle
                obst_x_end = int(input("obst_x_end: ")) # TEMP CODE
                obst_y_end = int(input("obst_y_end: ")) # TEMP CODE
                canvas.create_line(obst_x_start, obst_y_start, obst_x_end, obst_y_end, fill="black", width=5) # draw the line w/ start and end points
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
                        canvas.create_line(obst_x_end, obst_y_end, obst_x_end, obst_y_end + side1, fill="black", width=5)
                        canvas.create_line(obst_x_end, obst_y_end + side1, obst_x_end + side2, obst_y_end + side1, fill="black", width=5)
                    else: # if we went to the side then up/down
                        canvas.create_line(obst_x_end, obst_y_end, obst_x_end + side1, obst_y_end, fill="black", width=5)
                        canvas.create_line(obst_x_end + side1, obst_y_end, obst_x_end + side1, obst_y_end + side2, fill="black", width=5)

                    turned = False # reset variable for next obstacle

            # keep track of old coordinates before update to create dashed line
            car_x_old = car_x
            car_y_old = car_y

            # update the car
            car_x = 600
            car_y = 100
            canvas.delete(car)
            canvas.create_rectangle(car_x_old - 5, car_y_old - 5, car_x_old + 5, car_y_old + 5, fill="forest green", outline="forest green")
            car = canvas.create_rectangle(car_x - 15, car_y - 15, car_x + 15, car_y + 15, fill="light blue", outline="light blue")

            if obstLine == True and turned == False:
                user = input("keep going? ")
                if user == "no":
                    mapping = False

        go = False
        if go: # if there is no incoming data...
            pass
            # CODE TO END WHILE LOOP

    window.mainloop()

main()
