#include "Triangolazione.hpp"
#include "Utils.hpp"
#include <Eigen/Dense>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <iostream>
#include <cmath>

using namespace std;
using namespace Eigen;

namespace PoliedriLibrary {

struct CompareVector3d {
    bool operator()(const Vector3d& a, const Vector3d& b) const {
        for (int i = 0; i < 3; ++i) {
            if (abs(a[i] - b[i]) > 1e-9)
                return a[i] < b[i];
        }
        return false;
    }
};

// Triangolazione Classe II: suddivide ogni faccia triangolare in n^2 triangoli minori con b = c

void TriangolaFacceClasseII(const PoliedriMesh &meshIniziale, PoliedriMesh &meshRisultato, unsigned int b) {
    if (b == 0) {
        cerr << "Errore: b = 0 non valido per triangolazione di classe II." << endl;
        return;
    }

    const unsigned int livelloSuddivisione = b;

    meshRisultato.Cell0DsCoordinates.resize(3, 0);
    meshRisultato.Cell0DsId.clear();
    meshRisultato.Cell1DsExtrema.resize(2, 0);
    meshRisultato.Cell1DsId.clear();
    meshRisultato.Cell2DsVertices.clear();
    meshRisultato.Cell2DsEdges.clear();
    meshRisultato.Cell2DsId.clear();

    map<Vector3d, unsigned int, CompareVector3d> mappaVertici;
    map<pair<unsigned int, unsigned int>, unsigned int> mappaSpigoli;

    for (unsigned int idFaccia = 0; idFaccia < meshIniziale.Cell2DsVertices.size(); ++idFaccia) {
        const auto& faccia = meshIniziale.Cell2DsVertices[idFaccia];

        if (faccia.size() != 3) {
            cerr << "Triangolazione Classe II supporta solo facce triangolari." << endl;
            continue;
        }

        Vector3d A = meshIniziale.Cell0DsCoordinates.col(faccia[0]);
        Vector3d B = meshIniziale.Cell0DsCoordinates.col(faccia[1]);
        Vector3d C = meshIniziale.Cell0DsCoordinates.col(faccia[2]);

        vector<vector<unsigned int>> idVertici(livelloSuddivisione + 1);

        for (unsigned int i = 0; i <= livelloSuddivisione; ++i) {
            for (unsigned int j = 0; j <= i; ++j) {
                double a = 1.0 - static_cast<double>(i) / livelloSuddivisione;
                double b_coef = static_cast<double>(i - j) / livelloSuddivisione;
                double c_coef = static_cast<double>(j) / livelloSuddivisione;

                Vector3d punto = a * A + b_coef * B + c_coef * C;

                unsigned int indice;
                auto it = mappaVertici.find(punto);
                if (it != mappaVertici.end()) {
                    indice = it->second;
                } else {
                    indice = meshRisultato.Cell0DsCoordinates.cols();
                    meshRisultato.Cell0DsCoordinates.conservativeResize(3, indice + 1);
                    meshRisultato.Cell0DsCoordinates.col(indice) = punto;
                    meshRisultato.Cell0DsId.push_back(indice);
                    mappaVertici[punto] = indice;
                }

                idVertici[i].push_back(indice);
            }
        }

        // Generazione triangoli
        for (unsigned int i = 1; i <= livelloSuddivisione; ++i) {
            for (unsigned int j = 0; j < i; ++j) {
                unsigned int k1 = idVertici[i][j];
                unsigned int k2 = idVertici[i][j + 1];
                unsigned int k3 = idVertici[i - 1][j];
                unsigned int k4 = idVertici[i - 1][j + 1];

                // Primo triangolo
                unsigned int id1 = meshRisultato.Cell2DsVertices.size();
                meshRisultato.Cell2DsVertices.push_back({k1, k2, k3});
                meshRisultato.Cell2DsId.push_back(id1);
                meshRisultato.Cell2DsEdges.push_back({
                    TrovaSpigolo(mappaSpigoli, meshRisultato, k1, k2),
                    TrovaSpigolo(mappaSpigoli, meshRisultato, k2, k3),
                    TrovaSpigolo(mappaSpigoli, meshRisultato, k3, k1)
                });

                if (j + 1 < i) {
                    // Secondo triangolo
                    unsigned int id2 = meshRisultato.Cell2DsVertices.size();
                    meshRisultato.Cell2DsVertices.push_back({k2, k4, k3});
                    meshRisultato.Cell2DsId.push_back(id2);
                    meshRisultato.Cell2DsEdges.push_back({
                        TrovaSpigolo(mappaSpigoli, meshRisultato, k2, k4),
                        TrovaSpigolo(mappaSpigoli, meshRisultato, k4, k3),
                        TrovaSpigolo(mappaSpigoli, meshRisultato, k3, k2)
                    });
                }
            }
        }
    }

    // Finalizzazione
    meshRisultato.NumCell0Ds = meshRisultato.Cell0DsCoordinates.cols();
    meshRisultato.NumCell1Ds = meshRisultato.Cell1DsExtrema.cols();
    meshRisultato.NumCell2Ds = meshRisultato.Cell2DsVertices.size();
    meshRisultato.NumCell3Ds = 0;
    meshRisultato.Cell3DsId.clear();
    meshRisultato.Cell3DsVertices.clear();
    meshRisultato.Cell3DsEdges.clear();
    meshRisultato.Cell3DsFaces.clear();
    
    cout << "Classe II: triangoli generati = " << meshRisultato.Cell2DsVertices.size() << endl;

}



} // namespace PoliedriLibrary
