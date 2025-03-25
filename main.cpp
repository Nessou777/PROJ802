#include <iostream>
#include <fstream>
#include <vector>

struct Pixel {
    int r, g, b;
};

int main() {
    std::ifstream fichier("H:\PERSO\Github\imgTresSimple.ppm");
    if (!fichier) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier image.ppm !" << std::endl;
        return 1;
    }

    std::string format;
    int largeur, hauteur, maxVal;

    // Lire l'en-tête du fichier
    fichier >> format >> largeur >> hauteur >> maxVal;

    // Vérifier que le format est bien P3
    if (format != "P3") {
        std::cerr << "Erreur : Format non supporté (Attendu : P3) !" << std::endl;
        return 1;
    }

    // Vérifier que maxVal est bien dans une plage gérable
    if (maxVal > 255) {
        std::cerr << "Erreur : Valeur maximale des couleurs trop grande (" << maxVal << ") !" << std::endl;
        return 1;
    }

    // Stocker les pixels dans un vecteur
    std::vector<Pixel> image(largeur * hauteur);

    // Lire les pixels (R, G, B sous forme de nombres)
    for (int i = 0; i < largeur * hauteur; ++i) {
        fichier >> image[i].r >> image[i].g >> image[i].b;
    }

    // Afficher quelques informations sur l'image
    std::cout << "Lecture réussie !\n";
    std::cout << "Dimensions : " << largeur << " x " << hauteur << "\n";
    std::cout << "Premier pixel - R: " << image[0].r
              << " G: " << image[0].g
              << " B: " << image[0].b << std::endl;

    return 0;
}

