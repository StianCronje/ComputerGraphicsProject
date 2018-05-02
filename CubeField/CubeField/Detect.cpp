#include "Detect.h"

Detect::Detect(Model* one, Model* two)
{
	this->one = one;
	this->two = two;

}
Detect::Detect(Model* one, std::array<glm::vec3, 2> two)
{
	this->one = one;
	this->twoBounds = two;
}
bool Detect::compare(float set)
{
	float x1 = one->GetTranslation().x;
	float x2 = two->GetTranslation().x;
	float z1 = one->GetTranslation().z;
	float z2 = two->GetTranslation().z;
	float y1 = one->GetTranslation().y;
	float y2 = two->GetTranslation().y;
	float resulty = abs(y1 - y2);
	float resultz = abs(z1 - z2);
	float resultx = abs(x1 - x2);
	if (resulty<set && resultz<set && resultx<set)
		return true;
	else
		return false;
}
bool Detect::comapre_bounds()
{
	glm::vec3 minA = one->GetMinBounds() + one->GetTranslation();
	glm::vec3 maxA = one->GetMaxBounds() + one->GetTranslation();
	glm::vec3 minB = twoBounds[0];
	glm::vec3 maxB = twoBounds[1];

	bool inX = (minA.x <= maxB.x && maxA.x >= minB.x);
	bool inY = (minA.y <= maxB.y && maxA.y >= minB.y);
	bool inZ = (minA.z <= maxB.z && maxA.z >= minB.z);

	//std::cout << minA.z << " <= " << maxB.z << " | " << maxA.z << " >= " << minB.z << std::endl;

	return	inX && inY && inZ;
}