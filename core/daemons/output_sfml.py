#!/usr/bin/python2
# -*- coding: utf8 -*-

from threading import Thread
from sys import exc_info
from time import sleep

import sfml as sf
from sfml.graphics import *
from sfml.window import *


# documentation utilisée: http://www.python-sfml.org/api/graphics.html , 
# http://python-sfml.org/tutorials.html#graphics , http://python-sfml.org/examples.html

##########classe image############
class Image():
    
    # attributs
    rouge = 255
    vert = 255
    bleu = 255
    fondu = 255
    
    # méthodes
    def __init__(self, img): # int rvb, str img
        try:
            self.img = Sprite(Texture.from_file(img))
        # except IOError: self.img = Sprite(Texture.from_file("../data/visuel/img.jpg")) # image de secours 
        except:
            print "impossible de charger l'image"

    def dessiner(self, window, view, img1, img2): # pour afficher les modifications
    # attention cette fonction est dupliquée !
        window.view = view
        window.clear()
        window.draw(img1.img) # image du fond
        window.draw(img2.img) # image par dessus
        window.display()
            
    def secousse(self, amplitude): # simule une secousse (tremblement de l'img)
        view.move(amplitude,amplitude)
        dessiner()
        sleep(SLEEP)
        view.move(-amplitude,-amplitude)
        dessiner()
        sleep(SLEEP)
        view.move(amplitude,amplitude)
        dessiner()
        sleep(SLEEP)
        view.move(-amplitude,-amplitude)
        dessiner()
        sleep(SLEEP)
    
    def setCouleur(self, rouge, vert, bleu):
        self.rouge = rouge
        self.vert = vert
        self.bleu = bleu
    
    def couleur(self): # pour modifier la couleur globale de l'image, et donc l'assortir à l'ambiance
        self.img.color = Color(self.rouge, self.vert, self.bleu, self.fondu) 
        
    def disparitionFondu(self, window, view, ROTATION, ZOOM, SLEEP, img1, img2): # transition en fondu vers transparence
        while (self.rouge!=0 or self.vert!=0 or self.bleu!=0 or self.fondu!=0):
            sleep(SLEEP)
            if (self.rouge!=0):
                self.rouge-=1
            if (self.vert!=0):
                self.vert-=1
            if (self.bleu!=0):
                self.bleu-=1
            if (self.fondu!=0):
                self.fondu-=1
            view.rotate(ROTATION)
            view.zoom(ZOOM) # zoom avant lent
            self.couleur()
            self.dessiner(window, view, img1, img2)
            
    def apparitionFondu(self, window, view, ROTATION, ZOOM, SLEEP, img1, img2): # transition en fondu vers l'image
        while (self.rouge!=255 or self.vert!=255 or self.bleu!=255 or self.fondu!=150):
            sleep(SLEEP)
            if (self.rouge!=255):
                self.rouge+=1
            if (self.vert!=255):
                self.vert+=1
            if (self.bleu!=255):
                self.bleu+=1
            if (self.fondu!=150):
                self.fondu+=1
            view.rotate(ROTATION)
            view.zoom(ZOOM) # zoom avant lent
            self.couleur()
            self.dessiner(window, view, img1, img2)



class daemon_visuels(Thread):
    """Thread de gestion des visuels, notamment :

    * les images
      - les fractales
      - les lucioles
    * les vidéos
      - ???

    """

    def __init__(self, core_ref):
		Thread.__init__(self)
		self.core = core_ref
		self.must_end = False
		self.image1 = "data/visuel/img.jpg"
		self.image2 = "data/visuel/img3.jpg"

    def run(self):
        self.core.logger.p_log('(SFML) run main loop')

        def dessiner(window, view, img1, img2): # pour afficher les modifications
            window.view = view
            window.clear()
            window.draw(img1.img) # image du fond
            window.draw(img2.img) # image par dessus
            window.display()

        def alterner(temps, window, view, ROTATION, ZOOM, SLEEP, img1, img2):
            if (temps % 100 == 0):
                img2.apparitionFondu(window, view, ROTATION, ZOOM, SLEEP, img1, img2)
            elif (temps % 50 == 0):
                img2.disparitionFondu(window, view, ROTATION, ZOOM, SLEEP, img1, img2)
        '''
        exemple:
        img1 est bleue
        img2 est rouge
        img2 disparait et apparait: on voit l'image passer du rouge au bleu etc
        '''


        try:
            ##########Initialisation##########

            ROTATION = 0.01
            ZOOM = .9996
            SLEEP = .01

            #taille de la fenetre
            width = 600
            height = 600

            # émule la donnée de l'arduino ou du pc
            pic = 0 
            ambiance = 1

            # var de temps pour par exemple bataille rouge/bleu
            temps = 0 


            # création de la fenetre d'affichage
            window = RenderWindow(VideoMode(width, height), "pySFML Window")
            # configure la vue
            view = View()
            view.reset(Rectangle((50, 50), (550, 550)))
            #view.center((200, 200)) #todo: centrer la vue sur le centre de rotation, il faudra surement definir une taille d'img et fenetre constantes
            self.core.logger.p_log('(SFML) init')

        except:
            self.core.logger.p_log('(SFML) init error', error=exc_info())


        ##########Boucle principale:########## 

        while not(self.must_end):
            try:
                ##########Ambiances############

                #ambiance = PCtoPC(); # on récupère à chaque boucle la donnée PC - TODO: PCtoPC()
                if (ambiance == -1):
                    # s'il n'y a pas de nouvelle donnée d'ambiance (-1) on ne fait rien
                    pass
                elif (ambiance == 0):
                    # ambiance de base
                    img1 = Image(self.image1)
                    img2 = Image(self.image1)
                    alternance = False
                
                elif (ambiance == 1):
                    # ambiance bataille rouge/bleu
                    img1 = Image(self.image2)
                    img2 = Image(self.image2)
                    img1.setCouleur(0, 100, 255);
                    img2.setCouleur(255, 100, 0);
                    alternance = True # une fonction appelée à chaque boucle
                    ambiance = -1 # emule donnee revenant à -1
                    
                elif (ambiance == 2):
                    # ambiance mer
                    img1 = Image(self.image1)
                    img2 = Image(self.image1)
                    alternance = False
                # si aucune des ambiances programmées ne correspond à la donnée reçue, on ne modifie rien
                


                # émule donnée arduino ou pc
                pic += 1 
                temps += 1



                # if (pic % 200 == 0):
                #    img1.secousse(2)
                #    img2.secousse(2)
                
                
                if (alternance):
                    alterner(temps, window, view, ROTATION, ZOOM, SLEEP, img1, img2)
                
                view.rotate(ROTATION)

                #view.zoom(1.0001) # zoom arrière lent
                view.zoom(ZOOM) # zoom avant lent
                sleep(SLEEP)


                dessiner(window, view, img1, img2)
            except:
                self.core.logger.p_log('(SFML) error', error=exc_info())

        window.close()
