#include "cme/input/file_order_reader.hpp"

CMEFileOrderReader::CMEFileOrderReader(
    const std::string& filePath)
    : inputFile(filePath)
{
}

bool CMEFileOrderReader::isOpen() const
{
    return inputFile.is_open();
}

bool CMEFileOrderReader::readNextLine(
    std::string& line)
{
    return static_cast<bool>(
        std::getline(inputFile, line));
}