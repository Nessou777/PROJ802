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

std::vector<Droite> transformeeDeHough(const Image& img) {
    std::map<double, std::map<double, int>> accumulateur;

    // Parcourir chaque pixel de l'image
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

    // Définir un seuil pour les votes
    int seuilVotes = 5; // Par exemple, récupérer toutes les droites avec plus de 5 votes

    // Liste pour stocker toutes les droites détectées
    std::vector<Droite> droitesInteressantes;

    // Trouver toutes les droites ayant reçu un nombre de votes supérieur au seuil
    for (const auto& pairM : accumulateur) {
        for (const auto& pairB : pairM.second) {
            if (pairB.second > seuilVotes) { // Vérifie si le nombre de votes dépasse le seuil
                Droite droite = {pairM.first, pairB.first, pairB.second};
                droitesInteressantes.push_back(droite); // Ajouter la droite à la liste
            }
        }
    }

    // Affichage des droites intéressantes
    std::cout << "Droites détectées (avec plus de " << seuilVotes << " votes) :" << std::endl;
    for (const auto& droite : droitesInteressantes) {
        std::cout << "y = " << droite.m << " * x + " << droite.b << " avec "
                  << droite.votes << " votes." << std::endl;
    }

    return droitesInteressantes; // Retourner la liste des droites détectées
}
