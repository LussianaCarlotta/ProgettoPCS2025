#pragma once
#include "Utils.hpp"

namespace PoliedriLibrary {

	// Costruzione della meshTriangolata duale
	// mesh: PoliedriMesh struct
	// costruisce la mesh duale della mesh triangolata
	void CostruisciDualMesh(const PoliedriMesh& meshTriangolata, PoliedriMesh& meshDuale);
	
	// 
	// mesh: PoliedriMesh struct
	// crea una mappa di coppie e vettori avendo in input la meshTriangolata
	map<pair<unsigned int, unsigned int>, vector<unsigned int>> Mappa_LatoFaccia(const PoliedriMesh& meshTriangolata); //ex buildEdgesToFaces()
	
	// Proiezione sulla sfera unitaria
	// mesh: PoliedriMesh struct
	// fa in modo che la mesh ottenuta sia proiettata su una sfera unitaria
	void ProiettaSuSfera(PoliedriMesh& mesh);
}