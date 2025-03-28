//
//  main.cpp
//  Projet 802 en C++ - La transformée de Hough
//
//  
//


#include <iostream>
#include "image.hpp"
#include "hough.hpp"
#define PI 3.141592653589793
#define SEUIL_BORD 250  /// Seuil pour considérer un pixel comme un bord



int main() {

    try {
        /// Lecture de l'image
        Image image = lireImagePPM("/Users/inesveigneau/Documents/Cours/Fac/M1/projet802/PROJ802/Projet/Projet/img2-a.ppm");

        
        
        /// Appel de la transformée de Hough - équation cartésienne
        std::cout << "\n--- Équation cartésienne ---\n" << std::endl;
        std::vector<Droite> droites = transformeeDeHough(image);
        std::cout << "\nDroites détectées :" << std::endl;
        for (const auto& droite : droites) {
            if (droite.verticale) {
                std::cout << "x = " << droite.b << " avec " << droite.votes << " votes (verticale)" << std::endl;
            } else {
                std::cout << "y = " << droite.m << " * x + " << droite.b << " avec " << droite.votes << " votes." << std::endl;
            }
        }
        afficherHoughASCII(image, droites);


        /// Sauvegarde de l'image en niveaux de gris
        sauvegarderImagePPM("resultat_NvGris.ppm", image);
        tracerDroites(image, droites, "imageNaïve.ppm");
            
            
        /// Appel de la transformée de Hough - coordonnées polaires
        std::cout << "\n\n--- Coordonnées polaires ---\n" << std::endl;
        std::cout << "\nDroites détectées :" << std::endl;
        std::vector<Droite> droitesHough = transformeeDeHoughPolaire(image);
        afficherHoughASCIIPolaire(image, droitesHough);
        tracerDroitesPolaire(image, droitesHough, "imagePolaire.ppm");

        
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }

    return 0;
}

