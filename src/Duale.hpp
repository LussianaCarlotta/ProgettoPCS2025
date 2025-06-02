#pragma once
#include "Utils.hpp"

namespace PoliedriLibrary {
  
	void CostruisciDualMesh(const PoliedriMesh& meshTriangolata, PoliedriMesh& meshDuale);

	map<pair<unsigned int, unsigned int>, vector<unsigned int>> buildEdgeToFacesMap(const PoliedriMesh& meshTriangolata);
	
	void ProiettaSuSfera(PoliedriMesh& mesh);
}