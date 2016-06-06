#!/usr/bin/python2
# -*-coding:utf-8 -*

import serial #library to interact with serial port.. very similar to arduino version
from time import sleep #for delays

dicoarduino = {}
arduino = serial.Serial('/dev/ttyACM1',115200,timeout=1)


def ambianceToArduino(ambiance):
	arduino.write('A'+str(ambiance))

def arduinoToPC():
	if (arduino.inWaiting()>0):
		print "valeur"
		cle =  str(arduino.readline().strip())
		print cle
		if (cle.endswith("var")):
		      valeur = int(arduino.readline().strip())
		      dicoarduino[cle] = valeur
		      
		      print cle +"   "+ str(dicoarduino[cle])


# boucle infinie en correspondance avec l'arduino
while 1:
	ambianceToArduino(1)
	arduinoToPC()
	sleep(0.1)


'''
TODO
Initialiser la communication
lisser les valeurs (comparer ecart 1ere et 3eme avec ecart 1ere et 2eme)
prendre du plaisir !!!!!!!
À voir vérif envoi ambiance (envoye une seule fois)

'''
