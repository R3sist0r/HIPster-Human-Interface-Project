import speech
import serial

ser = serial.Serial('COM3', 9600, timeout=1)

def commandLED(phraseID):
    print phraseID
    result = {
            0: 'R',     #! Red max
            1: 'r',     #! Red up
            2: 'e',     #! Red down
            3: 'E',     #! Red off
            4: 'G',     #! Green max
            5: 'g',     #! Green up
            6: 'f',     #! Green down
            7: 'F',     #! Green off
            8: 'B',     #! Blue max
            9: 'b',     #! Blue up
            10: 'v',    #! Blue down
            11: 'V',    #! Blue off
            12: 'o',    #! All off
            13: 'w'     #! All on (white)
    }[phraseID]
    print result
    
phrases = ['Max Ruby', 'Increase Ruby', 'Decrease Ruby', 'Minimum Ruby', 'Max Green', 'Increase Green', 'Decrease Green', 'Minimum Green', 'Max Blue', 'Increase Blue', 'Decrease Blue', 'Min Blue', 'Off', 'L E D white']
while True:
    phrase = speech.input("Control the LED", phrases )
    print phrase
    commandLED(phrases.index(phrase)) 
#ser.close()


