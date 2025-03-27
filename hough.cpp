//
//  hough.cpp
//  Projet
//
//  
//



#include <iostream>
#include "image.hpp"
#include "hough.hpp"
#include <cmath>
#include <map>
#include <vector>
#define PI 3.141592653589793
#define SEUIL_BORD 250  /// Seuil pour considérer un pixel comme un bord
#define SEUIL_VOTES_CARTESIEN 5 /// Seuil pour filtrer les droites dans Hough Naive
#define SEUIL_VOTES_POLAIRE 7 /// Seuil pour filtrer les droites dans Hough moins naive






/// Supprimer deux droites proches
std::vector<Droite> supprimerDroitesProches(std::vector<Droite> droites, int tol1, int tol2) {
    std::vector<Droite> resultat;
    std::vector<bool> utilise(droites.size(), false);

    for (size_t i = 0; i < droites.size(); i++) {
        if (utilise[i]) continue;
        int m1 = droites[i].m;
        int b1 = droites[i].b;

        /// Vérifie si une droite proche existe déjà
        bool fusionnee = false;
        for (size_t j = 0; j < resultat.size(); j++) {
            int m2 = resultat[j].m;
            int b2 = resultat[j].b;
            if (std::abs(m1 - m2) < tol1 && std::abs(b1 - b2) < tol2) {
                fusionnee = true;
                break;
            }
        }
        if (!fusionnee) {
            Droite d;
            d.m = m1;
            d.b = b1;
            resultat.push_back(d);
        }
    }
    
    return resultat;
}










/// -------------- Cartésien --------------

std::vector<Droite> transformeeDeHough(const Image& img) {
    std::map<int, std::map<int, int>> accumulateur;
    std::vector<Droite> droites;

    /// Détection des droites verticales
    for (int x = 0; x < img.largeur; ++x) {
        int votes = 0;
        for (int y = 0; y < img.hauteur; ++y) {
            if (img.pixels[y][x] > SEUIL_BORD) votes++;
        }
    }
    /// Détection des droites
    for (int y = 0; y < img.hauteur; ++y) {
        for (int x = 0; x < img.largeur; ++x) {
            if (img.pixels[y][x] > SEUIL_BORD) {
                for (double m = -5.0; m <= 5.0; m += 0.1) {
                    double b = y - m * x;
                    accumulateur[m * 10][b]++; /// entier comme clé, préférable pour map
                }
            }
        }
    }
    /// Filtrage des droites les plus pertinentes
    for (const auto& m : accumulateur) { /// clé principale m (pente multipliée par 10).
        for (const auto& b : m.second) { /// std::map<int, int> associée, qui contient toutes les valeurs de b.
            if (b.second > SEUIL_VOTES_CARTESIEN) { /// b.first : ordonnée à l'origine et b.second nombre de votes
                Droite droite = {m.first / 10.0, static_cast<double>(b.first), b.second, false};
                droites.push_back(droite);
            }
        }
    }

    return droites;
}

void afficherHoughASCII(const Image& img, const std::vector<Droite>& droites){
    std::cout << "\nReprésentation ASCII des droites détectées :\n";
    
    for (int y = 0; y < img.hauteur; ++y) {
        for (int x = 0; x < img.largeur; ++x) {
            bool isPartOfLine = false;
            for (const auto& droite : droites) {
                if (droite.verticale) {
                    if (x == static_cast<int>(droite.b)) {  /// Vérifier si le pixel est sur une droite verticale
                        isPartOfLine = true;
                        break;
                    }
                } else {
                    int y_calc = static_cast<int>(droite.m * x + droite.b);
                    if (std::abs(y - y_calc) <= 1) { /// Tolérance pour les droites inclinées
                        isPartOfLine = true;
                        break;
                    }
                }
            }
            
            if (isPartOfLine) {
                std::cout << "*";  /// Représente la droite
            } else if (img.pixels[y][x] < SEUIL_BORD) {
                std::cout << ".";  /// Bords détectés
            } else {
                std::cout << " ";  /// Fond
            }
        }
        std::cout << "\n";
    }
}










/// -------------- Polaire --------------

std::vector<Droite> transformeeDeHoughPolaire(const Image& img) {
    int rhoMax = std::sqrt(img.largeur * img.largeur + img.hauteur * img.hauteur);
    int thetaRes = 180;
    std::vector<std::vector<int>> accumulateur(2 * rhoMax, std::vector<int>(thetaRes, 0));
    std::vector<Droite> lignesDetectees;

    for (int y = 0; y < img.hauteur; ++y) {
        for (int x = 0; x < img.largeur; ++x) {
            if (img.pixels[y][x] > SEUIL_BORD) {
                for (int theta = 0; theta < thetaRes; theta++) {
                    double rad = theta * PI / 180.0;
                    int rho = (int)(x * std::cos(rad) + y * std::sin(rad)) + rhoMax;
                    accumulateur[rho][theta]++;
                }
            }
        }
    }

    for (int rho = 0; rho < 2 * rhoMax; rho++) {
        for (int theta = 0; theta < thetaRes; theta++) {
            if (accumulateur[rho][theta] > SEUIL_VOTES_POLAIRE) {
                Droite d;
                d.m = rho - rhoMax;
                d.b = theta;
                d.votes = accumulateur[rho][theta];
                lignesDetectees.push_back(d);
            }
        }
    }
    
    for (size_t i = 0; i < lignesDetectees.size(); i++){
        std::cout << "rho = " << lignesDetectees[i].m << " et theta = " << lignesDetectees[i].b << " avec " << lignesDetectees[i].votes << " votes." << std::endl;
    }

    // supprimerDroitesProches(lignesDetectees, 20, 1);
    return lignesDetectees;
}

void afficherHoughASCIIPolaire(const Image& img, const std::vector<Droite>& droites) {
    std::cout << "\nReprésentation ASCII des droites détectées :\n";

    for (int y = 0; y < img.hauteur; ++y) {
        for (int x = 0; x < img.largeur; ++x) {
            bool isPartOfLine = false;

            for (const auto& droite : droites) {
                double thetaRad = droite.b * PI / 180.0;  /// Conversion en radians
                int rho = droite.m;
                int rhoCalc = static_cast<int>(x * std::cos(thetaRad) + y * std::sin(thetaRad));

                if (std::abs(rho - rhoCalc) <= 1) {  /// Tolérance pour correspondre à la droite
                    isPartOfLine = true;
                    break;
                }
            }

            if (isPartOfLine) {
                std::cout << "#";
            } else if (img.pixels[y][x] < 128) {
                std::cout << ".";
            } else {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
}









/// -------------- Affichage pour toutes les méthodes dans une image sauvegardée --------------

void tracerDroites(const Image& img, const std::vector<Droite>& droites, const std::string& fichierSortie) {
    Image image_droites = img;

    /// Parcours des pixels et dessin des droites détectées
    for (const auto& droite : droites) {
        for (int x = 0; x < img.largeur; ++x) {
            int y = static_cast<int>(droite.m * x + droite.b);
            if (y >= 0 && y < img.hauteur) {
                image_droites.pixels[y][x] = 128;
            }
        }
    }

    sauvegarderImagePPM(fichierSortie, image_droites);
}











/// -------------- Représentation de Bresenham --------------

/// Code de région en binaire pour la méthode line-clipping de Cohen-Sutherland
const int interieur = 0; //0000
const int gauche = 1; //0001
const int droite = 2; //0010
const int bas = 4; //0100
const int haut = 8; //1000

int obtenirCode(int x, int y, Image image){
    int code = interieur;
    
    if (x < 0) code |= gauche;
    
    else if (x > image.largeur) code |= droite;
    
    if (y < 0) code |= bas;
    
    else if (y > image.hauteur) code |= haut;

    return code;
}



/// Fonction line clipping d'après l'algorithme de Cohen-Sutherland
std::tuple<int, int, int, int> CohenSutherland(int x0, int y0, int x1, int y1, const Image& image) {
    int code0 = obtenirCode(x0, y0, image);
    int code1 = obtenirCode(x1, y1, image);
    int x;
    int y;
    bool accept = false;

    
    if (code0 == 0 && code1 == 0) {
        return {x0, y0, x1, y1}; /// Droite dans l'image

    } else if (code0 & code1) {
        return { -1, -1, -1, -1 }; /// Droite totalement à l'extérieur de l'image
        
    } else {
        /// Clipping
        int codeExt = (code0 != 0) ? code0 : code1; //Choisi le point en dehors de l'image
        
        if (codeExt & haut) {
            x = x0 + (x1 - x0) * (image.hauteur - y0) / (y1 - y0);
            y = image.hauteur;
    
        } else if (codeExt & bas) {
            x = x0 + (x1 - x0) * (0 - y0) / (y1 - y0);
            y = 0;

        } else if (codeExt & droite) {
            y = y0 + (y1 - y0) * (image.largeur - x0) / (x1 - x0);
            x = image.largeur;

        } else {
            y = y0 + (y1 - y0) * (0 - x0) / (x1 - x0);
            x = 0;
        }
        if (codeExt == code0) {
            x0 = x;
            y0 = y;
            code0 = obtenirCode(x0, y0, image);

        } else {
            x1 = x;
            y1 = y;
            code1 = obtenirCode(x1, y1, image);
        }
    }

    return {x0, y0, x1, y1};
}



void dessinerPixel(int x0, int y0, Image image){
    image.pixels[x0][y0] = 255;
}




/// Tracer de Bresenham
void tracerBresenham(int x0, int y0, int x1, int y1, Image& image) {
    std::tie(x0, y0, x1, y1) = CohenSutherland(x0, y0, x1, y1, image);

    if (x0 == -1 && y0 == -1) return;  /// droite totalement hors image

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = 1;
    int sy = 1;
    int err;

    if (x0 > x1){
        sx = -sx;
    }

    if (y0 > y1){
        sy = -sy;
    }

    if (dx > dy){
        err = dx / 2;
        while (x0 != x1){
            dessinerPixel(x0,y0, image);
            err = err - dy;
            if (err < 0){
                y0 = y0 + sy;
                err = err + dx;
            }
            x0 = x0 + sx;
        }
    }
    else {
        err = dy / 2;
        while (y0 != y1){
            dessinerPixel(x0,y0, image);
            err = err - dx;
            if (err < 0){
                x0 = x0 + sx;
                err = err + dy;
            }
            y0 = y0 + sy;
        }
    }
    
}




