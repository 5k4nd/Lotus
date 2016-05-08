# -*- coding: utf8 -*-

from threading import Thread
from time import sleep


class daemon_data(Thread):
    '''thread gérant toutes les données, leur réception et leur acheminement
        vers les threads qui en ont besoin, de préférence à l'aide de méthode get.
    
    pour le moment, on possède données en provenance de :
        - arduino

    NOTES :
        - la lecture des touches entrées sur le clavier du laptop se fait dans
            le core (apparemment obligatoire... ?)

    '''

    def __init__(self, core_ref, d_arduino_ref):
        Thread.__init__(self)
        self.core = core_ref
        self.d_arduino = d_arduino_ref
        self.core.logger.p_log('(DATA) init')

    def run(self):
        while 1:
            sleep(.01)
            try:
            # veille sur les switchs
                # switch1 sur {f2, f3, f4}
                if self.core.last_entry == 266:
                    self.scr.addstr(5, 47, 'son a--'+9*" ")
                if self.core.last_entry == 267:
                    self.scr.addstr(5, 47, 'son a pause/play')
                if self.core.last_entry == 268:
                    self.scr.addstr(5, 47, 'son a++'+9*" ")
            except:
                self.core.erreurs = "erreur dans input PC"


    def get_data_arduino(self, datum):
        return self.d_arduino.formated_data[datum]

    def get_data_switch(self, switch_number):
        return self.data_switch[switch_number]
