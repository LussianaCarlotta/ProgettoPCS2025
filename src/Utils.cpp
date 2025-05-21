#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;
using namespace Eigen;

namespace PoliedriLibrary {


void normalizza(double& x, double& y, double& z) {
    double norma = sqrt(x * x + y * y + z * z);
    x /= norma;
    y /= norma;
    z /= norma;
} // norm non andava bene perchè non da il risultato desiderato, la funzione che lo fa è normalize ma non l'abbiamo mai utilizzata

// Celle0Ds
void AggiungiVertici(PoliedriMesh& mesh, const vector<Vector3d>& vertici) {
	unsigned int IdOffset = mesh.NumCell0Ds;
	mesh.NumCell0Ds += vertici.size();
    mesh.Cell0DsId.resize(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates.conservativeResize(3, mesh.NumCell0Ds);
    
    // mi prende i poligoni creati in creaPoliedriNormalizzati e me li proietta in una sfera
    for (unsigned int i = 0; i < vertici.size(); ++i) {
        double x = vertici[i](0);
        double y = vertici[i](1);
        double z = vertici[i](2);
        normalizza(x, y, z);

        mesh.Cell0DsId[IdOffset + i] = IdOffset + i; 
        mesh.Cell0DsCoordinates(0, IdOffset + i) = x;
        mesh.Cell0DsCoordinates(1, IdOffset + i) = y;
        mesh.Cell0DsCoordinates(2, IdOffset + i) = z;
    }
}

void creaPoliedriNormalizzati(PoliedriMesh& mesh) {
	// mi creo i vertici del poliedro standard
	//Tetraedro
    vector<Vector3d> verticitetraedro = {
        {1.0, 1.0, 1.0}, {-1.0, -1.0, 1.0}, {-1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}
    };
    AggiungiVertici(mesh, verticitetraedro);

	//Cubo
    vector<Vector3d> verticicubo = {
        {-1.0, -1.0, -1.0}, {-1.0, -1.0,  1.0}, {-1.0,  1.0, -1.0}, {-1.0,  1.0,  1.0},
        { 1.0, -1.0, -1.0}, { 1.0, -1.0,  1.0}, { 1.0,  1.0, -1.0}, { 1.0,  1.0,  1.0}
    };
    AggiungiVertici(mesh, verticicubo);

	//Ottaedro
    vector<Vector3d> verticiottaedro = {
        { 1.0,  0.0,  0.0}, {-1.0,  0.0,  0.0}, { 0.0,  1.0,  0.0},
        { 0.0, -1.0,  0.0}, { 0.0,  0.0,  1.0}, { 0.0,  0.0, -1.0}
    };
    AggiungiVertici(mesh, verticiottaedro);

	
	const double PHI = (1.0 + sqrt(5.0)) / 2.0; // calcolo sezione aurea, per costruire il dodecaedro e l'icosaedro 
	
	
	//Dodecaedro
    vector<Vector3d> verticidodecaedro = {
        {0, 1 / PHI, PHI}, {0, -1 / PHI, PHI}, {0, 1 / PHI, -PHI}, {0, -1 / PHI, -PHI},
        {1 / PHI, PHI, 0}, {-1 / PHI, PHI, 0}, {1 / PHI, -PHI, 0}, {-1 / PHI, -PHI, 0},
        {PHI, 0, 1 / PHI}, {-PHI, 0, 1 / PHI}, {PHI, 0, -1 / PHI}, {-PHI, 0, -1 / PHI},
        {1, 1, 1}, {-1, 1, 1}, {1, -1, 1}, {-1, -1, 1}, {1, 1, -1}, {-1, 1, -1}, {1, -1, -1}, {-1, -1, -1}
    };
    AggiungiVertici(mesh, verticidodecaedro);

	//Icosaedro
	vector<Vector3d> verticiicosaedro = {
        {0, PHI, 1}, {0, -PHI, 1}, {0, PHI, -1}, {0, -PHI, -1},
        {PHI, 1, 0}, {-PHI, 1, 0}, {PHI, -1, 0}, {-PHI, -1, 0},
        {1, 0, PHI}, {-1, 0, PHI}, {1, 0, -PHI}, {-1, 0, -PHI}
    };
    AggiungiVertici(mesh, verticiicosaedro);
}


//Celle1Ds
void aggiungiLato(PoliedriMesh& mesh, unsigned int& idLato, unsigned int id1, unsigned int id2) {
    mesh.Cell1DsExtrema.conservativeResize(2, idLato + 1);
    mesh.Cell1DsExtrema(0, idLato) = id1;
    mesh.Cell1DsExtrema(1, idLato) = id2;
    mesh.Cell1DsId.push_back(idLato);
    idLato++;
} 

void AggiungiLati(PoliedriMesh& mesh) {
    unsigned int idLato = 0;
    
    // o_ .... serve tenere traccia di dove iniziano i vertici di ciascun solido, per poterli collegare correttamente con lati e facce
    // Tetraedro
    unsigned int o_tetra = 0; 
    vector<pair<unsigned int, unsigned int>> lati_tetra = {
        {0,1},{0,2},{0,3},{1,2},{1,3},{2,3}
    };
    for (auto& l : lati_tetra)
        aggiungiLato(mesh, idLato, l.first + o_tetra, l.second + o_tetra);


    // Cubo
    unsigned int o_cubo = o_tetra + 4;
    vector<pair<unsigned int, unsigned int>> lati_cubo = {
        {0,1},{1,3},{3,2},{2,0},
        {4,5},{5,7},{7,6},{6,4},
        {0,4},{1,5},{2,6},{3,7}
    };
    for (auto& l : lati_cubo)
        aggiungiLato(mesh, idLato,l.first + o_cubo, l.second + o_cubo);

    // Ottaedro
    unsigned int o_otta = o_cubo + 8;
    vector<pair<unsigned int, unsigned int>> lati_otta = {
        {0,2},{0,3},{0,4},{0,5},
        {1,2},{1,3},{1,4},{1,5},
        {2,4},{2,5},{3,4},{3,5}
    };
    for (auto& l : lati_otta)
        aggiungiLato(mesh, idLato, l.first + o_otta, l.second + o_otta);

    // Dodecaedro
    unsigned int o_dode = o_otta + 6;
	vector<pair<unsigned int, unsigned int>> lati_dode = {
		{0,1},{0,4},{0,5},
		{1,7},{1,8},{1,9},
		{2,3},{2,6},{2,7},{2,11},{2,14},
		{3,9},{3,10},{3,19},
		{4,5},{4,7},{4,16},
		{5,6},{5,12},{5,0}, {5, 16},
		{6,13},{6,7},{6,12},
		{7,14},{7,13},
		{8,15},{8,17},
		{9,13},{9,16},{9,17},
		{10,14},{10,18},
		{11,12},{11,15},
		{12,15},
		{13,16},
		{14,15},{14,19},{14,9},
		{15,19},
		{16,17},
		{17,18},
		{18,19}
	};
    for (auto& l : lati_dode)
        aggiungiLato(mesh, idLato, l.first + o_dode, l.second + o_dode);

    // Icosaedro
	unsigned int o_icosa = o_dode + 20;
	vector<pair<unsigned int, unsigned int>> lati_icosa = {
		{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
		{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 1},
		{1, 6}, {2, 6},
		{2, 7}, {3, 7},
		{3, 8}, {4, 8},
		{4, 9}, {5, 9},
		{5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 6}
	};
	
	for (auto& l : lati_icosa)
		aggiungiLato(mesh, idLato, l.first + o_icosa, l.second + o_icosa);

    mesh.NumCell1Ds = idLato;
}

void aggiungiFaccia(
    PoliedriMesh& mesh,
    unsigned int& idFaccia,
    const std::vector<unsigned int>& vertici,
    const std::map<std::pair<unsigned int, unsigned int>, std::pair<unsigned int, bool>>& lato_da_vertici)
{
    mesh.Cell2DsId.push_back(idFaccia++);
    mesh.Cell2DsVertices.push_back(vertici);

    std::vector<unsigned int> lati;
    for (size_t i = 0; i < vertici.size(); ++i) {
        unsigned int a = vertici[i];
        unsigned int b = vertici[(i + 1) % vertici.size()];

        auto it = lato_da_vertici.find({a, b});
        if (it == lato_da_vertici.end()) {
            std::cerr << "Errore: lato mancante tra " << a << " e " << b << std::endl;
            std::exit(EXIT_FAILURE);
        }

        unsigned int idLato = it->second.first;
        bool orientato = it->second.second;

        // Se non è orientato correttamente, inverti l'ordine dei vertici nella faccia
        if (!orientato) {
            std::cerr << "Attenzione: orientamento errato tra " << a << " e " << b << std::endl;
            // Qui potresti anche correggere l’ordine dei vertici oppure fare debug.
        }

        lati.push_back(idLato);
    }

    mesh.Cell2DsEdges.push_back(lati);
}


void AggiungiFacce(PoliedriMesh& mesh) {

    unsigned int idFaccia = 0;

    // Mappa (dizionario) per cercare ID lato da 2 vertici
	map<pair<unsigned int, unsigned int>, pair<unsigned int, bool>> lato_da_vertici;    
	for (unsigned int i = 0; i < mesh.NumCell1Ds; ++i) {
        unsigned int v1 = mesh.Cell1DsExtrema(0, i);
        unsigned int v2 = mesh.Cell1DsExtrema(1, i);
        lato_da_vertici[{v1, v2}] = {i, true};
        lato_da_vertici[{v2, v1}] = {i, false};  // bidirezionale, da v1->v2, v2->v1
    }

    // TETRAEDRO 
    unsigned int o_tetra = 0;
    aggiungiFaccia(mesh, idFaccia, {o_tetra+0, o_tetra+1, o_tetra+2}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia, {o_tetra+0, o_tetra+1, o_tetra+3}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia, {o_tetra+0, o_tetra+2, o_tetra+3}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia, {o_tetra+1, o_tetra+2, o_tetra+3}, lato_da_vertici);

    // CUBO 
    unsigned int o_cubo = o_tetra + 4;
    aggiungiFaccia(mesh, idFaccia, {o_cubo+0, o_cubo+1, o_cubo+3, o_cubo+2}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia, {o_cubo+4, o_cubo+5, o_cubo+7, o_cubo+6}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia, {o_cubo+0, o_cubo+4, o_cubo+5, o_cubo+1}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia, {o_cubo+1, o_cubo+5, o_cubo+7, o_cubo+3}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia, {o_cubo+3, o_cubo+7, o_cubo+6, o_cubo+2}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia, {o_cubo+2, o_cubo+6, o_cubo+4, o_cubo+0}, lato_da_vertici);

    // OTTAEDRO
    unsigned int o_otta = o_cubo + 8;
    aggiungiFaccia(mesh, idFaccia, {o_otta+0, o_otta+2, o_otta+4}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia, {o_otta+0, o_otta+4, o_otta+3}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia, {o_otta+0, o_otta+3, o_otta+5}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia, {o_otta+0, o_otta+5, o_otta+2}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia, {o_otta+1, o_otta+2, o_otta+5}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia, {o_otta+1, o_otta+5, o_otta+3}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia, {o_otta+1, o_otta+3, o_otta+4}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia, {o_otta+1, o_otta+4, o_otta+2}, lato_da_vertici);

    // DODECAEDRO
    unsigned int o_dode = o_otta + 6;
    aggiungiFaccia(mesh, idFaccia, {o_dode+0, o_dode+1, o_dode+9, o_dode+16, o_dode+5}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia,{o_dode+1, o_dode+8, o_dode+17, o_dode+9}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia,{o_dode+8, o_dode+10, o_dode+18, o_dode+17}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia,{o_dode+10, o_dode+3, o_dode+19, o_dode+18}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia,{o_dode+3, o_dode+2, o_dode+14, o_dode+19}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia,{o_dode+2, o_dode+11, o_dode+15, o_dode+14}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia,{o_dode+11, o_dode+6, o_dode+12, o_dode+15}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia,{o_dode+6, o_dode+7, o_dode+13, o_dode+12}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia,{o_dode+7, o_dode+4, o_dode+16, o_dode+13}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia,{o_dode+4, o_dode+5, o_dode+0, o_dode+16}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia,{o_dode+5, o_dode+4, o_dode+7, o_dode+6}, lato_da_vertici);
    aggiungiFaccia(mesh, idFaccia,{o_dode+9, o_dode+17, o_dode+18, o_dode+19, o_dode+14}, lato_da_vertici);

    // ICOSAEDRO – 20 facce triangolari
	unsigned int o_icosa = o_dode + 20;
	
	aggiungiFaccia(mesh, idFaccia,{o_icosa+0, o_icosa+1, o_icosa+2}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+0, o_icosa+2, o_icosa+3}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+0, o_icosa+3, o_icosa+4}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+0, o_icosa+4, o_icosa+5}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+0, o_icosa+5, o_icosa+1}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+1, o_icosa+6, o_icosa+2}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+2, o_icosa+6, o_icosa+7}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+2, o_icosa+7, o_icosa+3}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+3, o_icosa+7, o_icosa+8}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+3, o_icosa+8, o_icosa+4}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+4, o_icosa+8, o_icosa+9}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+4, o_icosa+9, o_icosa+5}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+5, o_icosa+9, o_icosa+6}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+5, o_icosa+6, o_icosa+1}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+6, o_icosa+9, o_icosa+8}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+6, o_icosa+8, o_icosa+7}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+7, o_icosa+8, o_icosa+6}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+8, o_icosa+9, o_icosa+6}, lato_da_vertici);
	aggiungiFaccia(mesh, idFaccia,{o_icosa+9, o_icosa+6, o_icosa+7}, lato_da_vertici);

    mesh.NumCell2Ds = idFaccia;
}



bool ImportMesh(PoliedriMesh& mesh)
{
	mesh.NumCell0Ds = 0;
    mesh.Cell0DsId.clear();
    mesh.Cell0DsCoordinates.resize(3, 0);

	creaPoliedriNormalizzati(mesh);
	
	AggiungiLati(mesh);
	

	// Importa i vertici 
    if(!Cell0Ds(mesh))
        return false;

	// Importa gli spigoli
    if(!Cell1Ds(mesh))
        return false;
	
	
	// Importa le celle
    if(!Cell2Ds(mesh))
        return false;

    return true;

}


// Scrittura Cell0Ds.txt
bool Cell0Ds(PoliedriMesh& mesh) {
	ofstream file ("./Cell0Ds.txt");
	if (file.fail() ) {
		cerr << "Errore nell'apertura del file Cell0Ds.txt" << endl;
		return false;
	}
	

	for(unsigned int i = 0; i < mesh.NumCell0Ds; i++){
		file << mesh.Cell0DsId[i] << " " 
		<< mesh.Cell0DsCoordinates(0, i) << " " 
		<< mesh.Cell0DsCoordinates(1, i) << " " 
		<< mesh.Cell0DsCoordinates(2, i) << endl;
    	}
	file.close();
	return true;
	}

// Scrittura Cell1Ds.txt
bool Cell1Ds(PoliedriMesh& mesh) {
    ofstream file("Cell1Ds.txt");
    if (file.fail()) {
        cerr << "Errore nell'apertura del file Cell1Ds.txt" << endl;
        return false;
    }

    for (unsigned int i = 0; i < mesh.NumCell1Ds; ++i) {
        file << mesh.Cell1DsId[i] << " "
             << mesh.Cell1DsExtrema(0, i) << " "
             << mesh.Cell1DsExtrema(1, i) << endl;
    }

    file.close();
    return true;

}

// Scrittura Cell2Ds.txt
bool Cell2Ds(PoliedriMesh& mesh) {
    ofstream file("Cell2Ds.txt");
    if (file.fail()) {
        cerr << "Errore nell'apertura del file Cell2Ds.txt" << endl;
        return false;
    }

    for (unsigned int i = 0; i < mesh.NumCell2Ds; ++i) {
        file << mesh.Cell2DsId[i] << " ";

        for (auto v : mesh.Cell2DsVertices[i])
            file << v << " ";
        file << "| ";

        for (auto e : mesh.Cell2DsEdges[i])
            file << e << " ";
        file << endl;
    }

    file.close();
    return true;
}

// Scrittura Cell3Ds.txt
bool Cell3Ds(PoliedriMesh& mesh) {
    ofstream file("Cell3Ds.txt");
    if (file.fail()) {
        cerr << "Errore nell'apertura del file Cell3Ds.txt" << endl;
        return false;
    }

    file << 0 << " "; // ID del poliedro

    // Vertici
    file << mesh.NumCell0Ds << " ";
    for (auto id : mesh.Cell0DsId)
        file << id << " ";

    // Lati
    file << mesh.NumCell1Ds << " ";
    for (auto id : mesh.Cell1DsId)
        file << id << " ";

    // Facce
    file << mesh.NumCell2Ds << " ";
    for (auto id : mesh.Cell2DsId)
        file << id << " ";

    file << endl;
    file.close();
    return true;
}

}