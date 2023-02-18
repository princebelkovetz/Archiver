#include "archive_maker.h"
#include "archive_unpacker.h"
#include "parser.h"
#include "exceptions.h"

const std::set<std::string> FLAGS{"-c", "-d", "-h"};

int main(int argc, char** argv) {
    try {
        ArgParser parser(argc, argv, FLAGS);
        if (parser.HaveFlag("-c")) {
            std::vector<std::string> files = parser.GetArgs("-c");
            std::string archive = files[0];
            for (size_t i = 0; i + 1 < files.size(); ++i) {
                files[i] = files[i + 1];
            }
            files.pop_back();

            ArchiveMaker archiver(files);
            archiver.ProcessArchiving(archive);

        } else if (parser.HaveFlag("-d")) {
            std::string archive = parser.GetArgs("-d")[0];
            ArchiveUnpacker unpacker(archive);
            unpacker.ProcessUnpacking();

        } else if (parser.HaveFlag("-h")) {
            std::cout << "Archiver program - user guide\n\n";
            std::cout
                << "Use <archiver -c archive_name file1 file2 ...> to archive file1, file2 into file 'archive_name'\n";
            std::cout << "Use <archiver -d archive_name> to decompress archive from file 'archive_name'\n";
            std::cout << "Use <archiver -h> to check tutorial for the program\n\n";
            std::cout << "archiver - made my G. Belkovetz - all rights reserved";
        } else {
            throw FlagNotFoundException();
        }
    } catch (FlagDoesNotExistException& exc) {
        std::cout << exc.what();
    } catch (FlagNotFoundException& exc) {
        std::cout << exc.what();
    } catch (std::exception& exc) {
        std::cout << exc.what();
        return 111;
    }

    return 0;
}
