#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
namespace PoliedriMesh;
namespace std;

void normalizza(double& x, double& y, double& z) {
    double norma = sqrt(x * x + y * y + z * z);
    x /= norma;
    y /= norma;
    z /= norma;
}

PoliedriMesh creaTetraedroNormalizzato() {
    PoliedriMesh mesh;

    vector<Vector3d> vertici = {
        {1.0, 1.0, 1.0},
        {-1.0, -1.0, 1.0},
        {-1.0, 1.0, -1.0},
        {1.0, -1.0, -1.0}
    };

    mesh.NumCell0Ds = vertici.size();
    mesh.Cell0DsId.resize(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates.resize(3, mesh.NumCell0Ds);

    for (unsigned int i = 0; i < vertici.size(); ++i) {
        double x = vertici ;
        double y = vertici ;
        double z = vertici ;
        normalizza(x, y, z);
        mesh.Cell0DsId[i] = idOffset + i;
        mesh.Cell0DsCoordinates(0, i) = x;
        mesh.Cell0DsCoordinates(1, i) = y;
        mesh.Cell0DsCoordinates(2, i) = z;
    }

    return mesh;
}

PoliedriMesh creaCuboNormalizzato() {
    PoliedriMesh mesh;

    std::vector<Eigen::Vector3d> vertici = {
        {-1.0, -1.0, -1.0},
        {-1.0, -1.0,  1.0},
        {-1.0,  1.0, -1.0},
        {-1.0,  1.0,  1.0},
        { 1.0, -1.0, -1.0},
        { 1.0, -1.0,  1.0},
        { 1.0,  1.0, -1.0},
        { 1.0,  1.0,  1.0}
    };

    mesh.NumCell0Ds = vertici.size();
    mesh.Cell0DsId.resize(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates.resize(3, mesh.NumCell0Ds);

    for (unsigned int i = 0; i < vertici.size(); ++i) {
        double x = vertici ;
        double y = vertici ;
        double z = vertici ;
        normalizza(x, y, z);

        mesh.Cell0DsId[i] = idOffset + i;
        mesh.Cell0DsCoordinates(0, i) = x;
        mesh.Cell0DsCoordinates(1, i) = y;
        mesh.Cell0DsCoordinates(2, i) = z;
    }

    return mesh;
}

PoliedriMesh creaOttaedroNormalizzato() {
    PoliedriMesh mesh;

    std::vector<Eigen::Vector3d> vertici = {
        { 1.0,  0.0,  0.0},
        {-1.0,  0.0,  0.0},
        { 0.0,  1.0,  0.0},
        { 0.0, -1.0,  0.0},
        { 0.0,  0.0,  1.0},
        { 0.0,  0.0, -1.0}
    };

    mesh.NumCell0Ds = vertici.size();
    mesh.Cell0DsId.resize(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates.resize(3, mesh.NumCell0Ds);

    for (unsigned int i = 0; i < vertici.size(); ++i) {
        double x = vertici ;
        double y = vertici ;
        double z = vertici ;
        normalizza(x, y, z);  // Anche se sono già unitari, teniamo la funzione per coerenza

        mesh.Cell0DsId[i] = idOffset + i;
        mesh.Cell0DsCoordinates(0, i) = x;
        mesh.Cell0DsCoordinates(1, i) = y;
        mesh.Cell0DsCoordinates(2, i) = z;
    }

    return mesh;
}

PoliedriMesh creaDodecaedroNormalizzato() {
    PoliedriMesh mesh;

    const double phi = (1.0 + std::sqrt(5.0)) / 2.0;
    const double inv_phi = 1.0 / phi;

    std::vector<Eigen::Vector3d> vertici;

    // (±1, ±1, ±1)
    for (int i : {-1, 1})
        for (int j : {-1, 1})
            for (int k : {-1, 1})
                vertici.emplace_back(i, j, k);

    // (0, ±φ, ±1/φ)
    for (int j : {-1, 1})
        for (int k : {-1, 1})
            vertici.emplace_back(0.0, j * phi, k * inv_phi);

    // (±1/φ, 0, ±φ)
    for (int i : {-1, 1})
        for (int k : {-1, 1})
            vertici.emplace_back(i * inv_phi, 0.0, k * phi);

    // (±φ, ±1/φ, 0)
    for (int i : {-1, 1})
        for (int j : {-1, 1})
            vertici.emplace_back(i * phi, j * inv_phi, 0.0);

    // Inserimento nel mesh
    mesh.NumCell0Ds = vertici.size();
    mesh.Cell0DsId.resize(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates.resize(3, mesh.NumCell0Ds);

    for (unsigned int i = 0; i < vertici.size(); ++i) {
        double x = vertici ;
        double y = vertici ;
        double z = vertici ;
        normalizza(x, y, z);

        mesh.Cell0DsId[i] = idOffset + i;
        mesh.Cell0DsCoordinates(0, i) = x;
        mesh.Cell0DsCoordinates(1, i) = y;
        mesh.Cell0DsCoordinates(2, i) = z;
    }

    return mesh;
}

PoliedriMesh creaIcosaedroNormalizzato() {
    PoliedriMesh mesh;

    const double phi = (1.0 + std::sqrt(5.0)) / 2.0;

    std::vector<Eigen::Vector3d> vertici;

    // (0, ±1, ±φ)
    for (int i : {-1, 1})
        for (int j : {-1, 1})
            vertici.emplace_back(0.0, i * 1.0, j * phi);

    // (±1, ±φ, 0)
    for (int i : {-1, 1})
        for (int j : {-1, 1})
            vertici.emplace_back(i * 1.0, j * phi, 0.0);

    // (±φ, 0, ±1)
    for (int i : {-1, 1})
        for (int j : {-1, 1})
            vertici.emplace_back(i * phi, 0.0, j * 1.0);

    mesh.NumCell0Ds = vertici.size();
    mesh.Cell0DsId.resize(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates.resize(3, mesh.NumCell0Ds);

    for (unsigned int i = 0; i < vertici.size(); ++i) {
        double x = vertici[i].x();
        double y = vertici[i].y();
        double z = vertici[i].z();
        normalizza(x, y, z);

        mesh.Cell0DsId[i] = idOffset + i;
        mesh.Cell0DsCoordinates(0, i) = x;
        mesh.Cell0DsCoordinates(1, i) = y;
        mesh.Cell0DsCoordinates(2, i) = z;
    }

    return mesh;
}


// Scrittura Cell0Ds.txt
bool Cell0Ds(PoliedriMesh& mesh) {
	ofstream file ("./Cell0Ds.txt");
	if (file.fail() )
		return false;
		
	mesh.Cell0DsId 
	for(unsigned int i = 0, i < 50, i++)
		file << mesh.Cell0DsId << " " << v.x << " " << v.y << " " << v.z << "\n";
    	file.close();


/*    std::vector<Cella1D> lati = {
        {0, 0, 1}, {1, 0, 2}, {2, 0, 3},
        {3, 1, 2}, {4, 1, 3}, {5, 2, 3}
    };

    std::vector<Cella2D> facce = {
        {0, 3, 3, {0, 1, 2}, {0, 3, 1}},
        {1, 3, 3, {0, 3, 1}, {2, 4, 0}},
        {2, 3, 3, {0, 2, 3}, {1, 5, 2}},
        {3, 3, 3, {1, 3, 2}, {4, 5, 3}}
    };

    Cella3D poliedro = {
        0,
        static_cast<int>(vertici.size()),
        static_cast<int>(lati.size()),
        static_cast<int>(facce.size()),
        {0, 1, 2, 3},
        {0, 1, 2, 3, 4, 5},
        {0, 1, 2, 3}
    }; */


    // Scrittura Cell1Ds.txt
    std::ofstream file1("./Cell1Ds.txt");
    for (const auto& l : lati)
        file1 << l.id << " " << l.id_vertice_iniziale << " " << l.id_vertice_finale << "\n";
    file1.close();

    // Scrittura Cell2Ds.txt
    std::ofstream file2("./Cell2Ds.txt");
    for (const auto& f : facce) {
        file2 << f.id << " " << f.numero_vertici;
        for (int id_vertice : f.id_vertici) file2 << " " << id_vertice;
        file2 << " " << f.numero_lati;
        for (int id_lato : f.id_lati) file2 << " " << id_lato;
        file2 << "\n";
    }
    file2.close();

    // Scrittura Cell3Ds.txt
    std::ofstream file3("./Cell3Ds.txt");
    file3 << poliedro.id << " " << poliedro.numero_vertici;
    for (int id_vertice : poliedro.id_vertici) file3 << " " << id_vertice;
    file3 << " " << poliedro.numero_lati;
    for (int id_lato : poliedro.id_lati) file3 << " " << id_lato;
    file3 << " " << poliedro.numero_facce;
    for (int id_faccia : poliedro.id_facce) file3 << " " << id_faccia;
    file3 << "\n";
    file3.close();

    std::cout << "Tutti i file Cell0Ds.txt–Cell3Ds.txt sono stati generati.\n";
    return 0;
}
