#include "PoliedriMesh.hpp"
#include "Utils.hpp"
#include "Triangolazione.hpp"
#include "Duale.hpp"
#include <set>
#include <iostream>
#include <vector>

using namespace std;
using namespace PoliedriLibrary;

int main() {

    PoliedriMesh meshPlatonico;

    unsigned int p, q;
    cout << "Inserisci i parametri p, q : ";
    cin >> p >> q;

	unsigned int b, c;
	cout << "Inserisci il parametro di triangolazione b: ";
	cin >> b;
	cout << "Inserisci il parametro di triangolazione c: ";
	cin >> c;

	if (b == 0 && c == 0) {
		cerr << "Errore: almeno uno tra b o c deve essere maggiore di 0." << endl;
		return 1;
	}
	
	bool richiedeDualizzazioneFinale; 
	if (p == 3 && q > 3) {
		richiedeDualizzazioneFinale = false;
	} else {
		swap(p, q);
		richiedeDualizzazioneFinale = true;
	}

	if (!ImportMesh(meshPlatonico, p, q)) {
        cerr << "Errore nell'importazione della mesh." << endl;
        return 1;
    }
	
	
	// Triangolazione geodetica
    PoliedriMesh meshTriangolata;
    TriangolaFacceClasseI(meshPlatonico, meshTriangolata, b + c);



    // Calcola il duale della mesh triangolata (Goldberg)
    if (richiedeDualizzazioneFinale == true) {
        PoliedriMesh meshDuale;
        CostruisciDualMesh(meshTriangolata, meshDuale);
        meshTriangolata = meshDuale;
		
    }
	ProiettaSuSfera(meshTriangolata);
	
	
	string nomeBaseOutput;
	if (richiedeDualizzazioneFinale == true) {
		nomeBaseOutput = "Goldberg";
	} else {
		nomeBaseOutput = "Geodetico";
	}

	if (!ScritturaCelle(meshTriangolata, nomeBaseOutput)) {
		cerr << "Errore nella scrittura dei file delle celle." << endl;
		return 1;
	}

	// Esporta per Paraview (solo vertici e lati)
	Export(meshTriangolata, nomeBaseOutput);

	cout << "Mesh di " << nomeBaseOutput << " esportata correttamente." << endl;


    return 0;
}
