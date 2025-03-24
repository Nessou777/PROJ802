//
//  ppm_image.cpp
//  test
//
//  Created by Inès Veigneau on 24/03/2025.
//

#include "ppm_image.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

PPMImage::PPMImage(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << std::endl;
        return;
    }

    std::string format;
    file >> format;
    if (format != "P3") {
        std::cerr << "Erreur : Format PPM non supporté (seul P3 est pris en charge)" << std::endl;
        return;
    }

    file >> width >> height;
    int maxVal;
    file >> maxVal;

    grayscaleImage.resize(height, std::vector<int>(width, 0));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int r, g, b;
            file >> r >> g >> b;
            int gray = static_cast<int>(0.3 * r + 0.59 * g + 0.11 * b);  // Conversion en niveau de gris
            grayscaleImage[y][x] = gray;
        }
    }
}

void PPMImage::saveAsPGM(const std::string& outputFilename) const {
    std::ofstream file(outputFilename);
    if (!file) {
        std::cerr << "Erreur : Impossible d'écrire dans le fichier " << outputFilename << std::endl;
        return;
    }

    file << "P2\n" << width << " " << height << "\n255\n";
    for (const auto& row : grayscaleImage) {
        for (int gray : row) {
            file << gray << " ";
        }
        file << "\n";
    }
    std::cout << "Image sauvegardée en niveaux de gris : " << outputFilename << std::endl;
}
