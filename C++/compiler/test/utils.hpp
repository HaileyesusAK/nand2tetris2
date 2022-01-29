#ifndef __UTILS_H__
#define __UTILS_H__

#include <filesystem>

/* A helper class that defines utility methods */
class Utils {
	public:
        static bool cmpFiles(const std::filesystem::path& p1, const std::filesystem::path& p2);
};

#endif
