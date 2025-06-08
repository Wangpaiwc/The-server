#pragma once

#include "moo.h"
#include <vector>

namespace My_Moo
{
	class plant
	{
	public:
		plant();

		virtual My_Moo::handle_one* get_Moo(int mark);

		My_Moo::Moo* pmoo;

	private:
		std::vector< My_Moo::handle_one*> moos;

		
	};
}