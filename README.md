# Quadtree Image Compression Tool README

## Introduction

This tool implements a quadtree-based image compression technique. It leverages the quadtree data structure to recursively divide an image into quadrants (or blocks) until each block meets a specified similarity criterion based on color homogeneity. This README outlines the method of implementation and functioning of this tool.

## Method of Implementation

The core functionalities of this tool are implemented in C and involve several key components and processes:

1. **Pixel Structure:** A basic structure (`Pixel`) represents the color of a pixel using RGB values.

2. **Node Structure:** Represents each node in the quadtree, containing pointers to its four child nodes (`nw`, `ne`, `sw`, `se`), the average color of the block it represents, the size of the block, and the level of the node in the tree.

3. **Quadtree Construction:** The function `build_quadtree` recursively divides the image into smaller blocks until each block's color similarity is below a predefined threshold or the block reaches a minimal size. This process starts from the entire image and proceeds by calculating each block's average color and similarity score.

4. **Similarity Score:** A function computes the similarity score for a block by comparing the color of each pixel within the block to the block's average color. This score determines whether a block needs further subdivision.

5. **Utility Functions:** Several utility functions are provided to calculate the average color of a block, determine the similarity score, count the tree's levels, count the blocks below a certain similarity threshold, find the largest undivided square, and deallocate the memory used by the quadtree.

6. **Main Functionality:** The tool supports two main operations based on command-line arguments:
   - Quadtree construction and analysis (`-c1`): Outputs the quadtree's depth, the number of blocks below the similarity threshold, and the size of the largest undivided square.
   - Quadtree compression (`-c2`): Outputs a compressed representation of the image based on the quadtree structure.

## Functioning

1. **Initialization:** The program initializes by reading the input image and processing command-line arguments to determine the operation mode (`-c1` or `-c2`) and the similarity threshold.

2. **Quadtree Construction:** For both operations, the program constructs the quadtree by dividing the image into quadrants recursively, based on the similarity score of the blocks compared to the threshold.

3. **Processing:**
   - In `-c1` mode, it analyzes the constructed quadtree to determine its depth, count the blocks meeting the similarity criteria, and identify the largest undivided square. The results are then outputted to a specified file.
   - In `-c2` mode, the program outputs the compressed image representation by traversing the quadtree and encoding its structure along with the average colors of the leaf nodes.

4. **Memory Management:** After processing, the program deallocates any dynamically allocated memory to prevent memory leaks.

5. **Output:** The results of the operations are saved to files specified by the user, either as an analysis report or a compressed binary file representing the image.
