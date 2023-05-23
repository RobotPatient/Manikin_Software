# import serial

# serial_port = "COM18"
# baud_rate   = 115200

# serial = serial.Serial(serial_port, baud_rate)

# def start_stream():
#     streamCommand = b"STREAM\r"
#     serial.write(streamCommand) # Should be in Bytes! 
    
#     # Open a file for writing
#     file_name = './_temp/serial_data.dat'
#     file = open(file_name, 'w')

#     try:
#         while True:
#             # Read a line from the serial terminal
#             line = serial.readline().decode('utf-8').strip()
            
#             # Save the line to the file
#             file.write(line + '\n')
#             file.flush()  # Ensure that the data is written immediately
            
#             # Print the line to the console
#             print(line)

#             # Check if there is any input from the user to stop the script
#             user_input = input().strip()
#             if user_input == "Z":
#                 break
#             else:
#                 continue
            
#     except KeyboardInterrupt:
#         pass

#     # Close the file and serial connection
#     file.close()
#     serial.close()

import serial
import threading
import sys

serial_port = "COM20"
baud_rate = 115200
file_name = './_Temp/serial_data.dat'
serial_connection = serial.Serial(serial_port, baud_rate)

# Flag to indicate if the script should stop
stop_flag = False

def start_stream():
    streamCommand = b"STREAM\r"
    serial_connection.write(streamCommand) # Should be in Bytes! 

def read_serial_stream():
    start_stream()
    file = open(file_name, 'w')

    try:
        while not stop_flag:
            # Read a line from the serial terminal if data is available
            if serial_connection.in_waiting:
                line = serial_connection.readline().decode('utf-8').strip()

                # Save the line to the file
                file.write(line + '\n')
                file.flush()  # Ensure that the data is written immediately

                # Print the line to the console
                print(line)

    except serial.SerialException:
        pass

    # Close the file and serial connection
    file.close()
    serial_connection.close()

def get_user_input():
    global stop_flag

    input()
    stop_flag = True

def startSerialThreads():
    # Create and start the threads
    serial_thread = threading.Thread(target=read_serial_stream)
    input_thread = threading.Thread(target=get_user_input)

    serial_thread.start()
    input_thread.start()

    # Wait for the threads to finish
    serial_thread.join()
    input_thread.join()
