#ifndef CME_INPUT_FILE_ORDER_READER_HPP
#define CME_INPUT_FILE_ORDER_READER_HPP

#include <fstream>
#include <string>

/* Reads order input lines from a file. */
class CMEFileOrderReader
{
public:
    /* Opens the supplied order input file. */
    explicit CMEFileOrderReader(const std::string& filePath);

    /* Returns whether the order input file was opened successfully. */
    bool isOpen() const;

    /* Reads the next order input line when one exists. */
    bool readNextLine(std::string& line);

private:
    // The input file containing order commands.
    std::ifstream inputFile;
};

#endif // CME_INPUT_FILE_ORDER_READER_HPP