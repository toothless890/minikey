# Concept:
This is a command line program that predicts words based off of home row input. The idea is to build a keyboard with a minimal key-count that doesn't require a steep learning curve in order to use effectively. Words are typed as normal as if touch typing, but instead of moving fingers up or down to the different rows, fingers will remain fixed in place. 

# Usage:
Compile and run the program by executing "make" and then "./minikey"

Enter words by only using the keys on the home row (Assumes QWERTY layout for now) 

Press space to cycle through candidate words

Typing a new key will insert a space and begin typing a new word. Pressing enter first will prevent a space from being pressed

Pressing enter again when no word is in progress will create a new line

Individual letters can be typed by typing a single letter, cycling through the candidates, and then pressing enter. This can be used to construct words not present in the dictionary. 

#Example:
<img width="96" height="42" alt="{0F421490-432B-404B-87B2-FC39A4E01752}" src="https://github.com/user-attachments/assets/bd0812c0-e93b-4ca6-9df0-8767a7bd38fc" />
<img width="89" height="41" alt="{CA907BA2-3EBF-469F-B08E-6316885CE807}" src="https://github.com/user-attachments/assets/e40f5851-917a-4786-ad7c-4a888a44a915" />
