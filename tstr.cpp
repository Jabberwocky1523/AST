// #include <iostream>
// #include <filesystem>

// namespace fs = std::filesystem;

// std::optional<fs::path> findFile(const fs::path &directory, const std::string &filename)
// {
//     for (const auto &entry : fs::recursive_directory_iterator(directory))
//     {
//         if (entry.is_regular_file() && entry.path().filename() == filename)
//         {
//             return fs::absolute(entry.path());
//         }
//     }
//     return std::nullopt;
// }

// int mainstr()
// {
//     fs::path directory = "./";
//     std::string filename = "vector.lua";

//     auto result = findFile(directory, filename);
//     if (result)
//     {
//         std::cout << "文件找到: " << result.value() << std::endl;
//     }
//     else
//     {
//         std::cout << "文件未找到: " << filename << std::endl;
//     }

//     return 0;
// }