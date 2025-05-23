#include <iostream>
#include "PoliedriMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"
#include <Eigen/Dense>
#include <vector>
#include <array>

using namespace PoliedriLibrary;
using namespace std;
using namespace Eigen;

int main() 
{
	PoliedriMesh mesh;
	
	unsigned int p, q;
	//c, b;

    cout << "Inserisci i parametri p, q : ";
    cin >> p >> q; //>> b >> c;
	
	if (!ScritturaCelle(mesh, p, q)) {
    cerr << "Errore nella scrittura dei file" << endl;
    return 1;
}
	
	 
	
	ImportMesh(mesh);
	
	// Crea oggetto esportatore
    Gedim::UCDUtilities esportatore;

    // Esporta i poliedri su file Paraview-compatible
    esportatore.ExportPolyhedra("output.inp", mesh.Cell0DsCoordinates, mesh.Cell3DsVertices, {}, {}, Eigen::VectorXi::Zero(mesh.NumCell3Ds));


}