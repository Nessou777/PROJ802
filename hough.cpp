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
#define SEUIL_BORD 128  // Seuil pour considérer un pixel comme un bord



std::vector<Droite> transformeeDeHough(const Image& img) {
    std::map<double, std::map<double, int>> accumulateur;
    std::vector<Droite> droitesInteressantes;

    // Détection des droites verticales
    for (int x = 0; x < img.largeur; ++x) {
        int votes = 0;
        for (int y = 0; y < img.hauteur; ++y) {
            if (img.pixels[y][x] < SEUIL_BORD) { // Pixel détecté comme un bord
                votes++;
            }
        }
        if (votes > 5) {  // Seuil pour considérer une droite verticale
            Droite verticale = {0, static_cast<double>(x), votes, true};
            droitesInteressantes.push_back(verticale);
        }
    }

    // Détection des droites inclinées
    for (int y = 0; y < img.hauteur; ++y) {
        for (int x = 0; x < img.largeur; ++x) {
            if (img.pixels[y][x] < SEUIL_BORD) { // Pixel détecté comme un bord
                for (double m = -5.0; m <= 5.0; m += 0.1) { // Balayage des pentes
                    double b = y - m * x;  // Calcul de l'ordonnée à l'origine pour la droite
                    accumulateur[m][b]++;   // Incrémenter le vote pour cette droite (m, b)
                }
            }
        }
    }

    // Filtrage des droites par le nombre de votes
    int seuilVotes = 2; // Seuil pour les votes des droites
    for (const auto& pairM : accumulateur) {
        for (const auto& pairB : pairM.second) {
            if (pairB.second > seuilVotes) { // Vérifie si le nombre de votes dépasse le seuil
                Droite droite = {pairM.first, pairB.first, pairB.second};
                droitesInteressantes.push_back(droite); // Ajouter la droite à la liste
            }
        }
    }

    return droitesInteressantes; // Retourner la liste des droites détectées
}


void tracerDroitesEtSauvegarder(const Image& img, const std::vector<Droite>& droites, const std::string& fichierSortie) {
    Image imageAvecDroites = img;  // Copie de l'image originale

    // Parcours des pixels et dessin des droites détectées
    for (const auto& droite : droites) {
        for (int x = 0; x < img.largeur; ++x) {
            int y = static_cast<int>(droite.m * x + droite.b);  // y = m*x + b
            if (y >= 0 && y < img.hauteur) {
                // On met en rouge (255,0,0)
                imageAvecDroites.pixels[y][x] = 1;
            }
        }
    }

    // Sauvegarde de l'image modifiée
    sauvegarderImagePPM(fichierSortie, imageAvecDroites);
}

// Supprimer deux droites proches
void supprimerDroitesProches(std::vector<Droite>& droites, int toleranceRho, int toleranceTheta) {
    std::vector<Droite> resultat;
    std::vector<bool> utilise(droites.size(), false);

    for (size_t i = 0; i < droites.size(); i++) {
        if (utilise[i]) continue;
        int rho1 = droites[i].m;
        int theta1 = droites[i].b;

        // Vérifier si une droite proche existe déjà
        bool fusionnee = false;
        for (size_t j = 0; j < resultat.size(); j++) {
            int rho2 = resultat[j].m;
            int theta2 = resultat[j].b;

            if (std::abs(rho1 - rho2) < toleranceRho && std::abs(theta1 - theta2) < toleranceTheta) {
                fusionnee = true;
                break;
            }
        }
        if (!fusionnee) {
            Droite d;
            d.m = rho1;
            d.b = theta1;
            resultat.push_back(d);
        }
    }
    droites = resultat;
}


// Transformée de Hough avec rho et theta
std::vector<Droite> transformeeDeHoughPolaire(const Image& img) {
    int rhoMax = std::sqrt(img.largeur * img.largeur + img.hauteur * img.hauteur);
    int thetaRes = 180;
    std::vector<std::vector<int>> accumulateur(2 * rhoMax, std::vector<int>(thetaRes, 0));
    std::vector<Droite> lignesDetectees;

    for (int y = 0; y < img.hauteur; ++y) {
        for (int x = 0; x < img.largeur; ++x) {
            if (img.pixels[y][x] < 128) { // Seuil simple
                for (int theta = 0; theta < thetaRes; theta++) {
                    double rad = theta * PI / 180.0;
                    int rho = (int)(x * std::cos(rad) + y * std::sin(rad)) + rhoMax;
                    accumulateur[rho][theta]++;
                }
            }
        }
    }

    
    int seuilVotes = 8;
    for (int rho = 0; rho < 2 * rhoMax; rho++) {
        for (int theta = 0; theta < thetaRes; theta++) {
            if (accumulateur[rho][theta] > seuilVotes) {
                Droite d;
                d.m = rho - rhoMax;
                d.b = theta;
                lignesDetectees.push_back(d);
            }
        }
    }

    supprimerDroitesProches(lignesDetectees, 10, 5);
    
    std::cout << "taille = " << lignesDetectees.size() << std::endl;
    for (size_t i = 0; i < lignesDetectees.size(); i++){
        std::cout << "Ligne " << i << ": rho = " << lignesDetectees[i].m << ", theta = " << lignesDetectees[i].b << std::endl;
    }

    return lignesDetectees;
}





// Code de région en binaire pour la méthode line-clipping de Cohen-Sutherland
const int interieur = 0; //0000
const int gauche = 1; //0001
const int droite = 2; //0010
const int bas = 4; //0100
const int haut = 8; //1000

int obtenirCode(int x, int y, Image image){
    int code = interieur;
    if(x < 0){
        code |= gauche;
    }else if (x > image.largeur){
        code |= droite;
    }
    if(y < 0){
        code |= bas;
    }else if (y > image.hauteur){
        code |= haut;
    }

    return code;
}

// Fonction line clipping d'après l'algorithme de Cohen-Sutherland
std::tuple<int, int, int, int> CohenSutherland(int x0, int y0, int x1, int y1, const Image& image) {
    int code0 = obtenirCode(x0, y0, image);
    int code1 = obtenirCode(x1, y1, image);
    bool accept = false;

    while (true) {
        if (code0 == 0 && code1 == 0) {
            accept = true;
            break;  // Ligne totalement dans l'image
        } else if (code0 & code1) {
            // Ligne totalement à l'extérieur d'une même région (ex: hors écran)
            return { -1, -1, -1, -1 };
        } else {
            // Clipping
            int codeExt = (code0 != 0) ? code0 : code1;
            int x, y;

            if (codeExt & haut) {
                x = x0 + (x1 - x0) * (image.hauteur - y0) / (y1 - y0);
                y = image.hauteur;
            } else if (codeExt & bas) {
                x = x0 + (x1 - x0) * (0 - y0) / (y1 - y0);
                y = 0;
            } else if (codeExt & droite) {
                y = y0 + (y1 - y0) * (image.largeur - x0) / (x1 - x0);
                x = image.largeur;
            } else if (codeExt & gauche) {
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
    }

    if (accept) {
        return {x0, y0, x1, y1};
    } else {
        return {-1, -1, -1, -1}; // Ligne rejetée
    }
}


void dessinerPixel(int x0, int y0, Image image){
    image.pixels[x0][y0] = 255;
}

// Tracer de Bresenham
void tracerBresenham(int x0, int y0, int x1, int y1, Image& image) {
    std::tie(x0, y0, x1, y1) = CohenSutherland(x0, y0, x1, y1, image);

    if (x0 == -1 && y0 == -1) return;  // Ligne totalement hors image

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        if (x0 >= 0 && x0 < image.largeur && y0 >= 0 && y0 < image.hauteur) {
            image.pixels[y0][x0] = 255;  // Dessiner pixel en blanc
        }
        if (x0 == x1 && y0 == y1) break;
        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}
