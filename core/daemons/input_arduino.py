# -*- coding: utf8 -*-

import socket
from threading import Thread
from random import randint
from time import sleep
from sys import exc_info
import serial  # arduino
import ast  # for str to dict cast


class daemon_arduino(Thread):
    '''Thread de réception des données en provenance de la carte Arduino
        via le protocole Serial de l'arduino (module pyserial côté python)

        Note :
          * côté Arduino, le mieux est d'envoyer depuis l'arduino Serial.print() sous la forme de dictionnaire python, c'est-à-dire
          un couple (nom de variable, valeur) entre crochets exemple : {'capeur1' : 42}
          * vous pouvez faire des tests sans arduino : modifier la première ligne de l'__init__ self.fake à True
    '''

    def __init__(self, core_ref, arduino_port, arduino_speed, arduino_fake):
        Thread.__init__(self)
        self.fake = arduino_fake
        self.MAX = 100  # valeur max du capteur pour l'initialisation : À SUPPRIMER après tests !
        self.core = core_ref
        self.erreurs = "none"
        self.data = {
            'capteur1var': self.MAX,
            'ambiancevar': 0,
        }
        self.current = -1  # fausse valeur de départ pour l'ambiance courante
        self.old = -1
        if not self.fake:
            self.ard_port=arduino_port
            self.ard_speed=arduino_speed
            try:
                self.arduino = serial.Serial(self.ard_port, self.ard_speed)
                self.core.logger.p_log('(ARDUINO) init')
            except:
                self.core.logger.p_log('(ARDUINO) serial_error')


    def run(self):
        while 1:
            sleep(.01)
            # self.core.logger.p_log('(CAPTEUR1): ' + str(self.data['capteur1']))

            # si python décide de changer d'ambiance, on avertit l'arduino
            if self.current != self.old:
                self.ambianceToArduino(self.current)
                self.old = self.current

            if not self.fake:
                try:
                    # listen from arduino
                    got = self.arduino.readline()
                    got = got.replace(" ", "")  # remove blanks
                    got = ast.literal_eval(got)  # cast str to dict

                    # looking for the data we want
                    if "capteur1var" in got.keys():
                        self.data['capteur1var'] = int(got['capteur1var'])
                        # self.core.logger.p_log('(CAPTEUR1): ' + str(got['capteur1var']))
                    if "capteur2var" in got.keys():
                        # insérer ici n'importe quoi :-)
                        a = 42
                    if "ambiancevar" in got.keys():
                        self.data['ambiancevar'] = int(got['ambiancevar'])

                except:
                    #quelle que soit l'erreur (formatage des donnees, valeurs aberrantes) on passe
                    # self.core.logger.p_log('(ARDUINO) data_error', error=exc_info())
                    pass



    def ambianceToArduino(self, ambiance):
        self.arduino.write('A'+str(ambiance))



    # def arduinoToPC(self):
    #     arduino = serial.Serial('/dev/ttyACM1',115200,timeout=1)
    #     if (arduino.inWaiting()>0):
    #         print "valeur"
    #         cle =  str(arduino.readline().strip())
    #         print cle
    #         if (cle.endswith("var")):
    #             valeur = int(arduino.readline().strip())
    #             self.dicoarduino[cle] = valeur
                  
    #             print cle +"   "+ str(self.dicoarduino[cle])
    #             self.core.logger.p_log('(ARDUINO) init')



# pour les tests uniquement
if __name__ == '__main__':
    # boucle infinie en correspondance avec l'arduino
    while 1:
        sleep(0.2)
        ambianceToArduino(1)
        arduinoToPC()
