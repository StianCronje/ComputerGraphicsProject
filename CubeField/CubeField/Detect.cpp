#include "Detect.h"

Detect::Detect(Model* one, Model* two)
{
	this->one = one;
	this->two = two;
}
bool Detect::compare()
{
	float y1 = one->GetTranslation().x;
	float y2 = two->GetTranslation().x;
	float z1 = one->GetTranslation().z;
	float z2 = two->GetTranslation().z;
	float resulty = abs(y1 - y2);
	float resultz = abs(z1 - z2);
	if (resulty<50&&resultz<50)
		return true;
	else
		return false;
}