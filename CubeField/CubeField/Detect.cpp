#include "Detect.h"
#include "Model.h"

Detect::Detect(Model* one, Model* two)
{
	this->one = one;
	this->two = two;
}
bool Detect::compare()
{
	if (one->GetTranslation() == two->GetTranslation())
		return true;
	else
		return false;
}

