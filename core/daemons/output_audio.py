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
        self.ambiances = {
            'ambiance': "data/audio/naaru.mp3",
            'scene1': "data/audio/03 The Offering.mp3"
        }
        self.current = "ambiance"
        self.old = "ambiance"
        try:
            mixer.init()
            self.core.logger.p_log('(AUDIO) init')
        except:
            self.core.logger.p_log('(AUDIO) sound init error', error=exc_info())

    def run(self):
        try:
            mixer.music.load(self.ambiances[self.current])
            mixer.music.play()
            while 1:
                sleep(0.1)
                self.volume = 1 - ( int(self.core.d_arduino.data['capteur1']) / 200.0 )
                mixer.music.set_volume(
                    self.volume
                )
                if self.current != self.old:
                    self.run_ambiance(self.current)

                self.old = self.current

        except:
            self.core.logger.p_log('(AUDIO) sound error', error=exc_info())

    def run_ambiance(self, ambiance):
        self.core.logger.p_log('(AUDIO) changement d\'ambiance')
        backup_volume = self.volume
        for i in range(int(backup_volume*100), 0, -10):
            sleep(.1)
            self.volume = i/100.
            mixer.music.set_volume(self.volume)
        mixer.music.stop()
        mixer.music.load(self.ambiances[ambiance])
        mixer.music.play()
        for i in range(0, int(backup_volume*100), 1):
            sleep(.1)
            self.volume = i/100.
            mixer.music.set_volume(self.volume)

