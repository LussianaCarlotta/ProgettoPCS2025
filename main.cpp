#include "PoliedriMesh.hpp"
#include "Utils.hpp"
#include "Triangolazione.hpp"
#include "CamminoMinimo.hpp"
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
	if (p == 3 && q >= 3) {
		richiedeDualizzazioneFinale = false;
	}
	else {
		swap(p, q);
		richiedeDualizzazioneFinale = true;
	}

	if (!ImportMesh(meshPlatonico, p, q)) {
        cerr << "Errore nell'importazione della mesh." << endl;
        return 1;
    }
	
	// cout << "[DEBUG] Numero facce nella mesh iniziale: " << meshPlatonico.Cell2DsVertices.size() << endl;
	
	TriangolaFacceNonTriangolari(meshPlatonico);

	// Triangolazione 
    PoliedriMesh meshTriangolata;
	if (b == c && b > 0) {
		// Classe II: b = c
		TriangolaFacceClasseII(meshPlatonico, meshTriangolata, b);
	}
	else if ((b == 0 && c > 0) || (c == 0 && b > 0)) {
		// Classe I: uno dei due è 0
		TriangolaFacceClasseI(meshPlatonico, meshTriangolata, b + c);
	}
	else {
		cerr << "Errore: combinazione b != c non supportata" << endl;
		return 1;
	}


    // Duale della mesh triangolata (Goldberg)
    if (richiedeDualizzazioneFinale == true) {
        PoliedriMesh meshDuale;
        CostruisciDualMesh(meshTriangolata, meshDuale);
        meshTriangolata = meshDuale;
    }
	ProiettaSuSfera(meshTriangolata);


	// Cammino minimo
	unsigned int id1, id2;
	cout << "Inserisci ID del primo vertice: ";
	cin >> id1;
	cout << "Inserisci ID del secondo vertice: ";
	cin >> id2;

    if (id1 >= meshTriangolata.NumCell0Ds || id2 >= meshTriangolata.NumCell0Ds) {
        cerr << "Errore: ID vertici non validi." << endl;
        return 1;
    }

    ShortestPath TrovaCammino(meshTriangolata);
    double lunghezzaTotale = 0.0;
    auto cammino = TrovaCammino.CalcolaShortPath(id1, id2, lunghezzaTotale);
    TrovaCammino.MarcaCammino(cammino);

	meshTriangolata.Cell0DsShortPath = TrovaCammino.getVerticiMarcati();
	meshTriangolata.Cell1DsShortPath = TrovaCammino.getLatiMarcati();

    cout << "Numero di lati: " << cammino.size() - 1 << endl;
    cout << "Lunghezza totale: " << lunghezzaTotale << endl;

    // Applica i marcatori alla mesh
    const auto& verticiMarcati = TrovaCammino.getVerticiMarcati();
    const auto& latiMarcati = TrovaCammino.getLatiMarcati();

    for(unsigned int i = 0; i < meshTriangolata.NumCell0Ds; ++i) {
        meshTriangolata.Cell0DsId[i] = verticiMarcati[i];
	}

    for(unsigned int i = 0; i < meshTriangolata.NumCell1Ds; ++i) {
        meshTriangolata.Cell1DsId[i] = latiMarcati[i];
	}
	
	string nomeBaseOutput;
	if (richiedeDualizzazioneFinale == true) {
		nomeBaseOutput = "Goldberg"; // se è stato costruito il duale
	}
	else {
		nomeBaseOutput = "Geodetico"; //se la triangolazione non ha richiesto il duale
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
