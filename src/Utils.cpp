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
    //Celle0Ds
    vector<Vector3d> vertici = {
        {1.0, 1.0, 1.0}, 
        {-1.0, -1.0, 1.0}, 
        {-1.0, 1.0, -1.0}, 
        {1.0, -1.0, -1.0}
    };

    for (auto& vertice : vertici)
        vertice.normalize();

    mesh.Cell0DsId.clear();
    mesh.Cell0DsCoordinates.resize(3, vertici.size());  // allocazione di memoria per le coordinate

    for (unsigned int i = 0; i < vertici.size(); ++i) {
        mesh.Cell0DsId.push_back(i);
        mesh.Cell0DsCoordinates.col(i) = vertici[i];
    }
    mesh.NumCell0Ds = vertici.size();


    //Celle1Ds
    vector<pair<unsigned int, unsigned int>> lati = {
        {0,1}, {0,2}, {0,3}, {1,2}, {1,3}, {2,3}
    };

    mesh.Cell1DsId.clear();
    mesh.Cell1DsExtrema.resize(2, lati.size());  // allocazione di memoria per gli estremi

    for (unsigned int i = 0; i < lati.size(); ++i) {
        mesh.Cell1DsId.push_back(i);
        mesh.Cell1DsExtrema(0, i) = lati[i].first;
        mesh.Cell1DsExtrema(1, i) = lati[i].second;
    }
    mesh.NumCell1Ds = lati.size();


    //Celle2Ds
	mesh.Cell2DsId.clear();
    mesh.Cell2DsVertices.clear();
    mesh.Cell2DsEdges.clear();
    
    mesh.Cell2DsId.resize(4);  // alloca uno spazio di dimensione 4 per gli Id
    mesh.Cell2DsVertices.resize(4);  // alloca uno spazio di dimensione 4 per i vertici
    mesh.Cell2DsEdges.resize(4);  // alloca uno spazio di dimensione 4 per i lati

	mesh.Cell2DsId = {0, 1, 2, 3};
	
	mesh.Cell2DsVertices = {
        {0, 1, 2},
        {0, 3, 1},
        {1, 3, 2},
        {2, 3, 0}
    };

    mesh.Cell2DsEdges = {
        {0, 1, 3},
        {2, 4, 0},
        {4, 5, 3},
        {5, 2, 1}
    };
	
    mesh.NumCell2Ds = 4;
    

    //Celle3Ds
	mesh.Cell3DsId.clear();
    mesh.Cell3DsNumVertices.clear();
    mesh.Cell3DsVertices.clear();
    mesh.Cell3DsNumEdges.clear();
    mesh.Cell3DsEdges.clear();
    mesh.Cell3DsNumFaces.clear();
    mesh.Cell3DsFaces.clear();

    mesh.Cell3DsId.push_back(0);
    mesh.Cell3DsVertices.push_back(mesh.Cell0DsId);
    mesh.Cell3DsNumVertices.push_back(mesh.Cell0DsId.size());

    mesh.Cell3DsEdges.push_back(mesh.Cell1DsId);
    mesh.Cell3DsNumEdges.push_back(mesh.Cell1DsId.size());

    mesh.Cell3DsFaces.push_back(mesh.Cell2DsId);
    mesh.Cell3DsNumFaces.push_back(mesh.Cell2DsId.size());

    mesh.NumCell3Ds = 1;
}

//********************************************************************************	
///CUBO
void CreaCubo(PoliedriMesh& mesh) {
	
	vector<Vector3d> vertici = {
		{-1.0, -1.0, -1.0}, {-1.0, -1.0, 1.0}, 
		{-1.0, 1.0, -1.0}, {-1.0, 1.0, 1.0},
		{1.0, -1.0, -1.0}, {1.0, -1.0, 1.0}, 
		{1.0, 1.0, -1.0}, {1.0, 1.0, 1.0}
	};

	for (auto& vertice : vertici) {
		vertice.normalize();
	}
	
	//Celle0Ds
	mesh.Cell0DsId.clear();
    mesh.Cell0DsCoordinates.resize(3, vertici.size());  // allocazione di memoria per le coordinate
	
	for (unsigned int i = 0; i < vertici.size(); ++i) {
		mesh.Cell0DsId.push_back(i);
		mesh.Cell0DsCoordinates.col(i) = vertici[i];
	}
	mesh.NumCell0Ds = vertici.size();
	
	
	//Celle1Ds
	vector<pair<unsigned int, unsigned int>> lati = {
		{0,1}, {1,3}, {3,2}, {2,0},
		{4,5}, {5,7}, {7,6}, {6,4},
		{0,4}, {1,5}, {3,7}, {2,6}
	};
	
	mesh.Cell1DsId.clear();
    mesh.Cell1DsExtrema.resize(2, lati.size());  // allocazione di memoria per gli estremi
	
	for (unsigned int i = 0; i < lati.size(); ++i) {
		mesh.Cell1DsId.push_back(i);
		mesh.Cell1DsExtrema(0, i) = lati[i].first;
		mesh.Cell1DsExtrema(1, i) = lati[i].second;
	}
	mesh.NumCell1Ds = lati.size();
	
	
	//Celle2Ds
	mesh.Cell2DsId.clear();
    mesh.Cell2DsVertices.clear();
    mesh.Cell2DsEdges.clear();
	
	mesh.Cell2DsId.resize(6);  // alloca uno spazio di dimensione 6 per gli Id
    mesh.Cell2DsVertices.resize(6);  // alloca uno spazio di dimensione 6 per i vertici
    mesh.Cell2DsEdges.resize(6);  // alloca uno spazio di dimensione 6 per i lati

	mesh.Cell2DsId = {0, 1, 2, 3, 4, 5};
	
	mesh.Cell2DsVertices = {
        {0, 1, 3, 2},
        {4, 5, 7, 6},
        {0, 4, 5, 1}, 
        {2, 3, 7, 6},
        {0, 2, 6, 4},
        {1, 5, 7, 3}
    };

    mesh.Cell2DsEdges = {
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {0, 8, 4, 9},
        {2, 11, 6, 10},
        {3, 10, 7, 8},
        {1, 9, 5, 11}
    };
	
	mesh.NumCell2Ds = mesh.Cell2DsId.size();
	
	//Celle3Ds
	mesh.Cell3DsId.clear();
    mesh.Cell3DsNumVertices.clear();
    mesh.Cell3DsVertices.clear();
    mesh.Cell3DsNumEdges.clear();
    mesh.Cell3DsEdges.clear();
    mesh.Cell3DsNumFaces.clear();
    mesh.Cell3DsFaces.clear();

    mesh.Cell3DsId.push_back(0);
    mesh.Cell3DsVertices.push_back(mesh.Cell0DsId);
    mesh.Cell3DsNumVertices.push_back(mesh.Cell0DsId.size());

    mesh.Cell3DsEdges.push_back(mesh.Cell1DsId);
    mesh.Cell3DsNumEdges.push_back(mesh.Cell1DsId.size());

    mesh.Cell3DsFaces.push_back(mesh.Cell2DsId);
    mesh.Cell3DsNumFaces.push_back(mesh.Cell2DsId.size());

    mesh.NumCell3Ds = 1;
}

//*****************************************************************************
///OTTAEDRO
void CreaOttaedro(PoliedriMesh& mesh) {
	
	vector<Vector3d> vertici = {
	{1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0},
	{0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}
	};

	for (auto& vertice : vertici) {
		vertice.normalize();
	}

	//Celle0Ds
	mesh.Cell0DsId.clear();
	mesh.Cell0DsCoordinates.resize(3, vertici.size());  // allocazione di memoria per le coordinate

	for (unsigned int i = 0; i < vertici.size(); ++i) {
		mesh.Cell0DsId.push_back(i);
		mesh.Cell0DsCoordinates.col(i) = vertici[i];
	}

	mesh.NumCell0Ds = vertici.size();
	
	
	//Celle1Ds
    vector<pair<unsigned int, unsigned int>> lati = {
		{0,2}, {0,3}, {0,4}, {0,5},
        {1,2}, {1,3}, {1,4}, {1,5},
        {2,4}, {2,5}, {3,5}, {3,4}
    };
	
	mesh.Cell1DsId.clear();
    mesh.Cell1DsExtrema.resize(2, lati.size());  // allocazione di memoria per gli estremi
	
	for (unsigned int i = 0; i < lati.size(); ++i) {
		mesh.Cell1DsId.push_back(i);
		mesh.Cell1DsExtrema(0, i) = lati[i].first;
		mesh.Cell1DsExtrema(1, i) = lati[i].second;
	}
	mesh.NumCell1Ds = lati.size();
	
	
	//Celle2Ds
	mesh.Cell2DsId.clear();
    mesh.Cell2DsVertices.clear();
    mesh.Cell2DsEdges.clear();
	
	mesh.Cell2DsId.resize(8);  // alloca uno spazio di dimensione 8 per gli Id
    mesh.Cell2DsVertices.resize(8);  // alloca uno spazio di dimensione 8 per i vertici
    mesh.Cell2DsEdges.resize(8);  // alloca uno spazio di dimensione 8 per i lati

	mesh.Cell2DsId = {0, 1, 2, 3, 4, 5, 6, 7};
	
	
	mesh.Cell2DsVertices = {
		{0, 4, 2},
		{0, 2, 5},
		{0, 5, 3},
		{0, 3, 4},
		{1, 4, 2}, 
		{1, 2, 5},
		{1, 5, 3},
		{1, 3, 4}
	};

	mesh.Cell2DsEdges = {
		{2, 8, 0},
		{0, 9, 3},
		{3, 10, 1},
		{1, 11, 2},
		{6, 8, 4},
		{4, 9, 7},
		{7, 10, 5},
		{5, 11, 6}
	};
	
	mesh.NumCell2Ds = mesh.Cell2DsId.size();
	
	
	//Celle3Ds
	mesh.Cell3DsId.clear();
    mesh.Cell3DsNumVertices.clear();
    mesh.Cell3DsVertices.clear();
    mesh.Cell3DsNumEdges.clear();
    mesh.Cell3DsEdges.clear();
    mesh.Cell3DsNumFaces.clear();
    mesh.Cell3DsFaces.clear();

    mesh.Cell3DsId.push_back(0);
    mesh.Cell3DsVertices.push_back(mesh.Cell0DsId);
    mesh.Cell3DsNumVertices.push_back(mesh.Cell0DsId.size());

    mesh.Cell3DsEdges.push_back(mesh.Cell1DsId);
    mesh.Cell3DsNumEdges.push_back(mesh.Cell1DsId.size());

    mesh.Cell3DsFaces.push_back(mesh.Cell2DsId);
    mesh.Cell3DsNumFaces.push_back(mesh.Cell2DsId.size());

    mesh.NumCell3Ds = 1;
}

//*****************************************************************************
///DODECAEDRO
void CreaDodecaedro(PoliedriMesh& mesh) {

	const double phi = (1.0 + sqrt(5.0)) / 2.0;  // calcolo sezione aurea
	
	vector<Vector3d> vertici = {
		{0, -1 / phi, phi},
		{1, -1, 1},
		{phi, 0, 1 / phi},
		{1, 1, 1},
		{0, 1 / phi, phi},
		{-1, 1, 1},
		{-phi, 0, 1 / phi},
		{-1, -1, 1},
		{-1 / phi, -phi, 0},
		{1 / phi, -phi, 0}, 
		{1, -1, -1},
		{phi, 0, -1 / phi},
		{1, 1, -1},
		{1 / phi, phi, 0},
		{-1 / phi, phi, 0},
		{-1, 1, -1}, 
		{-phi, 0, -1 / phi},
		{-1, -1, -1},
		{0, -1 / phi, -phi},
		{0, 1 / phi, -phi}
    };
	
	for (auto& vertice : vertici) {
		vertice.normalize();
	}


	//Celle0Ds
	mesh.Cell0DsId.clear();
	mesh.Cell0DsCoordinates.resize(3, vertici.size());  // allocazione di memoria per le coordinate

	for (unsigned int i = 0; i < vertici.size(); ++i) {
		mesh.Cell0DsId.push_back(i);
		mesh.Cell0DsCoordinates.col(i) = vertici[i];
	}

	mesh.NumCell0Ds = vertici.size();
	
	
	//Celle1Ds
	vector<pair<unsigned int, unsigned int>> lati = {
		{0,1}, {0,4}, {0,7}, {1,2}, {1,9},
		{2,3}, {2,11}, {3,4}, {3,13}, {4,5},
		{5,6}, {5,14}, {6,7}, {6,16}, {7,8},
		{8,9}, {8,17}, {9,10}, {10,11}, {10,18},
		{11,12}, {12,13}, {12,19}, {13,14}, {14,15}, 
		{15,16}, {16,17}, {17,18}, {18,19}, {19,15}
	};
	
	mesh.Cell1DsId.clear();
    mesh.Cell1DsExtrema.resize(2, lati.size());  // allocazione di memoria per gli estremi
	
	for (unsigned int i = 0; i < lati.size(); ++i) {
		mesh.Cell1DsId.push_back(i);
		mesh.Cell1DsExtrema(0, i) = lati[i].first;
		mesh.Cell1DsExtrema(1, i) = lati[i].second;
	}
	mesh.NumCell1Ds = lati.size();
	
	
	//Celle2Ds
	mesh.Cell2DsId.clear();
    mesh.Cell2DsVertices.clear();
    mesh.Cell2DsEdges.clear();
	
	mesh.Cell2DsId.resize(12);  // alloca uno spazio di dimensione 12 per gli Id
    mesh.Cell2DsVertices.resize(12);  // alloca uno spazio di dimensione 12 per i vertici
    mesh.Cell2DsEdges.resize(12);  // alloca uno spazio di dimensione 12 per i lati
	
	mesh.Cell2DsId = {0,1,2,3,4,5,6,7,8,9,10,11};

    mesh.Cell2DsVertices = {
        {0, 1, 2, 3, 4},
		{0, 1, 9, 8, 7},
		{0, 4, 5, 6, 7},
		{2, 1, 9, 10, 11},
		{3, 2, 11, 12, 13},
		{4, 3, 13, 14, 5},
		// facce dietro
		{15, 16, 6, 5, 14},
		{16, 17, 8, 7, 6},
		{17, 18, 10, 9, 8},
		{18, 19, 12, 11, 10},
		{19, 15, 14, 13, 12},
		{15, 16, 17, 18, 19},        
    };

    mesh.Cell2DsEdges = {
        {0, 3, 5, 7 , 1},
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

    mesh.NumCell2Ds = mesh.Cell2DsId.size();
	
	
	//Celle3Ds
    mesh.Cell3DsId.clear();
    mesh.Cell3DsNumVertices.clear();
    mesh.Cell3DsVertices.clear();
    mesh.Cell3DsNumEdges.clear();
    mesh.Cell3DsEdges.clear();
    mesh.Cell3DsNumFaces.clear();
    mesh.Cell3DsFaces.clear();

    mesh.Cell3DsId.push_back(0);
    mesh.Cell3DsVertices.push_back(mesh.Cell0DsId);
    mesh.Cell3DsNumVertices.push_back(mesh.Cell0DsId.size());

    mesh.Cell3DsEdges.push_back(mesh.Cell1DsId);
    mesh.Cell3DsNumEdges.push_back(mesh.Cell1DsId.size());

    mesh.Cell3DsFaces.push_back(mesh.Cell2DsId);
    mesh.Cell3DsNumFaces.push_back(mesh.Cell2DsId.size());

    mesh.NumCell3Ds = 1;
}

//**********************************************************************************
///ICOSAEDRO
void CreaIcosaedro(PoliedriMesh& mesh) {
	
	const double phi = (1.0 + sqrt(5.0)) / 2.0; // calcolo sezione aurea
	
	vector<Vector3d> vertici = {
			{-1, phi, 0},
			{-phi, 0, -1},
			{0, 1, -phi},
			{1, phi, 0},
			{0, 1, phi},
			{-phi, 0, 1},
			{-1, -phi, 0},
			{0, -1, -phi},
			{phi, 0, -1},
			{phi, 0, 1},
			{0, -1, phi},
			{1, -phi, 0}
	};
	
	for (auto& vertice : vertici) {
		vertice.normalize();
	}

	//Celle0Ds
	mesh.Cell0DsId.clear();
	mesh.Cell0DsCoordinates.resize(3, vertici.size());  // allocazione di memoria per le coordinate

	for (unsigned int i = 0; i < vertici.size(); ++i) {
		mesh.Cell0DsId.push_back(i);
		mesh.Cell0DsCoordinates.col(i) = vertici[i];
	}

	mesh.NumCell0Ds = vertici.size();
	
	
	//Celle1Ds
	vector<pair<unsigned int, unsigned int>> lati = {
		{0,1}, {0,2}, {0,3}, {0,4}, {0,5},
		{1,2}, {2,3}, {3,4}, {4,5}, {5,1},
		{1,6}, {1,7}, {2,7}, {2,8}, {3,8}, 
		{3,9}, {4,9}, {4,10},{5,10}, {5,6}, 
		{6,10}, {6,7}, {7,8}, {8,9}, {9,10},
		{11,6}, {11,7}, {11,8}, {11,9}, {11,10}	
	};
	
	
	mesh.Cell1DsId.clear();
    mesh.Cell1DsExtrema.resize(2, lati.size());  // allocazione di memoria per gli estemi
	
	for (unsigned int i = 0; i < lati.size(); ++i) {
		mesh.Cell1DsId.push_back(i);
		mesh.Cell1DsExtrema(0, i) = lati[i].first;
		mesh.Cell1DsExtrema(1, i) = lati[i].second;
	}
	mesh.NumCell1Ds = lati.size();
	
	
	//Celle2Ds
	mesh.Cell2DsId.clear();
    mesh.Cell2DsVertices.clear();
    mesh.Cell2DsEdges.clear();
	
	mesh.Cell2DsId.resize(20);  // alloca uno spazio di dimensione 20 per gli Id
    mesh.Cell2DsVertices.resize(20);  // alloca uno spazio di dimensione 20 per i vertici
    mesh.Cell2DsEdges.resize(20);  // alloca uno spazio di dimensione 20 per i lati

	mesh.Cell2DsId = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
	
	mesh.Cell2DsVertices = {
		{0, 1, 2},
		{0, 2, 3},
		{0, 3, 4},
		{0, 4, 5},
		{0, 5, 1},
		
		{1, 7, 2},
		{2, 8, 3},
		{3, 9, 4},
		{4, 10, 5},
		{5, 6, 1},

		{6, 7, 1},
		{7, 8, 2},
		{8, 9, 3},
		{9, 10, 4},
		{10, 6, 5},

		{6, 11, 7},
		{7, 11, 8},
		{8, 11, 9},
		{9, 11, 10},
		{10, 11, 6},
	};
	
	
	mesh.Cell2DsEdges = {
		{0, 5, 1}, 
		{1, 6, 2},
		{2, 7, 3},
		{3, 8, 4},
		{4, 9, 0},
			
		{11, 12, 5},
		{13, 14, 6},
		{15, 16, 7},
		{17, 18, 8},
		{19, 10, 9},
			
		{21, 11, 10},
		{22, 13, 12},
		{23, 15, 14},
		{24, 17, 16},
		{20, 19, 18},
			
		{25, 26, 21},
		{26, 27, 22},
		{27, 28, 23},
		{28, 29, 24},
		{29, 25, 20},	
	};
	
	mesh.NumCell2Ds = mesh.Cell2DsId.size();
	
	
	//Celle3Ds
    mesh.Cell3DsId.clear();
    mesh.Cell3DsNumVertices.clear();
    mesh.Cell3DsVertices.clear();
    mesh.Cell3DsNumEdges.clear();
    mesh.Cell3DsEdges.clear();
    mesh.Cell3DsNumFaces.clear();
    mesh.Cell3DsFaces.clear();

    mesh.Cell3DsId.push_back(0);
    mesh.Cell3DsVertices.push_back(mesh.Cell0DsId);
    mesh.Cell3DsNumVertices.push_back(mesh.Cell0DsId.size());

    mesh.Cell3DsEdges.push_back(mesh.Cell1DsId);
    mesh.Cell3DsNumEdges.push_back(mesh.Cell1DsId.size());

    mesh.Cell3DsFaces.push_back(mesh.Cell2DsId);
    mesh.Cell3DsNumFaces.push_back(mesh.Cell2DsId.size());

    mesh.NumCell3Ds = 1;
}

bool ScritturaCelle(const PoliedriMesh& mesh, const string& nomeBase) {
	
    // Cell0Ds
    ofstream file0(nomeBase + "_Cell0Ds.txt");
    if (!file0) {
        cerr << "Errore apertura file " << nomeBase << "_Cell0Ds.txt" << endl;
        return false;
    }

    file0 << "# ID x y z" << endl;
	
    for (size_t i = 0; i < mesh.NumCell0Ds; ++i) {
        file0 << mesh.Cell0DsId[i] << " "
              << mesh.Cell0DsCoordinates(0, i) << " "
              << mesh.Cell0DsCoordinates(1, i) << " "
              << mesh.Cell0DsCoordinates(2, i) << "\n";
    }
    file0.close();

    // Cell1Ds
    ofstream file1(nomeBase + "_Cell1Ds.txt");
    if (!file1) {
        cerr << "Errore apertura file " << nomeBase << "_Cell1Ds.txt" << endl;
        return false;
    }

    file1 << "# ID OriginId EndId" << endl;
	
    for (size_t i = 0; i < mesh.NumCell1Ds; ++i) {
        file1 << mesh.Cell1DsId[i] << " "
              << mesh.Cell1DsExtrema(0, i) << " "
              << mesh.Cell1DsExtrema(1, i) << "\n";
    }
    file1.close();

    // Cell2Ds
    ofstream file2(nomeBase + "_Cell2Ds.txt");
    if (!file2) {
        cerr << "Errore apertura file " << nomeBase << "_Cell2Ds.txt" << endl;
        return false;
    }

    file2 << "# ID NumVertices NumEdges VertexIds EdgeIds" << endl;
    for (size_t i = 0; i < mesh.NumCell2Ds; ++i) {
        file2 << mesh.Cell2DsId[i] << " "
              << mesh.Cell2DsVertices[i].size() << " "
              << mesh.Cell2DsEdges[i].size();

        for (auto v : mesh.Cell2DsVertices[i])
            file2 << " " << v;
        for (auto e : mesh.Cell2DsEdges[i])
            file2 << " " << e;

        file2 << "\n";
    }
    file2.close();
	
	// Cell3Ds
	ofstream file3(nomeBase + "_Cell3Ds.txt");
	if(!file3) {
		cerr << "Errore apertura file " << nomeBase << "_Cell3Ds.txt" << endl;
		return false;
	}
	
	file3 << "# ID NumVertices Vertices NumEdges Edges NumFaces Faces" << endl;
	for(size_t i = 0; i < mesh.NumCell3Ds; ++i) {
		file3 << mesh.Cell3DsId[i] << " "
			  << mesh.Cell3DsVertices[i].size();
	
		for (auto v : mesh.Cell3DsVertices[i])
			file3 << " " << v;
	
		file3 << " " << mesh.Cell3DsEdges[i].size();
	
		for (auto e : mesh.Cell3DsEdges[i])
			file3 << " " << e;
	
		file3 << " " << mesh.Cell3DsFaces[i].size();
	
		for (auto f : mesh.Cell3DsFaces[i])
			file3 << " " << f;
	
		file3 << "\n";
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
		cerr << "Poliedro {p = " << p << ", q = " << q << "} non supportato" << endl;
		return false;
	}

	return true;
}


unsigned int TrovaSpigolo(map<pair<unsigned int, unsigned int>, unsigned int>& mappaSpigoli, PoliedriMesh& mesh, unsigned int start, unsigned int end)
{
    if (start > end)
		swap(start, end);  // inverte le posizioni
    auto chiave = make_pair(start, end);  // crea una pair() il cui tipo è uguale a quello degli argomenti

    auto iter = mappaSpigoli.find(chiave);  // in mappaSpigoli cerca la chiave
    if (iter != mappaSpigoli.end())
		return iter->second;  // restituisce il valore associato alla chiave a cui punta l'iteratore nella mappa mappaSpigoli

    unsigned int nuovoIndice = mesh.Cell1DsExtrema.cols();
    mesh.Cell1DsExtrema.conservativeResize(2, nuovoIndice + 1);  // ridimensiona il numero di colonne in modo conservativo, cioè mantenendo i dati già presenti
    mesh.Cell1DsExtrema(0, nuovoIndice) = start;
    mesh.Cell1DsExtrema(1, nuovoIndice) = end;

    mesh.Cell1DsId.push_back(nuovoIndice);
    mappaSpigoli[chiave] = nuovoIndice;
    
	return nuovoIndice;
}

}
