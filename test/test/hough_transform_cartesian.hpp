//
//  hough_transform_cartesian.hpp
//  test
//
//  Created by Inès Veigneau on 24/03/2025.
//

#ifndef HOUGH_TRANSFORM_CARTESIAN_H
#define HOUGH_TRANSFORM_CARTESIAN_H

#include <vector>

class HoughTransformCartesian {
public:
    HoughTransformCartesian(int width, int height, double m_min, double m_max, double m_step);
    void applyTransform(const std::vector<std::vector<int>>& image);
    std::vector<std::pair<double, double>> getMaxLines(int threshold);

private:
    int width, height;
    double m_min, m_max, m_step;
    int m_bins, b_bins;
    std::vector<std::vector<int>> accumulator;
};

#endif
