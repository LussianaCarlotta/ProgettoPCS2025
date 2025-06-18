#pragma once
#include "Utils.hpp"

namespace PoliedriLibrary {
	
	// Ricerca degli spigoli per la costruzione della PoliedriMesh duale
	// mesh: PoliedriMesh struct
	// restituisce l'indice associato agli spigoli della mesh dopo aver eseguito la triangolazione per costruire la mesh duale
	unsigned int TrovaSpigolo(map<pair<unsigned int, unsigned int>, unsigned int> &mappaSpigoli, PoliedriMesh &mesh, unsigned int start, unsigned int end);
	
	// Triangolazione delle facce dei solidi di classe 1
	// mesh: PoliedriMesh struct
	// restituisce una mesh triangola triangolando le facce del poliedro della meshIniziale secondo un livello di suddivisione regolare
	void TriangolaFacceClasseI(const PoliedriMesh& meshIniziale, PoliedriMesh& meshRisultato, unsigned int livelloSuddivisione);
	
	// Triangolazione delle facce dei solidi di classe 2
	// mesh: PoliedriMesh struct
	// restituisce una mesh triangolata triangolando le facce del poliedro della meshIniziale secondo il livello di suddivisione b
	void TriangolaFacceClasseII(const PoliedriMesh& meshIniziale, PoliedriMesh& meshRisultato, unsigned int b);
}
