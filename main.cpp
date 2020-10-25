#include <Imagine/Graphics.h>
#include <algorithm>
#include <iostream>
#include "quadtree/quadtree.h"

using namespace Imagine;
using namespace std;

// ************************** MY CODE HERE ***************************************** //
// *************************** DECLARATION ***************************************** //
QuadTree<byte> *black = new QuadLeaf<byte>(byte(0));    // black leaf pointer (global variable)
QuadTree<byte> *white = new QuadLeaf<byte>(byte(255));  // white leaf pointer (global variable)

// forward declaration, you can find the definition down below
template<typename T>
QuadTree<T>* dfs(T*, int, int, int, int const);

template<typename T>
QuadTree<T>* quaddag(T*, int, int, int, int const);

// quaddag overload for all rectangle images (no matter whether the image is square or not)
template<typename T>
QuadTree<T>* quaddag(T*, int, int, int, int const, int const, bool const = true);

template<typename T>
void quadtreeDecoding(QuadTree<T>*, T*, int, int, int, int const);

int nextPow2(int);  // find the nearest superior power of 2

template<typename T>
int sizeQuadTree(QuadTree<T>*, bool const = false);

template<typename T>
double imageCompressionRatio(QuadTree<T>*, int const, int const);
// ************************** MY CODE HERE ***************************************** //


int main(int argc, char **argv) {
    cout << "************************ RUNNING HORSE SQUARE *********************************" << endl;
    // Get image file (default is running horse)
    const char *image_file =
        (argc > 1) ? argv[1] : srcPath("running-horse-square.png");
    // Load image
    byte* image;
    int width, height;
    cout << "Loading image: " << image_file << endl; 
    loadGreyImage(image_file, image, width, height);
    // Print statistics
    cout << "Image size: " << width << "x" << height << endl;
    cout << "Number of pixels: " << width*height << endl;
    
    QuadTree<byte>* q;
    q = dfs(image, 0, 0, width, width);
    //display(q);
    
    cout << "********************* RUNNING HORSE SQUARE: DAG TREE ***************************" << endl;
    QuadTree<byte>* qDAG;
    qDAG = quaddag(image, 0, 0, width, width, height, true);
    //display(qDAG);
    
    // image decoding
    byte* imageDecoded = new byte[width * height];
    quadtreeDecoding(qDAG, imageDecoded, 0, 0, width, width);
    
    // DAG tree size and compression ratio
    cout << "QuadTree DAG size (bytes in memory): " << sizeQuadTree(qDAG, true) << endl;
    cout << "Compression ratio (nbr leaves / nbr pixels): " << imageCompressionRatio(qDAG, width, height) << endl;
    
    // Display image decoded
    Window window = openWindow(width, height);
    putGreyImage(IntPoint2(0,0), imageDecoded, width, height);
    // Pause
    click();
    
    cout << "************************ RUNNING HORSE RECTANGLE *******************************" << endl;
    // Get image file (default is running horse)
    const char *image_file2 =
        (argc > 1) ? argv[1] : srcPath("running-horse-rect.png");
    // Load image
    byte* image2;
    int width2, height2;
    cout << "Loading image: " << image_file2 << endl;
    loadGreyImage(image_file2, image2, width2, height2);
    // Print statistics
    cout << "Image size: " << width2 << "x" << height2 << endl;
    cout << "Number of pixels: " << width2*height2 << endl;
    int length2 = nextPow2(max(width2, height2));

    QuadTree<byte>* qDAG2;
    qDAG2 = quaddag(image2, 0, 0, length2, width2, height2, true);
    
    byte* imageDecoded2 = new byte[length2 * length2];
    quadtreeDecoding(qDAG2, imageDecoded2, 0, 0, length2, length2);
    
    // Display image decoded
    //Window window2 = openWindow(length2, length2);
    putGreyImage(IntPoint2(0,0), imageDecoded2, length2, length2);
    // Pause
    click();
    
    // delete pointers (global variables)
    delete black;
    delete white;
    
    // Exit
    return 0;
}

// ************************** MY CODE HERE ***************************************** //
// *************************** FUNCTIONS ******************************************* //
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
            throw "Not a black and white image!";
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

// quaddag overload for all rectangle images (no matter whether the image is square or not)
template<typename T>
QuadTree<T>* quaddag(T* image, int x, int y, int length, int const width, int const height, bool const fillByWhite) {
    if (length == 1) {
        if (x < width && y < height) {
            if (image[width * y + x] == black->value()) {
                return black;
            }
            else if (image[width * y + x] == white->value()) {
                return white;
            }
            else {
                throw "Not a black and white image!";
            }
        }
        else {
            if (fillByWhite) {
                return white;
            }
            else {
                return black;
            }
        }
    }
    length /= 2;
    auto northWest = quaddag(image, x, y, length, width, height, fillByWhite);
    auto northEast = quaddag(image, x + length, y, length, width, height, fillByWhite);
    auto southEast = quaddag(image, x + length, y + length, length, width, height, fillByWhite);
    auto southWest = quaddag(image, x, y + length, length, width, height, fillByWhite);
    northWest->protect_leaves_from_destruction = true;
    northEast->protect_leaves_from_destruction = true;
    southEast->protect_leaves_from_destruction = true;
    southWest->protect_leaves_from_destruction = true;
    
    if (northWest->isLeaf() && northEast->isLeaf() && southEast->isLeaf() && southWest->isLeaf() && northWest->value() == northEast->value() && northEast->value() == southEast->value() && southEast->value() == southWest->value()) {
        T val = northWest->value();
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

int nextPow2(int num) {
    int rval = 1;
    while (rval < num) rval <<= 1;
    return rval;
};

template<typename T>
int sizeQuadTree(QuadTree<T>* q, bool const isBlackWhiteDAG) {
    int nbNodes = q->nNodes();
    int treeSize;
    if (isBlackWhiteDAG) {
        treeSize = 2 * sizeof(QuadLeaf<T>) + nbNodes * sizeof(QuadNode<T>);
    }
    else {
        int nbLeaves = q->nLeaves();
        treeSize = nbLeaves * sizeof(QuadLeaf<T>) + nbNodes * sizeof(QuadNode<T>);
    }
    return treeSize;
};

template<typename T>
double imageCompressionRatio(QuadTree<T>* q, int const width, int const height) {
    int nbLeaves = q->nLeaves();
    double compressionRatio = double(nbLeaves) / double(width * height);
    return compressionRatio;
};
// ************************** MY CODE HERE ***************************************** //
