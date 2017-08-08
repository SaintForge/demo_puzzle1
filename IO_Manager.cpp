#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <list>
#include <chrono>

#include <stdio.h>

#include "IO_Manager.h"
#include "Figure.h"

std::istream &operator>>(std::istream& in, Document& d){
     for(char ch; in.get(ch);){
	  d.line.back().push_back(ch);
	  if(ch == '\n')
	       d.line.push_back(Line{});
     }
     if(d.line.back().size()) d.line.push_back(Line{});
     return in;
}

std::ostream& operator<<(std::ostream& out, Document& doc){
     auto it = doc.begin();
     auto end = doc.end();
     
     for (; it != end; ++it) {
	  out << *it;
     }

     return out;
}

void Document::update_line(int row, const std::string& str){
     line[row].clear();

     for (int i = 0; i < str.size(); i++) {
	  line[row].push_back(str[i]);
     }
     
     line[row].push_back('\n');
}

bool match(Text_Iterator first, Text_Iterator last, const std::string& s){
     for(int i = 0; i < s.size(); ++i)
     {
	  if(first == last) return 0;
	  else if(*first != s[i]) return 0;
	  else ++first;
     }
     return 1;
}

Text_Iterator find_txt(Text_Iterator &first, Text_Iterator &last, const std::string& s){
     if(s.size() == 0)
	  return last;
     char first_char = s[0];
     while(true){
	  auto p = std::find(first, last, first_char);
	  if(p == last || match(p, last, s)){
	       return p;
	  }
	  first = ++p;
     }
}

int read_number(Text_Iterator &first){
     std::string number;
     int result = 0;
     while(*first != '\n'){
	  if(isdigit(*first)) number.push_back(*first);
	  ++first;
	  if(!number.empty() && isspace(*first))
	       break;
     }
     result = std::stoi(number);
     return result;
}

void read_variable(Document &doc, Text_Iterator &start, const std::string &str, int& variable){
     auto it = find_txt(start, doc.end(), str);
     if(it == doc.end())
	  cout << "Failed to find - \"" << str << "\"\n";
     else{
	  it = it + str.size();
	  start = it;
	  variable = read_number(start);
     }
}

void write_variable(Document &doc, Text_Iterator &start, std::string str, int& variable){
     auto it = find_txt(start, doc.end(), str);
     if(it == doc.end())
	  cout << "Failed to find - \"" << str << "\"\n";
     else{
	  int row = it - doc.begin();
	  str.append(" ");
	  str.append(std::to_string(variable));
	  doc.update_line(row, str);
     }
}


Figure_Form read_figure_form(char ch){
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
}

Figure_Type read_figure_type(char ch){
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
}

void read_bin(Document &doc){
     std::ifstream file;
     file.open("file.bin", std::ios::in | std::ios::binary);
     file >> doc;
     file.close();
}

void save_bin(Document &doc){
    std::ofstream file;
    file.open("file.bin", std::ios::out|std::ios::binary);
    
    if(file.is_open()){
	file << doc;
	file.close();
    }
    
}

Text_Iterator get_level_iterator(Document &doc, int number, const std::string& lvl_mode){
    std::string lvl_number = "#";
    lvl_number.append(std::to_string(number));

    Text_Iterator it = find_txt(doc.begin(), doc.end(), lvl_mode);
    it = find_txt(it, doc.end(), lvl_number);

    return it;
}

bool read_figures(Document &doc, Level_Info& lvl, Text_Iterator& it, int lvl_number){
     Text_Iterator start = it;
     start = find_txt(start, doc.end(), "{");
     if(start == doc.end()){
	  cout << "Failed to load figures: left bracket '{' wasn't found - "
	       << lvl_number << "\n";
	  return false;
     }
     ++start;
     
     Text_Iterator stop = start;
     stop = find_txt(stop, doc.end(), "}");
     if(start == doc.end()){
	  cout << "Failed to load figures: right bracket '}' wasn't found - "
	       << lvl_number << "\n";
	  return false;
     }

     while(start != stop){
     	  char ch = *start;
	  if(isspace(ch)){
	       ++start;
	       continue;
	  }
	  figure_info pair;
	  uint8_t angle;

	  pair.first = read_figure_form(ch);
	  ++start;
	  ch = *start;
	  if(ch != '_'){
	       cout << "Failed to load figures: wrong input - "
		    << lvl_number << "\n";
	       return false;
	  }
	  ++start;
	  ch = *start;
	  
	  pair.second = read_figure_type(ch);
	  ++start;
	  ch = *start;
	  angle = ch - '0';
	  lvl.angles.push_back(angle);
	  lvl.figures.push_back(pair);

	  ++start;
     }

     it = stop;
     
     return true;
}

bool read_grid(Level_Info& lvl,
	       Text_Iterator& begin,
	       Text_Iterator& end,
	       int lvl_number){
     
     Text_Iterator start = begin;
     start = find_txt(start, end, "{");
     if(start == end){
	  cout << "Failed to load grid: left bracket '{' wasn't found - "
	       << lvl_number << "\n";
	  return false;
     }
     ++start;
     
     Text_Iterator stop = start;
     stop = find_txt(stop, end, "}");
     if(start == end){
	  cout << "Failed to load grid: right bracket '}' wasn't found - "
	       << lvl_number << "\n";
	  return false;
     }

     while(start != stop){
     	  char ch = *start;
	  if(isspace(ch)){
	       ++start;
	       continue;
	  }
	  uint8_t bit;
	  std::pair<uint8_t, uint8_t> pos;

	  pos.first = ch - '0';
	  ++start;
	  
	  ch = *start;
	  if(ch != '_'){
	       cout << "Failed to load grid: wrong input format- "
		    << lvl_number << "\n";
	       return false;
	  }
	  ++start;
	  
	  ch = *start;
	  pos.second = ch - '0';
	  ++start;

	  ch = *start;
	  if(ch != ':'){
	       cout << "Failed to load grid: wrong input format - "
		    << lvl_number << "\n";
	       return false;
	  }
	  ++start;
	  
	  ch = *start;
	  bit = ch - '0';

	  lvl.bit.push_back(bit);
	  lvl.pos.push_back(pos);

	  ++start;
     }
     
     return true;
}

bool read_level_info(Document& doc, Text_Iterator& it,Level_Info& lvl, int lvl_number, const std::string& lvl_mode){
     if(lvl_number < 1) return false;

     if(it == doc.begin())
	 it = get_level_iterator(doc, lvl_number, lvl_mode);
     
     if(it == doc.end()){
	  cout << "Failed to find - " << lvl_number << "\n";
	  return false;
     }
     
     read_variable(doc, it, "lvl_status", lvl.status);
     read_variable(doc, it, "lvl_time", lvl.time);
     read_variable(doc, it, "lvl_row", lvl.row);
     read_variable(doc, it, "lvl_column", lvl.column);

     if(!lvl.figures.empty()) lvl.figures.clear();
     
     if(!read_figures(doc, lvl, it, lvl_number))
	  return false;
     if(!read_grid(lvl, it, doc.end(), lvl_number)){
	  return false;
     }

     return true;
}



void update_level_info(Document &doc, Level_Info& lvl, int lvl_number, const std::string& lvl_mode){
     if(lvl_number < 1) return;
     
     Text_Iterator it = get_level_iterator(doc, lvl_number, lvl_mode);
     if(it == doc.end()){
	  cout << "Failed to find - " << lvl_number << "\n";
	  return;
     }
     
     write_variable(doc, it, "lvl_status", lvl.status);
     write_variable(doc, it, "lvl_time", lvl.time);
}
