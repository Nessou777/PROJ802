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

struct Droite {
    double m, b;
    int votes;
};

// Fonction de transformée de Hough, maintenant retourne un vecteur de Droites
std::vector<Droite> transformeeDeHough(const Image& img);


#endif /* hough_hpp */
