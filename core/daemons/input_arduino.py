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

    def __init__(self, core_ref, arduino_port, arduino_speed):
        Thread.__init__(self)
        self.fake = True
        self.MAX = 100  # valeur max du capteur pour l'initialisation : À SUPPRIMER après tests !
        self.core = core_ref
        self.erreurs = "none"
        self.data = { 'capteur1': self.MAX }
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
            self.core.logger.p_log('(CAPTEUR1): ' + str(self.data['capteur1']))
            if not self.fake:
                try:
                    # listen from arduino
                    got = self.arduino.readline()
                    got = got.replace(" ", "")  # remove blanks
                    got = ast.literal_eval(got)  # cast str to dict

                    # looking for the data we want
                    if "capteur1" in got.keys():
                        self.data['capteur1'] = int(got['capteur1'])
                        self.core.logger.p_log('(CAPTEUR1): ' + str(got['capteur1']))
                    if "capteur2" in got.keys():
                        # insérer ici n'importe quoi :-)
                        a = 42

                except:
                    self.core.logger.p_log('(ARDUINO) data_error')




