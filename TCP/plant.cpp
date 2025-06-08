#include "plant.h"
#include <iostream>

My_Moo::Plant::Plant()
{
	My_Moo::Moo* pmoo = new Moo();

	handle_one* mid;

	mid = new handle_one(0);

	moos.push_back(mid);
}

My_Moo::Plant::~Plant()
{
	for (auto& moo : moos)
	{
		delete moo;
	}
	
}

std::string My_Moo::Plant::get_Moo(const std::string &s)
{
	int mark = 0;
	try
	{
		mark = std::stoi(s.substr(0, 4));
	}
	catch (...)
	{
		std::cout << "error_str" << std::endl;
		return "error_str";
	}

	My_Moo::handle_one* p = NULL;
	std::string re = "";
	if (mark < moos.size())
	{
		p = moos[mark];
		std::string mid = s.substr(4);
		re = p->handle_str(mid);
	}

	return std::move(re);
}


My_Moo::Plant& My_Moo::Plant::instance()
{
    static Plant instance;
    return instance;
}

