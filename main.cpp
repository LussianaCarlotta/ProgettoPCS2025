#include "PoliedriMesh.hpp"
#include "Utils.hpp"
#include "Triangolazione.hpp"
#include <iostream>

using namespace std;
using namespace PoliedriLibrary;

int main() 
{
    PoliedriMesh meshPlatonico;
    unsigned int p, q;
    cout << "Inserisci i parametri p, q : ";
    cin >> p >> q;

    if (!ImportMesh(meshPlatonico, p, q)) {
        cerr << "Errore nell'importazione della mesh" << endl;
        return 1;
    }

    // <---- CORREZIONE QUI ---->
    meshPlatonico.Cell2DsNumVertices.resize(meshPlatonico.NumCell2Ds);
    for (size_t i = 0; i < meshPlatonico.NumCell2Ds; ++i) {
        meshPlatonico.Cell2DsNumVertices[i] = meshPlatonico.Cell2DsVertices[i].size();
    }

    bool ok = ScritturaCelle(meshPlatonico, "Platonico");
    if (!ok) {
        cerr << "ScritturaCelle ha fallito" << endl;
        return 1;
    }

    cout << "ScritturaCelle completato" << endl;

    Export(meshPlatonico);

    unsigned int b;
    cout << "Inserisci il parametro di triangolazione b: ";
    cin >> b;

    PoliedriMesh meshGeodetico;
    TriangolaFacceClasseI(meshPlatonico, b, meshGeodetico);

    meshGeodetico.Cell2DsNumVertices.resize(meshGeodetico.NumCell2Ds);
    for (size_t i = 0; i < meshGeodetico.NumCell2Ds; ++i) {
        meshGeodetico.Cell2DsNumVertices[i] = meshGeodetico.Cell2DsVertices[i].size();
    }
    meshGeodetico.Cell2DsEdges.resize(meshGeodetico.NumCell2Ds);
	for (size_t i = 0; i < meshGeodetico.NumCell2Ds; ++i) {
		meshGeodetico.Cell2DsEdges[i].resize(meshGeodetico.Cell2DsVertices[i].size());
	}

    ScritturaCelle(meshGeodetico, "Geodetico");
    Export(meshGeodetico);

    PoliedriMesh meshDuale;
    CostruisciDualMesh(meshGeodetico, meshDuale);

    meshDuale.Cell2DsNumVertices.resize(meshDuale.NumCell2Ds);
    for (size_t i = 0; i < meshDuale.NumCell2Ds; ++i) {
        meshDuale.Cell2DsNumVertices[i] = meshDuale.Cell2DsVertices[i].size();
    }

    ScritturaCelle(meshDuale, "Duale");
    Export(meshDuale);

    return 0;
}

