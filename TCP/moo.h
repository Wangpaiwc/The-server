#pragma once

#include <string>

namespace My_Moo
{
	class Moo
	{
	public:
		Moo();
		~Moo();

		virtual bool run(const std::string& data);
	private:
		virtual bool run_tool(int mark, const std::string& data);

		Inspect in;
	};

	class Inspect
	{
	public:
		virtual bool inspect(const std::string& data);
	};
}
