#pragma once

#include "moo.h"
#include <vector>

namespace My_Moo
{
	class Plant
	{
	public:
		static Plant& instance();

		virtual std::string get_Moo(const std::string &s);

	private:
		Plant();

		~Plant();

		std::vector< My_Moo::handle_one*> moos;

	};
}