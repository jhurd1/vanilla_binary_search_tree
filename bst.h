/***********************************************************************
 * Component:
 *    Assignment 09, Binary Search Tree (BST)
 *    Brother <your instructor name here>, CS 235
 * Author:
 *    Daniel Perez, Jamie Hurd, Benjamin Dyas
 * Summary:
 *    Create a binary search tree
 ************************************************************************/

#ifndef BST_H
#define BST_H

namespace custom
{
   /************************************************
    * BST
    * Container class element that contains info
    * on previous and next elements in a list
    ***********************************************/
   template<class T>
   class BST {
      public:
         // constructors and destructor
         BST(): numElements{0}, root{} {}
         BST(const BST & rhs);
         ~BST() {deleteBinaryTree(root); numElements = 0;};

         //overloaded operators
         BST & operator =  (const BST & rhs); // assignment
         
         // standard BST interfaces
         int       size() const { return numElements; }
         bool      empty()const { return (numElements == 0); }
         void      clear();
         void      insert(const T& t);
         

         // nested classes
         class BNode;
         class iterator;
         
         // iterator interfaces
         void      erase(iterator it);
         iterator  find(const T & element);
         iterator  begin();
         iterator  end()    { return iterator(nullptr); }
         iterator  rbegin();
         iterator  rend()   { return iterator(nullptr); }

         // BNODE interfaces
         BNode *   getRoot() {return root;}

         //root control
         void rootUpdate()
         {
            // if the root moved out from under us, find it again.
            while (root->pParent != NULL)
            root = root->pParent;
         }

      private:
         //private members
         int numElements;
         BNode * root;
         
         // private interfaces
         //void deleteNode(BNode * nodeDelete, bool right); // used for red-black balancing
         void deleteBinaryTree(BNode * & nodeDelete);
         void copyBinaryTree(BNode * src, BNode * & dest);
         
   };

/********************************************
 * FUNCTION:  BST :: COPY CONSTRUCTOR
 * RETURNS:   ...
 * PARAMETER: ...
 ********************************************/
template <class T>
BST<T> :: BST(const BST<T> & rhs) : numElements{0}, root{nullptr}
{
   //copy the whole tree starting at the given node
   this->copyBinaryTree(rhs.root, this->root);

   //update the number of elements
   this->numElements = rhs.numElements;
   
}

/********************************************
 * FUNCTION:  Assignment Operator Overload
 * RETURNS:   ...
 * PARAMETER: ...
 ********************************************/
template <class T>
BST<T> & BST<T>::operator = (const BST & rhs)
{
   //delete the tree (clear)
   this->clear();

   //copy the incumbent tree into this BST
   this->copyBinaryTree(rhs.root, this->root);

   //update the number of Elements
   this->numElements = rhs.numElements;

   //return a reference to this BST
   return *this;
}

/********************************************
 * FUNCTION:     CLEAR
 * DESCRIPTION:  clears the list
 * PARAMETER:    N/A
 ********************************************/
template <class T>
void BST<T>::clear()
{  
   //delete starting at this node
   deleteBinaryTree(this->root);

   //update the number of elements
   numElements = 0;
}

/********************************************
 * FUNCTION:     INSERT
 * DESCRIPTION:  Inserts an element in the 
 *               appropriate place in a tree
 * PARAMETER:    N/A
 ********************************************/
template <class T>
void BST<T>::insert(const T & t)
{
   
   //if the root node is non-existent, make the
   //   new element the root
   if(this->root == nullptr)
   {
      // try to create the first node
      try
      {
         root = new BNode(t);
         //if new succeeds, update the number of elements
         numElements++;
         root->balance();
         rootUpdate();
         //exit if this is the first node
         return;
      }
      catch(std::bad_alloc)
      {
         throw "ERROR: Unable to allocate a node";
      }
   }

   //create a node to traverse the tree
   //   this could have done through iterators but I
   //   decided to do it like this for practice. There
   //   are more examples of traversal with iterators
   BNode * nodeTraverse;

   //start out with the root
   nodeTraverse = root;
   
   //while the current BNode is valid
   while(nodeTraverse)
   {
      //if the incumbent element is less than the nodes value
      if(t < nodeTraverse->data )
      {
         //if there is a left element, move left
         if(nodeTraverse->pLeft)
         {
            nodeTraverse = nodeTraverse->pLeft;
         }
         //if there is not, then t should go on the left
         else
         {
            //allocate the new left node and update pointers
            //   and number of elements
            try
            {
               nodeTraverse->pLeft = new BNode(t);
               nodeTraverse->pLeft->pParent = nodeTraverse;
               numElements++;
               nodeTraverse->pLeft->balance();
               rootUpdate();
               return;
            }
            catch(const std::exception& e)
            {
               std::cerr << e.what() << '\n';
            }
            
         }
      }
      //if the incumbent element is greater than the nodes value
      else if (t > nodeTraverse->data)
      {
         //if there is a right element
         if(nodeTraverse->pRight)
         {
            //move right
            nodeTraverse = nodeTraverse->pRight;
         }
         //if there is not, then t should go on the right
         else
         {
            //allocate the new right node and update pointers
            //   and number of elements
            try
            {
               nodeTraverse->pRight = new BNode(t);
               nodeTraverse->pRight->pParent = nodeTraverse;
               numElements++;
               nodeTraverse->pRight->balance();
               rootUpdate();
               return;
            }
            catch(const std::exception& e)
            {
               std::cerr << e.what() << '\n';
            }
            
         }
      }
   }
}

/********************************************
 * FUNCTION:     ERASE
 * DESCRIPTION:  
 * PARAMETER:    iterator it
 ********************************************/
template <class T>
void BST<T>::erase(iterator it)
{
   //if the node has no children
   if ( (it.pNode->pRight == nullptr) &&
        (it.pNode->pLeft  == nullptr) )
   {
      //if we are not at the root
      //check if we are the right node to the parent
      if ( (it.pNode->pParent != nullptr) && 
           (it.pNode->pParent->pRight == it.pNode) )
      {
         //update the appropriate right pointer to NULL
         it.pNode->pParent->pRight = nullptr;         
      }

      //if we are not at the root
      //check if we are the left node to the parent
      if ( (it.pNode->pParent != nullptr) && 
           (it.pNode->pParent->pLeft == it.pNode) )
      {
         //update the appropriate left pointer to NULL
         it.pNode->pParent->pLeft = nullptr;         
      }

      //delete the node
      delete it.pNode;
      //just tidying up
      it.pNode = nullptr;
      return;
   }

   //if the node has only one child (left)
   if ( (it.pNode->pRight == nullptr) &&
        (it.pNode->pLeft  != nullptr) )
   {
      //update the left child's parent pointer to now point
      //   to the parent of the node that is being deleted
      it.pNode->pLeft->pParent = it.pNode->pParent;

      //if we are not at the root
      //check if we are the right node to the parent
      if ( (it.pNode->pParent != nullptr) &&
           (it.pNode->pParent->pRight == it.pNode) )
      {
         //update the appropriate right pointer to now
         //   point to the child(left)
         it.pNode->pParent->pRight = it.pNode->pLeft;
      }

      //if we are not at the root
      //check if we are the left node to the parent
      if ( (it.pNode->pParent != nullptr) &&
           (it.pNode->pParent->pLeft == it.pNode) )
      {
         //update the appropriate left pointer to now
         //   point to the child(left)
         it.pNode->pParent->pLeft = it.pNode->pLeft;
      }

      //delete the node
      delete it.pNode;
      //just tidying up
      it.pNode = nullptr;
      return;
   }

   //if the node has only one child (Right)
   if ( (it.pNode->pRight == nullptr) &&
        (it.pNode->pLeft  != nullptr) )
   {
      //update the right child's parent pointer to now point
      //   to the parent of the node that is being deleted
      it.pNode->pRight->pParent = it.pNode->pParent;

      //if we are not at the root
      //check if we are the right node to the parent
      if ( (it.pNode->pParent != nullptr) &&
           (it.pNode->pParent->pRight == it.pNode) )
      {
         //update the appropriate right pointer to now
         //   point to the child(right)
         it.pNode->pParent->pRight = it.pNode->pRight;
      }

      //if we are not at the root
      //check if we are the left node to the parent
      if ( (it.pNode->pParent != nullptr) &&
           (it.pNode->pParent->pLeft == it.pNode) )
      {
         //update the appropriate left pointer to now
         //   point to the child(right)
         it.pNode->pParent->pLeft = it.pNode->pRight;
      }

      //delete the node
      delete it.pNode;
      //just tidying up
      it.pNode = nullptr;
      return;
   }

   //if the node has two children
   if ( (it.pNode->pRight != nullptr) &&
        (it.pNode->pLeft  != nullptr) )
   {
      //iterator to the successor
      iterator successor = it;
      //pointer to the successor's child
      BNode * childToSuccessor;

      //first find successor by incrementing the iterator
      //   it should take us straight to the next node in
      //   line.
      successor++;

      //if the successor has a child
      if (successor.pNode->pRight)
      {
         //assign the child to the pointer
         childToSuccessor = successor.pNode->pRight;
         //update child to have the same pointer to parent
         //   as the successor it is replacing.
         childToSuccessor->pParent = successor.pNode->pParent;

         //check if successor is a left node of the parent
         if(successor.pNode->pParent->pLeft == successor.pNode)
         {
            //update the parent to point to the new successor
            successor.pNode->pParent->pLeft = childToSuccessor;
         }
         //check if successor is a right node of the parent
         else if(successor.pNode->pParent->pRight == successor.pNode)
         {
            //update the parent to point to the new successor
            successor.pNode->pParent->pRight = childToSuccessor;
         }
      }
      //if it doesn't have a child, the successor's parent should
      //   be updated to make the pointer null
      else
      {
         //check if successor is a left node of the parent
         if(successor.pNode->pParent->pLeft == successor.pNode)
         {
            //update the parent to point to the new successor
            successor.pNode->pParent->pLeft = nullptr;
         }
         //check if successor is a right node of the parent
         else if(successor.pNode->pParent->pRight == successor.pNode)
         {
            //update the parent to point to the new successor
            successor.pNode->pParent->pRight = nullptr;
         }
      }
      

      //now that the successor's child (if any) has taken the
      //   successor's place, have the successor replace the node
      //   to be deleted.

         successor.pNode->pParent = it.pNode->pParent;
         successor.pNode->pLeft = it.pNode->pLeft;
         successor.pNode->pRight= it.pNode->pRight;

      
      //if pNode->pParent is different than null,
      //   update parent pointer
      if(it.pNode->pParent != nullptr)
      {
         //we don't know if this node is a right or left node
         //   to the parent so we have to check
         // if pNode is equal to it's parents left, we're in the left
         if (it.pNode->pParent->pLeft == it.pNode)
         {
            it.pNode->pParent->pLeft = successor.pNode;
         }
         //else we're in the right
         else
         {
            it.pNode->pParent->pRight = successor.pNode;
         }
      }
      //else if it is nullptr, it is the root and our root needs to be updated
      else
      {
         this->root = successor.pNode;
      }

      //if there is a left node, update it to point back at the 
      //   new successor
      if(it.pNode->pLeft != nullptr)
      {
         it.pNode->pLeft->pParent = successor.pNode;
      }
      //if there is a right node, update it to point back at the 
      //   new successor
      if(it.pNode->pRight != nullptr)
      {
         it.pNode->pRight->pParent = successor.pNode;
      }
      
      delete it.pNode;
      //just tidying up
      it.pNode = nullptr;
      return;
   }
}

/********************************************
 * FUNCTION:     FIND
 * DESCRIPTION:  Finds the location of an
 *               element on the list or
 *               returns NULL if not found
 * PARAMETER:    T element
 ********************************************/
template <class T>
typename BST<T>::iterator BST<T>::find(const T & element)
{
   //we start our iterator at the root
   iterator it(this->root);
   //this is the same as the end() function
   iterator itNull(nullptr);

   //if the root node is non-existent, return a null iterator
   if(this->root == nullptr)
   {
      return it;
   }
   
   //while the current BNode is valid
   while(it != itNull)
   {
      //if it's found return it
      if (element == *it)
      {
         return it;
      }
      //if its less than, decrement...
      if(element < *it )
      {
         it--;

         //if after decrementing, the next iterator is
         //   greater, it means the element is not there
         //   otherwise it would have been found
         if((it == nullptr) || (element > *it))
         {
            //return nullptr when not found
            return itNull;
         }
      }
      else if (element > *it)
      {
         it++;

         //if after incrementing, the next iterator is
         //   lesser, it means the element is not there
         //   otherwise it would have been found
         if((it == nullptr) || (element < *it))
         {
            //return nullptr when not found
            return itNull;
         }
      }
   }

   //it should never make it here, but as a caution, nullptr
   return itNull;
}

/********************************************
 * FUNCTION:     BEGIN
 * DESCRIPTION:  
 * PARAMETER:    N/A
 ********************************************/
template <class T>
typename BST<T>::iterator BST<T>::begin()
{
   //start iterator at the root
   iterator it(this->root);
   //same as end()
   iterator itNull(nullptr);

   //if the root node is non-existent, return a null iterator
   if(this->root == nullptr)
   {
      return it;
   }
   
   //while the current BNode is valid
   while(it != itNull)
   {
      //start itCompare at the value pre-decrement
      iterator itCompare = it;

      it--;

      //if after decrementing we hit nullptr
      if (it == itNull)
      {
         //return the value previous to it, which logically
         //  means that we made it to the first element.
         //  this is as a result of the behaviour of the iterator
         return itCompare;
      }

   }

   //if we make it here, return nullptr
   return iterator(nullptr);
}

/********************************************
 * FUNCTION:     RBEGIN
 * DESCRIPTION:  
 * PARAMETER:    N/A
 ********************************************/
template <class T>
typename BST<T>::iterator BST<T>::rbegin()
{
   //start iterator at the root
   iterator it(this->root);
   //same as end()
   iterator itNull(nullptr);

   //if the root node is non-existent, return a null iterator
   if(this->root == nullptr)
   {
      return it;
   }
   
   //while the current BNode is valid
   while(it != itNull)
   {
      //start itCompare at the value pre-decrement
      iterator itCompare = it;

      //opposite to begin()
      it++;
      
      //if after decrementing we hit nullptr
      if (it == itNull)
      {
         //return the value previous to it, which logically
         //  means that we made it to the first element.
         //  this is as a result of the behaviour of the iterator
         return itCompare;
      }

   }
   
   //if we make it here, return nullptr
   return iterator(nullptr);
}

/********************************************
 * FUNCTION:  DELETEBINARYTREE
 * RETURNS:   N/A
 * PARAMETER: node reference
 ********************************************/
template <class T>
void BST<T>::deleteBinaryTree(BNode * & nodeDelete)
{
   //if it's null, just exit the function
   if (!nodeDelete)
   {
      return;
   }
   
   // recursive call to delete all left nodes
   deleteBinaryTree(nodeDelete->pLeft);
   // recursive call to delete all right nodes
   deleteBinaryTree(nodeDelete->pRight);
   // deletes the provided node
   delete nodeDelete;
   // sets to nullptr to be caught by the base case
   nodeDelete = nullptr;
}

/********************************************
 * FUNCTION:  COPYBINARYTREE
 * RETURNS:   N/A
 * PARAMETER: BNode * src, BNode * dest
 ********************************************/
template <class T>
void BST<T>::copyBinaryTree(BNode * src, BNode * & dest)
{

   // check if null
   if (src == nullptr)
   {
      return;
   }
   
   // try to create the first node
   try
   {
      dest = new BNode(src->data);
   }
   catch(std::bad_alloc)
   {
      throw "ERROR: Unable to allocate a node";
   }

   // recursive call to reach the left end
   copyBinaryTree(src->pLeft, dest->pLeft);
   if (dest->pLeft)
   {
      dest->pLeft->pParent = dest;
   }
   
   // recursive call to reach the right end
   copyBinaryTree(src->pRight, dest->pRight);
   if (dest->pRight)
   {
      dest->pRight->pParent = dest;
   }
   
}

/*******************NESTED CLASS BNODE AND BNODE FUNCTIONS DEFINITION ***/

/************************************************
 * BNODE
 * Container class element that contains info
 * on previous and next elements in a list
 ***********************************************/
template<class T>
class BST<T>::BNode {

   friend class BST<T>;

   public:
      //constructors
      BNode() : data{}, pParent{nullptr}, pLeft{nullptr},
                           pRight{nullptr} {isRed = true;}
      BNode(const T & element) : data{element}, pParent{nullptr},
                                 pLeft{nullptr}, pRight{nullptr}
                                 {isRed = true;}
      

      T data;           // container for the data
      BNode * pParent;  // pointer to the parent node
      BNode * pLeft;    // pointer to left node
      BNode * pRight;   // pointer to right node
      bool isRed;

   private:
      void verifyRB(int depth);
      void verifyBST();
      void balance();
};

/********************************************
 * FUNCTION:  BALANCE
 * RETURNS:   N/A
 * PARAMETER: N/A
 ********************************************/

template<class T>
void BST<T>::BNode::balance()
{
   //Case 1: No Parent
   if (this->pParent == nullptr)
   {
      this->isRed = false;
      return;
   }

   //Case 2: Parent is black
   if (this->pParent->isRed == false)
   {
      return;
   }

   //To simplify pointer interaction
   BNode * parent = this->pParent;
   BNode * grandParent = this->pParent->pParent;
      //We don't know if we parent is right or left child: caveat
   BNode * aunt = nullptr;
      //if Aunt exists and is left child
   if ( (grandParent->pLeft != nullptr) &&
        (grandParent->pLeft == parent) )
   {
      aunt = grandParent->pRight;
   }
      //if Aunt exists and is right child
   else if ( (grandParent->pRight != nullptr) &&
             (grandParent->pRight == parent) )
   {
      aunt = grandParent->pLeft;
   }

   //Establish sibling
   BNode * sibling = nullptr;
   //if Sibling exists and is left child
   if ( (parent->pLeft != nullptr) &&
        (parent->pLeft == this) )
   {
      sibling = parent->pRight;
   }
      //if Aunt exists and is right child
   else if ( (parent->pRight != nullptr) &&
             (parent->pRight == this) )
   {
      sibling = parent->pLeft;
   }

   //Case 3:
   if (  (this->pParent->isRed == true) && //Parent is red
         (grandParent->isRed == false) && //Grandpa is black
         ((aunt != nullptr) && (aunt->isRed == true)) ) // aunt is Red
   {
      //Change colors (Page 232)
      parent->isRed = false;
      grandParent->isRed = true;
      aunt->isRed = false;
      
      //Recursion in order to change the balancing to
      //   the grandparent in case that changing colors
      //   caused problems up the tree.
      grandParent->balance();
   }

   //if any of these doesn't exist for whatever reason, we're done
   if(!grandParent)
   {
      return;
   }

   //Case 4.a
   if ( 
        ( parent->isRed && (!grandParent->isRed) ) &&
        /*( (!sibling->isRed) && (!aunt->isRed) )    &&*/
        ( (parent->pLeft == this) && (grandParent->pLeft == parent) )
      )
   {
      //if grandparent has a parent, and it is the right child of said parent
      //   update the parent to point now to parent instead
      if( (grandParent->pParent != nullptr) &&
          (grandParent->pParent->pRight == grandParent) )
      {
         grandParent->pParent->pRight = parent;
      }
      //if grandparent has a parent, and it is the left child of said parent
      //   update the parent to point now to parent instead
      else if ( (grandParent->pParent != nullptr) &&
                (grandParent->pParent->pLeft == grandParent) )
      {
         grandParent->pParent->pLeft = parent;
      }

      //update pointers
      parent->pRight = grandParent;
      parent->pParent = grandParent->pParent;
      grandParent->pParent = parent;
      grandParent->pLeft = sibling;

      //only have to update this if there are siblings
      if (sibling)
      {
         sibling->pParent = grandParent;
      }

      //set the right colors
      grandParent->isRed = true;
      parent->isRed = false;

      
      
   }

   //Case 4.b
   if ( 
        ( parent->isRed && (!grandParent->isRed) ) &&
        /*( (!sibling->isRed) && (!aunt->isRed) )    &&*/
        ( (parent->pRight == this) && (grandParent->pRight == parent) )
      )
   {
      //if grandparent has a parent, and it is the right child of said parent
      //   update the parent to point now to parent instead
      if( (grandParent->pParent != nullptr) &&
          (grandParent->pParent->pRight == grandParent) )
      {
         grandParent->pParent->pRight = parent;
      }
      //if grandparent has a parent, and it is the left child of said parent
      //   update the parent to point now to parent instead
      else if ( (grandParent->pParent != nullptr) &&
                (grandParent->pParent->pLeft == grandParent) )
      {
         grandParent->pParent->pLeft = parent;
      }

      //update pointers
      parent->pLeft = grandParent;
      parent->pParent = grandParent->pParent;
      grandParent->pParent = parent;
      grandParent->pRight = sibling;

      //only have to update this if there are siblings
      if (sibling)
      {
         
         sibling->pParent = grandParent;
      }

      //update colors
      grandParent->isRed = true;
      parent->isRed = false;
   }

   //case 4.c
   if (
        ( parent->isRed && (!grandParent->isRed) ) &&
        ( (parent->pRight == this) && (grandParent->pLeft == parent) )
      )
   {
      //grandParent adopts this' right node as a left child
      grandParent->pLeft = this->pRight;
      //if this' right child exists, update it to point to it's
      //   new parent
      if(this->pRight)
      {
         this->pRight->pParent = grandParent;
      }

      //parent adopts this' left node as a right child
      parent->pRight = this->pLeft;
      //if this' right child exists, update it to point to it's
      //   new parent
      if(this->pLeft)
      {
         this->pLeft->pParent = parent;
      }

      //update the head link
      this->pParent = grandParent->pParent;

      //if grandparent has a parent, and it is the right child of said parent
      //   update the parent to point now to parent instead
      if ( (grandParent->pParent != nullptr) &&
                (grandParent->pParent->pRight == grandParent) )
      {
         grandParent->pParent->pRight = this;
      }
      //if grandparent has a parent, and it is the left child of said parent
      //   update the parent to point now to parent instead
      else if ( (grandParent->pParent != nullptr) &&
                (grandParent->pParent->pLeft == grandParent) )
      {
         grandParent->pParent->pLeft = this;
      }

      //parent is the new left child
      this->pLeft = parent;
      //update parent to point to its new parent
      parent->pParent = this;

      //grandParent is the new right child
      this->pRight = grandParent;
      //update grandParent to point to its new parent
      grandParent->pParent = this;

      //update colors
      grandParent->isRed = true;
      this->isRed = false;

   }

   //case 4.d
   if (
        ( parent->isRed && (!grandParent->isRed) ) &&
        ( (parent->pLeft == this) && (grandParent->pRight == parent) )
      )
   {
      //grandParent adopts this' right node as a left child
      grandParent->pRight = this->pLeft;
      //if this' right child exists, update it to point to it's
      //   new parent
      if(this->pLeft)
      {
         this->pLeft->pParent = grandParent;
      }

      //parent adopts this' left node as a right child
      parent->pLeft = this->pRight;
      //if this' right child exists, update it to point to it's
      //   new parent
      if(this->pRight)
      {
         this->pRight->pParent = parent;
      }

      //update the head link
      this->pParent = grandParent->pParent;

      //if grandparent has a parent, and it is the right child of said parent
      //   update the parent to point now to parent instead
      if ( (grandParent->pParent != nullptr) &&
           (grandParent->pParent->pRight == grandParent) )
      {
         grandParent->pParent->pRight = this;
      }
      //if grandparent has a parent, and it is the left child of said parent
      //   update the parent to point now to parent instead
      else if ( (grandParent->pParent != nullptr) &&
                (grandParent->pParent->pLeft == grandParent) )
      {
         grandParent->pParent->pLeft = this;
      }

      //parent is the new right child
      this->pRight = parent;
      //update parent to point to its new parent
      parent->pParent = this;

      //grandParent is the new left child
      this->pLeft = grandParent;
      //update grandParent to point to its new parent
      grandParent->pParent = this;

      //update colors
      grandParent->isRed = true;
      this->isRed = false;

   }
}
/**********NESTED CLASS ITERATOR(S) AND ITERATOR FUNCTIONS DEFINITION ***/

/************************************************
 * BST ITERATOR
 * Container class element that contains info
 * on previous and next elements in a list
 ***********************************************/
template <class T>
class BST<T> :: iterator
{
   friend class BST<T>;

   public:
      // constructors, destructors, and assignment operator
      iterator() : pNode(NULL) {}
      iterator(BNode * pNode) : pNode{pNode} {}
      iterator(const iterator & rhs) { *this = rhs; }

      iterator & operator = (const iterator & rhs)
      {
         this->pNode = rhs.pNode;
         return *this;
      }

      // equals, not equals operator
      bool operator != (const iterator & rhs) const { return rhs.pNode != this->pNode; }
      bool operator == (const iterator & rhs) const { return rhs.pNode == this->pNode; }

      // dereference operator
            T & operator * ()       { return pNode->data; }
      const T & operator * () const { return pNode->data; }

      // prefix increment a
      iterator & operator ++ ();
      // postfix increment
      iterator   operator ++ (int postfix);
      // prefix decrement
      iterator & operator -- ();
      // postfix decrement
      iterator   operator -- (int postfix);


   private:
      BNode * pNode;
};

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 *     advance by one. 
 * Author:      Br. Helfrich
 * Performance: O(log n) though O(1) in the common case
 *************************************************/
template <class T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
   // do nothing if we have nothing
   if (NULL == pNode)
      return *this;

   // if there is a right node, take it
   if (NULL != pNode->pRight)
   {
      // go right
      pNode = pNode->pRight;

      // jig left - there might be more left-most children
      while (pNode->pLeft)
         pNode = pNode->pLeft;
      return *this;
   }

   // there are no right children, the left are done
   //assert(NULL == pNode->pRight);
      BNode * pSave = pNode;
  
   
   // go up
   pNode = pNode->pParent;

   // if the parent is the NULL, we are done!
   if (NULL == pNode)
      return *this;

   // if we are the left-child, got to the parent.
   if (pSave == pNode->pLeft)
      return *this;

   // we are the Right-child, go up as long as we are the right child!
   while (NULL != pNode && pSave == pNode->pRight)
   {
      pSave = pNode;
      pNode = pNode->pParent;
   }

   return *this;
}

/**************************************************
 * BST ITERATOR :: INCREMENT POSTFIX
 *     advance by one. 
 * Author:      Br. Helfrich
 * Performance: O(log n) though O(1) in the common case
 *************************************************/
template <class T>
typename BST <T> :: iterator BST <T> :: iterator :: operator ++ (int Postfix)
{
   iterator PostfixReturn;
   PostfixReturn.pNode = this->pNode;

   // do nothing if we have nothing
   if (NULL == pNode)
      return *this;

   // if there is a right node, take it
   if (NULL != pNode->pRight)
   {
      // go right
      pNode = pNode->pRight;

      // jig left - there might be more left-most children
      while (pNode->pLeft)
         pNode = pNode->pLeft;
      return *this;
   }

   // there are no right children, the left are done
   //assert(NULL == pNode->pRight);
   BNode * pSave = pNode;
   
   // go up
   pNode = pNode->pParent;

   // if the parent is the NULL, we are done!
   if (NULL == pNode)
      return *this;

   // if we are the left-child, got to the parent.
   if (pSave == pNode->pLeft)
      return *this;

   // we are the Right-child, go up as long as we are the right child!
   while (NULL != pNode && pSave == pNode->pRight)
   {
      pSave = pNode;
      pNode = pNode->pParent;
   }

   return PostfixReturn;
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 *     advance by one. 
 * Author:      Br. Helfrich
 * Performance: O(log n) though O(1) in the common case
 *************************************************/
template <class T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
   // do nothing if we have nothing
   if (NULL == pNode)
      return *this;

   // if there is a left node, take it
   if (NULL != pNode->pLeft)
   {
      // go left
      pNode = pNode->pLeft;

      // jig right - there might be more right-most children
      while (pNode->pRight)
         pNode = pNode->pRight;
      return *this;
   }

   // there are no left children, the right are done
   //assert(NULL == pNode->pLeft);
   BNode * pSave = pNode;


   // go up
   pNode = pNode->pParent;

   // if the parent is the NULL, we are done!
   if (NULL == pNode)
      return *this;

   // if we are the right-child, got to the parent.
   if (pSave == pNode->pRight)
      return *this;

   // we are the left-child, go up as long as we are the left child!
   while (NULL != pNode && pSave == pNode->pLeft)
   {
      pSave = pNode;
      pNode = pNode->pParent;
   }

   return *this;
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 *     advance by one. 
 * Author:      Br. Helfrich
 * Performance: O(log n) though O(1) in the common case
 *************************************************/
template <class T>
typename BST <T> :: iterator BST <T> :: iterator :: operator -- (int postfix)
{
   iterator PostfixReturn;
   PostfixReturn.pNode = this->pNode;

   // do nothing if we have nothing
   if (NULL == pNode)
      return *this;

   // if there is a left node, take it
   if (NULL != pNode->pLeft)
   {
      // go left
      pNode = pNode->pLeft;

      // jig right - there might be more right-most children
      while (pNode->pRight)
         pNode = pNode->pRight;
      return *this;
   }

   // there are no left children, the right are done
   //assert(NULL == pNode->pLeft);
   BNode * pSave = pNode;

   // go up
   pNode = pNode->pParent;

   // if the parent is the NULL, we are done!
   if (NULL == pNode)
      return *this;

   // if we are the right-child, got to the parent.
   if (pSave == pNode->pRight)
      return *this;

   // we are the left-child, go up as long as we are the left child!
   while (NULL != pNode && pSave == pNode->pLeft)
   {
      pSave = pNode;
      pNode = pNode->pParent;
   }

   return PostfixReturn;
}

} // namespace custom

#endif // BST_H