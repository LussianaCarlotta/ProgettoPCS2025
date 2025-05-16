#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;
using namespace Eigen;

namespace PoliedriLibrary {

void normalizza(double& x, double& y, double& z) {
    double norma = sqrt(x * x + y * y + z * z);
    x /= norma;
    y /= norma;
    z /= norma;
}

void AggiungiVertici(PoliedriMesh& mesh, const vector<Vector3d>& vertici) {
	unsigned int IdOffset = mesh.NumCell0Ds;
	mesh.NumCell0Ds += vertici.size();
    mesh.Cell0DsId.resize(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates.conservativeResize(3, mesh.NumCell0Ds);
    
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

	
	const double PHI = (1.0 + sqrt(5.0)) / 2.0;
	
	
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

bool ImportMesh(PoliedriMesh& mesh)
{
	mesh.NumCell0Ds = 0;
    mesh.Cell0DsId.clear();
    mesh.Cell0DsCoordinates.resize(3, 0);

	creaPoliedriNormalizzati(mesh);
	

	// Importa i vertici 
    if(!Cell0Ds(mesh))
        return false;
/*
	// Importa gli spigoli
    if(!Cell1Ds(mesh))
        return false;
	
	// Importa le celle
    if(!Cell2Ds(mesh))
        return false;
*/
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

/*
    // Scrittura Cell1Ds.txt
    std::ofstream file1("./Cell1Ds.txt");
    for (const auto& l : lati)
        file1 << l.id << " " << l.id_vertice_iniziale << " " << l.id_vertice_finale << "\n";
    file1.close();

    // Scrittura Cell2Ds.txt
    std::ofstream file2("./Cell2Ds.txt");
    for (const auto& f : facce) {
        file2 << f.id << " " << f.numero_vertici;
        for (int id_vertice : f.id_vertici) file2 << " " << id_vertice;
        file2 << " " << f.numero_lati;
        for (int id_lato : f.id_lati) file2 << " " << id_lato;
        file2 << "\n";
    }
    file2.close();

    // Scrittura Cell3Ds.txt
    std::ofstream file3("./Cell3Ds.txt");
    file3 << poliedro.id << " " << poliedro.numero_vertici;
    for (int id_vertice : poliedro.id_vertici) file3 << " " << id_vertice;
    file3 << " " << poliedro.numero_lati;
    for (int id_lato : poliedro.id_lati) file3 << " " << id_lato;
    file3 << " " << poliedro.numero_facce;
    for (int id_faccia : poliedro.id_facce) file3 << " " << id_faccia;
    file3 << "\n";
    file3.close();

    std::cout << "Tutti i file Cell0Ds.txt–Cell3Ds.txt sono stati generati.\n";
    return 0;
}*/


}
