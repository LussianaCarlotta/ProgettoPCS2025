#pragma once
#include "Utils.hpp"

namespace PoliedriLibrary {

	unsigned int TrovaSpigolo(map<pair<unsigned int, unsigned int>, unsigned int> &mappaSpigoli, PoliedriMesh &mesh, unsigned int start, unsigned int end);
	
	void TriangolaFacceClasseI(const PoliedriMesh &meshIniziale, PoliedriMesh &meshRisultato, unsigned int livelloSuddivisione); //cos'è il livelloSuddivisione?
	void TriangolaFacceClasseII(const PoliedriMesh &meshIniziale, PoliedriMesh &meshRisultato, unsigned int b);

}
