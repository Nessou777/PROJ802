//
//  ppm_image.hpp
//  test
//
//  Created by Inès Veigneau on 24/03/2025.
//

#ifndef PPM_IMAGE_H
#define PPM_IMAGE_H

#include <vector>
#include <string>

class PPMImage {
public:
    int width, height;
    std::vector<std::vector<int>> grayscaleImage;  // Stockage des niveaux de gris

    explicit PPMImage(const std::string& filename);
    void saveAsPGM(const std::string& outputFilename) const;
};

#endif
