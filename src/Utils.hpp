#pragma once
#include <iostream>
#include "PoliedriMesh.hpp"

using namespace std;

namespace PoliedriLibrary 
{
	// Importazione della PoliedriMesh
	// mesh: PoliedriMesh struct
	// viene restituito il risultato della lettura: true se ha avuto successo, false altrimenti
	bool ImportMesh(PoliedriMesh& mesh, unsigned int p, unsigned int q);

	bool ScritturaCelle(const PoliedriMesh& mesh, const string& nomeBase);
	
	void Export(const PoliedriMesh& mesh, const string& nomeBase);
	
	void CostruisciDualMesh(const PoliedriMesh& meshOriginale, PoliedriMesh& meshDuale);

}