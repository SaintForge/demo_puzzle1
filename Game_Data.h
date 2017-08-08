#ifndef GAME_DATA
#define GAME_DATA

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#include "Level_Info.h"

struct GameData
{
    GameData()
	{
	}
    virtual ~GameData()
	{
	}

    friend std::istream& operator>>(std::istream& in, GameData& game_data);
    friend std::ostream& operator<<(std::ostream& out, const GameData& game_data);

    bool open_data()
	{
	    if(!data.empty()) data.clear();
	    bool is_opened = false;

	    std::ifstream file;
	    file.open("file.bin", std::ios::in | std::ios::binary);
	    if(file.is_open())
	    {
		file >> *this;
		is_opened = true;
	    }
	    file.close();

	    return is_opened;
	}
    void save_data()
	{
	    if(data.empty()) return;

	    std::ofstream file;
	    file.open("file.bin", std::ios::out | std::ios::binary);
	    if(file.is_open())
	    {
		file << *this;
	    }

	    file.close();
	}
    bool is_empty()
	{
	    return data.empty();
	}

    void load_level(int level_number, Level_Info& level_info);
    void save_level(int level_number, Level_Info& level_info);

    void print_data()
	{
	    for (int i = 0; i < data.size(); i++)
	    {
		std::cout << data[i];
	    }
	}
    
    void set_searching_point(int search_p)
	{
	    if(search_p > 0) searching_point = search_p;
	}
    
    void   write_variable(int &start_index, const std::string& variable_name, int variable);
    size_t read_variable(int &start_index, const std::string& variable_name, int& variable);
private:
    int find_level_number(int start_point, int level_number);

    void read_variable_number(int row_index, int column_index, int& variable);
    void read_figures(int &start_index, Level_Info& level_info);
    void read_grid(int &start_index, Level_Info& level_info);

    void write_variable_number(int row_index, const std::string& variable);
    
    std::vector<std::string> data;
    int searching_point = 0;
};

#endif
