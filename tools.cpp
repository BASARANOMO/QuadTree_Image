//
//  tools.cpp
//  zipimage
//
//  Created by Essence on 24/10/2020.
//

#include "tools.h"

template<typename T>
QuadTree<T>* dfs(T* image, int x, int y, int length, int const lengthOriginal) {
    if (length == 1) {
        return new QuadLeaf<T>(image[lengthOriginal * y + x]);
    }
    length /= 2;
    auto northWest = dfs(image, x, y, length, lengthOriginal);
    auto northEast = dfs(image, x + length, y, length, lengthOriginal);
    auto southEast = dfs(image, x + length, y + length, length, lengthOriginal);
    auto southWest = dfs(image, x, y + length, length, lengthOriginal);
    
    if (northWest->isLeaf() && northEast->isLeaf() && southEast->isLeaf() && southWest->isLeaf() && northWest->value() == northEast->value() && northEast->value() == southEast->value() && southEast->value() == southWest->value()) {
        T val = northWest->value();
        delete northWest;
        delete northEast;
        delete southEast;
        delete southWest;
        return new QuadLeaf<T>(val);
    }
    else {
        return new QuadNode<T>(northWest, northEast, southEast, southWest);
    }
};

// for black and white photo
template<typename T>
QuadTree<T>* quaddag(T* image, int x, int y, int length, int const lengthOriginal) {
    if (length == 1) {
        if (image[lengthOriginal * y + x] == black->value()) {
            return black;
        }
        else if (image[lengthOriginal * y + x] == white->value()) {
            return white;
        }
        else {
            cout << "Other color!" << endl;
        }
    }
    length /= 2;
    auto northWest = quaddag(image, x, y, length, lengthOriginal);
    auto northEast = quaddag(image, x + length, y, length, lengthOriginal);
    auto southEast = quaddag(image, x + length, y + length, length, lengthOriginal);
    auto southWest = quaddag(image, x, y + length, length, lengthOriginal);
    northWest->protect_leaves_from_destruction = true;
    northEast->protect_leaves_from_destruction = true;
    southEast->protect_leaves_from_destruction = true;
    southWest->protect_leaves_from_destruction = true;
    
    if (northWest->isLeaf() && northEast->isLeaf() && southEast->isLeaf() && southWest->isLeaf() && northWest->value() == northEast->value() && northEast->value() == southEast->value() && southEast->value() == southWest->value()) {
        T val = northWest->value();
        /*
        delete northWest;
        delete northEast;
        delete southEast;
        delete southWest;
        */
        if (val == black->value()) {
            return black;
        }
        if (val == white->value()) {
            return white;
        }
    }
    else {
        QuadTree<T>* nodeTemp = new QuadNode<T>(northWest, northEast, southEast, southWest);
        nodeTemp->protect_leaves_from_destruction = true;
        return nodeTemp;
    }
};

template<typename T>
void quadtreeDecoding(QuadTree<T>* q, T* image, int x, int y, int length, int const lengthOriginal) {
    if (q->isLeaf()) {
        for (int j=0; j < length; j++) {
            for (int i=0; i < length; i++) {
                image[lengthOriginal * (y + j) + (x + i)] = q->value();
            }
        }
    }
    else {
        length /= 2;
        quadtreeDecoding(q->son(0), image, x, y, length, lengthOriginal);
        quadtreeDecoding(q->son(1), image, x + length, y, length, lengthOriginal);
        quadtreeDecoding(q->son(2), image, x + length, y + length, length, lengthOriginal);
        quadtreeDecoding(q->son(3), image, x, y + length, length, lengthOriginal);
    }
};
