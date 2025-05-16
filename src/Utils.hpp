#pragma once
#include <iostream>
#include "PoliedriMesh.hpp"

using namespace std;

namespace PoliedriLibrary 
{
	// Importazione della PoliedriMesh
	// mesh: PoliedriMesh struct
	// viene restituito il risultato della lettura: true se ha avuto successo, false altrimenti
	bool ImportMesh(PoliedriMesh& mesh);
	
	// Importazione delle proprietà di Cell0Ds
	// mesh: PoliedriMesh struct
	// viene restituito il risultato della scrittura: true se ha avuto successo, false altrimenti
	bool Cell0Ds(PoliedriMesh& mesh);

/*	
	// Importazione delle proprietà di Cell1Ds
	// mesh: PoliedriMesh struct
	// viene restituito il risultato della scrittura: true se ha avuto successo, false altrimenti
	bool Cell1Ds(PoliedriMesh& mesh);
	
	// Importazione delle proprietà di Cell2Ds
	// mesh: PoliedriMesh struct
	// viene restituito il risultato della scrittura: true se ha avuto successo, false altrimenti
	bool Cell2Ds(PoliedriMesh& mesh);
	
	// Importazione delle proprietà di Cell3Ds
	// mesh: PoliedriMesh struct
	// viene restituito il risultato della scrittura: true se ha avuto successo, false altrimenti
	bool Cell3Ds(PoliedriMesh& mesh);
	
	
*/

	void Export(PoliedriMesh& mesh);
}