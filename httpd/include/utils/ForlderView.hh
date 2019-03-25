#include <cstring>
#include <list>
#include <string>
#include <set>

#define FILE    "<i class=\"fa fa-file\" aria-hidden=\"true\"></i>"
#define FOLDER  "<i class=\"fa fa-folder\" aria-hidden=\"true\"></i>"

namespace zia
{
    namespace utils
    {
        class FolderView
        {
            struct FileProperty { bool isFolder; const std::string name; const std::string size; };
            using FileList = std::list<FileProperty>;

        public:
            const std::string view(const std::string &, const std::string &) const;

        private:
            FileList getDirectory(const std::string &, const std::string &) const;
            const std::string formatDirectory(const std::string &, const FileList &) const;
            const std::string getFilesize(const std::string &filename) const;
        };
    }
}