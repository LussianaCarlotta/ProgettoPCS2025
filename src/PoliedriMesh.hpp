#pragma once 

#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <Eigen/Eigen>
#include <list>

using namespace std;
using namespace Eigen;

namespace PoliedriLibrary {

struct PoliedriMesh
{
	unsigned int NumCell0Ds = 0;
	vector<unsigned int> Cell0DsId = {};
	MatrixXd Cell0DsCoordinates = {};
	
	unsigned int NumCell1Ds = 0;
	vector<unsigned int> Cell1DsId = {};
	MatrixXi Cell1DsExtrema = {};
	
	unsigned int NumCell2Ds = 0;
	vector<unsigned int> Cell2DsId = {};
	
// da modificare 
	unordered_map<unsigned int, vector <unsigned int>> Cell2DsVertices;
	unordered_map<unsigned int, vector<unsigned int>> Cell2DsEdges;
 
};

}