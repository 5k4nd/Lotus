# -*- coding: utf8 -*-

import socket
from threading import Thread
from random import randint
from time import sleep
from sys import exc_info
import serial  # arduino
import ast  # for str to dict cast

class daemon_arduino(Thread):
    '''reçoit des infos d'une carte Arduino via le protocole ???
    une méthode get_data_dist (qui devra être publique ***ToDo!***) permet de récupérer la data formatée depuis l'extérieur


    '''

    def __init__(self, core_ref, arduino_port, arduino_speed):
        Thread.__init__(self)
        self.MAX = 120
        self.ard_port=arduino_port
        self.ard_speed=arduino_speed
        self.core = core_ref
        self.erreurs = "none"
        self.data = {'capteur1': self.MAX}
        try:
            self.arduino = serial.Serial(self.ard_port, self.ard_speed)
            self.core.logger.p_log('(ARDUINO) init')
        except:
            self.core.logger.p_log('(ARDUINO) serial_error')


    def run(self):
        while 1:
            sleep(.01)

            try:
                # listen from arduino
                got = self.arduino.readline()
                got = got.replace(" ", "")  # remove blanks
                got = ast.literal_eval(got)  # cast str to dict

                # looking for the data we want
                if "capteur1" in got.keys():
                    self.data['capteur1'] = int(got['capteur1'])
                    self.core.logger.p_log('(CAPTEUR1): ' + str(int(got['capteur1'])))

            except:
                self.core.logger.p_log('(ARDUINO) data_error')




