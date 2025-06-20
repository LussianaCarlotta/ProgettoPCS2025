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

map<pair<unsigned int, unsigned int>, vector<unsigned int>> Mappa_LatoFaccia(const PoliedriMesh& mesh) {
    map<pair<unsigned int, unsigned int>, vector<unsigned int>> lato_faccia;  // la chiave è una coppia ordinata di vertici che indica un lato

    for (unsigned int faceId = 0; faceId < mesh.Cell2DsVertices.size(); ++faceId) {
		
        const auto& verts = mesh.Cell2DsVertices[faceId];
        unsigned int n = verts.size();
		
        for (unsigned int i = 0; i < n; ++i) {
            unsigned int v0 = verts[i];
            unsigned int v1 = verts[(i + 1) % n];
            if (v0 > v1)
				swap(v0, v1);
            lato_faccia[{v0, v1}].push_back(faceId);
        }
    }

    return lato_faccia;
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
    auto lato_faccia = Mappa_LatoFaccia(meshTriangolata);

    // 4. Per ogni spigolo condiviso da due facce, aggiungi un lato tra i rispettivi baricentri
    set<pair<unsigned int, unsigned int>> archiInseriti;  // contiene un insieme ordinato di oggetti univoci di tipo key
    vector<pair<unsigned int, unsigned int>> spigoliDuali;

    for (const auto& [edge, facce] : lato_faccia) {
        if (facce.size() == 2) {
            unsigned int f1 = facce[0];
            unsigned int f2 = facce[1];

            if (f1 > f2)
				swap(f1, f2);  // ordinamento per evitare duplicati

            if (!archiInseriti.count({f1, f2})) {  // count() restituisce il numero di elementi uguali a {f1, f2}
                spigoliDuali.emplace_back(f1, f2);  // inserimento degli elementi alla fine del vettore (a differenza di push_back costruisce direttamente l'elemento senza crearne una copia	)
                archiInseriti.insert({f1, f2});
            }
        }
    }

    // 5. Aggiungi spigoli alla mesh duale
    meshDuale.Cell1DsId.resize(spigoliDuali.size());
    meshDuale.Cell1DsExtrema.resize(2, spigoliDuali.size());

    for (unsigned int i = 0; i < spigoliDuali.size(); ++i) {
        meshDuale.Cell1DsId[i] = i;
        meshDuale.Cell1DsExtrema(0, i) = spigoliDuali[i].first;
        meshDuale.Cell1DsExtrema(1, i) = spigoliDuali[i].second;
    }
    meshDuale.NumCell1Ds = spigoliDuali.size();

    // 6. Nessuna faccia nel duale: è un grafo con solo vertici e spigoli
    meshDuale.Cell2DsId.clear();
    meshDuale.Cell2DsVertices.clear();
    meshDuale.Cell2DsEdges.clear();
    meshDuale.NumCell2Ds = 0;

    // 7. Cella 3D
    meshDuale.Cell3DsId.clear();
	meshDuale.Cell3DsVertices.clear();
	meshDuale.Cell3DsEdges.clear();
	meshDuale.Cell3DsFaces.clear();
	meshDuale.NumCell3Ds = 0;
	
	meshDuale.Cell3DsId.push_back(0);
	meshDuale.Cell3DsVertices.push_back(meshDuale.Cell0DsId);
	meshDuale.Cell3DsEdges.push_back(meshDuale.Cell1DsId);
	meshDuale.Cell3DsFaces.push_back({});  // vuoto
	meshDuale.NumCell3Ds = 1;
}


void ProiettaSuSfera(PoliedriMesh& mesh) {
    for (int i = 0; i < mesh.Cell0DsCoordinates.cols(); ++i) {
        mesh.Cell0DsCoordinates.col(i).normalize();  // proietta su sfera unitaria
    }
}


} 
