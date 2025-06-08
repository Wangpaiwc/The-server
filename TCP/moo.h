#pragma once
#include <map>
#include <string>

namespace My_Moo
{
	class Moo
	{
	public:
		Moo();
		
		~Moo();

		virtual bool run(const std::string& data);

		std::string str;
	protected:
		

	private:
		virtual bool run_tool(int mark, const std::string& data);

		
	};

	class handle_one
	{
	public:
		handle_one(int mark) :mark(mark)
		{

		}

		bool str_map(std::string& data, std::map<std::string, std::string>& result);

		virtual bool handle_str(std::string& data);
	protected:
		int mark;
	};


}
