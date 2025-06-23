#include "PoliedriMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"
#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <array>

using namespace std;
using namespace Eigen;

namespace PoliedriLibrary {

void Export(const PoliedriMesh& mesh, const string& nomeBase) {
    Gedim::UCDUtilities utilities;

    // Export per i vertici (Cell0Ds)
    vector<double> Cell0DsValues(mesh.Cell0DsShortPath.begin(), mesh.Cell0DsShortPath.end());

    vector<Gedim::UCDProperty<double>> vertici_props;
    vertici_props.push_back({
							 "ShortPath",  // Label
							 "",  // UnitLabel
							 static_cast<unsigned int>(Cell0DsValues.size()),  // Size
							 1,  // NumComponents; proprietà scalare
							 Cell0DsValues.data() // Data; puntatore diretto all'array di memoria utilizzato internamente dal vettore per memorizzare i propri elementi; può essere spostato per accedere a qualsiasi elemento dell'array
							});

    utilities.ExportPoints(nomeBase + "_Cell0Ds.inp",
                           mesh.Cell0DsCoordinates,
                           vertici_props); // ShortPath scalare

    // Export per i lati (Cell1Ds)
    vector<double> Cell1Ds_ShortPath(mesh.Cell1DsShortPath.begin(), mesh.Cell1DsShortPath.end());  // inizializzato copiando tutti i valori di mesh.Cell1DsShortPath e serve per definire quali lati fanno parte del cammino minimo e quali noboolalpha
    //vector<double> Cell1D_Ones(mesh.Cell1DsShortPath.size(), 1.0);  // ha la stessa dimensione di mesh.Cell1DsShortPath e inizializza tutti i valori a 1.0

    vector<Gedim::UCDProperty<double>> lati_props;
    lati_props.push_back({
						  "ShortPath",  // Label
						  "",  // UnitLabel
						  static_cast<unsigned int>(Cell1Ds_ShortPath.size()),  // Size
						  1,  // NumComponents; proprietà scalare
						  Cell1Ds_ShortPath.data()  // Data; puntatore diretto all'array di memoria utilizzato internamente dal vettore per memorizzare i propri elementi; può essere spostato per accedere a qualsiasi elemento dell'array
						 });

    /*lati_props.push_back({
						  "CostanteUno",  // Label
						  "",  // UnitLabel
						  static_cast<unsigned int>(Cell1D_Ones.size()),  // Size
						  1,  // NumComponents; proprietà scalare
						  Cell1D_Ones.data()  // Data; puntatore diretto all'array di memoria utilizzato internamente dal vettore per memorizzare i propri elementi; può essere spostato per accedere a qualsiasi elemento dell'array
						 });*/

    utilities.ExportSegments(nomeBase + "_Cell1Ds.inp",
                             mesh.Cell0DsCoordinates,
                             mesh.Cell1DsExtrema,
                             {},
                             lati_props);
}

}