#include "Detect.h"

Detect::Detect(Model* one, Model* two)
{
	this->one = one;
	this->two = two;
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
	if (resulty<set&&resultz<set&&resultx<set)
		return true;
	else
		return false;
}