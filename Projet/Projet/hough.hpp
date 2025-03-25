//
//  hough.hpp
//  Projet
//
//  Created by Inès Veigneau on 24/03/2025.
//

#ifndef hough_hpp
#define hough_hpp

#include "image.hpp"
#include <vector>

// Structure modifiée pour stocker les droites verticales
struct Droite {
    double m;  // Pente (utile pour les droites non verticales)
    double b;  // Ordonnée à l'origine (utile pour les droites non verticales)
    int votes;
    bool verticale;  // Flag pour indiquer si la droite est verticale
};

// Fonction de transformée de Hough, maintenant retourne un vecteur de Droites
std::vector<Droite> transformeeDeHough(const Image& img);


#endif /* hough_hpp */
