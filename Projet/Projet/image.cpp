//
//  image.cpp
//  Projet
//
//  Created by Inès Veigneau on 24/03/2025.
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
    
    std::string format;
    int largeur, hauteur, maxVal;
    fichier >> format >> largeur >> hauteur >> maxVal;

    if (format != "P3") throw std::runtime_error("Format non supporté (Attendu : P3).");

    Image img{ largeur, hauteur, std::vector<std::vector<int>>(hauteur, std::vector<int>(largeur)) };

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
