#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace PoliedriMesh
{
bool ImportMesh(PolygonalMesh& mesh)
{
	if(!ImportCell0Ds(mesh))
		return false;

	if(!ImportCell1Ds(mesh))
		return false;

	if(!ImportCell2Ds(mesh))
		return false;
	
	if(!ImportCell3Ds(mesh))
		return false;

	return true;
}
//******************************************************************

}