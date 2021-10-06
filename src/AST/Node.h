#pragma once

#include <iostream>

class Node
{
public:
    Node() = default;

    explicit Node(char nodeValue, Node* leftNode = nullptr, Node* rightNode = nullptr)
        : mNodeValue{nodeValue}
        , mLeftNode{leftNode}
        , mRightNode{rightNode}
    {
    }

    Node& operator=(const Node& nodeToCopy)
    {
        mNodeValue = nodeToCopy.mNodeValue;
        mLeftNode = nodeToCopy.mLeftNode;
        mRightNode = nodeToCopy.mRightNode;
        return *this;
    }

    char getNodeValue() const{
        return mNodeValue;
    }

    void printNode()
    {
        std::cout << "Node value: " << mNodeValue << "\n";
        if (mLeftNode) {
            std::cout << "Left ";
            mLeftNode->printNode();
        }

        if (mRightNode) {
            std::cout << "Right ";
            mRightNode->printNode();
        }
    }
    Node* getMLeftNode() const
    {
        return mLeftNode;
    }
    Node* getMRightNode() const
    {
        return mRightNode;
    }

private:
    char mNodeValue{};
    Node* mLeftNode{nullptr};
    Node* mRightNode{nullptr};
};