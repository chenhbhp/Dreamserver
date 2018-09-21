///some macros

#pragma once

#ifndef _CONFIG_H__
#define _CONFIG_H__

#define TRIE_SIZE 25

struct Global_Config
{
	std::string curExePath = "";

	Global_Config()
	{

	}
};

extern Global_Config g_Config;


#endif