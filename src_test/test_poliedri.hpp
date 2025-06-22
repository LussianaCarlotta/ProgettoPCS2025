#pragma once

#include <iostream>
#include <vector>
#include <Eigen/Dense>

#include <gtest/gtest.h>
#include "PoliedriMesh.hpp"
#include "Triangolazione.hpp"
#include "Duale.hpp"
#include "CamminoMinimo.hpp"

namespace PoliedriLibrary {
	
// Test sui vertici
unsigned int NumCell0Ds = 0;
std::vector<unsigned int> Cell0DsId;
Eigen::MatrixXd Cell0DsCoordinates;

void inizializzaVertici(unsigned int num) {
    NumCell0Ds = num;
    Cell0DsId.resize(num);
    Cell0DsCoordinates = Eigen::MatrixXd(3, num);

    for (unsigned int i = 0; i < num; ++i) {
        Cell0DsId[i] = i;
        Cell0DsCoordinates(0, i) = i * 1.0; // componente x
        Cell0DsCoordinates(1, i) = i * 2.0; // componente y
        Cell0DsCoordinates(2, i) = i * 3.0; // componente z
    }
}

TEST(Cell0DsTest, Inizializzazione) {
    inizializzaVertici(3);

    EXPECT_EQ(NumCell0Ds, 3);
    EXPECT_EQ(Cell0DsId.size(), 3);
    EXPECT_EQ(Cell0DsCoordinates.cols(), 3);
    EXPECT_EQ(Cell0DsCoordinates.rows(), 3);

    for (unsigned int i = 0; i < 3; ++i) {
        EXPECT_EQ(Cell0DsId[i], i);
        EXPECT_DOUBLE_EQ(Cell0DsCoordinates(0, i), i * 1.0);
        EXPECT_DOUBLE_EQ(Cell0DsCoordinates(1, i), i * 2.0);
        EXPECT_DOUBLE_EQ(Cell0DsCoordinates(2, i), i * 3.0);
    }
}


// Test sui lati
unsigned int NumCell1Ds = 0;
std::vector<unsigned int> Cell1DsId;
Eigen::MatrixXi Cell1DsExtrema;

void inizializzaLati(unsigned int num) {
    NumCell1Ds = num;
    Cell1DsId.resize(num);
    Cell1DsExtrema = Eigen::MatrixXi(2, num);

    for (unsigned int i = 0; i < num; ++i) {
        Cell1DsId[i] = i;
        Cell1DsExtrema(0, i) = i; // origine
        Cell1DsExtrema(1, i) = i + 1; // fine
    }
}

TEST(Cell1DsTest, Inizializzazione) {
    inizializzaLati(4);

    EXPECT_EQ(NumCell1Ds, 4);
    EXPECT_EQ(Cell1DsId.size(), 4);
    EXPECT_EQ(Cell1DsExtrema.cols(), 4);
    EXPECT_EQ(Cell1DsExtrema.rows(), 2);

    for (unsigned int i = 0; i < 4; ++i) {
        EXPECT_EQ(Cell1DsId[i], i);
        EXPECT_EQ(Cell1DsExtrema(0, i), i);
        EXPECT_EQ(Cell1DsExtrema(1, i), i + 1);
    }
}


// Test sulle facce
unsigned int NumCell2Ds = 0;
std::vector<unsigned int> Cell2DsId;
std::vector<unsigned int> Cell2DsNumVertices;
std::vector<std::vector<unsigned int>> Cell2DsVertices;
std::vector<unsigned int> Cell2DsNumEdges;
std::vector<std::vector<unsigned int>> Cell2DsEdges;

void inizializzaFacce(unsigned int num) {
    NumCell2Ds = num;
    Cell2DsId.resize(num);
    Cell2DsNumVertices.resize(num);
    Cell2DsVertices.resize(num);
    Cell2DsNumEdges.resize(num);
    Cell2DsEdges.resize(num);

    for (unsigned int i = 0; i < num; ++i) {
        Cell2DsId[i] = i;
        Cell2DsNumVertices[i] = 3 + i; // es. 3, 4, 5...
        Cell2DsNumEdges[i] = 3 + i;

        for (unsigned int j = 0; j < Cell2DsNumVertices[i]; ++j) {
            Cell2DsVertices[i].push_back(j + i); // valori fittizi
        }
        for (unsigned int j = 0; j < Cell2DsNumEdges[i]; ++j) {
            Cell2DsEdges[i].push_back(j + i); // valori fittizi
        }
    }
}

TEST(Cell2DsTest, Inizializzazione) {
    inizializzaFacce(3);

    EXPECT_EQ(NumCell2Ds, 3);
    EXPECT_EQ(Cell2DsId.size(), 3);
    EXPECT_EQ(Cell2DsNumVertices.size(), 3);
    EXPECT_EQ(Cell2DsVertices.size(), 3);
    EXPECT_EQ(Cell2DsNumEdges.size(), 3);
    EXPECT_EQ(Cell2DsEdges.size(), 3);

    for (unsigned int i = 0; i < 3; ++i) {
        EXPECT_EQ(Cell2DsId[i], i);
        EXPECT_EQ(Cell2DsVertices[i].size(), Cell2DsNumVertices[i]);
        EXPECT_EQ(Cell2DsEdges[i].size(), Cell2DsNumEdges[i]);

        for (unsigned int j = 0; j < Cell2DsNumVertices[i]; ++j) {
            EXPECT_GE(Cell2DsVertices[i][j], i);  // verifica che Cell2DsVertices[i][j] >= i
        }
        for (unsigned int j = 0; j < Cell2DsNumEdges[i]; ++j) {
            EXPECT_GE(Cell2DsEdges[i][j], i);  // verifica che Cell2DsEdges[i][j] >= i
        }
    }
}


// Test sui poliedri
unsigned int NumCell3Ds = 0;
std::vector<unsigned int> Cell3DsId;
std::vector<unsigned int> Cell3DsNumVertices;
std::vector<std::vector<unsigned int>> Cell3DsVertices;
std::vector<unsigned int> Cell3DsNumEdges;
std::vector<std::vector<unsigned int>> Cell3DsEdges;
std::vector<unsigned int> Cell3DsNumFaces;
std::vector<std::vector<unsigned int>> Cell3DsFaces;

void inizializzaPoliedri(unsigned int num) {
	NumCell3Ds = num;
	Cell3DsId.resize(num);
	Cell3DsNumVertices.resize(num);
	Cell3DsVertices.resize(num);
	Cell3DsNumEdges.resize(num);
	Cell3DsEdges.resize(num);
	Cell3DsNumFaces.resize(num);
	Cell3DsFaces.resize(num);
	
	for(unsigned int i = 0; i < num; ++i) {
		Cell3DsId[i] = i;
		Cell3DsNumVertices[i] = 4;
		Cell3DsVertices[i] = {0, 1, 2, 3};
		Cell3DsNumEdges[i] = 6;
		Cell3DsEdges[i] = {0, 1, 2, 3, 4, 5};
		Cell3DsNumFaces[i] = 4;
		Cell3DsFaces[i] = {0, 1, 2, 3};
	}
}

TEST(Cell3DsTest, Inizializzazione) {
	inizializzaPoliedri(5); // num è arbitrario e indica il numero di poliedri da testare
	
	EXPECT_EQ(NumCell3Ds, 5);
    EXPECT_EQ(Cell3DsId.size(), 5);
    EXPECT_EQ(Cell3DsNumVertices.size(), 5);
    EXPECT_EQ(Cell3DsVertices.size(), 5);
    EXPECT_EQ(Cell3DsNumEdges.size(), 5);
    EXPECT_EQ(Cell3DsEdges.size(), 5);
    EXPECT_EQ(Cell3DsNumFaces.size(), 5);
    EXPECT_EQ(Cell3DsFaces.size(), 5);
	
	for(unsigned int i = 0; i < 5; ++i) {
		EXPECT_EQ(Cell3DsId[i], i);
        EXPECT_EQ(Cell3DsNumVertices[i], Cell3DsVertices[i].size());
        EXPECT_EQ(Cell3DsNumEdges[i], Cell3DsEdges[i].size());
        EXPECT_EQ(Cell3DsNumFaces[i], Cell3DsFaces[i].size());
	}
}


// Test sulla costruzione dei solidi geodetici di classe 1

//Test sulla corretta suddivisione in triangoli di una faccia con più di tre vertici
TEST(TriangolazioneITest, TriangolaFacceNonTriangolari) {
	PoliedriMesh mesh;
	mesh.Cell2DsVertices = {{0, 1, 2, 3}};
	mesh.Cell0DsCoordinates.resize(3, 4); // allocazione di memoria per 4 vertici
	
	TriangolaFacceNonTriangolari(mesh);
	
	EXPECT_EQ(mesh.NumCell2Ds, 2); // suddivisione di un quadrilatero in due triangoli
	EXPECT_EQ(mesh.Cell2DsVertices.size(), 2);
	for(const auto& faccia : mesh.Cell2DsVertices) {
		EXPECT_EQ(faccia.size(), 3); // ogni faccia deve essere triangolare
	}
}

// Test sulla triangolazione con livello di suddivisione uguale a 1
TEST(TriangolazioneITest, TriangolaFacceClasseI_livello1) {
	PoliedriMesh meshIniziale;
	meshIniziale.Cell0DsCoordinates.resize(3, 3);
	meshIniziale.Cell0DsCoordinates << 0, 1, 0,
									   0, 0, 1,
									   0, 0, 0;
	meshIniziale.Cell2DsVertices = {{0, 1, 2}};
	
	PoliedriMesh meshRisultato;
	TriangolaFacceClasseI(meshIniziale, meshRisultato, 1);
	
	EXPECT_EQ(meshRisultato.NumCell2Ds, 1); // il livello di suddivisione 1 inidica che c'è un solo triangolo
	EXPECT_EQ(meshRisultato.Cell2DsVertices.size(), 1);
	EXPECT_EQ(meshRisultato.Cell2DsVertices[0].size(), 3);
}

// Test sulla triangolazione con livello di suddivisione uguale o superiore a 2
TEST(TriangolazioneITest, TriangolaFacceClasseI_livello2) {
	PoliedriMesh meshIniziale;
	meshIniziale.Cell0DsCoordinates.resize(3, 3);
	meshIniziale.Cell0DsCoordinates << 0, 1, 0,
									   0, 0, 1,
									   0, 0, 0;
	meshIniziale.Cell2DsVertices = {{0, 1, 2}};
	
	PoliedriMesh meshRisultato;
	TriangolaFacceClasseI(meshIniziale, meshRisultato, 2);
	
	EXPECT_GT(meshRisultato.NumCell2Ds, 1);  // verifica che meshRisultato.NumCell2Ds > 1, cioè verifica che ci siano più triangoli
	EXPECT_GT(meshRisultato.NumCell0Ds, 3);  // verifica che meshRisultato.NumCell0Ds > 3, cioè verifica che ci siano più di 3 vertici
}

// Test nel caso in cui il livello di suddivisione sia uguale a 0
TEST(TriangolazioneITest, TriangolaFacceClasseI_livello0) {
	PoliedriMesh meshIniziale;
	meshIniziale.Cell0DsCoordinates.resize(3, 3);
	meshIniziale.Cell2DsVertices = {{0, 1, 2}};
	
	PoliedriMesh meshRisultato;
	TriangolaFacceClasseI(meshIniziale, meshRisultato, 0);
	
	EXPECT_EQ(meshRisultato.NumCell2Ds, 0); // nessuna triangolazione
}


// Test sulla costruzione dei solidi geodetici di classe 2

// Test sulla triangolazione con livello di suddivisione b = 1
TEST(TriangolazioneIITest, TriangolaFacceClasseII_livello1) {
	PoliedriMesh meshIniziale;
	meshIniziale.Cell0DsCoordinates.resize(3, 3);
	meshIniziale.Cell0DsCoordinates << 0, 1, 0,
									   0, 0, 1,
									   0, 0, 0;
	meshIniziale.Cell2DsVertices = {{0, 1, 2}};
	
	PoliedriMesh meshTipoI;
	TriangolaFacceClasseI(meshIniziale, meshTipoI, 1);
	
	PoliedriMesh meshRisultato;
	TriangolaFacceClasseII(meshTipoI, meshRisultato, 1);
	
	EXPECT_EQ(meshTipoI.NumCell2Ds, 1);
	EXPECT_GT(meshRisultato.NumCell0Ds, 3);  // servono più di 3 vertici per ottenere i 6 triangoli (si rimanda al pdf del progetto per la visione grafica)
	EXPECT_GE(meshRisultato.NumCell2Ds, 6);  // si generano almeno 6 triangoli eseguendo una suddivisione baricentrica
}

// Test sulla triangolazione con livello di suddivisione b = 2
TEST(TriangolazioneIITest, TriangolaFacceClasseII_livello2) {
	PoliedriMesh meshIniziale;
	meshIniziale.Cell0DsCoordinates.resize(3, 3);
	meshIniziale.Cell0DsCoordinates << 0, 1, 0,
									   0, 0, 1,
									   0, 0, 0;
	meshIniziale.Cell2DsVertices = {{0, 1, 2}};
	
	PoliedriMesh meshTipoI;
	TriangolaFacceClasseI(meshIniziale, meshTipoI, 2);
	
	PoliedriMesh meshRisultato;
	TriangolaFacceClasseII(meshTipoI, meshRisultato, 2);
	
	EXPECT_EQ(meshTipoI.NumCell2Ds, 4);  // la triangolazione di classe 1 con b = 2 genera 4 triangoli
	EXPECT_GT(meshRisultato.NumCell0Ds, 3);  // servono più di 3 vertici per ottenere i 6 triangoli (si rimanda al pdf del progetto per la visione grafica)
	EXPECT_GE(meshRisultato.NumCell2Ds, 24);  // si generano almeno 6*4 triangoli eseguendo una suddivisione baricentrica
}

// Test sulla triangolazione con livello di suddivisione b = 0 (caso non valido)
TEST(TriangolazioneIITest, TriangolaFacceClasseII_livello0) {
	PoliedriMesh meshIniziale;
	meshIniziale.Cell0DsCoordinates.resize(3, 3);
	meshIniziale.Cell2DsVertices = {{0, 1, 2}};
	
	PoliedriMesh meshRisultato;
	TriangolaFacceClasseII(meshIniziale, meshRisultato, 0);
	
	EXPECT_EQ(meshRisultato.NumCell2Ds, 0);  // nessuna triangolazione
}

// Test sulla coerenza dei triangoli generati
TEST(TriangolazioneIITest, ValiditàTriangoli) {
	PoliedriMesh meshIniziale;
	meshIniziale.Cell0DsCoordinates.resize(3, 3);
	meshIniziale.Cell2DsVertices = {{0, 1, 2}};
	
	PoliedriMesh meshRisultato;
	TriangolaFacceClasseII(meshIniziale, meshRisultato, 1);
	
	for(const auto& faccia : meshRisultato.Cell2DsVertices) {
		EXPECT_EQ(faccia.size(), 3);  // ogni faccia deve avere esattamente 3 vertici
	}
	for(const auto& lati : meshRisultato.Cell2DsEdges) {
		EXPECT_EQ(lati.size(), 3);  // ogni faccia deve avere esattamente 3 lati
	}
}

// Test sull'incremento dei vertici, dato che si introducono nuovi punti (baricentro e punti medi dei lati), con livello di suddivisione b = 2
TEST(TriangolazioneIITest, AumentoVertici) {
	PoliedriMesh meshIniziale;
	meshIniziale.Cell0DsCoordinates.resize(3, 3);
	meshIniziale.Cell0DsCoordinates << 0, 1, 0,
                                       0, 0, 1,
                                       0, 0, 0;
	meshIniziale.Cell2DsVertices = {{0, 1, 2}};
	
	PoliedriMesh meshTipoI;
	TriangolaFacceClasseI(meshIniziale, meshTipoI, 2);
	
	PoliedriMesh meshRisultato;
	TriangolaFacceClasseII(meshTipoI, meshRisultato, 2);
	
	EXPECT_GT(meshRisultato.NumCell0Ds, meshIniziale.Cell0DsCoordinates.cols());  // verifica che il numero di vertici della meshRisultato siano maggiore del numero di vertici della meshIniziale
}


// Test sulla costruzione dei duali
TEST(DualeTest, DualeTriangolo) {
	PoliedriMesh mesh;
	mesh.Cell0DsCoordinates.resize(3, 3);
	mesh.Cell0DsCoordinates << 0, 1, 0,
							   0, 0, 1,
							   0, 0, 0;
	mesh.Cell2DsVertices = {{0, 1, 2}};
	
	PoliedriMesh meshDuale;
	CostruisciDualMesh(mesh, meshDuale);
	
	EXPECT_EQ(meshDuale.NumCell0Ds, 1);  // deve esserci un baricentro
	EXPECT_EQ(meshDuale.NumCell1Ds, 0);  // non devono esserci lati condivisi
	EXPECT_EQ(meshDuale.NumCell2Ds, 0);  // non deve esserci nessuna faccia
	EXPECT_EQ(meshDuale.NumCell3Ds, 1);  // deve esserci sono una cella 3D
}

TEST(DualeTest, DualeTriangoliAdiacenti) {
	PoliedriMesh mesh;
	mesh.Cell0DsId = {0, 1, 2, 3};
	mesh.Cell0DsCoordinates.resize(3, 4);
	mesh.Cell0DsCoordinates << 0, 1, 0, 1,
							   0, 0, 1, 1,
							   0, 0, 0, 0;
	mesh.Cell2DsId = {0, 1};
	mesh.Cell2DsVertices = {{0, 1, 2}, {1, 3, 2}};
	
	PoliedriMesh meshDuale;
	CostruisciDualMesh(mesh, meshDuale);
	
	EXPECT_EQ(meshDuale.NumCell0Ds, 2);  // devono esserci 2 baricentri
	EXPECT_EQ(meshDuale.NumCell1Ds, 1);  // deve esserci un lato condiviso
}


// Test sulla proiezione dei vertici sulla sfera
TEST(DualeTest, ProiezioneSuSfera) {
	PoliedriMesh mesh;
	mesh.Cell0DsCoordinates.resize(3, 1);
	mesh.Cell0DsCoordinates << 3, 0, 0;
	
	ProiettaSuSfera(mesh);
	
	double norma = mesh.Cell0DsCoordinates.col(0).norm();
	
	EXPECT_NEAR(norma, 1.0, 1e-9);  // verifica che il punto sia sulla sfera unitaria
}


// Test sul cammino minimo

// Test sulla costruzione del grafo
TEST(CamminoMinimoTest, CostruzioneGrafo){
	PoliedriMesh mesh;
	mesh.NumCell0Ds = 4;
	mesh.NumCell1Ds = 3;
	mesh.Cell0DsCoordinates.resize(3, 4);
	mesh.Cell0DsCoordinates << 0, 1, 1, 0,
							   0, 0, 1, 1,
							   0, 0, 0, 0;
	mesh.Cell1DsExtrema.resize(2, 3);
	mesh.Cell1DsExtrema << 0, 1, 2,
						   1, 2, 3;
						   
	ShortestPath sp(mesh);
	auto& lista = sp.lista_adiacenza;
	
	ASSERT_EQ(lista.size(), 4);
	EXPECT_EQ(lista[0].size(), 1);  // l'elemento 0 è connesso con 1
	EXPECT_EQ(lista[1].size(), 2);  // l'elemento 1 è connesso con 0 e 2
	EXPECT_EQ(lista[2].size(), 2);  // l'elemento 2 è connesso con 1 e 3
	EXPECT_EQ(lista[3].size(), 1);  // l'elemento 3 è connesso con 2
}

// Test sul calcolo della distanza
TEST(CamminoMinimoTest, CalcolaDistanza) {
	PoliedriMesh mesh;
	mesh.Cell0DsCoordinates.resize(3, 4);
	mesh.Cell0DsCoordinates << 0, 1, 1, 0,
							   0, 0, 1, 1,
							   0, 0, 0, 0;
	mesh.Cell1DsExtrema.resize(2, 3);
	mesh.Cell1DsExtrema << 0, 1, 2,
						   1, 2, 3;
						   
	ShortestPath sp(mesh);
	double dist = sp.CalcolaDistanza(0, 1);
	
	EXPECT_NEAR(dist, 1.0, 1e-6);
}

// Test su un grafo semplice
TEST(CamminoMinimoTest, GrafoSemplice) {
	PoliedriMesh mesh;
	mesh.Cell0DsCoordinates.resize(3, 3);
	mesh.Cell0DsCoordinates << 0, 1, 2,
							   0, 0, 0,
							   0, 0, 0;
	mesh.Cell1DsExtrema.resize(2, 2);
	mesh.Cell1DsExtrema << 0, 1,
						   1, 2;
	mesh.NumCell0Ds = 3;
	mesh.NumCell1Ds = 2;
	
	ShortestPath sp(mesh);
	double lunghezza = 0.0;
	auto cammino = sp.CalcolaShortPath(0, 2, lunghezza);
	
	std::vector<unsigned int> expected = {0, 1, 2};
	EXPECT_EQ(cammino, expected);
	EXPECT_NEAR(lunghezza, 2.0, 1e-9);
}

// Test sul cammino minimo inesistente
TEST(CamminoMinimoTest, CamminoMinimoInesistente) {
	PoliedriMesh mesh;
	mesh.Cell0DsCoordinates.resize(3, 2);
	mesh.Cell0DsCoordinates << 0, 1,
							   0, 1,
							   0, 0;
	mesh.Cell1DsExtrema.resize(2, 0);  // nessun lato
	mesh.NumCell0Ds = 2;
	mesh.NumCell1Ds = 0;
	
	ShortestPath sp(mesh);
	double lunghezza = 0.0;
	auto cammino = sp.CalcolaShortPath(0, 1, lunghezza);
	
	EXPECT_TRUE(cammino.empty());
	EXPECT_EQ(lunghezza, std::numeric_limits<double>::infinity());
}

// Test su cammino con un solo nodo
TEST(CamminoMinimoTest, StessoNodo) {
	PoliedriMesh mesh;
	mesh.Cell0DsCoordinates.resize(3, 4);
	mesh.Cell0DsCoordinates << 0, 1, 1, 0,
							   0, 0, 1, 1,
							   0, 0, 0, 0;
	mesh.Cell1DsExtrema.resize(2, 3);
	mesh.Cell1DsExtrema << 0, 1, 2,
						   1, 2, 3;
	
	mesh.NumCell0Ds = 4;
	mesh.NumCell1Ds = 3;
	
	ShortestPath sp(mesh);
	double lunghezza = -1;
	auto cammino = sp.CalcolaShortPath(0, 0, lunghezza);
	
	std::vector<unsigned int> expected = {0};
	EXPECT_EQ(cammino, expected);
	EXPECT_EQ(lunghezza, 0.0);
}

// Test sulla marcatura corretta di vertici e lati
TEST(CamminoMinimoTest, MarcaturaCorretta) {
	PoliedriMesh mesh;
	mesh.Cell0DsCoordinates.resize(3, 3);
	mesh.Cell0DsCoordinates << 0, 1, 2,
							   0, 0, 0,
							   0, 0, 0;
	mesh.Cell1DsExtrema.resize(2, 2);
	mesh.Cell1DsExtrema << 0, 1,
						   1, 2;
	mesh.NumCell0Ds = 3;
	mesh.NumCell1Ds = 2;
	
	ShortestPath sp(mesh);
	double lunghezza = 0.0;
	auto cammino = sp.CalcolaShortPath(0, 2, lunghezza);
	sp.MarcaCammino(cammino);
	
	const auto& vertici = sp.getVerticiMarcati();
	const auto& lati = sp.getLatiMarcati();
	
	EXPECT_EQ(vertici[0], 1);
	EXPECT_EQ(vertici[1], 1);
	EXPECT_EQ(vertici[2], 1);
	EXPECT_EQ(lati[0], 1);
	EXPECT_EQ(lati[1], 1);
}

}
