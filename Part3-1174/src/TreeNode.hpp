//============================================================================
//
//% Student Name 1: student1
//% Student 1 #: 123456781
//% Student 1 userid (email): stu1 (stu1@sfu.ca)
//
//% Student Name 2: student2
//% Student 2 #: 123456782
//% Student 2 userid (email): stu2 (stu2@sfu.ca)
//
//% Below, edit to list any people who helped you with the code in this file,
//%      or put ‘none’ if nobody helped (the two of) you.
//
// Helpers: _everybody helped us/me with the assignment (list names or put ‘none’)__
//
// Also, list any resources beyond the course textbook and the course pages on Piazza
// that you used in making your submission.
//
// Resources:  ___________
//
//%% Instructions:
//% * Put your name(s), student number(s), userid(s) in the above section.
//% * Enter the above information in Directory.hpp too.
//% * Edit the "Helpers" line and "Resources" line.
//% * Your group name should be "P3_<userid1>_<userid2>" (eg. P3_stu1_stu2)
//% * Form groups as described at:  https://courses.cs.sfu.ca/docs/students
//% * Submit one or two files to courses.cs.sfu.ca
//
// Name        : TreeNode.hpp
// Description : Course Project Part 3 (Directory Tree)
//============================================================================

#ifndef SRC_TREENODE_HPP_
#define SRC_TREENODE_HPP_

#include <iostream>
#include <vector>

//#define USE_OSS

#ifndef USE_OSS
#define OUT std::cout
#else
#include <sstream>
extern std::ostringstream oss;
#define OUT oss
#endif

namespace ensc251 {

	static unsigned count = 0;

	class TreeNode
	{
		typedef std::vector<TreeNode*> TreeNodePVect;

		protected:
			TreeNodePVect childPVector;
			typedef void (TreeNode::*action_func)(void);
			typedef void (TreeNode::*traverse_func)(void);

		public:
			TreeNode() {}
			TreeNode(const TreeNode& nodeToCopy) { 
				if (nodeToCopy.childPVector.size()){
					for(auto childP : nodeToCopy.childPVector){
						add_childP(childP->clone());
					}
				}
			}

			virtual TreeNode* clone() const { return new TreeNode(*this); }

			virtual ~TreeNode() {
				OUT << "Destroying TreeNode with " << childPVector.size() << " children."<< std::endl;
				for(auto child : childPVector){
					delete child;
				}
			}

			friend void swap(TreeNode& first, TreeNode& second) // the swap function (should never fail!)
		    {
		    	std::swap(first.childPVector, second.childPVector);
		    }

		    TreeNode & operator = (TreeNode other) // note: argument passed by value, copy made!
		    {
		    	//after passing into swap(first,second), other is passed by value, thus preserving
		    	//its value when originally called with the overloaded assignment operator
		    	swap(*this,other);
		        return *this; // by convention, always return *this
		    }

			void add_childP(TreeNode* child) { childPVector.push_back(child); }
			void add_children(const TreeNodePVect& childPV) { childPVector.insert(childPVector.end(), childPV.begin(), childPV.end()); }
			const TreeNodePVect& get_children() const { return childPVector; }

			void traverse_children_post_order(traverse_func tf, action_func af)
			{
			    for(auto childP : childPVector) {
			        (childP->*tf)();
			    }
			    (this->*af)();
			}

			void traverse_children_pre_order(traverse_func tf, action_func af)
			{
				(this->*af)();
				for(auto childP: childPVector){
					(childP->*tf)();
				}
			}

			void traverse_children_in_order(traverse_func tf, action_func af)
			{
				(childPVector[0]->*tf)();
				(this->*af)();
				(childPVector[1]->*tf)();
			}

			void count_action()
			{
				count++;
			}

			void count_traverse()
			{
				traverse_children_post_order(&ensc251::TreeNode::count_traverse, &ensc251::TreeNode::count_action);
			}
			void reset_count() { count = 0; }
			unsigned get_count() const { return count; }

	};
}

#endif /* SRC_TREENODE_HPP_ */