#ifndef IO_MANAGER
#define IO_MANAGER

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

#include "Figure.h"
#include "Level_Info.h"

using namespace std;
using namespace std::chrono;

using Line = std::vector<char>;

static const char* path = "file.bin";

class Text_Iterator{
public:
     Text_Iterator(std::vector<Line>::iterator ll, Line::iterator pp)
	  : ln{ll}, pos{pp} {}
     ~Text_Iterator(){}

     char& operator*() { return *pos; }
     Text_Iterator& operator++(){
	  ++pos;
	  if(pos == (*ln).end()){
	       ++ln;
	       pos = (*ln).begin();
	  }
	  return *this;
     }
     Text_Iterator& operator+(const int offset){
	  pos += offset;
	  return *this;
     }
     int operator-(const Text_Iterator& it){
	  int result = ln - it.ln;
	  return result; 
     }

     bool operator==(const Text_Iterator& other) const
	  { return ln==other.ln && pos==other.pos; }
     bool operator!=(const Text_Iterator& other) const
	  { return !(*this==other);}
     
private:
     std::vector<Line>::iterator ln;
     Line::iterator pos; // TODO(Max); we would probably need a std::string here
};

struct Document
{
     Document() {line.push_back(Line{});}
     ~Document() {}

     Text_Iterator begin()
	  { return Text_Iterator(line.begin(), line.begin()->begin());}
     Text_Iterator end()
	  {
	       auto last = line.end();
	       --last;
	       return Text_Iterator(last, (*last).end());
	  }

     void update_line(int row, const std::string&);

     friend std::istream& operator>>(std::istream&, Document&);
     friend std::ostream& operator<<(std::ostream&, const Document&);
private:
     std::vector<Line> line;
};

Text_Iterator find_txt(Text_Iterator &first, Text_Iterator &last, const std::string& s);

void read_bin(Document &doc);
void save_bin(Document &doc);
bool read_level_info(Document &doc, Text_Iterator& it, Level_Info& lvl, int lvl_number, const std::string& lvl_mode);

void update_level_info(Document &doc, Level_Info& lvl, int lvl_number, const std::string& lvl_mode);

void read_variable(Document &doc, Text_Iterator &start, const std::string &str, int& variable);
void write_variable(Document &doc, Text_Iterator &start, std::string str, int& variable);

#endif
