#pragma once
#include <map>
#include <string>
#include "mysql.h"

namespace My_Moo
{
	class Moo
	{
	public:
		Moo();
		
		virtual ~Moo();

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
			config = &ConfigManager::instance();
		}

		~handle_one() = default;

		bool str_map(std::string& data, std::map<std::string, std::string>& result);

		virtual std::string handle_str(std::string& data);

		virtual bool handle_str_tool(std::map<std::string, std::string>& result);
	protected:
		int mark;

		ConfigManager* config;
	};


}
