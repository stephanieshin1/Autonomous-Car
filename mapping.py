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
    canvas.pack()

    canvas.create_rectangle(5, 5, 750, 750, outline="black", width=5) # drawing the border
    canvas.update()

    # setting up variables
    mapping = True
    s = serial.Serial('/dev/cu.HC-05-DevB', timeout=None)
    if s.isOpen():
        print("Bluetooth detected!")

    # variable initialization
    car = None
    car_x = None
    car_y = None

    obst_x_start = 0
    obst_y_start = 0
    obst_x_end = 0
    obst_y_end = 0

    obstLine = True
    turned = False
    verticalFirst = True

    side1 = 0
    side2 = 0

    rect_x_1 = 0
    rect_y_1 = 0
    rect_x_2 = 0
    rect_y_2 = 0

    # while loop for actual mapping portion while car is moving
    while (mapping):
        data = s.readline() # convert bytes to ascii string
        data = str(data)

        if data == "STOP": # if there is no incoming data...
            return s

        # converting string to integers for x and y values
        x = ""
        y = ""
        i = 2
        while data[i] != 'y':
            x += data[i]
            i += 1
        x = int(x)
        i += 1
        while data[i] != '!':
            y += data[i]
            i += 1
        y = int(y)

        if data[0] == 'b': # data point is meant for obstacle
            if obstLine: # start point of new obstacle
                obst_x_start = x
                obst_y_start = y
                obstLine = False # line has been started
            else: # end point of new obstacle
                obst_x_end = x
                obst_y_end = y
                canvas.create_line(obst_x_start, obst_y_start, obst_x_end, obst_y_end, fill="black", width=5) # draw the line w/ start and end points
                canvas.update()
                obstLine = True # line has finished
                if turned == False: # if we have not turned a corner, only 1 side of the obstacle is drawn
                    # figure out if the x or the y was changing and calculate the length of side
                    if abs(obst_x_start - obst_x_end) < abs(obst_y_start - obst_y_end): # figure out if the x or the y was changing
                        side1 = (obst_y_start - obst_y_end)
                        verticalFirst = True

                    else:
                        side1 = (obst_x_start - obst_x_end)
                        verticalFirst = False
                    turned = True # tell the obstacle we have turned
                else: # if we have turned, we have drawn 2 of the sides of the obstacle
                    if abs(obst_x_start - obst_x_end) < abs(obst_y_start - obst_y_end):  # figure out if the x or the y was changing and calculate the length of side
                        side2 = (obst_y_start - obst_y_end)
                    else:
                        side2 = (obst_x_start - obst_x_end)
                    # drawing the rest of the box since we have 2 of the sides of the box
                    if verticalFirst: # if we went up/down then to the side
                        canvas.create_line(obst_x_end, obst_y_end, obst_x_end, obst_y_end + side1, fill="black", width=5)
                        canvas.update()
                        canvas.create_line(obst_x_end, obst_y_end + side1, obst_x_end + side2, obst_y_end + side1, fill="black", width=5)
                        canvas.update()
                    else: # if we went to the side then up/down
                        canvas.create_line(obst_x_end, obst_y_end, obst_x_end + side1, obst_y_end, fill="black", width=5)
                        canvas.update()
                        canvas.create_line(obst_x_end + side1, obst_y_end, obst_x_end + side1, obst_y_end + side2, fill="black", width=5)
                        canvas.update()

                    turned = False # reset variable for next obstacle
        else: # data point was meant for car position
            # update the car
            if (car != None):
                # keep track of old coordinates before update to create dashed line
                car_x_old = car_x
                car_y_old = car_y

                canvas.delete(car)
                canvas.create_rectangle(car_x_old - 3, car_y_old - 3, car_x_old + 3, car_y_old + 3, fill="forest green", outline="forest green")
                canvas.update()

            car_x = x
            car_y = y
            car = canvas.create_rectangle(car_x - 15, car_y - 15, car_x + 15, car_y + 15, fill="light blue", outline="light blue")
            canvas.update()

    window.mainloop()

s = main()
s.close()