//
//  hough.hpp
//  Projet
//
//  
//

#ifndef hough_hpp
#define hough_hpp

#include "image.hpp"
#include <vector>

struct Droite {
    double m;  // Pente
    double b;  // Ordonnée à l'origine
    int votes;
    bool verticale;  // Indique si la droite est verticale
};


std::vector<Droite> transformeeDeHough(const Image& img);
void tracerDroitesEtSauvegarder(const Image& img, const std::vector<Droite>& droites, const std::string& fichierSortie);
void supprimerDroitesProches(std::vector<Droite>& droites, int toleranceRho, int toleranceTheta);
std::vector<Droite> transformeeDeHoughPolaire(const Image& img);
int obtenirCode(int x, int y, Image image);
std::tuple<int, int, int, int> CohenSutherland(int x0, int y0, int x1, int y1, const Image& image);
void dessinerPixel(int x0, int y0, Image image);
void tracerBresenham(int x0, int y0, int x1, int y1, Image& image);


#endif /* hough_hpp */
