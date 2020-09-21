import tkinter as tk
import serial


# we will be given...
# s(x,y) coordinates of car every interval
# b(x,y) of left sensor when there is a drastic change

def main():
    # setting up the window
    window = tk.Tk()
    window.title('Environment Map')
    canvas = tk.Canvas(window, width=600, height=600, bg="white")
    canvas.pack()

    canvas.create_rectangle(5, 5, 600, 600, outline="black", width=5)  # drawing the border
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

    minX = 0
    maxX = 0
    minY = 0
    maxY = 0

    l = None
    obstLine = True

    # while loop for actual mapping portion while car is moving
    while (mapping):
        data = s.readline()  # convert bytes to ascii string
        data = str(data)
        print(data)
        if data == "STOP":  # if there is no incoming data...
            return s

        # converting string to integers for x and y values
        x = ""
        y = ""
        i = 3
        while data[i] != ',':
            x += data[i]
            i += 1
        x = int(x) * 4
        i += 1
        while data[i] != '!':
            y += data[i]
            i += 1
        y = int(y) * 4

        if data[2] == 'b':  # data point is meant for obstacle

            if obstLine:  # start point of new obstacle
                obst_x_start = x
                obst_y_start = y
                obstLine = False  # line has been started
            else:  # end point of new obstacle
                obst_x_end = x
                obst_y_end = y
                l = canvas.create_line(obst_x_start, obst_y_start, obst_x_end, obst_y_end, fill="black",
                                   width=5)  # draw the line w/ start and end points
                canvas.update()
                obstLine = True  # line has finished

                ## build min or max x and y values
                if obst_x_end > obst_x_start:
                    minX = obst_x_start
                    maxX = obst_x_end
                elif obst_x_end < obst_x_start:
                    minX = obst_x_end
                    maxX = obst_x_start
                if obst_y_end > obst_y_start:
                    minY = obst_y_start
                    maxY = obst_y_end
                elif obst_y_end < obst_y_start:
                    minY = obst_y_end
                    maxY = obst_y_start

            if( minX > 0 and maxX > 0 and minY > 0 and maxY > 0):
                canvas.create_rectangle(minX, minY, maxX, maxY, fill="grey")
                canvas.delete(l)
                minX=maxX=minY=maxY=0

        else:  # data point was meant for car position
            # update the car
            if (car != None):
                # keep track of old coordinates before update to create dashed line
                car_x_old = car_x
                car_y_old = car_y

                canvas.delete(car)
                canvas.create_rectangle(car_x_old - 3, car_y_old - 3, car_x_old + 3, car_y_old + 3, fill="forest green",
                                        outline="forest green")
                canvas.update()

            car_x = x
            car_y = y
            car = canvas.create_rectangle(car_x - 15, car_y - 15, car_x + 15, car_y + 15, fill="light blue",
                                          outline="light blue")
            canvas.update()

    window.mainloop()

s = main()
s.close()
