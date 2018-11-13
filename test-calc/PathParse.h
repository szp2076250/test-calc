#pragma once
#include <string>
#include <vector>

using namespace std;

class PathParse
{
protected:

	string mStrPath;

public:

	PathParse(string strPath) { mStrPath = strPath; }

	vector<string> * Parse();

	~PathParse();

};

