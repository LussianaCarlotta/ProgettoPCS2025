#include "Triangolazione.hpp"
#include <Eigen/Dense>
#include <vector>
#include <limits>
#include <map>
#include <utility>
#include <set>
#include <iostream>

using namespace Eigen;
using namespace std;

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
void TriangolaFacceClasseII(const PoliedriMesh& meshIniziale, PoliedriMesh& meshRisultato, unsigned int b) {
    if (b == 0) {
        cerr << "Errore: livelloSuddivisione = 0. Impossibile triangolare." << endl;
        return;
    }

    // Passaggio 1: usa triangolazione di tipo I per ottenere i sottotriangoli
    PoliedriMesh meshTipoI;
    TriangolaFacceClasseI(meshIniziale, meshTipoI, b);

    // Inizializza mesh risultato
    meshRisultato.Cell0DsCoordinates.resize(3, 0);
    meshRisultato.Cell0DsId.clear();
    meshRisultato.Cell1DsExtrema.resize(2, 0);
    meshRisultato.Cell1DsId.clear();
    meshRisultato.Cell2DsVertices.clear();
    meshRisultato.Cell2DsEdges.clear();
    meshRisultato.Cell2DsId.clear();

    map<Vector3d, unsigned int, CompareVector3d> mappaVertici;
    map<pair<unsigned int, unsigned int>, unsigned int> mappaSpigoli;

    // Funzione di supporto per ottenere o creare un vertice
    auto getIndiceVertice = [&](const Vector3d &punto) -> unsigned int {
        auto it = mappaVertici.find(punto);
        if (it != mappaVertici.end())
            return it->second;

        unsigned int indice = meshRisultato.Cell0DsCoordinates.cols();
        meshRisultato.Cell0DsCoordinates.conservativeResize(3, indice + 1);
        meshRisultato.Cell0DsCoordinates.col(indice) = punto;
        meshRisultato.Cell0DsId.push_back(indice);
        mappaVertici[punto] = indice;
        return indice;
    };

    // Per ogni sottotriangolo della mesh di tipo I, applica suddivisione baricentrica
    for (const auto &tri : meshTipoI.Cell2DsVertices) {
        Vector3d P0 = meshTipoI.Cell0DsCoordinates.col(tri[0]);
        Vector3d P1 = meshTipoI.Cell0DsCoordinates.col(tri[1]);
        Vector3d P2 = meshTipoI.Cell0DsCoordinates.col(tri[2]);

        Vector3d G = (P0 + P1 + P2) / 3.0;
        Vector3d M01 = (P0 + P1) / 2.0;
        Vector3d M12 = (P1 + P2) / 2.0;
        Vector3d M20 = (P2 + P0) / 2.0;

        unsigned int iP0 = getIndiceVertice(P0);
        unsigned int iP1 = getIndiceVertice(P1);
        unsigned int iP2 = getIndiceVertice(P2);
        unsigned int iG  = getIndiceVertice(G);
        unsigned int iM01 = getIndiceVertice(M01);
        unsigned int iM12 = getIndiceVertice(M12);
        unsigned int iM20 = getIndiceVertice(M20);

        // Costruisci i 6 triangoli attorno al baricentro
        vector<pair<unsigned int, unsigned int>> lati = {
            {iP0, iM01},
            {iM01, iP1},
            {iP1, iM12},
            {iM12, iP2},
            {iP2, iM20},
            {iM20, iP0}
        };

        for (size_t i = 0; i < lati.size(); ++i) {
            unsigned int v1 = lati[i].first;
            unsigned int v2 = lati[i].second;

            meshRisultato.Cell2DsVertices.push_back({iG, v1, v2});
            unsigned int idTri = meshRisultato.Cell2DsId.size();
            meshRisultato.Cell2DsId.push_back(idTri);
            meshRisultato.Cell2DsEdges.push_back({
                TrovaSpigolo(mappaSpigoli, meshRisultato, iG, v1),
                TrovaSpigolo(mappaSpigoli, meshRisultato, v1, v2),
                TrovaSpigolo(mappaSpigoli, meshRisultato, v2, iG)
            });
        }
    }

    // Finalizzazione metadati mesh
    meshRisultato.NumCell0Ds = meshRisultato.Cell0DsCoordinates.cols();
    meshRisultato.NumCell1Ds = meshRisultato.Cell1DsExtrema.cols();
    meshRisultato.NumCell2Ds = meshRisultato.Cell2DsVertices.size();
    meshRisultato.NumCell3Ds = 0;
    meshRisultato.Cell3DsId.clear();
    meshRisultato.Cell3DsNumVertices.clear();
    meshRisultato.Cell3DsVertices.clear();
    meshRisultato.Cell3DsNumEdges.clear();
    meshRisultato.Cell3DsEdges.clear();
    meshRisultato.Cell3DsNumFaces.clear();
    meshRisultato.Cell3DsFaces.clear();
}

} // namespace PoliedriLibrary
