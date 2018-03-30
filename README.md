# ENSC251
### REQUIREMENTS TO RUN TESTS

*WRITTEN IN ECLIPSE
*LINUX ENVIRONMENT
*LINUX - X86_64 GNU COMPILER

WRITTEN IN ECLIPSE, WITHIN A LINUX ENVIRONMENT, AND WITH THE LINUX - X86_64 GNU COMPILER 
In ENSC 251, we created a C++ subset language that would parse through C++ source code, construct an Abstract Syntax Tree, and evaluate the statements represented by the tree. 

Throughout the semester we continued to build off previous parts, up until part 5, where we completed our subset C++ Language.
For more details on what aspect of the C++ source parser each part was in charge of, refer to the PDF files, or continue reading below.



## Part 1 - Tokenizer
Part 1 required us to parse an input stream into a sequence of strings representing tokens for the C++ Language Subset. 
Part 1 is not included in the repository because it is used in subsequent parts.

## Part 2 - Token Classifier
Part 2 categorized the tokens gathered in Part 1 through an implementation of classes. Each time a token is found, it is constructed into objects of its corresponding classes and subclasses.

## Part 3 - Tree Traversals
Part 3 was an introduction into implementing tree traversals, recursion, and maintaining memory. Using a file directory as input, we printed directory names and file names using preorder, postorder, and inorder traversals, depending on the amount of files/directories within a given directory.

## Part 4 - Abstract Syntax Tree (AST)
Part 4 utilizes Part 1 and Part 2 in order to organize input tokens into a tree structure we titled an "Abstract Syntax Tree." Creating the tree was achieved through recursive descent parsing and with reference to defined grammar rules (c++SubsetBNF.txt). Furthermore, we traversed the tree using the tree traversals implemented in Part 3.

## Part 5 - Evaluating the AST
Part 5 finalizes our C++ Subset Language by evaluating the statements within the AST obtained through tree traversals in Part 4. In other words, using shared pointers and a map class template from the STL, we were able to store values into identifiers and as such, evaluate a multitude of statements with different operations.
