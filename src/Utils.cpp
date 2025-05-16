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

/// Celle0Ds
void normalizza(double& x, double& y, double& z) {
    double norma = sqrt(x * x + y * y + z * z);
    x /= norma;
    y /= norma;
    z /= norma;
}

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
	
/*
	// Importa le celle
    if(!Cell2Ds(mesh))
        return false;
*/
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

    
    /*
    

    // Scrittura Cell2Ds.txt
    std::ofstream file2("./Cell2Ds.txt");
    for (const auto& f : facce) {
        file2 << f.id << " " << f.numero_vertici;
        for (int id_vertice : f.id_vertici) file2 << " " << id_vertice;
        file2 << " " << f.numero_lati;
        for (int id_lato : f.id_lati) file2 << " " << id_lato;
        file2 << "\n";
    }
    file2.close();

    // Scrittura Cell3Ds.txt
    std::ofstream file3("./Cell3Ds.txt");
    file3 << poliedro.id << " " << poliedro.numero_vertici;
    for (int id_vertice : poliedro.id_vertici) file3 << " " << id_vertice;
    file3 << " " << poliedro.numero_lati;
    for (int id_lato : poliedro.id_lati) file3 << " " << id_lato;
    file3 << " " << poliedro.numero_facce;
    for (int id_faccia : poliedro.id_facce) file3 << " " << id_faccia;
    file3 << "\n";
    file3.close();

    std::cout << "Tutti i file Cell0Ds.txt–Cell3Ds.txt sono stati generati.\n";
    return 0;
}*/


}
