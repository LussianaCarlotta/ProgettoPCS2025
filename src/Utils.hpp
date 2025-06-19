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
	
	// Riempimento delle celle 3Ds
	// mesh: PoliedriMesh struct
	// forza la stampa dei valori delle celle 3Ds dei solidi nel file Cell3Ds.txt
	void AggiungiCelle3Ds(PoliedriMesh& mesh);
	
	// Scrittura delle celle
	// mesh: PoliedriMesh struct
	// viene restituito il risultato dell'apertura in scrittura: true se ha avuto successo, false altrimenti
	bool ScritturaCelle(const PoliedriMesh& mesh, const string& nomeBase);
		
	// Costruzione della PoliedriMesh duale
	// mesh: PoliedriMesh struct
	// prende in input la mesh di partenza e la dualizza
	void CostruisciDualMesh(const PoliedriMesh& meshOriginale, PoliedriMesh& meshDuale);
	
	// Esportazione della PoliedriMesh
	// mesh: PoliedriMesh struct
	// prende in input la mesh e le celle per la stampa su Paraview
	void Export(const PoliedriMesh& mesh, const string& nomeBase);

}