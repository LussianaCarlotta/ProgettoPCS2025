#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;
using namespace Eigen;

namespace PoliedriLibrary {


void normalizza(double& x, double& y, double& z) {
    double norma = sqrt(x * x + y * y + z * z);
    x /= norma;
    y /= norma;
    z /= norma;
}

/// Celle0Ds
void AggiungiVertici(PoliedriMesh& mesh, const vector<Vector3d>& vertici) {
	unsigned int IdOffset = mesh.NumCell0Ds;
	mesh.NumCell0Ds += vertici.size();
    mesh.Cell0DsId.resize(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates.conservativeResize(3, mesh.NumCell0Ds);
    
    for (unsigned int i = 0; i < vertici.size(); ++i) {
        double x = vertici[i](0);
        double y = vertici[i](1);
        double z = vertici[i](2);
        normalizza(x, y, z);

        mesh.Cell0DsId[IdOffset + i] = IdOffset + i;
        mesh.Cell0DsCoordinates(0, IdOffset + i) = x;
        mesh.Cell0DsCoordinates(1, IdOffset + i) = y;
        mesh.Cell0DsCoordinates(2, IdOffset + i) = z;
    }
}

void creaPoliedriNormalizzati(PoliedriMesh& mesh) {
	//Tetraedro
    vector<Vector3d> verticitetraedro = {
        {1.0, 1.0, 1.0}, {-1.0, -1.0, 1.0}, {-1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}
    };
    AggiungiVertici(mesh, verticitetraedro);

	//Cubo
    vector<Vector3d> verticicubo = {
        {-1.0, -1.0, -1.0}, {-1.0, -1.0,  1.0}, {-1.0,  1.0, -1.0}, {-1.0,  1.0,  1.0},
        { 1.0, -1.0, -1.0}, { 1.0, -1.0,  1.0}, { 1.0,  1.0, -1.0}, { 1.0,  1.0,  1.0}
    };
    AggiungiVertici(mesh, verticicubo);

	//Ottaedro
    vector<Vector3d> verticiottaedro = {
        { 1.0,  0.0,  0.0}, {-1.0,  0.0,  0.0}, { 0.0,  1.0,  0.0},
        { 0.0, -1.0,  0.0}, { 0.0,  0.0,  1.0}, { 0.0,  0.0, -1.0}
    };
    AggiungiVertici(mesh, verticiottaedro);

	
	const double PHI = (1.0 + sqrt(5.0)) / 2.0;
	
	
	//Dodecaedro
    vector<Vector3d> verticidodecaedro = {
        {0, 1 / PHI, PHI}, {0, -1 / PHI, PHI}, {0, 1 / PHI, -PHI}, {0, -1 / PHI, -PHI},
        {1 / PHI, PHI, 0}, {-1 / PHI, PHI, 0}, {1 / PHI, -PHI, 0}, {-1 / PHI, -PHI, 0},
        {PHI, 0, 1 / PHI}, {-PHI, 0, 1 / PHI}, {PHI, 0, -1 / PHI}, {-PHI, 0, -1 / PHI},
        {1, 1, 1}, {-1, 1, 1}, {1, -1, 1}, {-1, -1, 1}, {1, 1, -1}, {-1, 1, -1}, {1, -1, -1}, {-1, -1, -1}
    };
    AggiungiVertici(mesh, verticidodecaedro);

	//Icosaedro
	vector<Vector3d> verticiicosaedro = {
        {0, PHI, 1}, {0, -PHI, 1}, {0, PHI, -1}, {0, -PHI, -1},
        {PHI, 1, 0}, {-PHI, 1, 0}, {PHI, -1, 0}, {-PHI, -1, 0},
        {1, 0, PHI}, {-1, 0, PHI}, {1, 0, -PHI}, {-1, 0, -PHI}
    };
    AggiungiVertici(mesh, verticiicosaedro);
}


void AggiungiLati(PoliedriMesh& mesh) {
    unsigned int idLato = 0;

    // Funzione di supporto
    auto aggiungiLato = [&](unsigned int id1, unsigned int id2) {
        mesh.Cell1DsExtrema.conservativeResize(2, idLato + 1);
        mesh.Cell1DsExtrema(0, idLato) = id1;
        mesh.Cell1DsExtrema(1, idLato) = id2;
        mesh.Cell1DsId.push_back(idLato);
        idLato++;
    };

    // === Tetraedro ===
    unsigned int o_tetra = 0;
    vector<pair<unsigned int, unsigned int>> lati_tetra = {
        {0,1},{0,2},{0,3},{1,2},{1,3},{2,3}
    };
    for (auto& l : lati_tetra)
        aggiungiLato(l.first + o_tetra, l.second + o_tetra);

    // === Cubo ===
    unsigned int o_cubo = o_tetra + 4;
    vector<pair<unsigned int, unsigned int>> lati_cubo = {
        {0,1},{1,3},{3,2},{2,0},
        {4,5},{5,7},{7,6},{6,4},
        {0,4},{1,5},{2,6},{3,7}
    };
    for (auto& l : lati_cubo)
        aggiungiLato(l.first + o_cubo, l.second + o_cubo);

    // === Ottaedro ===
    unsigned int o_otta = o_cubo + 8;
    vector<pair<unsigned int, unsigned int>> lati_otta = {
        {0,2},{0,3},{0,4},{0,5},
        {1,2},{1,3},{1,4},{1,5},
        {2,4},{2,5},{3,4},{3,5}
    };
    for (auto& l : lati_otta)
        aggiungiLato(l.first + o_otta, l.second + o_otta);

    // === Dodecaedro ===
    unsigned int o_dode = o_otta + 6;
    vector<pair<unsigned int, unsigned int>> lati_dode = {
        {0,1},{0,4},{0,5},
        {1,7},{1,8},
        {2,3},{2,6},{2,7},
        {3,9},{3,10},
        {4,8},{4,11},
        {5,6},{5,12},
        {6,13},{7,14},{8,15},
        {9,13},{10,14},
        {11,12},{12,15},{13,16},{14,17},
        {15,19},{16,17},{16,18},{17,19},{18,19}
    };
    for (auto& l : lati_dode)
        aggiungiLato(l.first + o_dode, l.second + o_dode);

    // === Icosaedro ===
    unsigned int o_icosa = o_dode + 20;
    vector<pair<unsigned int, unsigned int>> lati_icosa = {
        {0,1},{0,2},{0,4},{0,5},
        {1,3},{1,6},{1,7},
        {2,4},{2,8},{2,10},
        {3,6},{3,9},{3,11},
        {4,8},{4,10},
        {5,7},{5,9},{5,11},
        {6,8},{6,10},
        {7,9},{7,11},
        {8,10},
        {9,11}
    };
    for (auto& l : lati_icosa)
        aggiungiLato(l.first + o_icosa, l.second + o_icosa);

    mesh.NumCell1Ds = idLato;
}

void AggiungiFacce(PoliedriMesh& mesh) {
    using namespace std;

    unsigned int idFaccia = 0;

    // Mappa per cercare ID lato da 2 vertici
    map<pair<unsigned int, unsigned int>, unsigned int> lato_da_vertici;
    for (unsigned int i = 0; i < mesh.NumCell1Ds; ++i) {
        unsigned int v1 = mesh.Cell1DsExtrema(0, i);
        unsigned int v2 = mesh.Cell1DsExtrema(1, i);
        lato_da_vertici[{v1, v2}] = i;
        lato_da_vertici[{v2, v1}] = i;  // bidirezionale
    }

    auto aggiungiFaccia = [&](vector<unsigned int> vertici) {
        mesh.Cell2DsId.push_back(idFaccia++);
        mesh.Cell2DsVertices.push_back(vertici);

        vector<unsigned int> lati;
        for (size_t i = 0; i < vertici.size(); ++i) {
            unsigned int a = vertici[i];
            unsigned int b = vertici[(i + 1) % vertici.size()];
            lati.push_back(lato_da_vertici[{a, b}]);
        }

        mesh.Cell2DsEdges.push_back(lati);
    };

    // === TETRAEDRO ===
    unsigned int o_tetra = 0;
    aggiungiFaccia({o_tetra+0, o_tetra+1, o_tetra+2});
    aggiungiFaccia({o_tetra+0, o_tetra+1, o_tetra+3});
    aggiungiFaccia({o_tetra+0, o_tetra+2, o_tetra+3});
    aggiungiFaccia({o_tetra+1, o_tetra+2, o_tetra+3});

    // === CUBO ===
    unsigned int o_cubo = o_tetra + 4;
    aggiungiFaccia({o_cubo+0, o_cubo+1, o_cubo+3, o_cubo+2});
    aggiungiFaccia({o_cubo+4, o_cubo+5, o_cubo+7, o_cubo+6});
    aggiungiFaccia({o_cubo+0, o_cubo+4, o_cubo+5, o_cubo+1});
    aggiungiFaccia({o_cubo+1, o_cubo+5, o_cubo+7, o_cubo+3});
    aggiungiFaccia({o_cubo+3, o_cubo+7, o_cubo+6, o_cubo+2});
    aggiungiFaccia({o_cubo+2, o_cubo+6, o_cubo+4, o_cubo+0});

    // === OTTAEDRO ===
    unsigned int o_otta = o_cubo + 8;
    aggiungiFaccia({o_otta+0, o_otta+2, o_otta+4});
    aggiungiFaccia({o_otta+0, o_otta+4, o_otta+3});
    aggiungiFaccia({o_otta+0, o_otta+3, o_otta+5});
    aggiungiFaccia({o_otta+0, o_otta+5, o_otta+2});
    aggiungiFaccia({o_otta+1, o_otta+2, o_otta+5});
    aggiungiFaccia({o_otta+1, o_otta+5, o_otta+3});
    aggiungiFaccia({o_otta+1, o_otta+3, o_otta+4});
    aggiungiFaccia({o_otta+1, o_otta+4, o_otta+2});

    // === DODECAEDRO ===
    unsigned int o_dode = o_otta + 6;
    aggiungiFaccia({o_dode+0, o_dode+1, o_dode+9, o_dode+16, o_dode+5});
    aggiungiFaccia({o_dode+1, o_dode+8, o_dode+17, o_dode+9, o_dode+1});
    aggiungiFaccia({o_dode+8, o_dode+10, o_dode+18, o_dode+17, o_dode+8});
    aggiungiFaccia({o_dode+10, o_dode+3, o_dode+19, o_dode+18, o_dode+10});
    aggiungiFaccia({o_dode+3, o_dode+2, o_dode+14, o_dode+19, o_dode+3});
    aggiungiFaccia({o_dode+2, o_dode+11, o_dode+15, o_dode+14, o_dode+2});
    aggiungiFaccia({o_dode+11, o_dode+6, o_dode+12, o_dode+15, o_dode+11});
    aggiungiFaccia({o_dode+6, o_dode+7, o_dode+13, o_dode+12, o_dode+6});
    aggiungiFaccia({o_dode+7, o_dode+4, o_dode+16, o_dode+13, o_dode+7});
    aggiungiFaccia({o_dode+4, o_dode+5, o_dode+0, o_dode+16, o_dode+4});
    aggiungiFaccia({o_dode+5, o_dode+4, o_dode+7, o_dode+6, o_dode+5});
    aggiungiFaccia({o_dode+9, o_dode+17, o_dode+18, o_dode+19, o_dode+14});

    // === ICOSAEDRO ===
    unsigned int o_icosa = o_dode + 20;
    aggiungiFaccia({o_icosa+0, o_icosa+1, o_icosa+2});
    aggiungiFaccia({o_icosa+0, o_icosa+2, o_icosa+3});
    aggiungiFaccia({o_icosa+0, o_icosa+3, o_icosa+4});
    aggiungiFaccia({o_icosa+0, o_icosa+4, o_icosa+5});
    aggiungiFaccia({o_icosa+0, o_icosa+5, o_icosa+1});
    aggiungiFaccia({o_icosa+1, o_icosa+6, o_icosa+2});
    aggiungiFaccia({o_icosa+2, o_icosa+6, o_icosa+7});
    aggiungiFaccia({o_icosa+2, o_icosa+7, o_icosa+3});
    aggiungiFaccia({o_icosa+3, o_icosa+7, o_icosa+8});
    aggiungiFaccia({o_icosa+3, o_icosa+8, o_icosa+4});
    aggiungiFaccia({o_icosa+4, o_icosa+8, o_icosa+9});
    aggiungiFaccia({o_icosa+4, o_icosa+9, o_icosa+5});
    aggiungiFaccia({o_icosa+5, o_icosa+9, o_icosa+6});
    aggiungiFaccia({o_icosa+5, o_icosa+6, o_icosa+1});
    aggiungiFaccia({o_icosa+6, o_icosa+9, o_icosa+8});
    aggiungiFaccia({o_icosa+6, o_icosa+8, o_icosa+7});

    mesh.NumCell2Ds = idFaccia;
}



bool ImportMesh(PoliedriMesh& mesh)
{
	mesh.NumCell0Ds = 0;
    mesh.Cell0DsId.clear();
    mesh.Cell0DsCoordinates.resize(3, 0);

	creaPoliedriNormalizzati(mesh);
	
	AggiungiLati(mesh);
	

	// Importa i vertici 
    if(!Cell0Ds(mesh))
        return false;

	// Importa gli spigoli
    if(!Cell1Ds(mesh))
        return false;
	
	
	// Importa le celle
    if(!Cell2Ds(mesh))
        return false;

    return true;

}


// Scrittura Cell0Ds.txt
bool Cell0Ds(PoliedriMesh& mesh) {
	ofstream file ("./Cell0Ds.txt");
	if (file.fail() ) {
		cerr << "Errore nell'apertura del file Cell0Ds.txt" << endl;
		return false;
	}
	

	for(unsigned int i = 0; i < mesh.NumCell0Ds; i++){
		file << mesh.Cell0DsId[i] << " " 
		<< mesh.Cell0DsCoordinates(0, i) << " " 
		<< mesh.Cell0DsCoordinates(1, i) << " " 
		<< mesh.Cell0DsCoordinates(2, i) << endl;
    	}
	file.close();
	return true;
	}

// Scrittura Cell1Ds.txt
bool Cell1Ds(PoliedriMesh& mesh) {
    ofstream file("Cell1Ds.txt");
    if (file.fail()) {
        cerr << "Errore nell'apertura del file Cell1Ds.txt" << endl;
        return false;
    }

    for (unsigned int i = 0; i < mesh.NumCell1Ds; ++i) {
        file << mesh.Cell1DsId[i] << " "
             << mesh.Cell1DsExtrema(0, i) << " "
             << mesh.Cell1DsExtrema(1, i) << endl;
    }

    file.close();
    return true;

}

// Scrittura Cell2Ds.txt
bool Cell2Ds(PoliedriMesh& mesh) {
    ofstream file("Cell2Ds.txt");
    if (file.fail()) {
        cerr << "Errore nell'apertura del file Cell2Ds.txt" << endl;
        return false;
    }

    for (unsigned int i = 0; i < mesh.NumCell2Ds; ++i) {
        file << mesh.Cell2DsId[i] << " ";

        for (auto v : mesh.Cell2DsVertices[i])
            file << v << " ";
        file << "| ";

        for (auto e : mesh.Cell2DsEdges[i])
            file << e << " ";
        file << endl;
    }

    file.close();
    return true;
}

// Scrittura Cell3Ds.txt
bool Cell3Ds(PoliedriMesh& mesh) {
    ofstream file("Cell3Ds.txt");
    if (file.fail()) {
        cerr << "Errore nell'apertura del file Cell3Ds.txt" << endl;
        return false;
    }

    file << 0 << " "; // ID del poliedro

    // Vertici
    file << mesh.NumCell0Ds << " ";
    for (auto id : mesh.Cell0DsId)
        file << id << " ";

    // Lati
    file << mesh.NumCell1Ds << " ";
    for (auto id : mesh.Cell1DsId)
        file << id << " ";

    // Facce
    file << mesh.NumCell2Ds << " ";
    for (auto id : mesh.Cell2DsId)
        file << id << " ";

    file << endl;
    file.close();
    return true;
}

}