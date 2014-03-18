#include "XMLParser.h"
#include "../Debugging/Macros.h"
#include "../Helpers/String.h"
#include <stack>
#include <fstream>

XMLTree::~XMLTree()
{
	for (auto it=Children.begin(); it != Children.end(); it++)
	{
		delete *it;
	}
}

XMLTree* XMLParser::ParseTree()
{
	std::ifstream in(xml_file, std::ios::in);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(static_cast<unsigned>(in.tellg()));
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return ParseTree(contents);
	}
	DEBUG_PRINT("XMLParser: Failed to read XML file." << std::endl);
	return nullptr;
}

XMLTree* XMLParser::ParseTree(std::string& input)
{
	XMLTree* xml_tree = new XMLTree();
	LexList L;
	Lex(L, input);
	std::stack<XMLTree*> trees_stack;
	bool read_tag = false, read_content = false;
	for (auto it = L.begin(); it != L.end(); it++)
	{
		XMLToken token = it->first;
		switch(token)
		{
		case XML_TAG:
			{
				XMLTree* newTree = new XMLTree();
				xml_tree->Children.push_back(newTree);
				trees_stack.push(xml_tree);
				xml_tree = newTree;
				read_content = false;
				xml_tree->Tag = it->second;
				read_tag = true;
			}
			break;
		case XML_ENDTAG:
			if (trees_stack.size() == 0) return xml_tree; // End of parse
			xml_tree = trees_stack.top();
			trees_stack.pop();
			read_content = false;
			break;
		case XML_CONTENT:
			if (read_content) DEBUG_PRINT("XML Parse: Unexpected XML content." << std::endl);
			xml_tree->Content = trim(it->second);
			read_content = true;
			break;
		default:
			DEBUG_PRINT("XMLParser: Encountered unexpected token." << std::endl);
			break;
		}
	}
	return xml_tree;
}

void XMLParser::Lex(LexList& Lex, std::string& input)
{
	unsigned pos = 0, count = 0;
	bool reading_tag = false, reading_content = false, reading_end_tag = false;
	std::stringstream ss;
	while (pos < input.length())
	{
		char c = input[pos];
		switch (c)
		{
		case '<': // Begin reading tag name
			if (reading_tag) DEBUG_PRINT("XML Lex: Error reading XML: unexpected '<'." << std::endl);
			if (count > 0 && reading_content)
			{
				Lex.push_back(std::make_pair(XML_CONTENT, ss.str()));
				ss.str(std::string());
				count = 0;
			}
			reading_tag = true;
			reading_content = false;
			break;
		case '>':
			if (reading_content) DEBUG_PRINT("XML Lex: Error reading XML: unexpected '>'." << std::endl);
			if (count > 0 && reading_tag)
			{
				Lex.push_back(std::make_pair(reading_end_tag ? XML_ENDTAG : XML_TAG, ss.str()));
				ss.str(std::string());
				count = 0;
			}
			reading_tag = false;
			reading_content = true;
			reading_end_tag = false;
			break;
		case '/':
			if (reading_tag) reading_end_tag = true;
			else ss << c;
			break;
		default:
			ss << c;
			count++;
			break;
		}
		pos++;
	}
}
