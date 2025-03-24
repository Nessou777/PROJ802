//
//  hough.cpp
//  Projet
//
//  Created by Inès Veigneau on 24/03/2025.
//

#include "hough.hpp"
#include <map>
#include <iostream>
#include <cmath>

#define SEUIL_BORD 100  // Seuil pour considérer un pixel comme un bord

Droite transformeeDeHough(const Image& img) {
    std::map<double, std::map<double, int>> accumulateur;

    // Parcourir chaque pixel de l'image
    for (int y = 0; y < img.hauteur; ++y) {
        for (int x = 0; x < img.largeur; ++x) {
            if (img.pixels[y][x] < SEUIL_BORD) { // Pixel détecté comme un bord
                for (double m = -5.0; m <= 5.0; m += 0.1) { // Balayage des pentes
                    double b = y - m * x;
                    accumulateur[m][b]++;
                }
            }
        }
    }

    // Trouver la droite ayant reçu le plus de votes
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

    std::cout << "Droite détectée : y = " << meilleurM << " * x + " << meilleurB
              << " avec " << maxVotes << " votes." << std::endl;

    return { meilleurM, meilleurB, maxVotes };
}

