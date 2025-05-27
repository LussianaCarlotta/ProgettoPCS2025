#include "Triangolazione.hpp"
#include <map>
#include <cmath>
#include <Eigen/Dense>
#include <vector>
#include <set>
#include <tuple>

using namespace Eigen;
using namespace std;

namespace PoliedriLibrary {

    // Funzione di interpolazione tra due punti
    Vector3d interpolate(const Vector3d& A, const Vector3d& B, double t) {
        return A + t * (B - A);
    }

    // Funzione che genera una griglia triangolare di classe I su una faccia
	void TriangolaFacceClasseI(const PoliedriMesh& meshIniziale, unsigned int b, PoliedriMesh& meshGeodetico) {
		meshGeodetico = {};
	
		map<Vector3d, int, bool(*)(const Vector3d&, const Vector3d&)> vertici([](const Vector3d& a, const Vector3d& b) {
			return lexicographical_compare(a.data(), a.data() + 3, b.data(), b.data() + 3);
		});
	
		vector<Vector3d> listaVertici;
		vector<array<unsigned int, 3>> triangoli;
		set<pair<int, int>> spigoli;
	
		for (size_t f = 0; f < meshIniziale.NumCell2Ds; ++f) {
			const auto& face = meshIniziale.Cell2DsVertices[f];
			if (face.size() != 3) continue; // Solo triangoli
	
			vector<Vector3d> originalVerts;
			for (auto vid : face) {
				originalVerts.push_back(meshIniziale.Cell0DsCoordinates.col(vid));
			}
	
			vector<vector<int>> indexGrid(b + 1);
			for (unsigned int i = 0; i <= b; ++i) {
				indexGrid[i].resize(b - i + 1);
				for (unsigned int j = 0; j <= b - i; ++j) {
					unsigned int k = b - i - j;
					Vector3d V = (originalVerts[0] * i + originalVerts[1] * j + originalVerts[2] * k) / b;
					V.normalize();
					int index;
					if (vertici.count(V)) {
						index = vertici[V];
					} else {
						index = listaVertici.size();
						vertici[V] = index;
						listaVertici.push_back(V);
					}
					indexGrid[i][j] = index;
				}
			}
	
			for (unsigned int i = 0; i < b; ++i) {
				for (unsigned int j = 0; j < b - i; ++j) {
					int a = indexGrid[i][j];
					int b1 = indexGrid[i + 1][j];
					int c = indexGrid[i][j + 1];
					triangoli.push_back({
						static_cast<unsigned int>(a),
						static_cast<unsigned int>(b1),
						static_cast<unsigned int>(c)
					});
					// perchè ho messo array prima e questi erano interi
					spigoli.insert({min(a, b1), max(a, b1)});
					spigoli.insert({min(a, c), max(a, c)});
					// perchè ho messo array prima e questi erano interi
					if (j + 1 < b - i) {
						int d = indexGrid[i + 1][j + 1];
						triangoli.push_back({
							static_cast<unsigned int>(b1),
							static_cast<unsigned int>(d),
							static_cast<unsigned int>(c)
						});
						
						spigoli.insert({min(b1, d), max(b1, d)});
						spigoli.insert({min(d, c), max(d, c)});
					}
				}
			}
		}
	
		// === Popolamento della mesh ===
	
		// Cell0Ds (vertici)
		meshGeodetico.NumCell0Ds = listaVertici.size();
		meshGeodetico.Cell0DsCoordinates.resize(3, meshGeodetico.NumCell0Ds);
		meshGeodetico.Cell0DsId.resize(meshGeodetico.NumCell0Ds);
		for (size_t i = 0; i < listaVertici.size(); ++i) {
			meshGeodetico.Cell0DsCoordinates.col(i) = listaVertici[i];
			meshGeodetico.Cell0DsId[i] = i;
		}
	
		// Cell1Ds (spigoli)
		meshGeodetico.NumCell1Ds = spigoli.size();
		meshGeodetico.Cell1DsId.resize(meshGeodetico.NumCell1Ds);
		meshGeodetico.Cell1DsExtrema.resize(2, meshGeodetico.NumCell1Ds);
		int edgeIndex = 0;
		for (const auto& edge : spigoli) {
			meshGeodetico.Cell1DsId[edgeIndex] = edgeIndex;
			meshGeodetico.Cell1DsExtrema(0, edgeIndex) = edge.first;
			meshGeodetico.Cell1DsExtrema(1, edgeIndex) = edge.second;
			++edgeIndex;
		}
	
		// Cell2Ds (triangoli)
		meshGeodetico.NumCell2Ds = triangoli.size();
		meshGeodetico.Cell2DsId.resize(meshGeodetico.NumCell2Ds);
		meshGeodetico.Cell2DsVertices.resize(meshGeodetico.NumCell2Ds);
		for (size_t i = 0; i < triangoli.size(); ++i) {
			meshGeodetico.Cell2DsId[i] = i;
			meshGeodetico.Cell2DsVertices[i] = {triangoli[i][0], triangoli[i][1], triangoli[i][2]};
		}
		// Inizializza il numero di vertici per ogni faccia
		meshGeodetico.Cell2DsNumVertices.resize(meshGeodetico.NumCell2Ds);
		for (size_t i = 0; i < meshGeodetico.NumCell2Ds; ++i) {
			meshGeodetico.Cell2DsNumVertices[i] = meshGeodetico.Cell2DsVertices[i].size();
		}
		
		// Inizializza Cell2DsEdges con 3 elementi per triangolo
		meshGeodetico.Cell2DsEdges.resize(meshGeodetico.NumCell2Ds);
		for (size_t i = 0; i < meshGeodetico.NumCell2Ds; ++i) {
			meshGeodetico.Cell2DsEdges[i].resize(3);
		}	
		for (size_t i = 0; i < meshGeodetico.NumCell2Ds; ++i) {
			const auto& tri = meshGeodetico.Cell2DsVertices[i];
			// Ogni triangolo ha 3 lati
			vector<unsigned int> edges(3);
		
			// Qui devi trovare gli indici degli spigoli che corrispondono alle coppie di vertici del triangolo
			// Gli spigoli sono in meshGeodetico.Cell1DsExtrema (2 x NumCell1Ds)
			// Per semplicità: cerca l'indice di ogni lato nel vettore di spigoli
		
			for (int e = 0; e < 3; ++e) {
				unsigned int v0 = tri[e];
				unsigned int v1 = tri[(e + 1) % 3];
				unsigned int minV = std::min(v0, v1);
				unsigned int maxV = std::max(v0, v1);
		
				// Cerca l'indice spigolo
				unsigned int edgeIndex = 0;
				for (; edgeIndex < meshGeodetico.NumCell1Ds; ++edgeIndex) {
					if (static_cast<unsigned int>(meshGeodetico.Cell1DsExtrema(0, edgeIndex)) == minV &&
    					static_cast<unsigned int>(meshGeodetico.Cell1DsExtrema(1, edgeIndex)) == maxV) {
						break;
					}
				}
				edges[e] = edgeIndex;
			}
		
			meshGeodetico.Cell2DsEdges[i] = edges;
		}
	
	} 

}
