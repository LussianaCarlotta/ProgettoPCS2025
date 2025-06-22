#include "PoliedriMesh.hpp"
#include <vector>
#include <utility>

using namespace std;

namespace PoliedriLibrary {

class ShortestPath {
public:
    ShortestPath(const PoliedriMesh& mesh);

    // Calcola il cammino minimo tra due vertici
    vector<unsigned int> CalcolaShortPath(unsigned int startId, unsigned int endId, double& totalLength);

    // Restituisce i vertici e lati marcati con ShortPath = 1
    void MarcaCammino(const vector<unsigned int>& path);

    // Accesso ai vertici e lati marcati
    const vector<int>& getVerticiMarcati() const;
    const vector<int>& getLatiMarcati() const;

    const PoliedriMesh& mesh;
    vector<vector<pair<unsigned int, double>>> lista_adiacenza;  // lista di archi
    vector<int> verticiMarcati;
    vector<int> latiMarcati;

    void CostruisciGrafo();
	
    double CalcolaDistanza(unsigned int id1, unsigned int id2);
};

}
