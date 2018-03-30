//============================================================================
// Name        : makeDirectoryTree.cpp
// Author      : Mohammad Akbari
// Description : Course Project Part 3 (Directory Tree) - Make Directory Tree
//============================================================================


#include <dirent.h>
#include <iterator>
#include <sys/stat.h>
#include <algorithm> // for find

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

#include "makeDirectoryTree.hpp"

Directory *make_directory_tree(const std::string& dirPath)
{
	path rootPath = path(dirPath);
	if (exists(rootPath))
	{
		auto rootDir = new Directory(rootPath.filename().generic_string());
		//directory_iterator it{rootPath};

		typedef std::vector<path> vec;
		vec v;
		copy(directory_iterator(rootPath), directory_iterator(), back_inserter(v));
		sort(v.begin(), v.end());
		vec::const_iterator it(v.begin()), it_end(v.end());

		while (it != it_end)
		{
			path subPath = path(*it);
			std::string subPathStr = subPath.generic_string();
			if (is_regular_file(subPath))
				rootDir->add_file(subPath.filename().generic_string());
			if (is_directory(subPath))
			{
				auto subDir = make_directory_tree(subPathStr);
				if (!subDir) // if the dir does not exist!
					subDir = new Directory(subPath.filename().generic_string());
				rootDir->add_childP(subDir);
			}
			it++;
		}
		return rootDir;
	}
	else
	{
		OUT << rootPath << " does not exist.\n";
		return nullptr;
	}
}
