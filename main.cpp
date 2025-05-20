#include <iostream>
#include "PoliedriMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"
#include <Eigen/Dense>
#include <vector>
#include <array>

using namespace PoliedriLibrary;
using namespace std;
using namespace Eigen;

int main()
{
	PoliedriMesh mesh; 
	
	ImportMesh(mesh);
	
	AggiungiFacce(mesh);

	Export(mesh);
	
	if (!Cell3Ds(mesh)) return false;


}