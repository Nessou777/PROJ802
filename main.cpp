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
#define SEUIL_BORD 128  // Seuil pour considérer un pixel comme un bord

int main() {
    std::string nomFichierSortie = "image_resultat.ppm";

    try {
        // Lecture de l'image
        Image image = lireImagePPM("/Users/inesveigneau/Documents/Cours/Fac/M1/projet802/PROJ802/Projet/Projet/imageM1.ppm");

        // Appel de la transformée de Hough qui retourne un vecteur de droites
        std::vector<Droite> droites = transformeeDeHough(image);
        std::vector<Droite> droitesPolaire = transformeeDeHoughPolaire(image);
        
        // Affichage des droites détectées
        std::cout << "Droites détectées :" << std::endl;
        for (const auto& droite : droites) {
            if (droite.verticale) {
                std::cout << "x = " << droite.b << " avec " << droite.votes << " votes (verticale)" << std::endl;
            } else {
                std::cout << "y = " << droite.m << " * x + " << droite.b << " avec " << droite.votes << " votes." << std::endl;
            }
        }

        // Affichage ASCII des droites détectées
        std::cout << "\nReprésentation ASCII des droites détectées :\n";
        for (int y = 0; y < image.hauteur; ++y) {
            for (int x = 0; x < image.largeur; ++x) {
                bool isPartOfLine = false;
                for (const auto& droite : droites) {
                    if (droite.verticale) {
                        if (x == static_cast<int>(droite.b)) {  // Vérifier si le pixel est sur une droite verticale
                            isPartOfLine = true;
                            break;
                        }
                    } else {
                        int y_calc = static_cast<int>(droite.m * x + droite.b);
                        if (std::abs(y - y_calc) <= 1) { // Tolérance pour les droites inclinées
                            isPartOfLine = true;
                            break;
                        }
                    }
                }

                if (isPartOfLine) {
                    std::cout << "*";  // Représente la droite
                } else if (image.pixels[y][x] > SEUIL_BORD) {
                    std::cout << ".";  // Bords détectés
                } else {
                    std::cout << " ";  // Fond
                }
            }
            std::cout << "\n";
        }

    // Sauvegarde de l'image
    sauvegarderImagePPM("resultat_NvGris.ppm", image);
    tracerDroitesEtSauvegarder(image, droites, "image_avec_droites.ppm");
        
    std::vector<Droite> lignesDetectees = transformeeDeHoughPolaire(image);
    std::cout << lignesDetectees.size() << std::endl;


    for (auto& ligne : lignesDetectees) {
        int rho = ligne.m;
        int theta = ligne.b;
        std::cout << rho << ", theta=" << theta << std::endl;
        // Convertir (rho, theta) en points (x1, y1) et (x2, y2)
        int x1 = 0, y1 = (int)(rho / std::sin(theta * PI / 180.0));
        int x2 = image.largeur, y2 = (int)((rho - x2 * std::cos(theta * PI / 180.0)) / std::sin(theta * PI / 180.0));

        tracerBresenham(x1, y1, x2, y2, image);
    }

    std::cout << "Image après tracé de Bressenham :\n";
    for (int y =0; y < image.hauteur; ++y){
        for (int x = 0; x < image.largeur; ++x) {
            std::cout << (image.pixels[y][x] > 128 ? '.' : '#');
        }

        std::cout << std::endl;

    }

        
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }

    return 0;
}

