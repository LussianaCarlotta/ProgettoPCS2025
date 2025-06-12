#pragma once
#include "Utils.hpp"

namespace PoliedriLibrary {
  
	void CostruisciDualMesh(const PoliedriMesh& meshTriangolata, PoliedriMesh& meshDuale);

	map<pair<unsigned int, unsigned int>, vector<unsigned int>> Mappa_LatoFaccia(const PoliedriMesh& meshTriangolata); //ex buildEdgesToFaces()
	
	void ProiettaSuSfera(PoliedriMesh& mesh);
}