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

/*
void normalizza(double& x, double& y, double& z) {
    double norma = sqrt(x * x + y * y + z * z);
    x /= norma;
    y /= norma;
    z /= norma;
}
*/
/// TETRAEDRO
void CreaTetraedro(PoliedriMesh& mesh){
	
	//Celle0Ds
	vector<Vector3d> vertici = {
		{1.0, 1.0, 1.0}, 
		{-1.0, -1.0, 1.0}, 
		{-1.0, 1.0, -1.0}, 
		{1.0, -1.0, -1.0}
	};
	
	for (auto& vertice : vertici) {
		vertice.normalize();

	}
	
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
		{0,1},{0,2},{0,3},
		{1,2},{1,3},{2,3}
	};
	
	mesh.Cell1DsId.resize(0);
	mesh.Cell1DsExtrema.resize(0, 0);
	mesh.Cell1DsExtrema.resize(2, lati.size());
	
	for (unsigned int i = 0; i < lati.size(); ++i) {
		auto [a, b] = lati[i];
		mesh.Cell1DsId.push_back(i);
		mesh.Cell1DsExtrema(0, i) = a;
		mesh.Cell1DsExtrema(1, i) = b;
	}
	mesh.NumCell1Ds = lati.size();
	


	//Celle2Ds
	vector<vector<unsigned int>> facce = {
		{0, 3, 1},  
		{1, 5, 2},  
		{0, 4, 2},  
		{3, 5, 4}
	};
	
	mesh.Cell2DsId.clear();
	mesh.Cell2DsVertices.clear();
	mesh.Cell2DsEdges.clear();
	
	
	for (unsigned int i = 0; i < facce.size(); ++i) {
		vector<unsigned int> latiFaccia = facce[i];
		vector<unsigned int> verticiFaccia;
		
		for (auto latoId : latiFaccia) {
			unsigned int a = mesh.Cell1DsExtrema(0, latoId);
			unsigned int b = mesh.Cell1DsExtrema(1, latoId);
			
			if (verticiFaccia.empty()) {
				verticiFaccia.push_back(a);
				verticiFaccia.push_back(b);
			} 
			else {
				if (verticiFaccia.back() == a)
					verticiFaccia.push_back(b);
				else if (verticiFaccia.back() == b)
					verticiFaccia.push_back(a);
			}
		}
		// chiudo il ciclo se necessario
		if (verticiFaccia.front() == verticiFaccia.back())
			verticiFaccia.pop_back();

		mesh.Cell2DsId.push_back(i);
		mesh.Cell2DsEdges.push_back(latiFaccia);
		mesh.Cell2DsVertices.push_back(verticiFaccia);
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
		{0,1},{1,2},{2,3},{3,0},
		{4,5},{5,6},{6,7},{7,4},
		{0,4},{1,5},{2,6},{3,7}
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
	
	
	for (unsigned int i = 0; i < facce.size(); ++i) {
		vector<unsigned int> latiFaccia = facce[i];
		vector<unsigned int> verticiFaccia;
		
		for (auto latoId : latiFaccia) {
			unsigned int a = mesh.Cell1DsExtrema(0, latoId);
			unsigned int b = mesh.Cell1DsExtrema(1, latoId);
			
			if (verticiFaccia.empty()) {
				verticiFaccia.push_back(a);
				verticiFaccia.push_back(b);
			} 
			else {
				if (verticiFaccia.back() == a)
					verticiFaccia.push_back(b);
				else if (verticiFaccia.back() == b)
					verticiFaccia.push_back(a);
			}
		}
		// chiudo il ciclo se necessario
		if (verticiFaccia.front() == verticiFaccia.back())
			verticiFaccia.pop_back();

		mesh.Cell2DsId.push_back(i);
		mesh.Cell2DsEdges.push_back(latiFaccia);
		mesh.Cell2DsVertices.push_back(verticiFaccia);
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
        {2,4},{2,5},{3,4},{3,5}
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
	
	
	for (unsigned int i = 0; i < facce.size(); ++i) {
		vector<unsigned int> latiFaccia = facce[i];
		vector<unsigned int> verticiFaccia;
		
		for (auto latoId : latiFaccia) {
			unsigned int a = mesh.Cell1DsExtrema(0, latoId);
			unsigned int b = mesh.Cell1DsExtrema(1, latoId);
			
			if (verticiFaccia.empty()) {
				verticiFaccia.push_back(a);
				verticiFaccia.push_back(b);
			} 
			else {
				if (verticiFaccia.back() == a)
					verticiFaccia.push_back(b);
				else if (verticiFaccia.back() == b)
					verticiFaccia.push_back(a);
			}
		}
		// chiudo il ciclo se necessario
		if (verticiFaccia.front() == verticiFaccia.back())
			verticiFaccia.pop_back();

		mesh.Cell2DsId.push_back(i);
		mesh.Cell2DsEdges.push_back(latiFaccia);
		mesh.Cell2DsVertices.push_back(verticiFaccia);
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
	vector<vector<unsigned int>> facce_vertici = {
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

    for (unsigned int i = 0; i < facce_vertici.size(); ++i) {
        const auto& verts = facce_vertici[i];
        vector<unsigned int> edges;

        // Trova gli spigoli corrispondenti ai lati consecutivi della faccia
        for (size_t j = 0; j < verts.size(); ++j) {
            unsigned int a = verts[j];
            unsigned int b = verts[(j + 1) % verts.size()];
            for (unsigned int k = 0; k < lati.size(); ++k) {
                if ((lati[k].first == a && lati[k].second == b) ||
                    (lati[k].first == b && lati[k].second == a)) {
                    edges.push_back(k);
                    break;
                }
            }
        }

        mesh.Cell2DsId.push_back(i);
        mesh.Cell2DsVertices.push_back(verts);
        mesh.Cell2DsEdges.push_back(edges);
    }
    mesh.NumCell2Ds = facce_vertici.size();
	
	
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
	
	for (unsigned int i = 0; i < facce.size(); ++i) {
        const auto& verts = facce[i];
        std::vector<unsigned int> edges;
        for (size_t j = 0; j < verts.size(); ++j) {
            unsigned int a = verts[j];
            unsigned int b = verts[(j + 1) % verts.size()];
            for (unsigned int k = 0; k < lati.size(); ++k) {
                if ((lati[k].first == a && lati[k].second == b) ||
                    (lati[k].first == b && lati[k].second == a)) {
                    edges.push_back(k);
                    break;
                }
            }
        }
		
		mesh.Cell2DsId.push_back(i);
        mesh.Cell2DsVertices.push_back(verts);
        mesh.Cell2DsEdges.push_back(edges);
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




bool ImportMesh(PoliedriMesh& mesh)
{
	CreaTetraedro(mesh);
	CreaCubo(mesh);
	CreaOttaedro(mesh);
	CreaDodecaedro(mesh);
	CreaIcosaedro(mesh);
	
	/*
	// Importa i vertici 
	if(!Cell0Ds(mesh))
		return false;

	// Importa gli spigoli
	if(!Cell1Ds(mesh))
		return false;
	
	
	// Importa le celle
	if(!Cell2Ds(mesh))
		return false;
	
	// Importa i poliedri
	if(!Cell3Ds(mesh))
		return false;
	*/
	return true;

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
        std::cerr << "Errore: combinazione (p=" << p << ", q=" << q << ") non supportata\n";
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
        unsigned int X = mesh.Cell0DsCoordinates(0, i);
        unsigned int Y = mesh.Cell0DsCoordinates(1, i);
        unsigned int Z = mesh.Cell0DsCoordinates(2, i);
		
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
	
	file.close();
	return true;

}

//void TriangolaFacceClasseI(const PoliedriMesh& meshIniziale, unsigned int b, PoliedriMesh& meshGeodetico);

}