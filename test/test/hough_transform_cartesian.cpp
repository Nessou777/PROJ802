//
//  hough_transform_cartesian.cpp
//  test
//
//  Created by Inès Veigneau on 24/03/2025.
//

#include "hough_transform_cartesian.hpp"
#include <cmath>
#include <iostream>

HoughTransformCartesian::HoughTransformCartesian(int width, int height, double m_min, double m_max, double m_step)
    : width(width), height(height), m_min(m_min), m_max(m_max), m_step(m_step) {

    m_bins = static_cast<int>((m_max - m_min) / m_step);
    b_bins = 2 * height;  // Plage de b

    accumulator.resize(b_bins, std::vector<int>(m_bins, 0));
}

void HoughTransformCartesian::applyTransform(const std::vector<std::vector<int>>& image) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (image[y][x] != 255) {  // On ignore les pixels blancs
                for (int m_index = 0; m_index < m_bins; m_index++) {
                    double m = m_min + m_index * m_step;
                    double b = y - m * x;
                    int b_index = static_cast<int>(b + height);

                    if (b_index >= 0 && b_index < b_bins) {
                        accumulator[b_index][m_index]++;
                    }
                }
            }
        }
    }
}

std::vector<std::pair<double, double>> HoughTransformCartesian::getMaxLines(int threshold) {
    std::vector<std::pair<double, double>> lines;
    for (int b_index = 0; b_index < b_bins; b_index++) {
        for (int m_index = 0; m_index < m_bins; m_index++) {
            if (accumulator[b_index][m_index] > threshold) {
                double m = m_min + m_index * m_step;
                double b = b_index - height;
                lines.emplace_back(m, b);
            }
        }
    }
    return lines;
}
