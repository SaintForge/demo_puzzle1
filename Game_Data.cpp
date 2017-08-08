#include "Game_Data.h"

void GameData::read_variable_number(int row_index, int column_index, int& variable)
{
    std::string number_text;

    for (int i = column_index; i < data[row_index].size(); i++)
    {
	if(isdigit(data[row_index][i]))
	    number_text.push_back(data[row_index][i]);
    }

    variable = std::stoi(number_text);
}

size_t GameData::read_variable(int &start_index,
			     const std::string& variable_name,
			     int& variable)
{
    size_t cursor_start = 0;

    for (int i = start_index; i < data.size(); i++)
    {
	if(variable_name.size() > data[i].size()) continue;
	if(data[i].find(":/") != std::string::npos) continue;

	cursor_start = data[i].find(variable_name);
	if(cursor_start != std::string::npos)
	{
	    start_index = i + 1;
	    read_variable_number(i, cursor_start + variable_name.size(), variable);
	    break;
	}
    }

    return start_index;
}

void GameData::read_grid(int &start_index, Level_Info& level_info)
{
    int low_index, high_index;
    bool lb_match = false, rb_match = false;

    for (int i = 0; i < data[start_index].size(); i++)
    {
	if(data[start_index][i] == '{')
	{
	    lb_match = true;
	    low_index = i;
	}

	if(data[start_index][data[start_index].size() - (1+i)] == '}')
	{
	    rb_match = true;
	    high_index = data[start_index].size() - (1+i);
	}
    }

    if(lb_match == false || rb_match == false)
    {
	std::cout << "Coudn't read grid at index #" << start_index << "\n";
	return;
    }

    for (int i = low_index+1; i < high_index; i++)
    {
	char ch = data[start_index][i];
	if(isspace(ch)) continue;
	else
	{
	    uint8_t cell_type;
	    std::pair<uint8_t, uint8_t> cell_pos;

	    cell_pos.first = ch - '0';
	    
	    ch = data[start_index][++i];
	    if(ch != '_'){
		std::cout << "Failed to load figures: wrong input - "
		     << start_index << "\n";
		return;
	    }
	    ch = data[start_index][++i];
	    
	    cell_pos.second = ch - '0';
	    	    printf("cell_pos.second = %d\n", cell_pos.second);
	    ch = data[start_index][++i];
	    if(ch != ':'){
		std::cout << "Failed to load grid: wrong input format - "
		     << start_index << "\n";
		return;
	    }
	    ch = data[start_index][++i];
	    cell_type = ch - '0';

	    level_info.cell_type.push_back(cell_type);
	    level_info.cell_pos.push_back(cell_pos);
	}
    }

    start_index++;
}

void GameData::read_figures(int &start_index, Level_Info& level_info)
{
    int low_index, high_index;
    bool lb_match = false, rb_match = false;

    auto convert_form = [](char ch)
	{
	    Figure_Form form;
	    switch(ch){
		case 'o': case 'O':
		    form = O_figure; break;
		case 'i': case 'I':
		    form = I_figure; break;
		case 'l': case 'L':
		    form = L_figure; break;
		case 'j': case 'J':
		    form = J_figure; break;
		case 'z': case 'Z':
		    form = Z_figure; break;
		case 's': case 'S':
		    form = S_figure; break;
		case 't': case 'T':
		    form = T_figure; break;
		default:
		    form = O_figure; break;
	    }

	    return form;
	};

    auto convert_type = [](char ch)
	{
	    Figure_Type type;
	    switch(ch){
		case 'd': case 'D':
		    type = Figure_Type::classic; break;
		case 's': case 'S':
		    type = Figure_Type::stone; break;
		case 'm': case 'M':
		    type = Figure_Type::mirror; break;
		default:
		    type = Figure_Type::classic; break;
	    }

	    return type;
	};


    for (int i = 0; i < data[start_index].size(); i++)
    {
	if(data[start_index][i] == '{')
	{
	    lb_match = true;
	    low_index = i;
	}

	if(data[start_index][data[start_index].size() - (1+i)] == '}')
	{
	    rb_match = true;
	    high_index = data[start_index].size() - (1+i);
	}
    }

    if(lb_match == false || rb_match == false)
    {
	std::cout << "Coudn't read figures at index #" << start_index << "\n";
	return;
    }

    for (int i = low_index+1; i < high_index; i++)
    {
	char ch = data[start_index][i];
	if(isspace(ch)) continue;
	else
	{
	    std::pair<Figure_Form, Figure_Type> figure_info;
	    uint8_t figure_angle;

	    figure_info.first = convert_form(ch);
	    
	    ch = data[start_index][++i];
	    if(ch != '_'){
		std::cout << "Failed to load figures: wrong input - "
		     << start_index << "\n";
		return;
	    }

	    ch = data[start_index][++i];

	    figure_info.second = convert_type(ch);
	    
	    ch = data[start_index][++i];
	    figure_angle = ch - '0';

	    level_info.figure_type.push_back(figure_info);
	    level_info.figure_angle.push_back(figure_angle);
	}
    }

    start_index++;
	    
}

int GameData::find_level_number(int start_point, int level_number)
{
    std::string index_number = "#" + std::to_string(level_number);

    for (int i = start_point; i < data.size(); i++)
    {
	if(data[i].find(index_number) != std::string::npos)
	{
	    return i;
	}
    }

    return -1;
}

void GameData::load_level(int level_number, Level_Info& level_info)
{
    if(level_number < 1) return;
    
    int start_index = find_level_number(searching_point, level_number);
    
    if(start_index != -1)
    {
	read_variable(start_index, "lvl_status", level_info.status);
	read_variable(start_index, "lvl_time", level_info.time);
	read_variable(start_index, "lvl_row", level_info.row);
	read_variable(start_index, "lvl_column", level_info.column);
	read_figures(start_index, level_info);
	read_grid(start_index, level_info);
    }
    else
    {
	std::cout << "Level #" << level_number << " - doesn't exist in database\n";
	return;
    }
}


void GameData::write_variable_number(int row_index, const std::string& variable)
{
    if(variable.empty()) return;

    data[row_index].clear();
    data[row_index] = variable;
    data[row_index].push_back('\n');
}

void GameData::write_variable(int &start_index, const std::string &variable_name, int variable)
{
    for (int i = start_index; i < data.size(); i++)
    {
	bool is_match = true;
	if(variable_name.size() > data[i].size()) continue;

	for (int j = 0; j < variable_name.size(); j++)
	{
	    if(data[i][j] != variable_name[j])
	    {
		is_match = false;
		break;
	    }
	}

	if(is_match)
	{
	    start_index = i + 1;
	    std::string new_variable = variable_name + " " + std::to_string(variable);
	    write_variable_number(i, new_variable);
	    break;    
	}
    }
}

void GameData::save_level(int level_number, Level_Info& level_info)
{
    if(level_number < 1) return;

    int start_index = find_level_number(searching_point, level_number);
    if(start_index != -1)
    {
	write_variable(start_index, "lvl_status", level_info.status);
	write_variable(start_index, "lvl_time", level_info.time);
    }
    else
    {
	std::cout << "Level #" << level_number << " - doesn't exist in database\n";
	return;
    }
}

std::ostream& operator<<(std::ostream& out, const GameData& game_data)
{
    for(int i = 0; i < game_data.data.size(); ++i)
    {
	out << game_data.data[i];
    }
}

std::istream& operator>>(std::istream& in, GameData& game_data)
{
    game_data.data.empty() ? game_data.data.push_back(std::string{}) : game_data.data.clear();

    for(char ch; in.get(ch);)
    {
	game_data.data.back().push_back(ch);
	if(ch == '\n')
	{
	    game_data.data.push_back(std::string{});
	}
    }
    if(game_data.data.back().size()) game_data.data.push_back(std::string{});
    return in;
}

