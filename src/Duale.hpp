#pragma once
#include "Utils.hpp"

namespace PoliedriLibrary {

	// Costruzione della meshTriangolata duale
	// mesh: PoliedriMesh struct
	// costruisce la mesh duale della mesh triangolata ed è un grafo duale che ha come vertici i baricentri e come archi gli spigoli condivisi tra le facce
	void CostruisciDualMesh(const PoliedriMesh& meshTriangolata, PoliedriMesh& meshDuale);
	
	// Determinazione delle facce adiacenti per creare la mesh duale
	// mesh: PoliedriMesh struct
	// crea una mappa che associa ogni lato alle facce che lo condividono, al fine di conoscere le facce adiacenti
	map<pair<unsigned int, unsigned int>, vector<unsigned int>> Mappa_LatoFaccia(const PoliedriMesh& meshTriangolata); //ex buildEdgesToFaces()
	
	// Proiezione sulla sfera unitaria
	// mesh: PoliedriMesh struct
	// fa in modo che la mesh ottenuta sia proiettata su una sfera unitaria
	void ProiettaSuSfera(PoliedriMesh& mesh);
}