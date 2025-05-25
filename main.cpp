#include <iostream>
#include "PoliedriMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"
#include "Triangolazione.hpp"

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
	cout << "Inserisci i parametri p, q : ";
    cin >> p >> q;
	
	if (!ImportMesh(mesh, p, q)) {
		cerr << "Errore nell'importazione della mesh" << endl;
    return 1;
	}
	
	if (!ScritturaCelle(mesh, p, q)) {
        cerr << "Errore nella scrittura delle celle" << endl;
        return 1;
    }


    // Esportazione
    Export(mesh);

    return 0;
}
