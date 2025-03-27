//
//  image.cpp
//  Projet
//
//  
//


#include "image.hpp"
#include <iostream>
#include <fstream>
#include <sstream>




int convertirEnGris(const Pixel& p) {
    return (p.r + p.g + p.b) / 3;
}



Image lireImagePPM(const std::string& fichierPPM) {
    std::ifstream fichier(fichierPPM);
    int largeur;
    int hauteur;
    int maxVal;
    
    /// Fonction interne pour nettoyer les commentaires dans le fichier PPM
    auto nettoyerFichier = [&fichier]() {
        char c;
        while (fichier >> c) {
            if (c == '#') {
                fichier.ignore(1024, '\n');  /// Ignorer la ligne entière
            }
            else {
                fichier.unget();  /// Remettre le caractère dans le flux
                break;
            }
        }
    };

    
    /// Vérifier que le fichier existe
    if (!fichier) throw std::runtime_error("Impossible d'ouvrir le fichier.");


    /// Lire et vérifier l'en-tête
    std::string format;
    fichier >> format;
    if (format != "P3") throw std::runtime_error("Format non supporté (Attendu : P3).");

    
    nettoyerFichier();
    fichier >> largeur >> hauteur;  /// on lit les dimensions
    nettoyerFichier();
    fichier >> maxVal;  /// on lit la valeur maximale des couleurs
    nettoyerFichier();
    
    
    /// Vérifier que les valeurs sont correctes
    if (largeur <= 0 || hauteur <= 0 || maxVal <= 0) throw std::string("Dimensions incorrectes dans l'en-tête du fichier.");
    

    /// Conversion en niveau de gris
    Image img = {largeur, hauteur, std::vector<std::vector<int>>(hauteur, std::vector<int>(largeur)) };
    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            Pixel p;
            fichier >> p.r >> p.g >> p.b;
            img.pixels[y][x] = convertirEnGris(p);
        }
    }
    
    return img;
}





void sauvegarderImagePPM(const std::string& fichierPPM, const Image& img) {
    std::ofstream fichier(fichierPPM);
    fichier << "P3\n" << img.largeur << " " << img.hauteur << "\n255\n";

    for (const auto& ligne : img.pixels) {
        for (int val : ligne) {
            fichier << val << " " << val << " " << val << " ";
        }
        fichier << "\n";
    }
}
