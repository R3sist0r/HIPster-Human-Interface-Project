import speech

def commandLED(phraseID): 
	print phraseID

phrases = ['Max Red', 'Increase Ruby', 'Decrease Ruby', 'Increase Green', 'Decrease Green', 'Increase Blue', 'Decrease Blue', 'Off', 'L E D white', 'turn off']
while True:
	phrase = speech.input("Control the LED", phrases )
	print phrase	
	commandLED(phrases.index(phrase))
	if phrase == "turn off":
		break
		

	