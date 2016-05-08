# -*- coding: utf8 -*-

from daemons.input_arduino import daemon_arduino
from daemons.output_graphic import daemon_curses
from daemons.output_audio import daemon_audio
# from daemons.data_manager import daemon_data

from threading import Thread
from time import sleep
from sys import exc_info
import traceback
from logger_02 import LogFile
from pprint import pprint

from curses import (
    endwin,
    BUTTON_CTRL, BUTTON_SHIFT, KEY_LEFT, # pas nécessaire, suffit de noter les analogies en INT
    A_NORMAL,

)


class core(Thread):
    '''thread principal, gère tous les autres threads à savoir :
    > contient la boucle infinie de capture de touche entrée au clavier.

    '''

    def __init__(self, SCR, arduino_port, arduino_speed):
        Thread.__init__(self)
        self.arduino_port=arduino_port
        self.arduino_speed=arduino_speed
        self.scr = SCR
        self.last_entry = "aucune"
        self.erreurs = "pas d'erreurs pour le moment"
        self.logger = LogFile('app.log')
        self.logger.initself()


    def run(self):
        self.logger.p_log('program started', newline=3)

        try:
            self.d_arduino = daemon_arduino(
                core_ref=self,
                arduino_port=self.arduino_port,
                arduino_speed=self.arduino_speed,
            )
            self.d_arduino.start()

            # self.d_data_manager = daemon_data(
            #     core_ref=self,
            #     d_arduino_ref=self.d_arduino
            # )

            self.d_audio = daemon_audio(
                core_ref=self,
                # d_data_ref=self.d_data_manager
            )
            self.d_audio.start()

            self.d_curses = daemon_curses(
                self.scr,
                core_ref=self,
                # d_data_ref=self.d_data_manager
            )
            self.d_curses.start()

            sleep(.01)  # wait, pour être sûr que les thread soient bien lancés
            self.d_curses.scr_init()

            # main loop
            while 1:
                sleep(0.1)
                self.scr.nodelay(1)  # rend getch() non-bloquant
                current_entry = self.scr.getch()
                if 0 < current_entry < 356:  # si lettre connue, on la traite
                    # on commence par le stocker
                    # on remplit avec des blancs pour que ça fasse tjrs 5 " "
                    self.last_entry = current_entry

                    if current_entry == 27:  # <échap>  # ord('q')
                        break
                else:
                    if current_entry == KEY_LEFT:
                        self.last_entry = str(current_entry)
        except:
            self.logger.p_log('(CORE) ERROR in daemons init ', error=exc_info())


        self.d_audio._Thread__stop()
        # self.d_data_manager._Thread__stop()
        self.d_arduino._Thread__stop()
        self.d_curses._Thread__stop()

        endwin()  # restaure le terminal à son état d'origine
        self.logger.p_log('program ends.')
        print "traceback:"
        traceback.print_exc()  # affiche exception si il y a
