//
//  image.hpp
//  Projet
//
//  
//


#ifndef image_hpp
#define image_hpp


#include <vector>
#include <string>



struct Pixel {
    int r, g, b;
};




struct Image {
    int largeur;
    int hauteur;
    std::vector<std::vector<int>> pixels;  /// Stockage en niveaux de gris
};



int convertirEnGris(const Pixel& p);
Image lireImagePPM(const std::string& fichierPPM);
void sauvegarderImagePPM(const std::string& fichierPPM, const Image& img);


#endif /* image_hpp */
