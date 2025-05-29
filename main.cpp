#include "PoliedriMesh.hpp"
#include "Utils.hpp"
#include "Triangolazione.hpp"
#include <iostream>

using namespace std;
using namespace PoliedriLibrary;

int main(){

    PoliedriMesh meshPlatonico;

    unsigned int p, q;
    cout << "Inserisci i parametri p, q : ";
    cin >> p >> q;

    if (!ImportMesh(meshPlatonico, p, q)) {
        cerr << "Errore nell'importazione della mesh." << endl;
        return 1;
    }

    meshPlatonico.Cell2DsNumVertices.resize(meshPlatonico.NumCell2Ds);
    for (size_t i = 0; i < meshPlatonico.NumCell2Ds; ++i)
        meshPlatonico.Cell2DsNumVertices[i] = meshPlatonico.Cell2DsVertices[i].size();

    if (!ScritturaCelle(meshPlatonico, "Platonico")) {
        cerr << "Errore nella scrittura della mesh platonica." << endl;
        return 1;
    }

    Export(meshPlatonico, "Platonico");
    cout << "Mesh platonica esportata correttamente.\n" << endl;

    unsigned int b, c;
    cout << "Inserisci il parametro di triangolazione b: ";
    cin >> b;
    cout << "Inserisci il parametro di triangolazione c: ";
    cin >> c;

    if (b == 0 && c == 0) {
        cerr << "Errore: almeno uno tra b o c deve essere maggiore di 0." << endl;
        return 1;
    }

    PoliedriMesh meshGeodetico;
    TriangolaFacceClasseI(b, c, meshPlatonico, meshGeodetico);

    meshGeodetico.Cell2DsNumVertices.resize(meshGeodetico.NumCell2Ds);
    for (size_t i = 0; i < meshGeodetico.NumCell2Ds; ++i)
        meshGeodetico.Cell2DsNumVertices[i] = meshGeodetico.Cell2DsVertices[i].size();

    for (size_t i = 0; i < meshGeodetico.Cell2DsEdges.size(); ++i)
        meshGeodetico.Cell2DsEdges[i].resize(meshGeodetico.Cell2DsVertices[i].size());

    if (!ScritturaCelle(meshGeodetico, "Geodetico")) {
        cerr << "Errore nella scrittura della mesh geodetica." << endl;
        return 1;
    }

    Export(meshGeodetico, "Geodetico");
    cout << "Mesh geodetica esportata correttamente.\n" << endl;

   



/*
    PoliedriMesh meshDuale;
    CostruisciDualMesh(meshGeodetico, meshDuale);

    meshDuale.Cell2DsNumVertices.resize(meshDuale.NumCell2Ds);
    for (size_t i = 0; i < meshDuale.NumCell2Ds; ++i) {
        meshDuale.Cell2DsNumVertices[i] = meshDuale.Cell2DsVertices[i].size();
    }

    ScritturaCelle(meshDuale, "Duale");
    Export(meshDuale, "Duale");
*/
    return 0;
}

