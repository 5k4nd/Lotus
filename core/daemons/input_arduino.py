# -*- coding: utf8 -*-

import socket
from threading import Thread
from random import randint
from time import sleep
from sys import exc_info


class daemon_arduino(Thread):
    '''reçoit des infos d'une carte Arduino via le protocole ???
    une méthode get_data_dist (qui devra être publique ***ToDo!***) permet de récupérer la data formatée depuis l'extérieur


    '''

    def __init__(self, core_ref):
        Thread.__init__(self)
        self.MIN = int(0)
        self.MAX = int(200)
        self.core = core_ref
        self.erreurs = "none"
        self.raw_data = self.MAX
        self.formated_data = self.MAX
        self.core.logger.p_log('(ARDUINO) init')

    def format_data(self, mode):
        str_length = len(str(self.raw_data))
        res = " "*(3-str_length) + str(self.raw_data) # for always having a 3 char long str
        return res


    def run(self):
        self.raw_data = self.MIN
        while 1:
            sleep(.05)
            # listen from arduino
            if self.raw_data == 200:
                self.raw_data = 0
            else:
                self.raw_data += 1
            self.formated_data = self.format_data(self.raw_data)
            
            

    def get_data_dist(self):
        """distance should be between MIN and MAX
        """
        return self.formated_data


