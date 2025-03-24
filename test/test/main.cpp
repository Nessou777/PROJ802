//
//  main.cpp
//  test
//
//  Created by Inès Veigneau on 24/03/2025.
//

#include "ppm_image.hpp"
#include "hough_transform_cartesian.hpp"
#include <iostream>


int main(int argc, char** argv) {

    PPMImage image("/Users/inesveigneau/Documents/Cours/Fac/M1/projet802/PROJ802/test/test/image.ppm");

    // Sauvegarde de l'image en niveaux de gris
    image.saveAsPGM("output.pgm");

    // Transformée de Hough naïve
    HoughTransformCartesian hough(image.width, image.height, -5.0, 5.0, 0.1);
    hough.applyTransform(image.grayscaleImage);

    // Récupérer les droites détectées
    auto lines = hough.getMaxLines(10);
    for (const auto& line : lines) {
        std::cout << "Droite détectée : y = " << line.first << "x + " << line.second << std::endl;
    }

    return 0;
}
