#include "Utils.hpp"
#include "PoliedriMesh.hpp"
#include <fstream>
#include <sstream>
#include <Eigen/Dense> 
#include <cmath>
#include <map>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <set>

using namespace std;
using namespace Eigen;

namespace PoliedriLibrary {

std::map<std::pair<unsigned int, unsigned int>, std::vector<unsigned int>>
buildEdgeToFacesMap(const PoliedriMesh& mesh) {
    std::map<std::pair<unsigned int, unsigned int>, std::vector<unsigned int>> edgeToFaces;

    for (unsigned int faceId = 0; faceId < mesh.Cell2DsVertices.size(); ++faceId) {
        const auto& verts = mesh.Cell2DsVertices[faceId];
        unsigned int n = verts.size();
        for (unsigned int i = 0; i < n; ++i) {
            unsigned int v0 = verts[i];
            unsigned int v1 = verts[(i + 1) % n];
            if (v0 > v1) std::swap(v0, v1);
            edgeToFaces[{v0, v1}].push_back(faceId);
        }
    }

    return edgeToFaces;
}


void CostruisciDualMesh(const PoliedriMesh& meshTriangolata, PoliedriMesh& meshDuale) {
  

    meshDuale = PoliedriMesh();  // reset struttura

    const auto& faces = meshTriangolata.Cell2DsVertices;
    const auto& vertices = meshTriangolata.Cell0DsCoordinates;

    // 1. Calcolo baricentri di tutte le facce
    vector<Vector3d> baricentri(faces.size());
    for (size_t i = 0; i < faces.size(); ++i) {
        Vector3d bary = Vector3d::Zero();
        for (unsigned int v : faces[i]) {
            bary += vertices.col(v);
        }
        bary /= faces[i].size();
        baricentri[i] = bary;
    }

    // 2. Salva baricentri come vertici del duale
    meshDuale.Cell0DsCoordinates.resize(3, baricentri.size());
    meshDuale.Cell0DsId.resize(baricentri.size());
    for (unsigned int i = 0; i < baricentri.size(); ++i) {
        meshDuale.Cell0DsCoordinates.col(i) = baricentri[i];
        meshDuale.Cell0DsId[i] = i;
    }
    meshDuale.NumCell0Ds = baricentri.size();

    // 3. Costruzione mappa da spigoli → facce
    auto edgeToFaces = buildEdgeToFacesMap(meshTriangolata);
	
	

    // 4. Per ogni spigolo condiviso da due facce, aggiungi un lato tra i rispettivi baricentri
    set<pair<unsigned int, unsigned int>> archiInseriti;
    vector<pair<unsigned int, unsigned int>> dualEdges;

    for (const auto& [edge, facce] : edgeToFaces) {
        if (facce.size() == 2) {
            unsigned int f1 = facce[0];
            unsigned int f2 = facce[1];

            if (f1 > f2) swap(f1, f2);  // ordinamento per evitare duplicati

            if (!archiInseriti.count({f1, f2})) {
                dualEdges.emplace_back(f1, f2);
                archiInseriti.insert({f1, f2});
            }
        }
    }

    // 5. Aggiungi spigoli alla mesh duale
    meshDuale.Cell1DsId.resize(dualEdges.size());
    meshDuale.Cell1DsExtrema.resize(2, dualEdges.size());

    for (unsigned int i = 0; i < dualEdges.size(); ++i) {
        meshDuale.Cell1DsId[i] = i;
        meshDuale.Cell1DsExtrema(0, i) = dualEdges[i].first;
        meshDuale.Cell1DsExtrema(1, i) = dualEdges[i].second;
    }
    meshDuale.NumCell1Ds = dualEdges.size();

    // 6. Nessuna faccia nel duale: è un grafo con solo vertici e spigoli
    meshDuale.Cell2DsId.clear();
    meshDuale.Cell2DsVertices.clear();
    meshDuale.Cell2DsEdges.clear();
    meshDuale.NumCell2Ds = 0;

    // 7. Cella 3D (opzionale)
    meshDuale.Cell3DsId = {0};
    meshDuale.Cell3DsVertices = {meshDuale.Cell0DsId};
    meshDuale.Cell3DsEdges = {meshDuale.Cell1DsId};
    meshDuale.Cell3DsFaces = {};  // vuoto
    meshDuale.NumCell3Ds = 1;
}


void ProiettaSuSfera(PoliedriMesh& mesh) {
    for (int i = 0; i < mesh.Cell0DsCoordinates.cols(); ++i) {
        mesh.Cell0DsCoordinates.col(i).normalize();  // proietta su sfera unitaria
    }
}


} 
