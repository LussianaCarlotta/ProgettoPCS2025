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

    // Funzione interna per ordinare i lati e ricavare i vertici
    auto try_order = [](vector<pair<unsigned int, pair<unsigned int, unsigned int>>> edges,
                        vector<unsigned int>& out_vertices,
                        vector<unsigned int>& out_edges) -> bool
    {
        for (int flip = 0; flip < 2; ++flip) {
            auto edges_copy = edges;
            out_vertices.clear();
            out_edges.clear();

            auto [id0, ab0] = edges_copy[0];
            unsigned int start = flip == 0 ? ab0.first : ab0.second;
            unsigned int next  = flip == 0 ? ab0.second : ab0.first;

            out_vertices.push_back(start);
            out_vertices.push_back(next);
            out_edges.push_back(id0);
            edges_copy.erase(edges_copy.begin());

            while (!edges_copy.empty()) {
                bool found = false;
                for (auto it = edges_copy.begin(); it != edges_copy.end(); ++it) {
                    auto [id, ab] = *it;
                    if (ab.first == out_vertices.back()) {
                        out_vertices.push_back(ab.second);
                        out_edges.push_back(id);
                        edges_copy.erase(it);
                        found = true;
                        break;
                    } else if (ab.second == out_vertices.back()) {
                        out_vertices.push_back(ab.first);
                        out_edges.push_back(id);
                        edges_copy.erase(it);
                        found = true;
                        break;
                    }
                }
                if (!found)
                    break;
            }

            if (out_vertices.size() >= 3 && out_vertices.front() == out_vertices.back())
                out_vertices.pop_back(); // chiude il ciclo

            if (edges_copy.empty())
                return true; // successo
        }
        return false;
    };

    for (unsigned int i = 0; i < facce.size(); ++i) {
        const auto& f = facce[i];
        vector<pair<unsigned int, pair<unsigned int, unsigned int>>> edges;
        for (auto idx : f) {
            unsigned int a = mesh.Cell1DsExtrema(0, idx);
            unsigned int b = mesh.Cell1DsExtrema(1, idx);
            edges.emplace_back(idx, make_pair(a, b));
        }

        vector<unsigned int> orderedVertices, orderedEdges;
        if (!try_order(edges, orderedVertices, orderedEdges)) {
            cerr << "Errore nell'ordinamento dei lati per la faccia " << i << endl;
            return;
        }

        mesh.Cell2DsId.push_back(i);
        mesh.Cell2DsVertices.push_back(orderedVertices);
        mesh.Cell2DsEdges.push_back(orderedEdges);
    }

    mesh.NumCell2Ds = facce.size();

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
		{0,1},{1,2},{2,3},{3,0},
		{4,5},{5,6},{6,7},{7,4},
		{0,4},{1,5},{2,6},{3,7}
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
		{2, 11, 6, 10},
		{3, 8, 7, 11}
	};
	
	mesh.Cell2DsId.clear();
    mesh.Cell2DsVertices.clear();
    mesh.Cell2DsEdges.clear();
	
	
	// Funzione interna per ordinare i lati e ricavare i vertici
    auto try_order = [](vector<pair<unsigned int, pair<unsigned int, unsigned int>>> edges,
                        vector<unsigned int>& out_vertices,
                        vector<unsigned int>& out_edges) -> bool
    {
        for (int flip = 0; flip < 2; ++flip) {
            auto edges_copy = edges;
            out_vertices.clear();
            out_edges.clear();

            auto [id0, ab0] = edges_copy[0];
            unsigned int start = flip == 0 ? ab0.first : ab0.second;
            unsigned int next  = flip == 0 ? ab0.second : ab0.first;

            out_vertices.push_back(start);
            out_vertices.push_back(next);
            out_edges.push_back(id0);
            edges_copy.erase(edges_copy.begin());

            while (!edges_copy.empty()) {
                bool found = false;
                for (auto it = edges_copy.begin(); it != edges_copy.end(); ++it) {
                    auto [id, ab] = *it;
                    if (ab.first == out_vertices.back()) {
                        out_vertices.push_back(ab.second);
                        out_edges.push_back(id);
                        edges_copy.erase(it);
                        found = true;
                        break;
                    } else if (ab.second == out_vertices.back()) {
                        out_vertices.push_back(ab.first);
                        out_edges.push_back(id);
                        edges_copy.erase(it);
                        found = true;
                        break;
                    }
                }
                if (!found)
                    break;
            }

            if (out_vertices.size() >= 3 && out_vertices.front() == out_vertices.back())
                out_vertices.pop_back(); // chiude il ciclo

            if (edges_copy.empty())
                return true; // successo
        }
        return false;
    };

    for (unsigned int i = 0; i < facce.size(); ++i) {
        const auto& f = facce[i];
        vector<pair<unsigned int, pair<unsigned int, unsigned int>>> edges;
        for (auto idx : f) {
            unsigned int a = mesh.Cell1DsExtrema(0, idx);
            unsigned int b = mesh.Cell1DsExtrema(1, idx);
            edges.emplace_back(idx, make_pair(a, b));
        }

        vector<unsigned int> orderedVertices, orderedEdges;
        if (!try_order(edges, orderedVertices, orderedEdges)) {
            cerr << "Errore nell'ordinamento dei lati per la faccia " << i << endl;
            return;
        }

        mesh.Cell2DsId.push_back(i);
        mesh.Cell2DsVertices.push_back(orderedVertices);
        mesh.Cell2DsEdges.push_back(orderedEdges);
    }
	mesh.NumCell2Ds = facce.size();
	
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
	
	
	// Funzione interna per ordinare i lati e ricavare i vertici
    auto try_order = [](vector<pair<unsigned int, pair<unsigned int, unsigned int>>> edges,
                        vector<unsigned int>& out_vertices,
                        vector<unsigned int>& out_edges) -> bool
    {
        for (int flip = 0; flip < 2; ++flip) {
            auto edges_copy = edges;
            out_vertices.clear();
            out_edges.clear();

            auto [id0, ab0] = edges_copy[0];
            unsigned int start = flip == 0 ? ab0.first : ab0.second;
            unsigned int next  = flip == 0 ? ab0.second : ab0.first;

            out_vertices.push_back(start);
            out_vertices.push_back(next);
            out_edges.push_back(id0);
            edges_copy.erase(edges_copy.begin());

            while (!edges_copy.empty()) {
                bool found = false;
                for (auto it = edges_copy.begin(); it != edges_copy.end(); ++it) {
                    auto [id, ab] = *it;
                    if (ab.first == out_vertices.back()) {
                        out_vertices.push_back(ab.second);
                        out_edges.push_back(id);
                        edges_copy.erase(it);
                        found = true;
                        break;
                    } else if (ab.second == out_vertices.back()) {
                        out_vertices.push_back(ab.first);
                        out_edges.push_back(id);
                        edges_copy.erase(it);
                        found = true;
                        break;
                    }
                }
                if (!found)
                    break;
            }

            if (out_vertices.size() >= 3 && out_vertices.front() == out_vertices.back())
                out_vertices.pop_back(); // chiude il ciclo

            if (edges_copy.empty())
                return true; // successo
        }
        return false;
    };

    for (unsigned int i = 0; i < facce.size(); ++i) {
        const auto& f = facce[i];
        vector<pair<unsigned int, pair<unsigned int, unsigned int>>> edges;
        for (auto idx : f) {
            unsigned int a = mesh.Cell1DsExtrema(0, idx);
            unsigned int b = mesh.Cell1DsExtrema(1, idx);
            edges.emplace_back(idx, make_pair(a, b));
        }

        vector<unsigned int> orderedVertices, orderedEdges;
        if (!try_order(edges, orderedVertices, orderedEdges)) {
            cerr << "Errore nell'ordinamento dei lati per la faccia " << i << endl;
            return;
        }

        mesh.Cell2DsId.push_back(i);
        mesh.Cell2DsVertices.push_back(orderedVertices);
        mesh.Cell2DsEdges.push_back(orderedEdges);
    }
	mesh.NumCell2Ds = facce.size();
	
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

    // Funzione interna per ordinare i lati e ricavare i vertici
    auto try_order = [](vector<pair<unsigned int, pair<unsigned int, unsigned int>>> edges,
                        vector<unsigned int>& out_vertices,
                        vector<unsigned int>& out_edges) -> bool
    {
        for (int flip = 0; flip < 2; ++flip) {
            auto edges_copy = edges;
            out_vertices.clear();
            out_edges.clear();

            auto [id0, ab0] = edges_copy[0];
            unsigned int start = flip == 0 ? ab0.first : ab0.second;
            unsigned int next  = flip == 0 ? ab0.second : ab0.first;

            out_vertices.push_back(start);
            out_vertices.push_back(next);
            out_edges.push_back(id0);
            edges_copy.erase(edges_copy.begin());

            while (!edges_copy.empty()) {
                bool found = false;
                for (auto it = edges_copy.begin(); it != edges_copy.end(); ++it) {
                    auto [id, ab] = *it;
                    if (ab.first == out_vertices.back()) {
                        out_vertices.push_back(ab.second);
                        out_edges.push_back(id);
                        edges_copy.erase(it);
                        found = true;
                        break;
                    } else if (ab.second == out_vertices.back()) {
                        out_vertices.push_back(ab.first);
                        out_edges.push_back(id);
                        edges_copy.erase(it);
                        found = true;
                        break;
                    }
                }
                if (!found)
                    break;
            }

            if (out_vertices.size() >= 3 && out_vertices.front() == out_vertices.back())
                out_vertices.pop_back(); // chiude il ciclo

            if (edges_copy.empty())
                return true; // successo
        }
        return false;
    };

    for (unsigned int i = 0; i < facce.size(); ++i) {
        const auto& f = facce[i];
        vector<pair<unsigned int, pair<unsigned int, unsigned int>>> edges;
        for (auto idx : f) {
            unsigned int a = mesh.Cell1DsExtrema(0, idx);
            unsigned int b = mesh.Cell1DsExtrema(1, idx);
            edges.emplace_back(idx, make_pair(a, b));
        }

        vector<unsigned int> orderedVertices, orderedEdges;
        if (!try_order(edges, orderedVertices, orderedEdges)) {
            cerr << "Errore nell'ordinamento dei lati per la faccia " << i << endl;
            return;
        }

        mesh.Cell2DsId.push_back(i);
        mesh.Cell2DsVertices.push_back(orderedVertices);
        mesh.Cell2DsEdges.push_back(orderedEdges);
    }
	mesh.NumCell2Ds = facce.size();
	
	
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
	
	// Funzione interna per ordinare i lati e ricavare i vertici
    auto try_order = [](vector<pair<unsigned int, pair<unsigned int, unsigned int>>> edges,
                        vector<unsigned int>& out_vertices,
                        vector<unsigned int>& out_edges) -> bool
    {
        for (int flip = 0; flip < 2; ++flip) {
            auto edges_copy = edges;
            out_vertices.clear();
            out_edges.clear();

            auto [id0, ab0] = edges_copy[0];
            unsigned int start = flip == 0 ? ab0.first : ab0.second;
            unsigned int next  = flip == 0 ? ab0.second : ab0.first;

            out_vertices.push_back(start);
            out_vertices.push_back(next);
            out_edges.push_back(id0);
            edges_copy.erase(edges_copy.begin());

            while (!edges_copy.empty()) {
                bool found = false;
                for (auto it = edges_copy.begin(); it != edges_copy.end(); ++it) {
                    auto [id, ab] = *it;
                    if (ab.first == out_vertices.back()) {
                        out_vertices.push_back(ab.second);
                        out_edges.push_back(id);
                        edges_copy.erase(it);
                        found = true;
                        break;
                    } else if (ab.second == out_vertices.back()) {
                        out_vertices.push_back(ab.first);
                        out_edges.push_back(id);
                        edges_copy.erase(it);
                        found = true;
                        break;
                    }
                }
                if (!found)
                    break;
            }

            if (out_vertices.size() >= 3 && out_vertices.front() == out_vertices.back())
                out_vertices.pop_back(); // chiude il ciclo

            if (edges_copy.empty())
                return true; // successo
        }
        return false;
    };

    for (unsigned int i = 0; i < facce.size(); ++i) {
        const auto& f = facce[i];
        vector<pair<unsigned int, pair<unsigned int, unsigned int>>> edges;
        for (auto idx : f) {
            unsigned int a = mesh.Cell1DsExtrema(0, idx);
            unsigned int b = mesh.Cell1DsExtrema(1, idx);
            edges.emplace_back(idx, make_pair(a, b));
        }

        vector<unsigned int> orderedVertices, orderedEdges;
        if (!try_order(edges, orderedVertices, orderedEdges)) {
            cerr << "Errore nell'ordinamento dei lati per la faccia " << i << endl;
            return;
        }

        mesh.Cell2DsId.push_back(i);
        mesh.Cell2DsVertices.push_back(orderedVertices);
        mesh.Cell2DsEdges.push_back(orderedEdges);
    }
	mesh.NumCell2Ds = facce.size();
	
	
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


//void TriangolaFacceClasseI(const PoliedriMesh& meshIniziale, unsigned int b, PoliedriMesh& meshGeodetico);

}