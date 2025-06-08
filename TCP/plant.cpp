#include "plant.h"

My_Moo::plant::plant()
{
	pmoo = new Moo();

	handle_one* mid;

	mid = new handle_one(0);

	moos.push_back(mid);
}

My_Moo::handle_one* My_Moo::plant::get_Moo(int mark)
{
	My_Moo::handle_one* p = nullptr;
	if (mark < moos.size())
	{
		p = moos[mark];

	}

	return p;
}
