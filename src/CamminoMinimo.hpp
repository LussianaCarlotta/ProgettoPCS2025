#include "PoliedriMesh.hpp"
#include <vector>
#include <utility>

using namespace std;

namespace PoliedriLibrary {

class CamminoMinimo {
public:
    CamminoMinimo(const PoliedriMesh& mesh);

    // Calcola il cammino minimo tra due vertici
    vector<unsigned int> CalcolaShortestPath(unsigned int startId, unsigned int endId, double& peso);

    // Restituisce i vertici e lati marcati con ShortPath = 1
    void CamminoMarcato(const vector<unsigned int>& cammino);

    // Accesso ai vertici e lati marcati
    const vector<int>& getVerticeMarcato() const;
    const vector<int>& getLatoMarcato() const;

private:
    const PoliedriMesh& mesh;
    vector<vector<pair<unsigned int, double>>> lista_adiacenza;
    vector<int> vertice_marcato;
    vector<int> lato_marcato;

    void CostruisciGrafo();
    double CalcolaDistanza(unsigned int id1, unsigned int id2);
};

}
