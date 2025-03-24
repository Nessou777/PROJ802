//
//  main.cpp
//  Projet 802 en C++ - La transformée de Hough
//
//  Created by Inès Veigneau on 19/03/2025.
//



#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>



struct Pixel{
    // Valeurs entre 0 et 255
    int r;
    int g;
    int b;
};

void nettoyerFichier(std::ifstream& fichier){
    char c;
    while (fichier >> c){
        if (c == '#') {
            fichier.ignore(1024, '\n');  // on ignore la ligne entière
        }else{
            fichier.unget();  // sinon on remet le caractère c dans le flux
            break;
        }
    }
}

void convertirNvGris();

/// Méthode naïve



/// Méthode moins naïve



/// Pour diminuer la complexité ?



/// Dessiner une droite : marche de Bresenham



/// Supprimer deux droites proches





int main(int argc, const char * argv[]) {
    
    std::ifstream fichier("/Users/inesveigneau/Documents/Cours/Fac/M1/projet802/PROJ802/Projet/Projet/image2.ppm");
        
    if (!fichier){
        throw std::string("Impossible d'ouvrir le fichier.");
        return 1;
    }

    std::string format;
    int largeur = 0;
    int hauteur = 0;
    int maxVal = 0;

    /// Lire et vérifier l'en-tête
    fichier >> format;
    if (format != "P3") {
        throw std::string("Format non supporté (Attendu : P3).");
        return 1;
    }

    nettoyerFichier(fichier);
    fichier >> largeur >> hauteur;  // on lit les dimensions
    
    nettoyerFichier(fichier);
    fichier >> maxVal;  // on lit la valeur maximale des couleurs
    
    nettoyerFichier(fichier);
    
    /// Vérifier que les valeurs sont correctes
    if (largeur <= 0 || hauteur <= 0 || maxVal <= 0) {
        throw std::string("Dimensions incorrectes dans l'en-tête du fichier.");
        return 1;
    }

    /// Afficher l'en-tête pour vérifier
    std::cout << "Format : " << format << std::endl;
    std::cout << "Dimensions : " << largeur << " x " << hauteur << std::endl;
    std::cout << "Valeur max : " << maxVal << std::endl;

    /// Stocker les pixels
    std::vector<Pixel> image(largeur * hauteur);

    /// Lire les pixels (R, G, B sous forme de nombres)
    for (int i = 0; i < largeur * hauteur; ++i) {
        fichier >> image[i].r;
        fichier >> image[i].g;
        fichier >> image[i].b;
    }

    /// Vérification : Afficher le premier pixel
    std::cout << "Premier pixel - R: " << image[0].r
              << " G: " << image[0].g
              << " B: " << image[0].b << std::endl;

    return 0;
    
    
}
