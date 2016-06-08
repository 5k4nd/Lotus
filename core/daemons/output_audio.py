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

    # intitalisation du daemon
    def __init__(self, core_ref):
        Thread.__init__(self)
        self.core = core_ref
        self.volume = 0
        self.ambiances = {
            0: "data/audio/naaru.mp3",
            1: "data/audio/scene_1_colere.mp3",
            2: "data/audio/scene_4_apaisement.mp3"
        }
        self.current = 0
        self.old = 0
        try:
            mixer.init()
            self.core.logger.p_log('(AUDIO) init')
        except:
            self.core.logger.p_log('(AUDIO) sound init error', error=exc_info())

    def run(self):
        try:
            # on initialise le mixer audio
            mixer.music.load(self.ambiances[self.current])
            mixer.music.play()
            while 1:
                sleep(0.1)
                # on modifie le volume
                self.volume = 1 - ( int(self.core.d_arduino.data['capteur1var']) / 200.0 )
                mixer.music.set_volume(
                    self.volume
                )

                # si l'ambiance courante n'est plus la scène de départ
                if self.current != 0:
                    # on lance la nouvelle scene pendant 30 secondes avant de revenir à la scène de départ
                    self.lancer_scene(self.current, 20)
                    self.current = 0


        except:
            self.core.logger.p_log('(AUDIO) play sound error', error=exc_info())

    def lancer_scene(self, ambiance, duree):
        DUREE_SCENE = duree  # en s

        self.core.logger.p_log('(AUDIO) changement d\'ambiance')
        backup_volume = self.volume
        
        # on baisse le volume
        for i in range(int(backup_volume*100), 0, -10):
            sleep(.1)
            self.volume = i/100.
            mixer.music.set_volume(self.volume)
        
        # on change de morceau en jouant l'ambiance demandée
        mixer.music.stop()
        mixer.music.load(self.ambiances[ambiance])
        mixer.music.play()
        for i in range(0, int(backup_volume*100), 10):
            sleep(.1)
            self.volume = i/100.
            mixer.music.set_volume(self.volume)

        sleep(DUREE_SCENE)

        # puis on rétablit l'ambiance de départ
        for i in range(int(backup_volume*100), 0, -10):
            sleep(.1)
            self.volume = i/100.
            mixer.music.set_volume(self.volume)

        mixer.music.stop()
        mixer.music.load(self.ambiances[0])
        mixer.music.play()
        for i in range(0, int(backup_volume*100), 10):
            sleep(.1)
            self.volume = i/100.
            mixer.music.set_volume(self.volume)
