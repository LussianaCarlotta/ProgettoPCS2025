#include "Triangolazione.hpp"
#include <Eigen/Dense>
#include <vector>
#include <limits>

using namespace Eigen;
using namespace std;

namespace PoliedriLibrary {

void TrovaOAggiungiSpigolo(
    int a, int b,
    PoliedriMesh& mesh,
    unsigned int& indiceSpigolo,
    unsigned int indiceFaccia)
{
    bool trovato = false;
    for (unsigned int i = 0; i < indiceSpigolo; ++i) {
        if ((mesh.Cell1DsExtrema(0, i) == a && mesh.Cell1DsExtrema(1, i) == b) ||
            (mesh.Cell1DsExtrema(0, i) == b && mesh.Cell1DsExtrema(1, i) == a)) {
            mesh.Cell2DsEdges[indiceFaccia].push_back(i);
            trovato = true;
            break;
        }
    }
    if (!trovato) {
        mesh.Cell1DsExtrema(0, indiceSpigolo) = a;
        mesh.Cell1DsExtrema(1, indiceSpigolo) = b;
        mesh.Cell1DsId[indiceSpigolo] = indiceSpigolo;
        mesh.Cell2DsEdges[indiceFaccia].push_back(indiceSpigolo);
        ++indiceSpigolo;
    }
}

void TriangolaFacceClasseI(int b, int c, const PoliedriMesh& meshIniziale, PoliedriMesh& meshRisultato) {
    unsigned int livelloSuddivisione = b + c;
    if (livelloSuddivisione == 0) return;

    unsigned int numVerticiPrevisti = (livelloSuddivisione + 1) * (livelloSuddivisione + 2) / 2;
    unsigned int maxSpigoli = 3 * livelloSuddivisione * livelloSuddivisione;
    unsigned int maxFacce = livelloSuddivisione * livelloSuddivisione;

    meshRisultato.Cell0DsCoordinates = MatrixXd::Zero(3, numVerticiPrevisti * meshIniziale.Cell2DsVertices.size());
    meshRisultato.Cell0DsId.resize(numVerticiPrevisti * meshIniziale.Cell2DsVertices.size());
    meshRisultato.Cell1DsExtrema = MatrixXi::Zero(2, maxSpigoli * meshIniziale.Cell2DsVertices.size());
    meshRisultato.Cell1DsId.resize(maxSpigoli * meshIniziale.Cell2DsVertices.size());
    meshRisultato.Cell2DsVertices.resize(maxFacce * meshIniziale.Cell2DsVertices.size());
    meshRisultato.Cell2DsEdges.resize(maxFacce * meshIniziale.Cell2DsVertices.size());
    meshRisultato.Cell2DsId.resize(maxFacce * meshIniziale.Cell2DsVertices.size());

    unsigned int indiceVertice = 0, indiceSpigolo = 0, indiceFaccia = 0;

    for (unsigned int idFaccia = 0; idFaccia < meshIniziale.Cell2DsVertices.size(); ++idFaccia) {
        const auto& faccia = meshIniziale.Cell2DsVertices[idFaccia];
        Vector3d V0 = meshIniziale.Cell0DsCoordinates.col(faccia[0]);
        Vector3d V1 = meshIniziale.Cell0DsCoordinates.col(faccia[1]);
        Vector3d V2 = meshIniziale.Cell0DsCoordinates.col(faccia[2]);

        vector<vector<int>> grigliaVertici;

        for (unsigned int i = 0; i <= livelloSuddivisione; ++i) {
            vector<int> riga;
            Vector3d inizio = ((double)i / livelloSuddivisione) * V1 + ((double)(livelloSuddivisione - i) / livelloSuddivisione) * V0;
            Vector3d fine   = ((double)i / livelloSuddivisione) * V2 + ((double)(livelloSuddivisione - i) / livelloSuddivisione) * V0;

            for (unsigned int j = 0; j <= i; ++j) {
                Vector3d punto;
                if (i == 0) {
                    punto = V0;
                } else {
                    punto = ((double)j / i) * fine + ((double)(i - j) / i) * inizio;
                }

                meshRisultato.Cell0DsCoordinates.col(indiceVertice) = punto;
                meshRisultato.Cell0DsId[indiceVertice] = indiceVertice;
                riga.push_back(indiceVertice);
                ++indiceVertice;
            }
            grigliaVertici.push_back(riga);
        }

        for (unsigned int i = 0; i < livelloSuddivisione; ++i) {
            for (unsigned int j = 0; j < i; ++j) {
                unsigned int a = grigliaVertici[i][j];
                unsigned int b = grigliaVertici[i + 1][j];
                unsigned int c = grigliaVertici[i + 1][j + 1];

                meshRisultato.Cell2DsVertices[indiceFaccia] = {a, b, c};
                meshRisultato.Cell2DsId[indiceFaccia] = indiceFaccia;
                meshRisultato.Cell2DsEdges[indiceFaccia] = {};
                for (int k = 0; k < 3; ++k)
                    TrovaOAggiungiSpigolo(meshRisultato.Cell2DsVertices[indiceFaccia][k], meshRisultato.Cell2DsVertices[indiceFaccia][(k + 1) % 3], meshRisultato, indiceSpigolo, indiceFaccia);
                ++indiceFaccia;

                unsigned int d = grigliaVertici[i][j];
                unsigned int e = grigliaVertici[i + 1][j + 1];
                unsigned int f = grigliaVertici[i][j + 1];

                meshRisultato.Cell2DsVertices[indiceFaccia] = {d, e, f};
                meshRisultato.Cell2DsId[indiceFaccia] = indiceFaccia;
                meshRisultato.Cell2DsEdges[indiceFaccia] = {};
                for (int k = 0; k < 3; ++k)
                    TrovaOAggiungiSpigolo(meshRisultato.Cell2DsVertices[indiceFaccia][k], meshRisultato.Cell2DsVertices[indiceFaccia][(k + 1) % 3], meshRisultato, indiceSpigolo, indiceFaccia);
                ++indiceFaccia;
            }

            unsigned int a = grigliaVertici[i][i];
            unsigned int b = grigliaVertici[i + 1][i];
            unsigned int c = grigliaVertici[i + 1][i + 1];

            meshRisultato.Cell2DsVertices[indiceFaccia] = {a, b, c};
            meshRisultato.Cell2DsId[indiceFaccia] = indiceFaccia;
            meshRisultato.Cell2DsEdges[indiceFaccia] = {};
            for (int k = 0; k < 3; ++k)
                TrovaOAggiungiSpigolo(meshRisultato.Cell2DsVertices[indiceFaccia][k], meshRisultato.Cell2DsVertices[indiceFaccia][(k + 1) % 3], meshRisultato, indiceSpigolo, indiceFaccia);
            ++indiceFaccia;
        }
    }

    meshRisultato.NumCell0Ds = indiceVertice;
    meshRisultato.NumCell1Ds = indiceSpigolo;
    meshRisultato.NumCell2Ds = indiceFaccia;
    meshRisultato.Cell3DsId.clear();
    meshRisultato.Cell3DsVertices.clear();
    meshRisultato.Cell3DsEdges.clear();
    meshRisultato.Cell3DsFaces.clear();
    meshRisultato.NumCell3Ds = 0;
}

} // namespace PoliedriLibrary