#include "CamminoMinimo.hpp"
#include <queue>
#include <limits>
#include <cmath>
#include <iostream>

using namespace std;

namespace PoliedriLibrary {

ShortestPath::ShortestPath(const PoliedriMesh& mesh) : mesh(mesh) {  // salva il riferimento alla mesh
    CostruisciGrafo();
    verticiMarcati.resize(mesh.NumCell0Ds, 0);
    latiMarcati.resize(mesh.NumCell1Ds, 0);
}

void ShortestPath::CostruisciGrafo() {
    lista_adiacenza.resize(mesh.NumCell0Ds);
	
    for (unsigned int i = 0; i < mesh.NumCell1Ds; ++i) {
        unsigned int from = mesh.Cell1DsExtrema(0, i);
        unsigned int to = mesh.Cell1DsExtrema(1, i);
        double length = CalcolaDistanza(from, to);
        lista_adiacenza[from].emplace_back(to, length);
        lista_adiacenza[to].emplace_back(from, length);
    }
}

double ShortestPath::CalcolaDistanza(unsigned int id1, unsigned int id2) {
    auto a = mesh.Cell0DsCoordinates.col(id1);
    auto b = mesh.Cell0DsCoordinates.col(id2);
    return (a - b).norm();  // distanza euclidea 3D
}


// Implementazione dell'algoritmo Dijkstra per il calcolo del cammino minimo

vector<unsigned int> ShortestPath::CalcolaShortPath(unsigned int startId, unsigned int endId, double& totalLength) {
    vector<double> dist(mesh.NumCell0Ds, numeric_limits<double>::infinity());  // vettore delle distanze; la distanza minima dal nodo sorgente è inizialmente +inf
    vector<int> pred(mesh.NumCell0Ds, -1);  // vettore dei predecessori; il predecessore vale -1 inizialmente
    priority_queue<pair<double, unsigned int>, vector<pair<double, unsigned int>>, greater<>> pq;  // coda con priorità: gli elementi della coda sono i vertici e la priorità è associata alla distanza; ordinata in base alla distanza minima

    dist[startId] = 0.0;
	pred[startId] = startId;
    pq.push({0.0, startId});  // inserimento in alto nello stack
	
    while (!pq.empty()) {
        auto [d, u] = pq.top();  // prende il primo elemento dello stack
        pq.pop();  // rimuove il primo elemento dello stack

        if (u == endId)
			break;
		
		// aggiorna le distanze dei nodi v vicini se trova il cammino più corto
        for (const auto& [v, peso] : lista_adiacenza[u]) {
            double alt = dist[u] + peso;
            if (alt < dist[v]) {
                dist[v] = alt;
                pred[v] = u;
                pq.push({alt, v});  // inserimento nello stack
            }
        }
    }

    vector<unsigned int> path;
	
	if(pred[endId] == -1) {
		totalLength = numeric_limits<double>::infinity();
		return path;  // non esiste un camminimo 
	}
	
    for (unsigned int at = endId; at != startId; at = pred[at]) {
        path.insert(path.begin(), at);
	}
	
	path.insert(path.begin(), startId);
    totalLength = dist[endId];
    return path;  // restituisce il cammino a ritroso
}

void ShortestPath::MarcaCammino(const vector<unsigned int>& path) {
    fill(verticiMarcati.begin(), verticiMarcati.end(), 0);  // azzera i vertici marcati
    fill(latiMarcati.begin(), latiMarcati.end(), 0);  // azzera i lati marcati
	
	// per ogni coppia di vertici consecutivi, li marca con 1 e cerca il lato che li collega marcando anch'esso con 1
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        unsigned int u = path[i];
		unsigned int v = path[i + 1];
        verticiMarcati[u] = 1;
        verticiMarcati[v] = 1;

        for (unsigned int j = 0; j < mesh.NumCell1Ds; ++j) {
            unsigned int a = mesh.Cell1DsExtrema(0, j);
            unsigned int b = mesh.Cell1DsExtrema(1, j);
            if ((a == u && b == v) || (a == v && b == u)) {
                latiMarcati[j] = 1;
                break;
            }
        }
    }
}

const vector<int>& ShortestPath::getVerticiMarcati() const {
    return verticiMarcati;
}

const vector<int>& ShortestPath::getLatiMarcati() const {
    return latiMarcati;
}

}
