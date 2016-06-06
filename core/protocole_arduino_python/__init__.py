#!/usr/bin/python2
# -*-coding:utf-8 -*

"""
ATTENTION
CETTE CLASSE N'EST UTILISÉE QUE POUR LES TESTS !
"""


import serial #library to interact with serial port.. very similar to arduino version
from time import sleep #for delays

import socket
from sys import exc_info
import ast

dicoarduino = {}
arduino = serial.Serial('/dev/ttyACM2',115200,timeout=1)


def ambianceToArduino(ambiance):
	arduino.write('A'+str(ambiance))

def arduinoToPC():
	if (arduino.inWaiting()>0):
		cle =  str(arduino.readline().strip())
		print cle
		if (cle.endswith("var")):
		      valeur = int(arduino.readline().strip())
		      dicoarduino[cle] = valeur
		      
		      print cle +"   "+ str(dicoarduino[cle])


# boucle infinie en correspondance avec l'arduino
while 1:
	# data = { 'capteur1var': 200 }
	sleep(0.1)
	try:
		# listen from arduino
		got = arduino.readline()
		got = got.replace(" ", "")  # remove blanks
		got = ast.literal_eval(got)  # cast str to dict
		print got


		if "capteur1var" in got.keys():
			data['capteur1var'] = int(got['capteur1var'])
			print data['capteur1var']
		if "ambiancevar" in got.keys():
			data['ambiancevar'] = int(got['ambiancevar'])
			print data['ambiancevar']
	except:
    	#quelle que soit l'erreur (formatage des donnees, valeurs aberrantes) on passe
		a = 42

	# got = arduino.readline()
	# got = got.replace(" ", "")  # remove blanks
	# got = ast.literal_eval(got)  # cast str to dict
	# print got
	# ambianceToArduino(0)
	# arduinoToPC()


'''
TODO
Initialiser la communication
lisser les valeurs (comparer ecart 1ere et 3eme avec ecart 1ere et 2eme)
prendre du plaisir !!!!!!!
À voir vérif envoi ambiance (envoye une seule fois)

'''
