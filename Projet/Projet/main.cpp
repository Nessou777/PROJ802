//
//  main.cpp
//  Projet 802 en C++ - La transformée de Hough
//
//  Created by Inès Veigneau on 19/03/2025.
//



#include <iostream>
#include "image.hpp"
#include "hough.hpp"
#define SEUIL_BORD 128  // Seuil pour considérer un pixel comme un bord


int main() {
    std::string nomFichier = "/Users/inesveigneau/Documents/Cours/Fac/M1/projet802/PROJ802/Projet/Projet/image2.ppm";
    std::string nomFichierSortie = "image_resultat.ppm";

    try {
        Image image = lireImagePPM(nomFichier);
        Droite droite = transformeeDeHough(image);

        // Affichage ASCII rapide
        std::cout << "\nReprésentation ASCII de l'image avec la droite détectée :\n";
        for (int y = 0; y < image.hauteur; ++y) {
            for (int x = 0; x < image.largeur; ++x) {
                int y_calc = static_cast<int>(droite.m * x + droite.b);
                if (y_calc == y) {
                    std::cout << "#";  // Représente la droite
                } else if (image.pixels[y][x] < SEUIL_BORD) {
                    std::cout << ".";  // Bords détectés
                } else {
                    std::cout << " ";  // Fond
                }
            }
            std::cout << "\n";
        }

        // Sauvegarde de l'image (facultatif)
        sauvegarderImagePPM(nomFichierSortie, image);

    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }

    return 0;
}
