#pragma once

#include <string>

/*!
 * Struct that keep node of tree
 */
struct Node
{
    std::string question;
    Node* yes;
    Node* no;
    Node* parent;
    bool comeBy;

    /*!
     * Constructor that just save string
     * @param str - question or character
     */
    Node(std::string str) :
            question(str),
            yes(nullptr),
            no(nullptr),
            parent(nullptr),
            comeBy(0) {}

    /*!
     * Constructor that connect new node with tree
     * @param str - question or character
     * @param parent - parent in the tree
     * @param comeBy - by what edge came to current node (yes or no)
     */
    Node(std::string str, Node* parent, bool comeBy) :
            question(std::move(str)),
            yes(nullptr), no(nullptr),
            parent(parent),
            comeBy(comeBy) {}

    /*!
     * Split edge: add new question and character
     * @param newQuestion
     * @param newCharacter
     */
    void Split(std::string newQuestion, std::string newCharacter)
    {
        yes = new Node(question, this, 1);
        no = new Node(std::move(newCharacter), this, 0);
        question = std::move(newQuestion);
    }
};
