#pragma once

#include "moo.h"

namespace My_Moo
{
	class plant
	{
	public:
		virtual My_Moo::Moo * get_Moo(char* input);
	};
}