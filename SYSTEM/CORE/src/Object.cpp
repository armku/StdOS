#include "stdio.h"
#include "Object.h"

void Object::Show(bool newLine) const
{
	if(newLine)
	{
		printf("\n");
	}
}
