import serial as ser

menu = """                        Menu 
1. Blink RGB LED, color by color with delay of X[ms] 
2. Counting up onto LCD screen with delay of X[ms] 
3. Circular tone series via Buzzer with delay of X[ms] 
4. Get delay time X[ms]: 
5. Potentiometer 3-digit value [v] onto LCD  
6. Clear LCD screen 
7. Show menu 
8. Sleep """

def main():
    s = ser.Serial('COM3', baudrate=9600, bytesize=ser.EIGHTBITS,
                   parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                   timeout=1)   # timeout of 1 sec so that the read and write operations are blocking,
                                # after the timeout the program continues
    enableTX = True
    # clear buffers
    s.reset_input_buffer()
    s.reset_output_buffer()

    print(menu)
    
    while True:
        try:
            # Check for incoming data from the MSP430
            if s.in_waiting > 0:
                line = s.read_until(terminator=b'\n').decode('ascii').strip()
                print(f"Received: {line}")
                if line == '8':
                    print(menu)
                elif line == '7':
                    print(menu)
                elif line == '4':
                    delay = input("Enter delay time in ms: ")
                    s.write((delay + '\n').encode('ascii'))

            # Get user input
            inChar = input("Enter char: ")
            if inChar == '8':
                s.write(bytes(inChar, 'ascii'))
                break
            elif inChar == '7':
                print(menu)
            else:
                s.write(bytes(inChar, 'ascii'))
                
        except KeyboardInterrupt:
            break

if __name__ == '__main__':
    main()
