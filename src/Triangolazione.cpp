#include "Triangolazione.hpp"
#include <unordered_map>
#include <map>
#include <cmath>
#include <Eigen/Dense>
#include <vector>
#include <tuple>
#include <iostream>

using namespace Eigen;
using namespace std;

namespace PoliedriLibrary {

struct EdgeKey {
    int v1, v2;
    EdgeKey(int a, int b) {
        if (a < b) { v1 = a; v2 = b; }
        else       { v1 = b; v2 = a; }
    }
    bool operator==(const EdgeKey& other) const {
        return v1 == other.v1 && v2 == other.v2;
    }
    bool operator<(const EdgeKey& other) const {
        return tie(v1, v2) < tie(other.v1, other.v2);
    }
};

} // namespace PoliedriLibrary

namespace std {
    template<>
    struct hash<PoliedriLibrary::EdgeKey> {
        size_t operator()(const PoliedriLibrary::EdgeKey& k) const {
            return hash<int>()(k.v1) ^ (hash<int>()(k.v2) << 1);
        }
    };
}

namespace PoliedriLibrary {

void TriangolaFacceClasseI(int b, const PoliedriMesh& meshIniziale, PoliedriMesh& meshGeodetico) {
    if (b == 1) {
        meshGeodetico = meshIniziale;
        return;
    }

    vector<Vector3d> nuoviVertici;
    // Hash map con tolleranza per vertici
    struct Vector3dHash {
        size_t operator()(const Vector3d& v) const {
            auto h1 = std::hash<int>()(static_cast<int>(v.x() * 1e6));
            auto h2 = std::hash<int>()(static_cast<int>(v.y() * 1e6));
            auto h3 = std::hash<int>()(static_cast<int>(v.z() * 1e6));
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
    struct Vector3dEqual {
        bool operator()(const Vector3d& a, const Vector3d& b) const {
            return (a - b).norm() < 1e-6;
        }
    };

    unordered_map<Vector3d, int, Vector3dHash, Vector3dEqual> verticeToIndex;

    unordered_map<EdgeKey, vector<int>> edgeVertices;
    map<tuple<int, int, int>, int> internalVertexMap;
    vector<array<int, 3>> nuoveFacce;

    // Aggiungi vertici iniziali
    for (unsigned int i = 0; i < meshIniziale.NumCell0Ds; ++i) {
        Vector3d p = meshIniziale.Cell0DsCoordinates.col(i);
        nuoviVertici.push_back(p);
        verticeToIndex[p] = i;
    }

    for (const auto& face : meshIniziale.Cell2DsVertices) {
        int n = face.size();
        if (n < 3) continue;

        int v0 = face[0];

        for (int t = 1; t < n - 1; ++t) {
            internalVertexMap.clear();
            int idA = v0;
            int idB = face[t];
            int idC = face[t + 1];

            Vector3d A = meshIniziale.Cell0DsCoordinates.col(idA);
            Vector3d B = meshIniziale.Cell0DsCoordinates.col(idB);
            Vector3d C = meshIniziale.Cell0DsCoordinates.col(idC);

            auto getEdgePoints = [&](int from, int to, const Vector3d& p1, const Vector3d& p2) {
                EdgeKey key(from, to);
                if (edgeVertices.find(key) == edgeVertices.end()) {
                    vector<int> ids;
                    for (int i = 1; i < b; ++i) {
                        double alpha = static_cast<double>(i) / b;
                        Vector3d p = (1 - alpha) * p1 + alpha * p2;
                        if (verticeToIndex.find(p) == verticeToIndex.end()) {
                            int id = nuoviVertici.size();
                            nuoviVertici.push_back(p);
                            verticeToIndex[p] = id;
                            ids.push_back(id);
                        } else {
                            ids.push_back(verticeToIndex[p]);
                        }
                    }
                    edgeVertices[key] = ids;
                }
                return edgeVertices[key];
            };

            auto AB = getEdgePoints(idA, idB, A, B);
            auto BC = getEdgePoints(idB, idC, B, C);
            auto CA = getEdgePoints(idC, idA, C, A);

            vector<vector<int>> localIndex(b + 1);

            for (int i = 0; i <= b; ++i) {
                for (int j = 0; j <= b - i; ++j) {
                    int k = b - i - j;
                    double alpha = static_cast<double>(i) / b;
                    double beta = static_cast<double>(j) / b;
                    double gamma = 1.0 - alpha - beta;
                    Vector3d p = gamma * A + alpha * B + beta * C;

                    int id;

                    if (i == 0 && j == 0) {
                        id = verticeToIndex[A];
                    } else if (i == b && j == 0) {
                        id = verticeToIndex[B];
                    } else if (i == 0 && j == b) {
                        id = verticeToIndex[C];
                    } else if (j == 0) {
                        id = AB[i - 1];
                    } else if (i == 0) {
                        id = CA[j - 1];
                    } else if (i + j == b) {
                        id = BC[j - 1];
                    } else {
                        tuple<int, int, int> key = {i, j, k};
                        if (internalVertexMap.find(key) == internalVertexMap.end()) {
                            id = nuoviVertici.size();
                            nuoviVertici.push_back(p);
                            internalVertexMap[key] = id;
                        } else {
                            id = internalVertexMap[key];
                        }
                    }
                    localIndex[i].push_back(id);
                }
            }

            for (int i = 0; i < b; ++i) {
                for (int j = 0; j < b - i; ++j) {
                    int v0 = localIndex[i][j];
                    int v1 = localIndex[i + 1][j];
                    int v2 = localIndex[i][j + 1];
                    nuoveFacce.push_back({v0, v1, v2});

                    if (j + 1 < b - i) {
                        int v3 = localIndex[i + 1][j + 1];
                        nuoveFacce.push_back({v1, v3, v2});
                    }
                }
            }
        }
    }

    meshGeodetico.Cell0DsId.resize(nuoviVertici.size());
    meshGeodetico.Cell0DsCoordinates.resize(3, nuoviVertici.size());
    for (size_t i = 0; i < nuoviVertici.size(); ++i) {
        meshGeodetico.Cell0DsId[i] = i;
        meshGeodetico.Cell0DsCoordinates.col(i) = nuoviVertici[i];
    }
    meshGeodetico.NumCell0Ds = nuoviVertici.size();

    meshGeodetico.Cell2DsId.resize(nuoveFacce.size());
    meshGeodetico.Cell2DsVertices.resize(nuoveFacce.size());
    for (size_t i = 0; i < nuoveFacce.size(); ++i) {
        meshGeodetico.Cell2DsId[i] = i;
        meshGeodetico.Cell2DsVertices[i] = vector<unsigned int>{
            static_cast<unsigned int>(nuoveFacce[i][0]),
            static_cast<unsigned int>(nuoveFacce[i][1]),
            static_cast<unsigned int>(nuoveFacce[i][2])
        };
    }
    meshGeodetico.NumCell2Ds = nuoveFacce.size();

    unordered_map<EdgeKey, int> edgeToId;
    vector<unsigned int> edgeStarts, edgeEnds;
    int edgeIdCounter = 0;

    for (const auto& faccia : nuoveFacce) {
        array<int, 3> verts = faccia;
        for (int i = 0; i < 3; ++i) {
            int v1 = verts[i];
            int v2 = verts[(i + 1) % 3];
            EdgeKey e(v1, v2);
            if (edgeToId.find(e) == edgeToId.end()) {
                edgeToId[e] = edgeIdCounter++;
                edgeStarts.push_back(e.v1);
                edgeEnds.push_back(e.v2);
            }
        }
    }

    meshGeodetico.Cell1DsId.resize(edgeIdCounter);
    meshGeodetico.Cell1DsExtrema.resize(2, edgeIdCounter);
    meshGeodetico.NumCell1Ds = edgeIdCounter;
    for (int i = 0; i < edgeIdCounter; ++i) {
        meshGeodetico.Cell1DsId[i] = i;
        meshGeodetico.Cell1DsExtrema(0, i) = edgeStarts[i];
        meshGeodetico.Cell1DsExtrema(1, i) = edgeEnds[i];
    }

    meshGeodetico.Cell2DsEdges.resize(nuoveFacce.size());
    for (size_t i = 0; i < nuoveFacce.size(); ++i) {
        array<int, 3> verts = nuoveFacce[i];
        vector<unsigned int> edges(3);
        for (int j = 0; j < 3; ++j) {
            EdgeKey e(verts[j], verts[(j + 1) % 3]);
            edges[j] = edgeToId[e];
        }
        meshGeodetico.Cell2DsEdges[i] = edges;
    }

    meshGeodetico.Cell3DsId.clear();
    meshGeodetico.Cell3DsVertices.clear();
    meshGeodetico.Cell3DsEdges.clear();
    meshGeodetico.Cell3DsFaces.clear();
    meshGeodetico.NumCell3Ds = 0;
}

} // namespace PoliedriLibrary
