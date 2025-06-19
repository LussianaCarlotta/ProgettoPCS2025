#include <iostream>
#include "PoliedriMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"
#include <Eigen/Dense>
#include <vector>
#include <array>

using namespace std;
using namespace Eigen;

namespace PoliedriLibrary {

void Export(const PoliedriMesh& mesh, const std::string& nomeBase) {
    Gedim::UCDUtilities utilities;

    // Esporta punti (Cell0Ds)
    utilities.ExportPoints(nomeBase + "_Cell0Ds.inp",
                           mesh.Cell0DsCoordinates,
                           {}); // nessun valore scalare

    // Esporta segmenti (Cell1Ds)
    utilities.ExportSegments(nomeBase + "_Cell1Ds.inp",
                             mesh.Cell0DsCoordinates,
                             mesh.Cell1DsExtrema,
                             {}, {}); // nessun vettore/scalare associato

}

}
