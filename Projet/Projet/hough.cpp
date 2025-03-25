//
//  hough.cpp
//  Projet
//
//  Created by Inès Veigneau on 24/03/2025.
//

#include <iostream>
#include "image.hpp"
#include "hough.hpp"
#include <cmath>
#include <map>
#include <vector>

#define SEUIL_BORD 128  // Seuil pour considérer un pixel comme un bord



// Fonction de transformation de Hough modifiée pour gérer les droites verticales
std::vector<Droite> transformeeDeHough(const Image& img) {
    std::map<double, std::map<double, int>> accumulateur;
    std::vector<Droite> droitesInteressantes;

    // Détection des droites verticales (basé sur les colonnes)
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


