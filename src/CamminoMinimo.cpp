#include "CamminoMinimo.hpp"
#include <queue>
#include <limits>
#include <cmath>
#include <iostream>

namespace PoliedriLibrary {
/*
void CostruisciGrafo() {
    lista_adiacenza.resize(mesh.NumCell0Ds);
    for(unsigned int i = 0; i < mesh.NumCell1Ds; ++i) {
        unsigned int from = mesh.Cell1DsExtrema(0, i);
        unsigned int to = mesh.Cell1DsExtrema(1, i);
        double peso = (from, to);
        adjList[from].emplace_back(to, length);
        adjList[to].emplace_back(from, length); // grafo non orientato
    }
}

double ShortestPathFromMesh::computeDistance(unsigned int id1, unsigned int id2) {
    auto a = mesh.Cell0DsCoordinates.col(id1);
    auto b = mesh.Cell0DsCoordinates.col(id2);
    return (a - b).norm();
}

std::vector<unsigned int> ShortestPathFromMesh::compute(unsigned int startId, unsigned int endId, double& totalLength) {
    std::vector<double> dist(mesh.NumCell0Ds, std::numeric_limits<double>::infinity());
    std::vector<int> prev(mesh.NumCell0Ds, -1);
    std::priority_queue<std::pair<double, unsigned int>,
                        std::vector<std::pair<double, unsigned int>>,
                        std::greater<>> pq;

    dist[startId] = 0.0;
    pq.push({0.0, startId});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (u == endId) break;

        for (const auto& [v, weight] : adjList[u]) {
            double alt = dist[u] + weight;
            if (alt < dist[v]) {
                dist[v] = alt;
                prev[v] = u;
                pq.push({alt, v});
            }
        }
    }

    std::vector<unsigned int> path;
    for (int at = endId; at != -1; at = prev[at])
        path.insert(path.begin(), at);

    totalLength = dist[endId];
    return path;
}

void ShortestPathFromMesh::markPath(const std::vector<unsigned int>& path) {
    std::fill(vertexMarks.begin(), vertexMarks.end(), 0);
    std::fill(edgeMarks.begin(), edgeMarks.end(), 0);

    for (size_t i = 0; i + 1 < path.size(); ++i) {
        unsigned int u = path[i], v = path[i + 1];
        vertexMarks[u] = 1;
        vertexMarks[v] = 1;

        for (unsigned int j = 0; j < mesh.NumCell1Ds; ++j) {
            unsigned int a = mesh.Cell1DsExtrema(0, j);
            unsigned int b = mesh.Cell1DsExtrema(1, j);
            if ((a == u && b == v) || (a == v && b == u)) {
                edgeMarks[j] = 1;
                break;
            }
        }
    }
}

const std::vector<int>& ShortestPathFromMesh::getVertexMarks() const {
    return vertexMarks;
}

const std::vector<int>& ShortestPathFromMesh::getEdgeMarks() const {
    return edgeMarks;
}
*/
}
