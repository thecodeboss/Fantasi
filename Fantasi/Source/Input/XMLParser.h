#ifndef XMLParser_h__
#define XMLParser_h__
#include <vector>
#include <string>
#include <sstream>
#include "../Debugging/Macros.h"

struct XMLTree
{
	std::string Tag;
	std::string Content;
	std::vector<XMLTree*> Children;
	~XMLTree();
};

enum XMLToken
{
	XML_TAG,
	XML_ENDTAG,
	XML_CONTENT
};

class XMLParser
{
	typedef std::vector<std::pair<XMLToken,std::string> > LexList;
	std::string xml_file;
	XMLTree* ParseTree(std::string& input);
	void Lex(LexList& Lex, std::string& input);
public:
	XMLTree* ParseTree();
	XMLParser() {}
	XMLParser(std::string file) : xml_file(file) {}
};

#endif // XMLParser_h__
