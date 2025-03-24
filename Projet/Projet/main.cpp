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
    std::string nomFichierSortie = "image_resultat.ppm";

    try {
        // Lecture de l'image
        Image image = lireImagePPM("/Users/inesveigneau/Documents/Cours/Fac/M1/projet802/PROJ802/Projet/Projet/imageM1.ppm");

        // Appel de la transformée de Hough qui retourne un vecteur de droites
        std::vector<Droite> droites = transformeeDeHough(image);

        // Affichage ASCII rapide avec toutes les droites détectées
        std::cout << "\nReprésentation ASCII de l'image avec les droites détectées :\n";
        for (int y = 0; y < image.hauteur; ++y) {
            for (int x = 0; x < image.largeur; ++x) {
                bool isDroite = false;

                // Vérifier si le pixel appartient à l'une des droites détectées
                for (const auto& droite : droites) {
                    int y_calc = static_cast<int>(droite.m * x + droite.b);
                    if (y_calc == y) {
                        isDroite = true;
                        break;
                    }
                }

                if (isDroite) {
                    std::cout << "#";  // Représente une droite
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

