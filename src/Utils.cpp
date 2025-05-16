#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

namespace PoliedriMesh
{
/*bool ImportMesh(PolygonalMesh& mesh)
{
	if(!ImportCell0Ds(mesh))
		return false;

	if(!ImportCell1Ds(mesh))
		return false;

	if(!ImportCell2Ds(mesh))
		return false;
	
	if(!ImportCell3Ds(mesh))
		return false;

	return true;
}*/
//******************************************************************
/*bool Cell0Ds(PoliedriMesh& mesh)
{
	ofstream file("./Cell0Ds.txt")
	if(file.fail())
	{
		cerr << "Impossibile aprire il file in scrittura" << endl;
		return false;
	}
	
	
	
}*/

PoliedriMesh creaTetraedroNormalizzato() {
    PoliedriMesh mesh;

    vector<Vector3d> vertici = {
        {1.0, 1.0, 1.0},
        {-1.0, -1.0, 1.0},
        {-1.0, 1.0, -1.0},
        {1.0, -1.0, -1.0}
    };

    mesh.NumCell0Ds = vertici.size();
    mesh.Cell0DsId.resize(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates.resize(3, mesh.NumCell0Ds);

    for (unsigned int i = 0; i < vertici.size(); ++i) {
        double x = vertici ;
        double y = vertici ;
        double z = vertici ;
        double x_n = norm(x);
		double y_n = norm(y);
		double z_n = norm(z);
        mesh.Cell0DsId[i] = idOffset + i;
        mesh.Cell0DsCoordinates(0, i) = x;
        mesh.Cell0DsCoordinates(1, i) = y;
        mesh.Cell0DsCoordinates(2, i) = z;
    }

    return mesh;
}


}
