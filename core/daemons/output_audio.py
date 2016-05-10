# -*- coding: utf8 -*-

from threading import Thread
from sys import exc_info
from time import sleep

# on utilisera probablement plus cette lib plus tard !
from pygame import mixer

"""
ToDo :
  * chemin relatif pour accéder au dossier data/

"""

class daemon_audio(Thread):
    """Thread de gestion de l'audio

    """

    def __init__(self, core_ref):
        Thread.__init__(self)
        self.core = core_ref
        self.volume = 0
        try:
            mixer.init()
            self.core.logger.p_log('(AUDIO) init')
        except:
            self.core.logger.p_log('(AUDIO) sound init error', error=exc_info())


    def run(self):
        mixer.music.load('data/naaru.mp3')
        mixer.music.play()
        while 1:
            sleep(0.1)
            self.volume = 1 - ( int(self.core.d_arduino.data['capteur1']) / 100.0 )
            mixer.music.set_volume(
                self.volume
            )
