#pragma once

#include <map>

#include "Text.h"
#include "Node.h"

/*!
 * Character guesser
 */
class Akinator
{
public:
    /*!
     * Default constructor
     */
    Akinator();

    /*!
     * Constructor which create tree from the data in the file
     * @param fileName - name of the file with data
     */
    Akinator(const char* fileName, const char* dotFileName = "tree.dot");

    /*!
     * Destructor which save data to the file
     */
    ~Akinator();

    //There are 3 operating mode

    /*!
     * Training and guessing
     */
    void Run();

    /*!
     * Give definition to character
     * @param name - name of the character
     */
    void Definition(const std::string& name);

    /*!
     * Compare two characters
     * @param first - name of the first character
     * @param second - name of the second character
     */
    void Compare(const std::string& first, const std::string& second);

    void ShowDotFile();

private:
    /*!
     * Root of the tree
     */
    Node* _root;
    /*!
     * Map with all characters
     */
    std::map<std::string, Node*> _characters;

    /*!
     * Source with tree
     */
    Text text;

    const char* fileName;
    const char* dotFileName;

    std::string buffer;

    char prologue[15] = {'s', 'a', 'y', ' ', '-', 'v', ' ', 'M', 'i', 'l', 'e', 'n', 'a', ' ', '\"'};
    char epilogue;
    void write();

    /*!
     * Write tree to the file with dot format
     * @param fileOut - out file name
     */
    void WriteToDotFile(const char* fileOut);
    /*!
     * Write tree to the file
     * @param fileOut - out file name
     */
    void WriteToBaseFile(const char* fileOut);

    /*!
     * Auxiliary function to write
     * @param node - current node
     * @param file
     */
    void DfsWriteDot(Node* node, FILE* file);
    /*!
     * Auxiliary function to write
     * @param node - current node
     * @param file
     */
    void DfsWriteBase(Node* node, FILE* file);

    /*!
     * Auxiliary function to delete tree
     * @param node - current node
     */
    void Delete(Node* node);

    /*!
     * Parsing source text and build tree
     * @param text - source
     * @return 1 if there is a mistake
     */
    int ParseText(const Text& text);

    /*!
     * Read current node and add it to the tree
     * @param text - source text
     * @param currentInd - current ind in the text
     * @param parent - parent of current node
     * @param comeBy - come by "yes" or "no"
     * @return pointer to the new node
     */
    Node* ReadNode(const Text& text, int& currentInd, Node* parent, bool comeBy);
};
