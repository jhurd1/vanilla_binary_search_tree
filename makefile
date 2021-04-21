###############################################################
# Program:
#     Assignment 09, Binary Search Tree
#     Brother Kirby, CS235
# Author:
#     Daniel Perez, Jamie Hurd, Benjamin Dyas
# Summary:
#     This program contains the implementation of binary search
#     tree. insertion and traversal of the tree preserve the
#     primitive order of values. Deletion reorganizes the tree
#     to preserve the order.
# Time:
#     Pair programming: 06 hours
#     Daniel:           06 hours
#     Benjamin:         06 hours
#		Jamie:            06 hours
# The most difficult part:
#     Daniel:   Figuring out the insertion and deletion logic.
#               In specific, the order in which the pointers
#               to and from the successor (and children)
#               should be updated.
#	   Benjamin: The most difficult part was working through the
#               logic for understanding the find and insert
#               functions. We discussed how it should function
#               and came up with a process to implement it
#               into our program. We were able to get it
#               working as desired.
#	   Jamie:    The most difficult part was comprehending 
#				    the role of successor in navigating the tree 
#				    for the erase() function. Additionally, the
#               iterators proved a challenge to understand.
###############################################################

##############################################################
# The main rule
##############################################################
a.out: assignment09.o
	g++ -o a.out assignment09.o -g
	tar -cf assignment09.tar *.h *.cpp makefile

##############################################################
# The individual components
#      assignment09.o     : the driver program
##############################################################
assignment09.o: bst.h assignment09.cpp
	g++ -c assignment09.cpp -g

