#pragma once

#include <iostream>

class Node
{
public:
    explicit Node(char nodeValue,
                  std::shared_ptr<Node> leftNode = nullptr,
                  std::shared_ptr<Node> rightNode = nullptr)
        : mNodeValue{nodeValue}
        , mLeftNode{std::move(leftNode)}
        , mRightNode{std::move(rightNode)}
    {
    }

    char getNodeValue() const
    {
        return mNodeValue;
    }

    Node* getMLeftNode() const
    {
        return mLeftNode.get();
    }
    Node* getMRightNode() const
    {
        return mRightNode.get();
    }

private:
    char mNodeValue{};
    std::shared_ptr<Node> mLeftNode{nullptr};
    std::shared_ptr<Node> mRightNode{nullptr};
};