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
#include <map>
#include <cmath>



#define PI 3.141592653589793


struct Pixel{
    // Valeurs entre 0 et 255
    int r;
    int g;
    int b;
};

struct Image {
    int largeur, hauteur;
    std::vector<std::vector<int>> pixels;
};

int convertirNvGris(Pixel pix) {
    return (pix.r + pix.g + pix.b) / 3;
}


// Fonction pour lire une image PPM
Image lireImagePPM(const std::string& nomFichier) {
    std::ifstream fichier(nomFichier);

    // Fonction interne pour nettoyer les commentaires dans le fichier PPM
    auto nettoyerFichier = [&fichier]() {
        char c;
        while (fichier >> c) {
            if (c == '#') {
                fichier.ignore(1024, '\n');  // Ignorer la ligne entière
            }
            else {
                fichier.unget();  // Remettre le caractère dans le flux
                break;
            }
        }
        };

    if (!fichier) {
        throw std::runtime_error("Impossible d'ouvrir le fichier.");
    }

    /// Lire et vérifier l'en-tête
    std::string format;
    fichier >> format;
    if (format != "P3") {
        throw std::runtime_error("Format non supporté (Attendu : P3).");

    }

    int largeur, hauteur, maxVal;

    nettoyerFichier();
    fichier >> largeur >> hauteur;  // on lit les dimensions

    nettoyerFichier();
    fichier >> maxVal;  // on lit la valeur maximale des couleurs

    nettoyerFichier();
    
    /// Vérifier que les valeurs sont correctes
    if (largeur <= 0 || hauteur <= 0 || maxVal <= 0) {
        throw std::string("Dimensions incorrectes dans l'en-tête du fichier.");
    }

    Image img = { largeur, hauteur, std::vector<std::vector<int>>(hauteur, std::vector<int>(largeur)) };

    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            Pixel p;
            fichier >> p.r >> p.g >> p.b;
            img.pixels[y][x] = convertirNvGris(p);  // Conversion en niveau de gris
        }
    }
    return img;
}




// Détection de droites avec la transformée de Hough naïve
void transformeeDeHoughNaive(const Image& img) {

    std::map<double, std::map<double, int>> accumulateur;

    // Parcourir chaque pixel de l'image
    for (int y = 0; y < img.hauteur; ++y) {
        for (int x = 0; x < img.largeur; ++x) {
            if (img.pixels[y][x] < 128) { // Seuil simple : pixels clairs considérés comme des bords
                for (double m = -5.0; m <= 5.0; m += 0.1) { // Balayage des pentes possibles
                    double b = y - m * x;
                    accumulateur[m][b]++; // Incrémentation de l'accumulateur
                }
            }
        }
    }

    // Trouver le maximum dans l'accumulateur
    int maxVotes = 0;
    double meilleurM = 0, meilleurB = 0;
    for (const auto& pairM : accumulateur) {
        for (const auto& pairB : pairM.second) {
            if (pairB.second > maxVotes) {
                maxVotes = pairB.second;
                meilleurM = pairM.first;
                meilleurB = pairB.first;
            }
        }
    }

    // Affichage des résultats
    std::cout << "Droite détectée : y = " << meilleurM << " * x + " << meilleurB
        << " avec " << maxVotes << " votes." << std::endl;

    // Affichage ASCII de l'image avec la droite détectée
    std::cout << "\nReprésentation ASCII de l'image et de la droite détectée :\n";
    for (int y = 0; y < img.hauteur; ++y) {
        for (int x = 0; x < img.largeur; ++x) {
            int y_calc = static_cast<int>(meilleurM * x + meilleurB);
            if (y_calc == y) {
                std::cout << "#"; // Représente la droite
            }
            else if (img.pixels[y][x] > 128) {
                std::cout << "."; // Point de bord
            }
            else {
                std::cout << " "; // Fond
            }
        }
        std::cout << std::endl;
    }

}

/// Supprimer deux droites proches
int supprimerDroitesProches(std::vector<std::pair<int, int>>& droites, int toleranceRho, int toleranceTheta) {
    std::vector<std::pair<int, int>> resultat;
    std::vector<bool> utilise(droites.size(), false);

    for (size_t i = 0; i < droites.size(); i++) {
        if (utilise[i]) continue;
        int rho1 = droites[i].first;
        int theta1 = droites[i].second;

        // Vérifier si une droite proche existe déjà
        bool fusionnee = false;
        for (size_t j = 0; j < resultat.size(); j++) {
            int rho2 = resultat[j].first;
            int theta2 = resultat[j].second;

            if (std::abs(rho1 - rho2) < toleranceRho && std::abs(theta1 - theta2) < toleranceTheta) {
                fusionnee = true;
                break;
            }
        }
        if (!fusionnee) {
            resultat.push_back({rho1, theta1});
        }
    }
    droites = resultat;
}

// Transformée de Hough avec rho et theta
std::vector<std::pair<int,int>> transformeeDeHough(const Image& img) {
    int rhoMax = std::sqrt(img.largeur * img.largeur + img.hauteur * img.hauteur);
    int thetaRes = 180;
    std::vector<std::vector<int>> accumulateur(2 * rhoMax, std::vector<int>(thetaRes, 0));
    std::vector<std::pair<int,int>> lignesDetectees;

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

    int maxVotes = 0;
    int meilleurRho = 0, meilleurTheta = 0;
    for (int rho = 0; rho < 2 * rhoMax; rho++) {
        for (int theta = 0; theta < thetaRes; theta++) {
            if (accumulateur[rho][theta] > maxVotes) {
                maxVotes = accumulateur[rho][theta];
                meilleurRho = rho - rhoMax;
                meilleurTheta = theta;
            }
        }
    }
    
    int seuilVotes = 10;
    for (int rho = 0; rho < 2 * rhoMax; rho++) {
        for (int theta = 0; theta < thetaRes; theta++) {
            if (accumulateur[rho][theta] > maxVotes-1) {
                lignesDetectees.push_back({rho-rhoMax, theta});
            }
        }
    }

    std::cout << "Ligne détectée : rho = " << meilleurRho << ", theta = " << meilleurTheta
        << "° avec " << maxVotes << " votes.\n";

    // Affichage ASCII avec la ligne détectée
    std::cout << "\nReprésentation ASCII :\n";
    for (int y = 0; y < img.hauteur; ++y) {
        for (int x = 0; x < img.largeur; ++x) {
            int rhoCalc = (int)(x * std::cos(meilleurTheta * PI / 180.0) + y * std::sin(meilleurTheta * PI / 180.0));
            if (rhoCalc == meilleurRho) {
                std::cout << "#"; // Ligne détectée
            }
            else if (img.pixels[y][x] > 128) {
                std::cout << "."; // Point de bord
            }
            else {
                std::cout << " "; // Fond
            }
        }
        std::cout << std::endl;
    }
    std::cout << "FIN" << std::endl;
    supprimerDroitesProches(lignesDetectees, 10, 5);
    for (auto& ligne : lignesDetectees){
        std::cout << "Rho: " << ligne.first << ", Thetha: " << ligne.second << std::endl;
    }

    std::cout << "taille = " << lignesDetectees.size() << std::endl;
    for (size_t i = 0; i < lignesDetectees.size(); i++){
        std::cout << "Ligne " << i << ": rho = " << lignesDetectees[i].first << ", theta = " << lignesDetectees[i].second << std::endl;
    }

    return lignesDetectees;
}


/// Dessiner une droite : marche de Bresenham
void tracerBresenham(std::vector<std::vector<int>>& pixels, int x1, int y1, int x2, int y2, int couleur) {
    int dx = std::abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -std::abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        pixels[y1][x1] = couleur; // Dessine le pixel

        if (x1 == x2 && y1 == y2) break;

        e2 = 2 * err;

        if (e2 >= dy) { 
            err += dy; x1 += sx; 
        }

        if (e2 <= dx)
         { 
            err += dx; y1 += sy; 
        }
    }
}




int main(int argc, const char * argv[]) {
    

    Image image;
    image = lireImagePPM("imageM1.ppm");
    


    // Appliquer la transformée de Hough naïve
    transformeeDeHoughNaive(image);
    
    // Appliquer la transformée de Hough
    std::vector<std::vector<int>> accumulateur;
    
    std::vector<std::pair<int,int>> lignesDetectees = transformeeDeHough(image);


    for (auto& ligne : lignesDetectees) {
        int rho = ligne.first;
        int theta = ligne.second;
        // Convertir (rho, theta) en points (x1, y1) et (x2, y2)
        int x1 = 0, y1 = (int)(rho / std::sin(theta * PI / 180.0));
        int x2 = image.largeur, y2 = (int)((rho - x2 * std::cos(theta * PI / 180.0)) / std::sin(theta * PI / 180.0));

        tracerBresenham(image.pixels, x1, y1, x2, y2, 255);
    }

    std::cout << "Image après tracé de Bressenham :\n";
    for (int y =0; y < image.hauteur; ++y){
        for (int x = 0; x < image.largeur; ++x) {
            std::cout << (image.pixels[y][x] > 128 ? '.' : '#');
        }

        std::cout << std::endl;

    }


    return 0;
    
    
}
