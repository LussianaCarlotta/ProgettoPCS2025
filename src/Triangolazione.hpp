#pragma once
#include "Utils.hpp"

namespace PoliedriLibrary {

	void TriangolaFacceClasseI(const PoliedriMesh &meshIniziale, PoliedriMesh &meshRisultato, unsigned int livelloSuddivisione);

	unsigned int TrovaSpigolo(map<pair<unsigned int, unsigned int>, unsigned int> &mappaSpigoli, PoliedriMesh &mesh, unsigned int start, unsigned int end);

	void TriangolaFacceClasseII(const PoliedriMesh &, PoliedriMesh &, unsigned int);

}
