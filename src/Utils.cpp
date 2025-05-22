#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <set>

using namespace std;
using namespace Eigen;

namespace PoliedriLibrary {


void normalizza(double& x, double& y, double& z) {
    double norma = sqrt(x * x + y * y + z * z);
    x /= norma;
    y /= norma;
    z /= norma;
} // norm non andava bene perchè non da il risultato desiderato, la funzione che lo fa è normalize ma non l'abbiamo mai utilizzata

// Celle0Ds
void AggiungiVertici(PoliedriMesh& mesh, const vector<Vector3d>& vertici) {
	unsigned int IdOffset = mesh.NumCell0Ds;
	mesh.NumCell0Ds += vertici.size();
    mesh.Cell0DsId.resize(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates.conservativeResize(3, mesh.NumCell0Ds);
    
    // mi prende i poligoni creati in creaPoliedriNormalizzati e me li proietta in una sfera
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
	// mi creo i vertici del poliedro standard
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

	
	const double PHI = (1.0 + sqrt(5.0)) / 2.0; // calcolo sezione aurea, per costruire il dodecaedro e l'icosaedro 
	
	
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


//Celle1Ds
void aggiungiLato(PoliedriMesh& mesh, unsigned int& idLato, unsigned int id1, unsigned int id2) {
    mesh.Cell1DsExtrema.conservativeResize(2, idLato + 1);
    mesh.Cell1DsExtrema(0, idLato) = id1;
    mesh.Cell1DsExtrema(1, idLato) = id2;
    mesh.Cell1DsId.push_back(idLato);
    idLato++;
} 

void AggiungiLati(PoliedriMesh& mesh) {
    unsigned int idLato = 0;
    
    // o_ .... serve tenere traccia di dove iniziano i vertici di ciascun solido, per poterli collegare correttamente con lati e facce
    // Tetraedro
    unsigned int o_tetra = 0; 
    vector<pair<unsigned int, unsigned int>> lati_tetra = {
        {0,1},{0,2},{0,3},{1,2},{1,3},{2,3}
    };
    for (auto& l : lati_tetra)
        aggiungiLato(mesh, idLato, l.first + o_tetra, l.second + o_tetra);


    // Cubo
    unsigned int o_cubo = o_tetra + 4;
    vector<pair<unsigned int, unsigned int>> lati_cubo = {
        {0,1},{1,3},{3,2},{2,0},
        {4,5},{5,7},{7,6},{6,4},
        {0,4},{1,5},{2,6},{3,7}
    };
    for (auto& l : lati_cubo)
        aggiungiLato(mesh, idLato,l.first + o_cubo, l.second + o_cubo);

    // Ottaedro
    unsigned int o_otta = o_cubo + 8;
    vector<pair<unsigned int, unsigned int>> lati_otta = {
        {0,2},{0,3},{0,4},{0,5},
        {1,2},{1,3},{1,4},{1,5},
        {2,4},{2,5},{3,4},{3,5}
    };
    for (auto& l : lati_otta)
        aggiungiLato(mesh, idLato, l.first + o_otta, l.second + o_otta);

    // Dodecaedro
    unsigned int o_dode = o_otta + 6;
	vector<pair<unsigned int, unsigned int>> lati_dode = {
		{0, 1}, {0, 4}, {0, 7},
		{1, 2}, {1, 9},
		{2, 3}, {2, 11},
		{3, 4}, {3, 13},
		{4, 5},
		{5, 6}, {5, 14},
		{6, 7}, {6, 16},
		{7, 8},
		{8, 9}, {8, 17},
		{9, 10},
		{10, 11}, {10, 18},
		{11, 12},
		{12, 13}, {12, 19},
		{13, 14},
		{14, 15}, {15, 16},
		{16, 17},
		{17, 18},
		{18, 19},
		{19, 15}
	};
    for (auto& l : lati_dode)
        aggiungiLato(mesh, idLato, l.first + o_dode, l.second + o_dode);

    // Icosaedro
	unsigned int o_icosa = o_dode + 20;
	vector<pair<unsigned int, unsigned int>> lati_icosa = {
		{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
		{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 1},
		{1, 6}, {1, 7}, {2, 7}, {2, 8}, 
		{3, 8}, {3, 9}, {4, 9}, {4, 10},
		{5, 10}, {5, 6}, {6, 10}, {6, 7}, {7, 8}, {8, 9}, {9,10},
		{11, 6}, {11, 7}, {11, 8}, {11, 9}, {11, 10}	
	};
	
	for (auto& l : lati_icosa)
		aggiungiLato(mesh, idLato, l.first + o_icosa, l.second + o_icosa);

    mesh.NumCell1Ds = idLato;
}

	void aggiungiFacce(PoliedriMesh& mesh) {
		unsigned int idFaccia = 0;
	
		struct FacciaInfo {
			vector<unsigned int> lati;
		};
	
		vector<FacciaInfo> facce_tetra = {
			{{0, 1, 3}},  // Faccia 0
			{{1, 2, 5}},  // Faccia 1
			{{0, 4, 2}},  // Faccia 2
			{{3, 4, 5}}   // Faccia 3
		};
		
		vector<FacciaInfo> facce_cubo = { 
			{{6, 7, 8, 9}},
			{{10, 11, 12, 13}}, 
			{{6, 14, 10, 15}},  
			{{7, 17, 11, 15}},  
			{{8, 16, 12, 17}},  
			{{9, 16, 13, 14}}
		};
		
		vector<FacciaInfo> facce_otta = {
			{{18, 26, 20}}, 
			{{18, 21, 27}}, 
			{{19, 29, 21}}, 
			{{22, 24, 26}}, 
			{{22, 25, 27}},
			{{23, 28, 24}}, 
			{{23, 29, 25}},
			{{28, 20, 19}}
		};
	
		vector<FacciaInfo> tutteLeFacce;
		tutteLeFacce.insert(tutteLeFacce.end(), facce_tetra.begin(), facce_tetra.end());
		tutteLeFacce.insert(tutteLeFacce.end(), facce_cubo.begin(), facce_cubo.end());
		tutteLeFacce.insert(tutteLeFacce.end(), facce_otta.begin(), facce_otta.end());
		
		for (const auto& faccia : tutteLeFacce) {

			vector<unsigned int> originalEdges = faccia.lati;
			vector<pair<unsigned int, pair<unsigned int, unsigned int>>> edges;
	
			// Prepara i lati come (id, (origin, end))
			for (auto eid : originalEdges) {
				unsigned int a = mesh.Cell1DsExtrema(0, eid);
				unsigned int b = mesh.Cell1DsExtrema(1, eid);
				edges.emplace_back(eid, make_pair(a, b));
			}
	
			vector<unsigned int> orderedEdges;
			vector<unsigned int> orderedVertices;
	
			// Partenza dal primo lato
			auto [id0, ab0] = edges[0];
			orderedEdges.push_back(id0);
			orderedVertices.push_back(ab0.first);
			orderedVertices.push_back(ab0.second);
	
			// Rimuovi il primo dalla lista dei lati rimanenti
			edges.erase(edges.begin());
	
			while (!edges.empty()) {
				bool found = false;
				for (auto it = edges.begin(); it != edges.end(); ++it) {
					auto [eid, ab] = *it;
					if (ab.first == orderedVertices.back()) {
						orderedEdges.push_back(eid);
						orderedVertices.push_back(ab.second);
						edges.erase(it);
						found = true;
						break;
					} else if (ab.second == orderedVertices.back()) {
						orderedEdges.push_back(eid);
						orderedVertices.push_back(ab.first);
						edges.erase(it);
						found = true;
						break;
					}
				}
				if (!found) {
					cerr << "Errore nell'ordinamento dei lati per la faccia " << idFaccia << endl;
					return;
				}
			}
	
			// Chiude il ciclo rimuovendo l’ultimo vertice se è uguale al primo
			if (orderedVertices.front() == orderedVertices.back())
				orderedVertices.pop_back();
	
			mesh.Cell2DsId.push_back(idFaccia);
			mesh.Cell2DsEdges.push_back(orderedEdges);
			mesh.Cell2DsVertices.push_back(orderedVertices);
			idFaccia++;
		}
	
		mesh.NumCell2Ds = idFaccia;
	}


	bool ImportMesh(PoliedriMesh& mesh)
	{
		mesh.NumCell0Ds = 0;
		mesh.Cell0DsId.clear();
		mesh.Cell0DsCoordinates.resize(3, 0);
	
		creaPoliedriNormalizzati(mesh);
		
		AggiungiLati(mesh);
		
		aggiungiFacce(mesh);
		
	
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