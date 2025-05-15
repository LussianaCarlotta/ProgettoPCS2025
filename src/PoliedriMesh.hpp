#pragma once 

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace PoliedriLibrary {

struct PoliedriMesh
{
	unsigned int NumCell0Ds = 0; //vertici
	vector<unsigned int> Cell0DsId = {}; //Id, dimensione: 1 x NumCell0Ds
	MatrixXd Cell0DsCoordinates = {}; //(X,Y,Z), dimensione: 3 x NumCell0Ds
	map<unsigned int, list<unsigned int>> Cell0DsMarker = {}; //Marker
	
	unsigned int NumCell1Ds = 0; //lati
	vector<unsigned int> Cell1DsId = {}; //Id, dimensione: 1 x NumCell1Ds
	MatrixXi Cell1DsExtrema = {}; //(Origin, End), dimensione: 2 x NumCell1Ds
	<map<unsigned int>, list<unsigned int>> Cell1DsMarker = {}; //Marker
	
	unsigned int NumCell2Ds = 0; //facce
	vector<unsigned int> Cell2DsId = {}; //Id, dimensione: 1 x NumCell2Ds
	vector<unsigned int> Cell2DsNumVertices = {}; //numero di vertici, dimensione: 1 x NumCell2Ds
	vector<array<unsigned int, 20>> Cell2DsVertices = {}; //vettore dei vertici di dimensione massima
	vector<unsigned int> Cell2DsNumEdges = {}; //numero di lati, dimensione: 1 x NumCell2Ds
	vector<array<unsigned int, 30>> Cell2DsEdges = {}; //vettore dei lati di dimensione massima
	
	unsigned int NumCell3Ds = 0; //poliedri
	vector<unsigned int> Cell3DsId = {}; //Id, dimensione: 1 x NumCell3Ds
	vector<unsigned int> Cell3DsNumVertices = {}; //numero di vertici, dimensione: 1 x NumCell3Ds
	vector<array<unsigned int, 20>> Cell3DsVertices = {}; //vettore dei vertici di dimensione massima
	vector<unsigned int> Cell3DsNumEdges = {}; //numero di lati, dimensione: 1 x NumCell3Ds
	vector<array<unsigned int, 30>> Cell3DsEdges = {}; //vettore di lati di dimensione massima
	vector<unsigned int> Cell3DsNumFaces = {}; //numero di facce, dimensione 1 x NumCell3Ds
	vector<array<unsigned int, 20>> Cell3DsFaces = {}; //vettore di facce di dimensione massima
	
};

}