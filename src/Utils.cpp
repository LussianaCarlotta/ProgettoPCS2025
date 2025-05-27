#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <Eigen/Dense>
#include <vector>
#include <cmath>
#include <set>

using namespace std;
using namespace Eigen;

namespace PoliedriLibrary {

/// TETRAEDRO
void CreaTetraedro(PoliedriMesh& mesh)
{
    //Celle0D
    vector<Vector3d> vertici = {
        {1.0, 1.0, 1.0}, 
        {-1.0, -1.0, 1.0}, 
        {-1.0, 1.0, -1.0}, 
        {1.0, -1.0, -1.0}
    };

    for (auto& v : vertici)
        v.normalize();

    mesh.Cell0DsId.clear();
    mesh.Cell0DsCoordinates.resize(3, vertici.size());

    for (unsigned int i = 0; i < vertici.size(); ++i) {
        mesh.Cell0DsId.push_back(i);
        mesh.Cell0DsCoordinates.col(i) = vertici[i];
    }
    mesh.NumCell0Ds = vertici.size();


    //Celle 1D
    vector<pair<unsigned int, unsigned int>> lati = {
        {0,1},{0,2},{0,3},{1,2},{1,3},{2,3}
    };

    mesh.Cell1DsId.clear();
    mesh.Cell1DsExtrema.resize(2, lati.size());

    for (unsigned int i = 0; i < lati.size(); ++i) {
        mesh.Cell1DsId.push_back(i);
        mesh.Cell1DsExtrema(0, i) = lati[i].first;
        mesh.Cell1DsExtrema(1, i) = lati[i].second;
    }
    mesh.NumCell1Ds = lati.size();


    //Celle2D
    vector<vector<unsigned int>> facce = {
        {0, 1, 3},
        {1, 5, 2},
        {0, 4, 2},
        {3, 4, 5}
    };

    mesh.Cell2DsId.clear();
    mesh.Cell2DsVertices.clear();
    mesh.Cell2DsEdges.clear();
    mesh.Cell2DsEdges.resize(mesh.NumCell2Ds);
	mesh.Cell2DsNumVertices.resize(mesh.NumCell2Ds);
	mesh.Cell2DsNumEdges.resize(mesh.NumCell2Ds);


    unsigned int idFaccia = 0;
	for (const auto& f : facce) {
		vector<unsigned int> originalEdges = f;
		vector<pair<unsigned int, pair<unsigned int, unsigned int>>> edges;

		// Prepara i lati come (id, (a, b))
		for (auto i : originalEdges) {
			unsigned int a = mesh.Cell1DsExtrema(0, i);
			unsigned int b = mesh.Cell1DsExtrema(1, i);
			edges.emplace_back(i, make_pair(a, b));
		}

		vector<unsigned int> orderedEdges;
		vector<unsigned int> orderedVertices;

		// Inizia dal primo lato
		auto [id0, ab0] = edges[0];
		orderedEdges.push_back(id0);
		orderedVertices.push_back(ab0.first);
		orderedVertices.push_back(ab0.second);
		edges.erase(edges.begin());

		while (!edges.empty()) {
			bool found = false;
			for (auto it = edges.begin(); it != edges.end(); ++it) {
				auto [i, ab] = *it;
				if (ab.first == orderedVertices.back()) {
					orderedEdges.push_back(i);
					orderedVertices.push_back(ab.second);
					edges.erase(it);
					found = true;
					break;
				} else if (ab.second == orderedVertices.back()) {
					orderedEdges.push_back(i);
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

		if (orderedVertices.front() == orderedVertices.back())
			orderedVertices.pop_back();

		mesh.Cell2DsId.push_back(idFaccia);
		mesh.Cell2DsEdges.push_back(orderedEdges);
		mesh.Cell2DsVertices.push_back(orderedVertices);
		idFaccia++;
	}

	mesh.NumCell2Ds = idFaccia;
	


    //Celle3D
    mesh.Cell3DsId.clear();
    mesh.Cell3DsVertices.clear();
    mesh.Cell3DsEdges.clear();
    mesh.Cell3DsFaces.clear();

    mesh.Cell3DsId.push_back(0);
    mesh.Cell3DsVertices.push_back(mesh.Cell0DsId);
    mesh.Cell3DsEdges.push_back(mesh.Cell1DsId);
    mesh.Cell3DsFaces.push_back(mesh.Cell2DsId);

    mesh.NumCell3Ds = 1;
}






//********************************************************************************	
///CUBO
void CreaCubo(PoliedriMesh& mesh) {
	
	vector<Vector3d> vertici = {
		{-1.0, -1.0, -1.0}, {-1.0, -1.0,  1.0}, 
		{-1.0,  1.0, -1.0}, {-1.0,  1.0,  1.0},
		{ 1.0, -1.0, -1.0}, { 1.0, -1.0,  1.0}, 
		{ 1.0,  1.0, -1.0}, { 1.0,  1.0,  1.0}
	};

	for (auto& vertice : vertici) {
		vertice.normalize();
	}
	
	//Celle0Ds
	mesh.Cell0DsId.clear();
	mesh.Cell0DsCoordinates.resize(3, 0);
    mesh.Cell0DsCoordinates.resize(3, vertici.size());
	
	for (unsigned int i = 0; i < vertici.size(); ++i) {
		mesh.Cell0DsId.push_back(i);
		mesh.Cell0DsCoordinates.col(i) = vertici[i];
	}
	
	mesh.NumCell0Ds = vertici.size();
	
	
	//Celle1DS
	vector<pair<unsigned int, unsigned int>> lati = {
		{0,1},{1,3},{3,2},{2,0},
		{4,5},{5,7},{7,6},{6,4},
		{0,4},{1,5},{3,7},{2,6}
	};
	
	mesh.Cell1DsId.clear();
	mesh.Cell1DsExtrema.resize(2, 0);
    mesh.Cell1DsExtrema.resize(2, lati.size());
	
	for (unsigned int i = 0; i < lati.size(); ++i) {
		mesh.Cell1DsId.push_back(i);
		mesh.Cell1DsExtrema(0, i) = lati[i].first;
		mesh.Cell1DsExtrema(1, i) = lati[i].second;
	}
	mesh.NumCell1Ds = lati.size();
	
	
	//Celle2Ds
	vector<vector<unsigned int>> facce = {
		{0, 1, 2, 3},
		{4, 5, 6, 7},
		{0, 9, 4, 8},
		{1, 10, 5, 9},
		{2, 10, 6, 11}, 
		{11, 7, 8, 3}
	};
	
	mesh.Cell2DsId.clear();
    mesh.Cell2DsVertices.clear();
    mesh.Cell2DsEdges.clear();
    mesh.Cell2DsEdges.resize(mesh.NumCell2Ds);
	mesh.Cell2DsNumVertices.resize(mesh.NumCell2Ds);
	mesh.Cell2DsNumEdges.resize(mesh.NumCell2Ds);
	
	
	
	unsigned int idFaccia = 0;
	for (const auto& f : facce) {
		vector<unsigned int> originalEdges = f;
		vector<pair<unsigned int, pair<unsigned int, unsigned int>>> edges;

		// Prepara i lati come (id, (a, b))
		for (auto i : originalEdges) {
			unsigned int a = mesh.Cell1DsExtrema(0, i);
			unsigned int b = mesh.Cell1DsExtrema(1, i);
			edges.emplace_back(i, make_pair(a, b));
		}

		vector<unsigned int> orderedEdges;
		vector<unsigned int> orderedVertices;

		// Inizia dal primo lato
		auto [id0, ab0] = edges[0];
		orderedEdges.push_back(id0);
		orderedVertices.push_back(ab0.first);
		orderedVertices.push_back(ab0.second);
		edges.erase(edges.begin());

		while (!edges.empty()) {
			bool found = false;
			for (auto it = edges.begin(); it != edges.end(); ++it) {
				auto [i, ab] = *it;
				if (ab.first == orderedVertices.back()) {
					orderedEdges.push_back(i);
					orderedVertices.push_back(ab.second);
					edges.erase(it);
					found = true;
					break;
				} else if (ab.second == orderedVertices.back()) {
					orderedEdges.push_back(i);
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

		if (orderedVertices.front() == orderedVertices.back())
			orderedVertices.pop_back();

		mesh.Cell2DsId.push_back(idFaccia);
		mesh.Cell2DsEdges.push_back(orderedEdges);
		mesh.Cell2DsVertices.push_back(orderedVertices);
		idFaccia++;
	}

	mesh.NumCell2Ds = idFaccia;
	
	//Celle3Ds
	mesh.Cell3DsId.clear();
	mesh.Cell3DsVertices.clear();
	mesh.Cell3DsEdges.clear();
	mesh.Cell3DsFaces.clear();
	
	mesh.Cell3DsId.push_back(0);
	
	mesh.Cell3DsVertices.push_back(mesh.Cell0DsId);
	mesh.Cell3DsEdges.push_back(mesh.Cell1DsId);
	mesh.Cell3DsFaces.push_back(mesh.Cell2DsId);
	
	mesh.NumCell3Ds = 1;

}

//*****************************************************************************
///OTTAEDRO
void CreaOttaedro(PoliedriMesh& mesh) {
	
	vector<Vector3d> vertici = {
	{ 1.0,  0.0,  0.0}, {-1.0,  0.0,  0.0}, { 0.0,  1.0,  0.0},
	{ 0.0, -1.0,  0.0}, { 0.0,  0.0,  1.0}, { 0.0,  0.0, -1.0}
	};

	for (auto& vertice : vertici) {
		vertice.normalize();
	}

	//Celle0Ds
	mesh.Cell0DsId.resize(0);
	mesh.Cell0DsCoordinates.resize(0, 0);
	mesh.Cell0DsId.reserve(vertici.size());
	mesh.Cell0DsCoordinates.resize(3, vertici.size());

	for (unsigned int i = 0; i < vertici.size(); ++i) {
		mesh.Cell0DsId.push_back(i);
		mesh.Cell0DsCoordinates.col(i) = vertici[i];
	}

	mesh.NumCell0Ds = vertici.size();
	
	//Celle1Ds
    vector<pair<unsigned int, unsigned int>> lati = {
		{0,2},{0,3},{0,4},{0,5},
        {1,2},{1,3},{1,4},{1,5},
        {2,4},{2,5},{3,5},{3,4}
    };
	
	mesh.Cell1DsId.resize(0);
	mesh.Cell1DsExtrema.resize(0, 0);
	
	mesh.Cell1DsExtrema.resize(2, lati.size());
	mesh.Cell1DsId.reserve(lati.size());
	
	for (unsigned int i = 0; i < lati.size(); ++i) {
		mesh.Cell1DsId.push_back(i);
		mesh.Cell1DsExtrema(0, i) = lati[i].first;
		mesh.Cell1DsExtrema(1, i) = lati[i].second;
	}
	mesh.NumCell1Ds = lati.size();
	
	//Celle2Ds
	vector<vector<unsigned int>> facce = {
		{0, 8, 2}, 
		{0, 3, 9}, 
		{1, 10, 3}, 
		{4, 6, 8}, 
		{4, 7, 9},
		{5, 11, 6}, 
		{5, 10, 7},
		{1, 2, 11}
	};
	
	mesh.Cell2DsId.clear();
	mesh.Cell2DsVertices.clear();
	mesh.Cell2DsEdges.clear();
	mesh.Cell2DsEdges.resize(mesh.NumCell2Ds);
	mesh.Cell2DsNumVertices.resize(mesh.NumCell2Ds);
	mesh.Cell2DsNumEdges.resize(mesh.NumCell2Ds);
	
	
	unsigned int idFaccia = 0;
	for (const auto& f : facce) {
		vector<unsigned int> originalEdges = f;
		vector<pair<unsigned int, pair<unsigned int, unsigned int>>> edges;

		// Prepara i lati come (id, (a, b))
		for (auto i : originalEdges) {
			unsigned int a = mesh.Cell1DsExtrema(0, i);
			unsigned int b = mesh.Cell1DsExtrema(1, i);
			edges.emplace_back(i, make_pair(a, b));
		}

		vector<unsigned int> orderedEdges;
		vector<unsigned int> orderedVertices;

		// Inizia dal primo lato
		auto [id0, ab0] = edges[0];
		orderedEdges.push_back(id0);
		orderedVertices.push_back(ab0.first);
		orderedVertices.push_back(ab0.second);
		edges.erase(edges.begin());

		while (!edges.empty()) {
			bool found = false;
			for (auto it = edges.begin(); it != edges.end(); ++it) {
				auto [i, ab] = *it;
				if (ab.first == orderedVertices.back()) {
					orderedEdges.push_back(i);
					orderedVertices.push_back(ab.second);
					edges.erase(it);
					found = true;
					break;
				} else if (ab.second == orderedVertices.back()) {
					orderedEdges.push_back(i);
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

		if (orderedVertices.front() == orderedVertices.back())
			orderedVertices.pop_back();

		mesh.Cell2DsId.push_back(idFaccia);
		mesh.Cell2DsEdges.push_back(orderedEdges);
		mesh.Cell2DsVertices.push_back(orderedVertices);
		idFaccia++;
	}

	mesh.NumCell2Ds = idFaccia;
	
	//Celle3Ds
	mesh.Cell3DsId.clear();
	mesh.Cell3DsVertices.clear();
	mesh.Cell3DsEdges.clear();
	mesh.Cell3DsFaces.clear();
	
	mesh.Cell3DsId.push_back(0);
	
	mesh.Cell3DsVertices.push_back(mesh.Cell0DsId);
	mesh.Cell3DsEdges.push_back(mesh.Cell1DsId);
	mesh.Cell3DsFaces.push_back(mesh.Cell2DsId);
	
	mesh.NumCell3Ds = 1;
}

//*****************************************************************************
///DODECAEDRO
void CreaDodecaedro(PoliedriMesh& mesh) {

	const double phi = (1.0 + sqrt(5.0)) / 2.0; // calcolo sezione aurea,
	
	vector<Vector3d> vertici = {
        {0, 1 / phi, phi}, {0, -1 / phi, phi}, {0, 1 / phi, -phi}, {0, -1 / phi, -phi},
        {1 / phi, phi, 0}, {-1 / phi, phi, 0}, {1 / phi, -phi, 0}, {-1 / phi, -phi, 0},
        {phi, 0, 1 / phi}, {-phi, 0, 1 / phi}, {phi, 0, -1 / phi}, {-phi, 0, -1 / phi},
        {1, 1, 1}, {-1, 1, 1}, {1, -1, 1}, {-1, -1, 1}, 
		{1, 1, -1}, {-1, 1, -1}, {1, -1, -1}, {-1, -1, -1}
    };
	
	for (auto& vertice : vertici) {
		vertice.normalize();
	}

	//Celle0Ds
	mesh.Cell0DsId.resize(0);
	mesh.Cell0DsCoordinates.resize(0, 0);
	mesh.Cell0DsId.reserve(vertici.size());
	mesh.Cell0DsCoordinates.resize(3, vertici.size());

	for (unsigned int i = 0; i < vertici.size(); ++i) {
		mesh.Cell0DsId.push_back(i);
		mesh.Cell0DsCoordinates.col(i) = vertici[i];
	}

	mesh.NumCell0Ds = vertici.size();
	
	
	//Celle1DS
	vector<pair<unsigned int, unsigned int>> lati = {
		{0, 1}, {0, 4}, {0, 7}, {1, 2}, {1, 9},
		{2, 3}, {2, 11}, {3, 4}, {3, 13}, {4, 5},
		{5, 6}, {5, 14}, {6, 7}, {6, 16}, {7, 8},
		{8, 9}, {8, 17}, {9, 10}, {10, 11}, {10, 18},
		{11, 12}, {12, 13}, {12, 19}, {13, 14}, {14, 15}, 
		{15, 16}, {16, 17}, {17, 18}, {18, 19}, {19, 15}
	};
	
	mesh.Cell1DsId.resize(0);
	mesh.Cell1DsExtrema.resize(0, 0);
	
	mesh.Cell1DsExtrema.resize(2, lati.size());
	mesh.Cell1DsId.reserve(lati.size());
	
	for (unsigned int i = 0; i < lati.size(); ++i) {
		mesh.Cell1DsId.push_back(i);
		mesh.Cell1DsExtrema(0, i) = lati[i].first;
		mesh.Cell1DsExtrema(1, i) = lati[i].second;
	}
	mesh.NumCell1Ds = lati.size();
	
	
	//Celle2Ds
	vector<vector<unsigned int>> facce = {
		{0, 3, 5, 7, 1}, 
		{0, 4, 15, 14, 2},
		{1, 9, 10, 12, 2},
		{3, 4, 17, 18, 6},
		{5, 6, 20, 21, 8},
		{7, 8, 23, 11, 9},
		// facce dietro
		{25, 13, 10, 11, 24},
		{26, 16, 14, 12, 13},
		{27, 19, 17, 15, 16},
		{28, 22, 20, 18, 19},
		{29, 24, 23, 21, 22},
		{25, 26, 27, 28, 29},	
	};
	
	mesh.Cell2DsId.clear();
    mesh.Cell2DsVertices.clear();
    mesh.Cell2DsEdges.clear();
    mesh.Cell2DsEdges.resize(mesh.NumCell2Ds);
	mesh.Cell2DsNumVertices.resize(mesh.NumCell2Ds);
	mesh.Cell2DsNumEdges.resize(mesh.NumCell2Ds);
	
		unsigned int idFaccia = 0;

		for (const auto& edgeIDs : facce) {
		std::vector<unsigned int> orderedVertices;
		std::vector<unsigned int> orderedEdges = edgeIDs;

		// Mappa: edge ID → (from, to)
		std::vector<std::pair<unsigned int, unsigned int>> edges;
		for (unsigned int eid : edgeIDs) {
			unsigned int from = mesh.Cell1DsExtrema(0, eid);
			unsigned int to   = mesh.Cell1DsExtrema(1, eid);
			edges.emplace_back(from, to);
		}

		// Inizializza con il primo lato
		unsigned int current = edges[0].second;
		orderedVertices.push_back(edges[0].first);
		orderedVertices.push_back(current);
		edges.erase(edges.begin());

		// Ricostruisci cammino
		while (!edges.empty()) {
			bool found = false;
			for (auto it = edges.begin(); it != edges.end(); ++it) {
				auto [from, to] = *it;
				if (from == current) {
					orderedVertices.push_back(to);
					current = to;
					edges.erase(it);
					found = true;
					break;
				} else if (to == current) {
					orderedVertices.push_back(from);
					current = from;
					edges.erase(it);
					found = true;
					break;
				}
			}
			if (!found) {
				std::cerr << "Errore: non riesco a chiudere la faccia " << idFaccia << std::endl;
				return;
			}
		}

		// Verifica che la faccia sia chiusa
		if (orderedVertices.front() != orderedVertices.back()) {
			std::cerr << "Errore: faccia " << idFaccia << " non chiusa correttamente\n";
			return;
		}

		// Rimuovi l'ultimo duplicato se chiusura esplicita
		orderedVertices.pop_back();

		// Salva nella mesh
		mesh.Cell2DsId.push_back(idFaccia);
		mesh.Cell2DsEdges.push_back(orderedEdges);
		mesh.Cell2DsVertices.push_back(orderedVertices);
		++idFaccia;
		}

		mesh.NumCell2Ds = idFaccia;
	
	//Celle3Ds
	mesh.Cell3DsId.clear();
	mesh.Cell3DsVertices.clear();
	mesh.Cell3DsEdges.clear();
	mesh.Cell3DsFaces.clear();
	
	mesh.Cell3DsId.push_back(0);
	
	mesh.Cell3DsVertices.push_back(mesh.Cell0DsId);
	mesh.Cell3DsEdges.push_back(mesh.Cell1DsId);
	mesh.Cell3DsFaces.push_back(mesh.Cell2DsId);
	
	mesh.NumCell3Ds = 1;
}

//**********************************************************************************

///ICOSAEDRO
void CreaIcosaedro(PoliedriMesh& mesh){
	
	const double phi = (1.0 + sqrt(5.0)) / 2.0; // calcolo sezione aurea,
	
	vector<Vector3d> vertici = {
        {0, phi, 1}, {0, -phi, 1}, {0, phi, -1}, {0, -phi, -1},
        {phi, 1, 0}, {-phi, 1, 0}, {phi, -1, 0}, {-phi, -1, 0},
        {1, 0, phi}, {-1, 0, phi}, {1, 0, -phi}, {-1, 0, -phi}
    };
	
	for (auto& vertice : vertici) {
		vertice.normalize();
	}
	
	//Celle0Ds
	mesh.Cell0DsId.resize(0);
	mesh.Cell0DsCoordinates.resize(0, 0);
	mesh.Cell0DsId.reserve(vertici.size());
	mesh.Cell0DsCoordinates.resize(3, vertici.size()); //dimensione 3xN vertici
	
	
	for (unsigned int i = 0; i < vertici.size(); ++i){
		mesh.Cell0DsId.push_back(i);
		mesh.Cell0DsCoordinates.col(i) = vertici[i];
		
	}
	mesh.NumCell0Ds = vertici.size();
	
	
	//Celle1Ds
	vector<pair<unsigned int, unsigned int>> lati = {
		{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
		{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 1},
		{1, 6}, {1, 7}, {2, 7}, {2, 8}, {3, 8}, 
		{3, 9}, {4, 9}, {4, 10},{5, 10}, {5, 6}, 
		{6, 10}, {6, 7}, {7, 8}, {8, 9}, {9,10},
		{11, 6}, {11, 7}, {11, 8}, {11, 9}, {11, 10}	
	};
	
	mesh.Cell1DsId.resize(0);
	mesh.Cell1DsExtrema.resize(0, 0);
	
	mesh.Cell1DsExtrema.resize(2, lati.size());
	mesh.Cell1DsId.reserve(lati.size());
	
	for (unsigned int i = 0; i < lati.size(); ++i) {
		mesh.Cell1DsId.push_back(i);
		mesh.Cell1DsExtrema(0, i) = lati[i].first;
		mesh.Cell1DsExtrema(1, i) = lati[i].second;
	}
	mesh.NumCell1Ds = lati.size();
	
	
	//Celle2Ds
	vector<vector<unsigned int>> facce= {
		{0, 5, 1}, 
		{0, 9, 4},
		{1, 6, 2},
		{2, 7, 3},
		{3, 8, 4},
			
		{5, 11, 12},
		{12, 22, 13},
		{6, 13, 14},
		{14, 23, 15},
		{15, 16, 7},
			
		// facce dietro
		{25, 26, 21},
		{25, 29, 20},
		{26, 22, 27},
		{27, 23, 28},
		{28, 24, 29},
			
		{21, 10, 11},
		{10, 19, 9},
		{19, 20, 18},
		{18, 17, 8},
		{17, 24, 16},
			
	};
	
	mesh.Cell2DsId.clear();
    mesh.Cell2DsVertices.clear();
    mesh.Cell2DsEdges.clear();
    mesh.Cell2DsEdges.resize(mesh.NumCell2Ds);
	mesh.Cell2DsNumVertices.resize(mesh.NumCell2Ds);
	mesh.Cell2DsNumEdges.resize(mesh.NumCell2Ds);
	
	unsigned int idFaccia = 0;
	for (const auto& f : facce) {
		vector<unsigned int> originalEdges = f;
		vector<pair<unsigned int, pair<unsigned int, unsigned int>>> edges;

		// Prepara i lati come (id, (a, b))
		for (auto i : originalEdges) {
			unsigned int a = mesh.Cell1DsExtrema(0, i);
			unsigned int b = mesh.Cell1DsExtrema(1, i);
			edges.emplace_back(i, make_pair(a, b));
		}

		vector<unsigned int> orderedEdges;
		vector<unsigned int> orderedVertices;

		// Inizia dal primo lato
		auto [id0, ab0] = edges[0];
		orderedEdges.push_back(id0);
		orderedVertices.push_back(ab0.first);
		orderedVertices.push_back(ab0.second);
		edges.erase(edges.begin());

		while (!edges.empty()) {
			bool found = false;
			for (auto it = edges.begin(); it != edges.end(); ++it) {
				auto [i, ab] = *it;
				if (ab.first == orderedVertices.back()) {
					orderedEdges.push_back(i);
					orderedVertices.push_back(ab.second);
					edges.erase(it);
					found = true;
					break;
				} else if (ab.second == orderedVertices.back()) {
					orderedEdges.push_back(i);
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

		if (orderedVertices.front() == orderedVertices.back())
			orderedVertices.pop_back();

		mesh.Cell2DsId.push_back(idFaccia);
		mesh.Cell2DsEdges.push_back(orderedEdges);
		mesh.Cell2DsVertices.push_back(orderedVertices);
		idFaccia++;
	}

	mesh.NumCell2Ds = idFaccia;
	
	
	//Celle3Ds
	mesh.Cell3DsId.clear();
	mesh.Cell3DsVertices.clear();
	mesh.Cell3DsEdges.clear();
	mesh.Cell3DsFaces.clear();
	
	mesh.Cell3DsId.push_back(0);
	
	mesh.Cell3DsVertices.push_back(mesh.Cell0DsId);
	mesh.Cell3DsEdges.push_back(mesh.Cell1DsId);
	mesh.Cell3DsFaces.push_back(mesh.Cell2DsId);
	
	mesh.NumCell3Ds = 1;
}



// Scrittura Celle.txt
bool ScritturaCelle(PoliedriMesh& mesh, unsigned int p, unsigned int q) {
	
	if (p == 3 && q == 3) {
		CreaTetraedro(mesh);
	}
	else if (p == 4 && q == 3) {
        CreaCubo(mesh);
    }
    else if (p == 3 && q == 4) {
		CreaOttaedro(mesh);
	}
	else if (p == 5 && q == 3) {
        CreaDodecaedro(mesh);
    }
	else if (p == 3 && q == 5) {
		CreaIcosaedro(mesh);
	}
    else {
        std::cerr << "Errore: combinazione (p=" << p << ", q=" << q << ") non supportata" << endl;
        return false;
    }
	
	//Scrittura Celle0Ds
	ofstream file ("./Cell0Ds.txt");
	if (file.fail() ) {
		cerr << "Errore nell'apertura del file Cell0Ds.txt" << endl;
		return false;
	}
	
    file << "# ID x y z" << endl;
    for (unsigned int i = 0; i < mesh.NumCell0Ds; ++i) {
        unsigned int Id=  mesh.Cell0DsId[i];
        double X = mesh.Cell0DsCoordinates(0, i);
        double Y = mesh.Cell0DsCoordinates(1, i);
        double Z = mesh.Cell0DsCoordinates(2, i);
		
		file << Id << " " << X << " " << Y << " " << Z << endl;
    }
	
	file.close();
	
	
	//Scrittura Celle1Ds
	ofstream file1("Cell1Ds.txt");
    if (file1.fail()) {
        cerr << "Errore nell'apertura file Cell1Ds.txt" << endl;
        return false;
    }

    file1 << "# ID OriginId EndId" << endl;
    for (unsigned int i = 0; i < mesh.NumCell1Ds; ++i) {
		
        unsigned int id = mesh.Cell1DsId[i];
        unsigned int origin = mesh.Cell1DsExtrema(0, i);
        unsigned int end = mesh.Cell1DsExtrema(1, i);

        file1 << id << " " << origin << " " << end << "\n";
    }

    file1.close();

	
	
	// Scrittura Cell2Ds.txt
	ofstream file2("Cell2Ds.txt");
	if (file2.fail()) {
		cerr << "Errore nell'apertura del file Cell2Ds.txt" << endl;
		return false;
	}
	
	file2 << "# ID NumVertices NumEdges VertexIds EdgeIds" << endl;
	for (unsigned int i = 0; i < mesh.NumCell2Ds; ++i) {
		unsigned int id = mesh.Cell2DsId[i];
        const vector<unsigned int>& vertici = mesh.Cell2DsVertices[i];
        const vector<unsigned int>& lati = mesh.Cell2DsEdges[i];
		
		file2 << id << " " << vertici.size() << " " << lati.size();
		
		for (unsigned int v : vertici)
            file2 << " " << v;
        for (unsigned int e : lati)
            file2 << " " << e;
		
		file2 << endl;
		}
	
	file2.close();
		
	
	// Scrittura Cell3Ds.txt
	ofstream file3("Cell3Ds.txt");
	if (file3.fail()) {
		cerr << "Errore nell'apertura del file Cell3Ds.txt" << endl;
		return false;
	}
	
	file3 << "# ID NumVertices NumEdges NumFaces VertexIds EdgeIds FaceIds" << endl;

    for (unsigned int i = 0; i < mesh.NumCell3Ds; ++i) {
        unsigned int id = mesh.Cell3DsId[i];
        const vector<unsigned int>& vertici = mesh.Cell3DsVertices[i];
        const vector<unsigned int>& lati = mesh.Cell3DsEdges[i];
        const vector<unsigned int>& facce = mesh.Cell3DsFaces[i];

        file3 << id << " " << vertici.size() << " " << lati.size() << " " << facce.size();

        for (unsigned int v : vertici)
            file3 << " " << v;
        for (unsigned int e : lati)
            file3 << " " << e;
        for (unsigned int f : facce)
            file3 << " " << f;

        file3 << endl;
    }
	
	file3.close();
	return true;
	
	}

bool ImportMesh(PoliedriMesh& mesh, unsigned int p, unsigned int q)
{
	if (p == 3 && q == 3)
		CreaTetraedro(mesh);
	else if (p == 4 && q == 3)
		CreaCubo(mesh);
	else if (p == 3 && q == 4)
		CreaOttaedro(mesh);
	else if (p == 5 && q == 3)
		CreaDodecaedro(mesh);
	else if (p == 3 && q == 5)
		CreaIcosaedro(mesh);
	else
	{
		cerr << "Poliedro {p=" << p << ", q=" << q << "} non supportato" << endl;
		return false;
	}

	return true;
}
/*
void CostruisciDualMesh(const PoliedriMesh& meshOriginale, PoliedriMesh& meshDuale) {
    meshDuale = {};

    // Calcolo dei baricentri delle facce
    std::vector<Vector3d> baricentri;
    for (size_t i = 0; i < meshOriginale.NumCell2Ds; ++i) {
        const auto& verticiFaccia = meshOriginale.Cell2DsVertices[i];
        Vector3d centro = Vector3d::Zero();
        for (auto vid : verticiFaccia) {
            centro += meshOriginale.Cell0DsCoordinates.col(vid);
        }
        centro /= verticiFaccia.size();
        centro.normalize();
        baricentri.push_back(centro);
    }

    // Inserimento baricentri come nuovi vertici
    meshDuale.NumCell0Ds = baricentri.size();
    meshDuale.Cell0DsCoordinates.resize(3, baricentri.size());
    meshDuale.Cell0DsId.resize(baricentri.size());
    for (unsigned int i = 0; i < baricentri.size(); ++i) {
        meshDuale.Cell0DsId[i] = i;
        meshDuale.Cell0DsCoordinates.col(i) = baricentri[i];
    }

    // Mappa da vertici a facce che lo contengono
    std::map<unsigned int, std::vector<unsigned int>> verticeToFacce;
    for (unsigned int i = 0; i < meshOriginale.NumCell2Ds; ++i) {
        for (auto vid : meshOriginale.Cell2DsVertices[i]) {
            verticeToFacce[vid].push_back(i);
        }
    }

    // Creazione facce del duale
    std::set<std::vector<unsigned int>> facceDualiSet;
    for (const auto& [v, facce] : verticeToFacce) {
        if (facce.size() < 3) continue;
        std::vector<unsigned int> nuovaFaccia = facce;
        std::sort(nuovaFaccia.begin(), nuovaFaccia.end());
        facceDualiSet.insert(nuovaFaccia);
    }

    // Inserimento facce nella nuova mesh
    meshDuale.NumCell2Ds = facceDualiSet.size();
    meshDuale.Cell2DsId.resize(meshDuale.NumCell2Ds);
    meshDuale.Cell2DsVertices.resize(meshDuale.NumCell2Ds);
    meshDuale.Cell2DsNumVertices.resize(meshDuale.NumCell2Ds);
    meshDuale.Cell2DsEdges.resize(meshDuale.NumCell2Ds);
    meshDuale.Cell2DsNumEdges.resize(meshDuale.NumCell2Ds);

    unsigned int fId = 0;
    for (const auto& faccia : facceDualiSet) {
        meshDuale.Cell2DsId[fId] = fId;
        meshDuale.Cell2DsVertices[fId] = faccia;
        meshDuale.Cell2DsNumVertices[fId] = faccia.size();
        ++fId;
    }

    // Generazione spigoli
    std::set<std::pair<unsigned int, unsigned int>> spigoli;
    for (const auto& faccia : meshDuale.Cell2DsVertices) {
        for (size_t i = 0; i < faccia.size(); ++i) {
            unsigned int a = faccia[i];
            unsigned int b = faccia[(i + 1) % faccia.size()];
            spigoli.insert({std::min(a, b), std::max(a, b)});
        }
    }

    meshDuale.NumCell1Ds = spigoli.size();
    meshDuale.Cell1DsExtrema.resize(2, spigoli.size());
    meshDuale.Cell1DsId.resize(spigoli.size());

    unsigned int eId = 0;
    for (const auto& [a, b] : spigoli) {
        meshDuale.Cell1DsId[eId] = eId;
        meshDuale.Cell1DsExtrema(0, eId) = a;
        meshDuale.Cell1DsExtrema(1, eId) = b;
        ++eId;
    }
}

*/
bool ScritturaCelle(const PoliedriMesh& mesh, const std::string& nomeBase) {
    
    if (mesh.NumCell0Ds != mesh.Cell0DsId.size() ||
        mesh.NumCell0Ds != mesh.Cell0DsCoordinates.cols()) {
        cerr << "Errore: dati dei vertici inconsistenti\n";
        return false;
    }

    ofstream file0(nomeBase + "_Cell0Ds.txt");
    ofstream file1(nomeBase + "_Cell1Ds.txt");
    ofstream file2(nomeBase + "_Cell2Ds.txt");
    ofstream file3(nomeBase + "_Cell3Ds.txt");

    if (!file0 || !file1 || !file2 || !file3) return false;

    file0 << "# ID x y z\n";
    for (size_t i = 0; i < mesh.NumCell0Ds; ++i) {
        file0 << mesh.Cell0DsId[i] << " "
              << mesh.Cell0DsCoordinates(0, i) << " "
              << mesh.Cell0DsCoordinates(1, i) << " "
              << mesh.Cell0DsCoordinates(2, i) << "\n";
    }

    file1 << "# ID OriginId EndId\n";
    for (size_t i = 0; i < mesh.NumCell1Ds; ++i) {
        file1 << mesh.Cell1DsId[i] << " "
              << mesh.Cell1DsExtrema(0, i) << " "
              << mesh.Cell1DsExtrema(1, i) << "\n";
    }

    file2 << "# ID NumVertices NumEdges VertexIds EdgeIds\n";
    for (size_t i = 0; i < mesh.NumCell2Ds; ++i) {
        file2 << mesh.Cell2DsId[i] << " "
              << mesh.Cell2DsNumVertices[i] << " "
              << mesh.Cell2DsEdges[i].size() << " ";

        for (auto v : mesh.Cell2DsVertices[i]) file2 << v << " ";
        for (auto e : mesh.Cell2DsEdges[i]) file2 << e << " ";
        file2 << "\n";
    }

    file3 << "# ID NumVertices NumEdges NumFaces VertexIds EdgeIds FaceIds\n";
    file3 << "0 "
          << mesh.NumCell0Ds << " "
          << mesh.NumCell1Ds << " "
          << mesh.NumCell2Ds << " ";
    for (auto id : mesh.Cell0DsId) file3 << id << " ";
    for (auto id : mesh.Cell1DsId) file3 << id << " ";
    for (auto id : mesh.Cell2DsId) file3 << id << " ";
    file3 << "\n";

    return true;
}
//void TriangolaFacceClasseI(const PoliedriMesh& meshIniziale, unsigned int b, PoliedriMesh& meshGeodetico);

}