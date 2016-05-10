# -*- coding: utf8 -*-

from daemons.input_arduino import daemon_arduino
from daemons.output_audio import daemon_audio
from daemons.output_graphic import daemon_curses

from threading import Thread

from time import sleep
from sys import exc_info, stdin, stdout
from select import select

import traceback
from logger_02 import LogFile

from curses import endwin, A_NORMAL

# le processus principal, qui gère tous les autres
class core(Thread):
    '''thread principal, gère tous les autres threads à savoir :
    > contient la boucle infinie de capture de touche entrée au clavier.

    '''

    def __init__(self, SCR, arduino_port, arduino_speed, ncurses):
        Thread.__init__(self)
        self.arduino_port = arduino_port
        self.arduino_speed = arduino_speed
        self.logger = LogFile('app.log')
        self.logger.initself()
        self.ncurses=ncurses
        if self.ncurses:
            self.scr = SCR
            self.last_entry = "aucune"
            self.erreurs = "pas d'erreurs pour le moment"



    def run(self):

        self.logger.p_log('program started', newline=3)

        try:
            # on lance le processus qui gère l'arduino
            self.d_arduino = daemon_arduino(
                core_ref=self,
                arduino_port=self.arduino_port,
                arduino_speed=self.arduino_speed,
            )
            self.d_arduino.start()

            # on lance le processus qui gère l'audio
            self.d_audio = daemon_audio(
                core_ref=self,
            )
            self.d_audio.start()

            # on lance l'interface ncurses si l'utilisateur le veut
            if self.ncurses:
                self.d_curses = daemon_curses(
                    self.scr,
                    core_ref=self,
                )
                self.d_curses.start()

            sleep(.01)  # wait, pour être sûr que les thread soient bien lancés

            if self.ncurses:
                self.d_curses.scr_init()

            if self.ncurses:
                # boucle principale ncurses
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
                        if current_entry == 260:
                            self.last_entry = str(current_entry)
            else:
                # instruction principale si ncurses est désactivé
                print 'press ENTER to quit'
                raw_input()

                    
        except:
            self.logger.p_log('(CORE) ERROR in daemons init ', error=exc_info())


        # on tue tous les processus, le plus proprement possible...
        self.d_audio._Thread__stop()
        self.d_arduino._Thread__stop()

        if self.ncurses:
            self.d_curses._Thread__stop()
            endwin()  # restaure le terminal à son état d'origine
        
        # fin du programme
        self.logger.p_log('program ends.')
        
        # on affiche les erreurs s'il y a
        print "traceback:"
        traceback.print_exc()
