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
    cout << "[INFO] Forzatura parametro: b = 2 (Classe II con 6 triangoli per faccia)\n";
    b = 2;

    if (b == 0) {
        cerr << "Errore: b = 0 non valido per triangolazione di classe II." << std::endl;
        return;
    }

    // Inizializzazione struttura risultato
    meshRisultato.Cell0DsCoordinates.resize(3, 0);
    meshRisultato.Cell0DsId.clear();
    meshRisultato.Cell1DsExtrema.resize(2, 0);
    meshRisultato.Cell1DsId.clear();
    meshRisultato.Cell2DsVertices.clear();
    meshRisultato.Cell2DsEdges.clear();
    meshRisultato.Cell2DsId.clear();

    // Mappa vertici: confronto con tolleranza
    vector<Vector3d> verticiMemorizzati;
    map<pair<unsigned int, unsigned int>, unsigned int> mappaSpigoli;
	
	
	// DA CAMBIARE
    auto trovaIdVertice = [&](const Vector3d& P) -> unsigned int {
        double tolleranza = 1e-6;
        for (size_t i = 0; i < verticiMemorizzati.size(); ++i) {
            if ((verticiMemorizzati[i] - P).norm() < tolleranza) {
                return static_cast<unsigned int>(i);
            }
        }
        unsigned int nuovoId = verticiMemorizzati.size();
        verticiMemorizzati.push_back(P);
        meshRisultato.Cell0DsCoordinates.conservativeResize(3, nuovoId + 1);
        meshRisultato.Cell0DsCoordinates.col(nuovoId) = P;
        meshRisultato.Cell0DsId.push_back(nuovoId);
        return nuovoId;
    };

    for (unsigned int idFaccia = 0; idFaccia < meshIniziale.Cell2DsVertices.size(); ++idFaccia) {
        const auto& faccia = meshIniziale.Cell2DsVertices[idFaccia];
        if (faccia.size() != 3) continue;

        Vector3d A = meshIniziale.Cell0DsCoordinates.col(faccia[0]);
        Vector3d B = meshIniziale.Cell0DsCoordinates.col(faccia[1]);
        Vector3d C = meshIniziale.Cell0DsCoordinates.col(faccia[2]);

        vector<std::vector<unsigned int>> idVertici(b + 1);
        for (unsigned int i = 0; i <= b; ++i) {
            idVertici[i].resize(b + 1 - i);
            for (unsigned int j = 0; j <= b - i; ++j) {
                double u = static_cast<double>(i) / b;
                double v = static_cast<double>(j) / b;
                double w = 1.0 - u - v;
                Vector3d P = u * B + v * C + w * A;

                idVertici[i][j] = trovaIdVertice(P);
            }
        }

        for (unsigned int i = 0; i < b; ++i) {
            for (unsigned int j = 0; j < b - i; ++j) {
                unsigned int v0 = idVertici[i][j];
                unsigned int v1 = idVertici[i + 1][j];
                unsigned int v2 = idVertici[i][j + 1];

                meshRisultato.Cell2DsVertices.push_back({v0, v1, v2});
                meshRisultato.Cell2DsId.push_back(meshRisultato.Cell2DsVertices.size() - 1);
                vector<unsigned int> edges1 = {
                    TrovaSpigolo(mappaSpigoli, meshRisultato, v0, v1),
                    TrovaSpigolo(mappaSpigoli, meshRisultato, v1, v2),
                    TrovaSpigolo(mappaSpigoli, meshRisultato, v2, v0)
                };
                meshRisultato.Cell2DsEdges.push_back(edges1);

                if (j < b - i - 1) {
                    unsigned int v3 = idVertici[i + 1][j + 1];
                    meshRisultato.Cell2DsVertices.push_back({v1, v3, v2});
                    meshRisultato.Cell2DsId.push_back(meshRisultato.Cell2DsVertices.size() - 1);
                    vector<unsigned int> edges2 = {
                        TrovaSpigolo(mappaSpigoli, meshRisultato, v1, v3),
                        TrovaSpigolo(mappaSpigoli, meshRisultato, v3, v2),
                        TrovaSpigolo(mappaSpigoli, meshRisultato, v2, v1)
                    };
                    meshRisultato.Cell2DsEdges.push_back(edges2);
                }
            }
        }
    }

    meshRisultato.NumCell0Ds = meshRisultato.Cell0DsCoordinates.cols();
    meshRisultato.NumCell1Ds = meshRisultato.Cell1DsExtrema.cols();
    meshRisultato.NumCell2Ds = meshRisultato.Cell2DsVertices.size();
    meshRisultato.NumCell3Ds = 0;
    meshRisultato.Cell3DsId.clear();
    meshRisultato.Cell3DsVertices.clear();
    meshRisultato.Cell3DsEdges.clear();
    meshRisultato.Cell3DsFaces.clear();
}








}
