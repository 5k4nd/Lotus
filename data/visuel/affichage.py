#!/usr/bin/python2
# -*- coding: utf8 -*-


import sfml as sf
from sfml.graphics import *
from time import sleep

# documentation utilisée: http://www.python-sfml.org/api/graphics.html , 
# http://python-sfml.org/tutorials.html#graphics , http://python-sfml.org/examples.html




##########initialisation##########

#taille de la fenetre
X = 600
Y = 600

a = 0 # émule la donnée de l'arduino ou du pc


# création de la fenetre d'affichage
window = RenderWindow(sf.window.VideoMode(X, Y), "pySFML Window")

try:# importe l'image
   sprite = Sprite(Texture.from_file("img.jpg"))
   sprite2 = Sprite(Texture.from_file("img2.jpg"))
except IOError:
    print("Erreur chargement image") # à modifier pour avoir une solution de secours et continuer le prog

# configure la vue 
view = View()
view.reset(Rectangle((50, 50), (550, 550)))
#view.center((200, 200)) #todo: centrer la vue sur le centre de rotation, il faudra surement definir une taille d'img et fenetre constantes




##########Fonctions: effets sur l'image en temps réel##########

def dessiner(): # pour afficher les modifications
    window.view = view
    window.clear()
    window.draw(sprite2)
    window.draw(sprite)
    window.display()
    
def secousse(amplitude): # simule une secousse (tremblement de l'img)
    view.move(amplitude,amplitude)
    dessiner()
    sleep(0.1)
    view.move(-amplitude,-amplitude)
    dessiner()
    sleep(0.1)
    view.move(amplitude,amplitude)
    dessiner()
    sleep(0.1)
    view.move(-amplitude,-amplitude)
    dessiner()
    sleep(0.1)

# émule les données de l'arduino ou du pc:
r = 255
v = 255
b = 255
f = 150 # opacité (nécessite également de baisser les couleurs): 0 = transparent, 255 = opaque (noir)


def couleur(rouge, vert, bleu, fondu): # pour modifier la couleur globale de l'image, et donc l'assortir à l'ambiance
   sprite.color = Color(rouge, vert, bleu, fondu) 
   # attention, va simplement supprimer les couleurs selon les niveaux demandés (ex: (0,255,255,255) supprimera le rouge)
   # ne transforme pas l'image en profondeur: on retrouve l'image d'origine en réutilisant la fonction avec (255, 255, 255, 255)
   

# changement d'image (quand changement d'ambiance)
def choixImage(name):
    sprite = Sprite(Texture.from_file(name + ".jpg")) # ne fonctionne pas
    # todo: reussir à vraiment modifier le sprite (modif la variable globale) + ajouter un try

def transition(r,v,b,f): # transition en fondu vers transparence
    while (r!=0 or v!=0 or b!=0 or f!=0):
        if (r!=0):
            r-=1;
        if (v!=0):
            v-=1;
        if (b!=0):
            b-=1;
        if (f!=0):
            f-=1;
        couleur(r,v,b,f)
        dessiner()
# à mieux implémenter selon ce qu'on veut: une transition qu'entre deux scènes, ou dans une même scène entre deux images/couleurs..
# possibilité de continuer à zoomer grâce à un booleen: ebauche d'algo:
'''
transi := vrai
if transi == vrai
    if r ou v ou b ou f != 0
        décrémenter 
    else
        transi := faux
'''





##########Boucle principale:########## 

while window.is_open:

   for event in window.events:
      if type(event) is sf.CloseEvent:
        window.close()
   
   a = a+1 # émule donnée arduino ou pc
      
   if (a % 100 == 0):
       secousse(2)
    
   if (a==100): # todo: modifier le max de taille de texture qui est actuellement 8192x8192
       #choixImage("img2") #permet de demander l'image selon un code numerique, ex: 001, 002, etc ; todo: ne marche pas (var glob)
       #couleur(r, v, b, f)
       transition(255,255,255,255)


   view.rotate(0.01)

   #view.zoom(1.0001) # zoom arrière lent
   view.zoom(0.9996) # zoom avant lent
   
   dessiner()
