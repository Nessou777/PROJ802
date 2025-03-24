# PROJ802
Projet de C++


### Introduction

La transformée de Hough est une technique mathématique permettant de détecter des lignes droites dans une image.
Ce projet vise à implémenter cette méthode en C++ afin la transformée de Hough en C++ afin d'identifier et de tracer des lignes détectées sur une image.




### Objectif du projet

L'objectif est de développer un programme en C++ permettant :

    - La détection des lignes dans une image en utilisant la transformée de Hough.
    - Le filtrage et la sélection des lignes les plus représentatives avec une méthode d’optimisation.
    - L'optimisation de la détection en appliquant la transformée de Hough uniquement aux contours de l’image.




### Architecture du programme

Le programme sera organisé en trois modules principaux :

    1. Détection des droites
        - Conversion de l'image en niveaux de gris
        - Extraction des contours
        - Calcul de la transformée de Hough
        - Génération d'une réplique de l’image avec le tracé des lignes détectées

    2. Filtrage et sélection des droites
        - Suppression des doublons et fausses détections à l’aide d’une méthode d’optimisation
        - Sélection des lignes les plus pertinentes en fonction de critères spécifiques

    3. Optimisation par application aux contours uniquement
        - Application de la transformée de Hough uniquement sur les contours détectés
        - Comparaison des performances et de la précision par rapport à la méthode initiale






## Méthode naïve

L’idée est de parcourir chaque pixel de l’image et pour chaque pixel de tracer toutes les droites possibles passant par ce pixel dans l’espace de Hough. 
Ces droites sont représentées sous la forme cartésienne : b = y - mx 
                                                         avec m la pente de la droite,
                                                           et b l’ordonnée à l’origine.
                                                           
On utilise un accumulateur (une matrice 2D) pour stocker le nombre de fois où une droite donnée apparaît.
Ensuite, on identifie les valeurs maximales de cet accumulateur pour récupérer les lignes les plus probables.
     
     
                                                           
### Étapes :

  On parcourt tous les pixels (x,y) de l'image. 
  Pour chaque pixel détecté comme un contour (seuil) :on fait varier m dans une plage donnée.
  On calcule b avec b = y − mx.
  On incrémente un accumulateur (m,b).
  À la fin, les pics dans l’accumulateur correspondent aux droites les plus probables.



### Inconvénients

    - complexité : chaque pixel doit être parcouru individuellement (très coûteux pour une image de grande taille).
    - mémoire : l'accumulateur si très grand doit stocker les votes pour toutes les combinaisons.
