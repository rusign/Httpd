#ifdef _WIN32
#include "system/dirent.h"
#else
#include <dirent.h>
#endif

#include <algorithm>
#include <sys/stat.h>
#include <tuple>
#include "utils/ForlderView.hh"

#include <iostream>

const std::string zia::utils::FolderView::view(const std::string &fullPath, const std::string &relativePath) const
{
    std::string relativePath_(relativePath);

    auto fileList = getDirectory(fullPath, relativePath);
    if (fileList.empty() && relativePath_.find('/') != std::string::npos)
    {
        relativePath_ = relativePath_.substr(0, relativePath_.find_last_of('/'));
        fileList      = getDirectory(fullPath, relativePath);
    }
    if (fileList.empty())
        return "";

    return formatDirectory(relativePath, fileList);
}

zia::utils::FolderView::FileList zia::utils::FolderView::getDirectory(const std::string &fullPath,
                                                                      const std::string &relativePath) const
{
    struct dirent *epdf;
    auto          fileList = FileList();

    auto dpdf = opendir((fullPath + relativePath).c_str());
    if (dpdf != NULL)
    {
        while ((epdf = readdir(dpdf)))
            if (std::string(epdf->d_name) != ".")
                fileList.push_back({
                                           epdf->d_type == DT_DIR,
                                           epdf->d_name,
                                           getFilesize(fullPath + relativePath + "/" + epdf->d_name)
                                   });
        closedir(dpdf);
    }
    return fileList;
}

const std::string
zia::utils::FolderView::formatDirectory(const std::string &relativePath, const FileList &fileList) const
{
    std::string body;

    body =
"<!DOCTYPE html>\n"
"<html>\n"
"<head>\n"
"    <title>Azia :: Format Viewer</title>\n"
"    <link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">\n"
"</head>\n"
"<body>\n"
"    <div style=\" padding-left: 30%; width: 40%;\">\n"
"        <div style=\" border: solid; border-width: 1px; margin-bottom: 2%; padding-left: 10px; padding-right: 10px; padding-bottom: 5px; padding-top: 5px; border-radius: 0.2em;\">\n"
"           " + relativePath + ""
"        </div>\n"
"        <div style=\" border-color: black; border: 1px; border-style: solid; border-radius: 0.2em; padding-left: 2%; padding-right: 2%; padding-bottom: 2%; padding-top: 2%;\">\n"
"            <table style=\" width: 100%\">\n";

    auto      link = relativePath.back() == '/' ? relativePath.substr(0, relativePath.size() - 1) : relativePath;
    for (auto elem : fileList)
        body +=
"                <tr style=\" text-align: left;\">\n"
"                    <th style=\" width: 4%;\">" + std::string(elem.isFolder ? FOLDER : FILE) + "</th>\n"
"                    <th>\n"
"                        <a href=\"" + link + "/" + elem.name + "\">" + elem.name + "</a>\n"
"                    </th>\n"
"                    <th style=\"text-align: right;\">" + elem.size + "</th>\n"
"                </tr>\n";

    body +=
"            </table>\n"
"        </div>\n"
"    </div>\n"
"</body>\n"
"\n"
"</html>";
    return body;
}

const std::string zia::utils::FolderView::getFilesize(const std::string &filename) const
{
    struct stat st;
    int         i;
    std::string units[] = {" B", " kB", " MB", " GB", " TB", " PB", " EB", " ZB", " YB"};

    if (stat(filename.c_str(), &st) != 0)
        return "NAN";
    auto size = st.st_size;
    for (i = 0; size > 1024; ++i)
        size /= 1024;

    return i < units->size() ? std::to_string(size) + units[i] : " YB+";
}
