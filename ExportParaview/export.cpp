#include <iostream>
#include "PoliedriMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"
#include <Eigen/Dense>
#include <vector>
#include <array>

using namespace std;
using namespace Eigen;

namespace PoliedriLibrary {

void Export (PoliedriMesh& mesh){
	/*	
	if(!ImportMesh(mesh, p, q))
	{
		cerr << "File non trovato" << endl;
	}*/
	
	Gedim::UCDUtilities utilities;
	
	utilities.ExportPoints("./Cell0Ds.inp", 
							mesh.Cell0DsCoordinates, {} );
	
	utilities.ExportSegments("./Cell1Ds.inp", 
							mesh.Cell0DsCoordinates, 
							mesh.Cell1DsExtrema, {}, {} );

}
}