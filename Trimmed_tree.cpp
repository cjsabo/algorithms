//CS452 Parallel algoriths
//Final Project: 2-3 Tree
//31 April 2019
//Cobi Sabo & Tim Winter

#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;


class Node {
    Node * parent;
    int value[4];
    Node * child[4];
    int numberChildren;


public:
    Node(int val); // constructor
    bool numChildren(); 
    /* void absorb(Node * newChild);
    void discard(Node * removeChild); */
    Node * getChild(int spot);
    void setParent(Node* Parent);
    void setChildren(Node * kid, int place);
    void setValues(int Value, int place);
    int getValues(int place);
    void printChildren();
    Node * getParent();
    void printValues();
    int findSpot();
    int grandFindSpot(int val); 
    Node * megaRoot(Node * nodeToAdd, int spot);
    Node * megaRoot2(Node * nodeToAdd, int spot);
    void nodeSetNumberChildren(int num);
    int getNumberChildren();
};

class Tree {       
    Node * root;
    

public:
    Tree(); // constructor
    bool hasRoot();
    Node * search(int valToFind, Node * node);
    bool insert(int valToAdd);
    Node * absorb(Node * newChild, Node * successor, bool recurse);
    void discard(Node * removeChild);
    bool Delete(int valToKill);
    void setRoot(Node * node);
    Node * getRoot();
    void print(Node * start);
    void treeSetNumChildren(Node * start);
    void reset(Node * start);
    
};

//Constructor
Node::Node (int val)
{   
    parent = NULL;
    value[0] = val;
    value[1] = -1;
    value[2] = -1;
    numberChildren = 0; 

    //initialize children as NULL
    for(int i = 0; i < 4; i++)
        child[i] = NULL;
}

//sets parent of node 
void Node::setParent(Node* Parent)
{
    parent = Parent; 
}
//sets children of node
void Node::setChildren(Node * kid, int place)
{
    child[place] = kid;
}
//sets values of node
void Node::setValues(int Value, int place)
{
    value[place] = Value;
}

//returns values of node based on place 
int Node::getValues(int place)
{   
    return value[place];
}

//Returns child of node based on spot
Node * Node::getChild(int spot)
{
    return child[spot];
}

//Prints children of node 
void Node::printChildren()
{   int sum = 0;
    for(int i = 0; i < 3; i++){
        if(child[i] != NULL){
            child[i] -> printValues();
            cout<<" ";
            sum++;
        }
        else
            cout<<"child "<<i<<" is NULL"<<endl;
        }
}

//returns parent of node 
Node * Node::getParent()
{
    return parent;
}

//Prints values of node 
void Node::printValues()
{   Node * node = this; 
    if(node != NULL)
        for(int i = 0; i < 3; i++)
            cout<<value[i]<<" ";
}

//finds loacation of successor in parent array 
int Node::findSpot()
{
    if(this == parent -> getChild(0))
        return 0;
    if(this == parent -> getChild(1))
        return 1;
    else
        return 2;
}

//function finds spot of node in grandparent
int Node::grandFindSpot(int val)
{
    if(val == parent -> getValues(0))
        return 0;
    if(val == parent -> getValues(1))
        return 1;
    else
        return 2;
}

//function sets the number of children 
void Node::nodeSetNumberChildren(int num)
{
    numberChildren = num;
}

//This megaroot funtion is used for creation of new root when insertion is directly related to root 
Node * Node::megaRoot(Node * nodeToAdd, int spot)
{   // *this* corresponds to the parent node that is being turned into the megaroot
    Node * parent = this; 
    Node * newParent;
    //Shift values over 
    for(int i = 2; i >= spot; i--)
    {   
        this -> child[i + 1] = this -> child[i];
        this -> value[i + 1] = this -> value[i];
    }
    
    if(nodeToAdd ->  getValues(2) == -1) //The node to add is a leaf
    {
        this -> value[spot] = nodeToAdd -> getValues(0);
        this -> child[spot] = nodeToAdd;
    } 
    
    else
    {   //the node to add is not a leaf 
        this -> value[spot] = nodeToAdd -> getChild(spot) -> getValues(0); 
        this -> child[spot] = nodeToAdd -> getChild(0);
    }
    

    //Create megaroot left/right (MRL, MRR)
    //split 4 values/children among MRL 
    Node * empty1 = new Node(-1);
    Node * empty2 = new Node(-1);
    Node * empty3 = new Node(-1);
    Node * MRL = new Node(value[0]);
    Node * MRR = new Node(value[2]);
    MRL -> setValues(value[1],2);
    MRR -> setValues(value[3],2);
    MRL -> setChildren(child[0],0);
    MRL -> setChildren(empty1, 1);
    MRL -> setChildren(child[1],2);
    
    MRR -> setChildren(parent -> getChild(2),0);
    MRR -> setChildren(empty2, 1);
    MRR -> setChildren(parent -> getChild(3),2);

    child[0] -> setParent(MRL);
    child[1] -> setParent(MRL);
    child[2] -> setParent(MRR);
    child[3] -> setParent(MRR);

    //Create new root value that has MRR and MRL as children 
    Node * newRoot = new Node(parent-> getValues(1));
    newRoot -> setValues(parent -> getValues(3), 2);
    newRoot -> setChildren(MRL, 0);
    newRoot -> setChildren(empty3, 1);
    newRoot -> setChildren(MRR, 2);
    MRR -> setParent(newRoot);
    MRL -> setParent(newRoot);

    MRR -> nodeSetNumberChildren(2);
    MRL -> nodeSetNumberChildren(2);
    newRoot -> nodeSetNumberChildren(2);

    return newRoot; 

}
//Megaroot2 is used for absorb of recursion to preven the modification of the parent node 
Node * Node::megaRoot2(Node * nodeToAdd, int spot)
{   // *this* corresponds to the parent node that is being turned into the megaroot
    Node * parent = this; 
    Node * newParent;
    int tempValue[4];
    Node * tempChild[4];

    //copy values and children into temp arrays 
    for(int i = 0; i < 4; i++)
    {
        tempValue[i] = value[i];
        tempChild[i] = child[i];
    }

    
    //shift over the children of the nodes in 
    for(int i = 2; i >= spot; i--)
    {   
        tempChild[i + 1] = tempChild[i];
        tempValue[i + 1] = tempValue[i];
    }
    
    if(nodeToAdd ->  getValues(2) == -1) //The node to add is a leaf
    {
        tempValue[spot] = nodeToAdd -> getValues(0);
        tempChild[spot] = nodeToAdd;
    } 
    
    else //Need to look look at specific location for value 
    {   
        tempValue[spot] = nodeToAdd -> getChild(spot) -> getValues(0); 
        tempChild[spot] = nodeToAdd -> getChild(0);
        
        
    }
    
    

    //Create megaroot left/right (MRL, MRR)
    //split 4 values/children among MRL 
    Node * empty1 = new Node(-1);
    Node * empty2 = new Node(-1);
    Node * empty3 = new Node(-1);
    Node * MRL = new Node(tempValue[0]);
    Node * MRR = new Node(tempValue[2]);
    MRL -> setValues(tempValue[1],2);
    MRR -> setValues(tempValue[3],2);
    MRL -> setChildren(tempChild[0],0);
    MRL -> setChildren(empty1, 1);
    MRL -> setChildren(tempChild[1],2);
    
    MRR -> setChildren(tempChild[2],0);
    MRR -> setChildren(empty2, 1);
    MRR -> setChildren(tempChild[3],2);

    tempChild[0] -> setParent(MRL);
    tempChild[1] -> setParent(MRL);
    tempChild[2] -> setParent(MRR);
    tempChild[3] -> setParent(MRR);

    //Create new root value that has MRR and MRL as children 
    Node * newRoot = new Node(tempValue[1]);
    newRoot -> setValues(tempValue[3], 2);
    newRoot -> setChildren(MRL, 0);
    newRoot -> setChildren(empty3, 1);
    newRoot -> setChildren(MRR, 2);
    MRR -> setParent(newRoot);
    MRL -> setParent(newRoot);

    MRR -> nodeSetNumberChildren(2);
    MRL -> nodeSetNumberChildren(2);
    newRoot -> nodeSetNumberChildren(2);

    return newRoot; 

}


//function to return the number of children in the tree
int Node::getNumberChildren()
{
    return numberChildren;
}


//constructor for tree
Tree::Tree()
{
    root = NULL;
}

//function that sets the root of the tree
void Tree::setRoot(Node * node)
{
    root = node;
}

//function that returns the root of the tree
Node * Tree::getRoot()
{
    return root;
}

//function to print tree
void Tree::print(Node * start)
{   
    if(start != NULL){
        if(start -> getChild(0) == NULL && start -> getValues(0) != -1){
            cout<<start -> getValues(0)<<endl;
        }
        else if(start -> getValues(0) == -1 )
            cout<<"empty"<<endl;
        else{
            start -> printValues();
            cout<<endl;
            for(int i = 0; i < 3; i++)
                print(start -> getChild(i)); 
            cout<<endl;
            }
    }
} 

//function to search for value in tree
Node * Tree::search(int val, Node * node)
{  
    if(node -> getChild(0) != NULL){
        if(val <= node -> getValues(0) && node -> getValues(0) != -1)
            return search(val, node -> getChild(0)); //traverse down left of node
        else if(val <= node -> getValues(1) && node -> getValues(1) != -1)
            return search(val, node -> getChild(1)); //traverse down middle of node
        else if (val <= node -> getValues(2) && node -> getValues(2) != -1)
            return search(val, node -> getChild(2)); //traverse down right of node 
        else{
            cout<<"Your value is greater than the largest in the tree. \nThe value returned is 2000"<<endl;
            return NULL;
        }
    }   
    else
       return node; //return the node that has been searched for 
 
}

bool Tree::insert(int val)
{
    Tree * tree = this;
    Node * newNode = new Node(val);
    
    if(root == NULL) //If the tree is empty: create the new root 
        {   cout<<"tree is empty"<<endl;
            root = newNode; 
            Node * newNode2 = new Node(val);
            Node * empty = new Node(-1);
            Node * empty1 = new Node(-1);
            root -> setValues(val, 0);
            root -> setChildren(newNode2, 0);
            root -> setChildren(empty, 1);
            root -> setChildren(empty1, 2);
            root -> nodeSetNumberChildren(1);
            newNode2 -> setParent(root);
            empty -> setParent(root);
            empty1 -> setParent(root);

        }
    else if(root -> getNumberChildren() < 2 && root -> getChild(0) -> getValues(2) == -1) 
    {   //leaves are connected to the root  and the root only has one child 
        if(root -> getValues(0) == val) //value is aready there 
            return false;
        else
        {  
            int place = root -> getNumberChildren();
            if(place == 1) //new node value is greater than the existing value 
            { 
                if(val > root -> getValues(0)){
                    root -> setValues(val, 2);
                    root -> setChildren(newNode, 2);
                    root -> nodeSetNumberChildren(2);
                    newNode -> setParent(root);
                }
                else{ //new node value is less than the existing value 
                    root -> setValues(root -> getValues(0), 2);
                    root -> setValues(val, 0);
                    Node * temp = root -> getChild(0);
                    root -> setChildren(temp, 2);
                    root -> setChildren(newNode, 0);
                    root -> nodeSetNumberChildren(2);
                    newNode -> setParent(root);
                    temp -> setParent(root);
                }
                
            }
        }
    }
    else{   //Structure of root/tree has lready been set, we need to absorb the new node
        Node * newRoot;
        Node * successor = search(val, tree -> root); //find succesor
        Node * successorParent = successor -> getParent(); //Find parent of successor 
        if(successor == NULL || successor -> getValues(0) == val) //Value is already in tree
            return false;
        else 
        { 
            cout<<"new node is ";
            newNode -> printValues(); cout<<endl;
            newRoot = this -> absorb(newNode, successor, false); //absorb the new node 
            //set the new root of the tree 
            tree -> setRoot(newRoot);
            
        }
    }

    tree -> treeSetNumChildren(root);
    return true; 
}

Node * Tree::absorb(Node * newChild, Node * successor, bool recurse)
{   
    int spot = successor -> findSpot();
    Node * parent = successor -> getParent();
    Node * grandParent = parent -> getParent();



    if(parent -> getNumberChildren() < 3) //simple insertion case 
    {
        Node * parent = successor -> getParent();
        if(spot == 0){//shift values to right to insert in first child spot 
            parent -> setChildren(parent -> getChild(0), 1);
            parent -> setValues(parent -> getChild(0) -> getValues(0), 1);
            parent -> setChildren(newChild, 0);
            parent -> setValues(newChild -> getValues(0), 0);
            newChild -> setParent(parent);
            parent -> nodeSetNumberChildren(3);
        }
        if(spot == 2){ //shift values to the left if trying to insert into right child
            parent -> setChildren(newChild, 1);
            parent -> setValues(newChild -> getValues(0), 1);
            newChild -> setParent(parent);
            parent -> nodeSetNumberChildren(3);
        }

        return this -> root;
    }

    else if(parent -> getNumberChildren() == 3 && parent -> getValues(1) !=-1)
    {
        //Need to create a megaroot and insert into middle node of the parent
        
        Node * grandParent = parent -> getParent();
        //grandparent is NULL

        //First time to create a megaroot
        if(parent == root)
        {   
            parent = parent -> megaRoot(newChild, spot);
            this -> setRoot(parent);
            return this -> root;

        }

        else if (grandParent -> getValues(1) == -1)
        {   //simple insertion into grandparent 
            
            Node * left;
            Node * right;
            Node * temp;
            Node * childLeft; 
            Node * childRight; 
            int value = parent -> getValues(2);
            int grandSpot = parent -> grandFindSpot(value);
            temp = parent -> megaRoot2(newChild, spot);

            if(recurse == true)
            {   //we need to look further down the tree if we are bubbling up through recursion 
                childLeft = temp -> getChild(0) -> getChild(0);
                childRight = temp -> getChild(2) -> getChild(2);
            } 
            else
            {   //if we are close to root, the children are right next to temp 
                childLeft = temp -> getChild(0);
                childRight = temp -> getChild(2);
            }

            if(grandSpot == 0 ) //break apart to left
            {   
                left = childLeft;
                right = childRight; 
                grandParent -> setChildren(left, 0);
                grandParent -> setChildren(right, 1);
                grandParent -> setValues(left -> getValues(2), 0);
                grandParent -> setValues(right -> getValues(2), 1);
                left -> setParent(grandParent);
                right -> setParent(grandParent);
                grandParent -> nodeSetNumberChildren(3);

            }
            else //spot==2 and breka off to right on grandparent 
            {
                left = childLeft;
                right = childRight;
                grandParent -> setChildren(left, 1);
                grandParent -> setChildren(right, 2);
                grandParent -> setValues(left -> getValues(2), 1);
                grandParent -> setValues(right -> getValues(2), 2);
                left -> setParent(grandParent);
                right -> setParent(grandParent);
                grandParent -> nodeSetNumberChildren(3);
            }
            
        }
           
        else if(grandParent -> getNumberChildren() == 3) //the grandparent of the successor is is full
        {  
            int value = parent -> getValues(2);
            int grandSpot = parent -> grandFindSpot(value); //Parent's spot in grandparent
           
            if(grandParent -> getParent() == NULL) //We have reached the top of the tree
            {   Node * temp = parent -> megaRoot(newChild, spot);
                Node * newRoot = new Node(-1);
                Node * empty = new Node(-1);
            
                if(grandSpot == 0)
                {   
                    newRoot -> setChildren(temp, 0);
                    newRoot -> setChildren(root, 2);

                    if(root -> getChild(1) -> getValues(2)== -1) //leaf insertion 
                        root -> setValues(root -> getChild(1) -> getValues(0), 0);
                    else //node with children insertion 
                        root -> setValues(root -> getChild(1) -> getValues(2), 0);   

                    //shift the children in root
                    root -> setChildren(root -> getChild(1), 0);
                    root -> setValues(-1,1);
                    root -> setChildren(empty, 1);
                    root -> setParent(newRoot);
                    temp -> setParent(newRoot);
                    //create a new root that takes values from old root and temp as the left/right children 
                    newRoot -> setValues(temp -> getValues(2), 0);
                    newRoot -> setValues(root -> getValues(2), 2);
                    this -> setRoot(newRoot);
                }
                else if(grandSpot == 1) //Give one child to right, give one to left and make new root
                { 
                    Node * newLeft = new Node(-1); Node * newRight = new Node(-1); 

                    
                    newLeft -> setChildren(temp -> getChild(0), 2);
                    newLeft -> setChildren(grandParent -> getChild(0), 0);
                    newRight -> setChildren(temp -> getChild(2), 0);
                    newRight -> setChildren(grandParent -> getChild(2), 2);



                    //splitting up the children/values because we are trying to insert into the middle of grandparent 
                    if(newLeft -> getChild(1) -> getValues(2)== -1) //simple leaf insertion 
                    {
                        newLeft -> setValues(newLeft -> getChild(0) -> getValues(0),0);
                        newLeft -> setValues(newLeft -> getChild(2) -> getValues(0),2);
                        newRight -> setValues(newRight -> getChild(0) -> getValues(0),0);
                        newRight -> setValues(newRight -> getChild(2) -> getValues(0),2);
                    }
                    else //inserting node with children 
                    {
                        newLeft -> setValues(newLeft -> getChild(0) -> getValues(2),0);
                        newLeft -> setValues(newLeft -> getChild(2) -> getValues(2),2);
                        newRight -> setValues(newRight -> getChild(0) -> getValues(2),0);
                        newRight -> setValues(newRight -> getChild(2) -> getValues(2),2);
                    }


                    newRoot -> setChildren(newLeft, 0);
                    newRoot -> setChildren(newRight, 2);
                    newLeft -> setParent(newRoot);
                    newRight -> setParent(newRoot);
                    newRoot -> setValues(newLeft -> getValues(2), 0);
                    newRoot -> setValues(newRight -> getValues(2), 2);
                    this -> setRoot(newRoot);
                }

                else//Grandspot is 2
                {   //Same scenario as grandspot == 1, just on other side 
                    newRoot -> setChildren(temp, 2);
                    newRoot -> setChildren(root, 0);

                    if(root -> getChild(1) -> getValues(2) == -1)
                        root -> setValues(root -> getChild(1) -> getValues(0), 2);
                    else
                        root -> setValues(root -> getChild(1) -> getValues(2), 2);

                    root -> setChildren(root -> getChild(1), 2);
                    root -> setChildren(empty, 1);
                    root -> setValues(-1,1);   
                    root -> setParent(newRoot);
                    temp -> setParent(newRoot);
                    newRoot -> setValues(temp -> getValues(2), 2);
                    newRoot -> setValues(root -> getValues(2), 0);
                    this -> setRoot(newRoot);
                } 
            }
            else
            {   //None of the base cases above have been hit, we must recurse 
                
                //split up node that needs to be absorbed using megaroot
                Node * temp = parent -> megaRoot2(newChild, spot); //megaroot 2 doesnt alter the parent node
                int value = parent -> getValues(2);
                int grandSpot = parent -> grandFindSpot(value); //Parent's spot in grandparent
                Node* newSuccessor = grandParent -> getChild(grandSpot);
                grandParent -> setChildren(temp, grandSpot);
                temp -> setParent(grandParent);


                this -> absorb(temp, parent, true);
            } 
            
        }
           
    }
    
    return this -> root;
}

//This is a helper function to set number of children
//I forgot to do it when I set up the tree and am too lazy to do it manually
void Tree::treeSetNumChildren(Node * start)
{
    int num = 0;
    if(start == NULL)
        return;
    else
    {
        for(int i = 0; i < 3; i ++)
            if(start -> getValues(i) != -1)
                num++;
        start -> nodeSetNumberChildren(num);

        for(int i = 0; i < 3; i++)
            this -> treeSetNumChildren(start -> getChild(i));
        
    }
}

//Delete Function
bool Tree::Delete(int valToKill){
    Node * b = search(valToKill, this -> root);
    if(b->getValues(0) == valToKill){
        this -> discard(search(valToKill, this -> root));
        return true;
    }
    return false;
}
 
//Discard function
void Tree::discard(Node * childToRemove){
    //NOTE: Skewing everything left for simplicity
    Node * p = childToRemove->getParent(); //get the parent
    
    for(int i = 0; i < 3; i++){
        if(p->getChild(i) == childToRemove){
            p->setChildren(NULL, i);
            p->setValues(-1,i);
        }
    }
    
    //check if p still has two children, this works
    int count = 0; 
    for(int i = 0; i < 3; i++)
        if(p->getChild(i) != NULL)
            count++;
    
    if(count == 0){
        p->setValues(-1,0);
        return;
    }
    
    if(count == 2){
        if(p->getChild(0) == NULL){
            p->setValues(p->getValues(1),0);
            p->setValues(-1,1);
            p->setChildren(p->getChild(1), 0);
            p->setChildren(NULL, 1);
        }
        else if(p->getChild(2) == NULL){
            p->setValues(p->getValues(1),2);
            p->setValues(-1,1);
            p->setChildren(p->getChild(1), 2);
            p->setChildren(NULL, 1);
        }
        return;
    }
    //if we have reached here then p has one child
    //if p has one child, need to get number of children from siblings of p
    if(p->getParent() == NULL){
        //1 child AKA p is the root
        if(p->getChild(0) == NULL){
            Node * r = p->getChild(2);
            r->setParent(NULL);
            this->setRoot(r);
            p->setChildren(NULL,2);
            //p->getChild(2)->printChildren();
            return;
        }
        else{
            Node * r = p->getChild(0);
            r->setParent(NULL);
            this->setRoot(r);
            p->setChildren(NULL,0);
            return;
        }
    }
    
    int totchild = 0;
    Node * gp = p->getParent();
    Node * neph[7]; //will hold pointers for all "nieces and nephews"
    for(int i = 0; i < 7; i++)
        neph[i] = NULL;
    for(int i = 0; i < 3; i++){
        if(gp->getChild(i) == NULL)
            continue;
        else{
            Node * sibling = gp->getChild(i);
            for(int j = 0; j < 3; j++){
                if(sibling->getChild(j) != NULL){
                    neph[totchild] = sibling->getChild(j);
                    totchild++;
                }
            }
        }
    }
    
    //cases where no discard is needed
    if(totchild >= 4){ 
        if(totchild == 4){
            //left child
            Node * r = gp->getChild(0);
            r->setChildren(neph[0], 0);
            r->setChildren(NULL, 1);
            r->setValues(-1,1);
            r->setChildren(neph[1], 2);
            
            //right child
            Node * s = gp->getChild(2);
            s->setChildren(neph[2], 0);
            s->setChildren(NULL, 1);
            s->setValues(-1,1);
            s->setChildren(neph[3], 2);
            return;
        }
        else if(totchild == 5){
            //left child
            Node * r = gp->getChild(0);
            r->setChildren(neph[0], 0);
            r->setChildren(neph[1], 1);
            r->setChildren(neph[2], 2);
            
            //right child
            Node * s = gp->getChild(2);
            s->setChildren(neph[3], 0);
            s->setChildren(NULL, 1);
            s->setChildren(neph[4], 2);
            
            //get rid of middle child
            gp->setChildren(NULL, 1);
            return;
        }
        else if(totchild == 6){
            //left child
            Node * r = gp->getChild(0);
            r->setChildren(neph[0], 0);
            r->setChildren(neph[1], 1);
            r->setChildren(neph[2], 2);
            
            //right child
            Node * s = gp->getChild(2);
            s->setChildren(neph[3], 0);
            s->setChildren(neph[4], 1);
            s->setChildren(neph[5], 2);
            
            //still get rid of middle child in case of 6 get rid of middle child
            gp->setChildren(NULL, 1);
            return;
        }
        else{
            //left child
            Node * r = gp->getChild(0);
            r->setChildren(neph[0], 0);
            r->setChildren(neph[1], 1);
            r->setChildren(neph[2], 2);
            
            //now we keep the middle child and give it two children
            Node * s = gp->getChild(1);
            s->setChildren(neph[3], 0);
            s->setChildren(NULL, 1);
            s->setChildren(neph[4], 2);
            
            //right child
            Node * t = gp->getChild(2);
            t->setChildren(neph[5], 0);
            t->setChildren(NULL, 1);
            t->setChildren(neph[6], 2);
            return;
        }
    }
    
    //total number of children is either 1 or 3. This means p now only has the one child
    //3 children
    if(gp->getChild(0) == p){
        gp->getChild(2)->setChildren(neph[0], 0);
        gp->getChild(2)->setChildren(neph[1], 1);
        gp->getChild(2)->setChildren(neph[2], 2);
        
        p->setChildren(NULL,0);
        p->setChildren(NULL,1);
        p->setChildren(NULL,2);
    }
    else{
        gp->getChild(0)->setChildren(neph[0], 0);
        gp->getChild(0)->setChildren(neph[1], 1);
        gp->getChild(0)->setChildren(neph[2], 2);
        
        p->setChildren(NULL,0);
        p->setChildren(NULL,1);
        p->setChildren(NULL,2);
    }
    //if we get here, have to call discard on a level above to get correct tree
    discard(p);
}


//this is now working
void Tree::reset(Node * start)
{
   if(start != NULL)
   {
       if(start->getChild(0)->getChild(0) == NULL){
           for(int i = 0; i < 3; i++){
               if(start->getChild(i) != NULL)
                start->setValues(start->getChild(i)->getValues(0),i);
           }
           return;
       }
       else{
           for(int i = 0; i < 3; i++){
               if(start->getChild(i) != NULL){
                    this->reset(start->getChild(i));
                }
           }
           for(int i = 0; i < 3; i++){
               if(start->getChild(i) != NULL){
                    start->setValues(start->getChild(i)->getValues(2), i);
                }
           }
       }
   }
    return;
}
 

int main()
{
    int leaf[27] = {1,-1,3,5,-1,8,11,14,17,19,-1,21,-1,-1,-1,24,-1,28,31,-1,33,-1,-1,-1,36,-1,39};
    vector<Node *> Nodeleaf (27);



    for(int i = 0; i < 27; i++)
    {
        //Creating the leafs where the value is in the first spot of the array      
        Node newNode1 = Node(leaf[i]);
        Nodeleaf[i] = &newNode1; 
    }

        Nodeleaf[0] = new Node(leaf[0]);
        Nodeleaf[1] = new Node(leaf[1]);
        Nodeleaf[2] = new Node(leaf[2]);
        Nodeleaf[3] = new Node(leaf[3]);
        Nodeleaf[4] = new Node(leaf[4]);
        Nodeleaf[5] = new Node(leaf[5]);
        Nodeleaf[6] = new Node(leaf[6]);
        Nodeleaf[7] = new Node(leaf[7]);
        Nodeleaf[8] = new Node(leaf[8]);
        Nodeleaf[9] = new Node(leaf[9]);
        Nodeleaf[10] = new Node(leaf[10]);
        Nodeleaf[11] = new Node(leaf[11]);
        Nodeleaf[12] = new Node(leaf[12]);
        Nodeleaf[13] = new Node(leaf[13]);
        Nodeleaf[14] = new Node(leaf[14]);
        Nodeleaf[15] = new Node(leaf[15]);
        Nodeleaf[16] = new Node(leaf[16]);
        Nodeleaf[17] = new Node(leaf[17]);
        Nodeleaf[18] = new Node(leaf[18]);
        Nodeleaf[19] = new Node(leaf[19]);
        Nodeleaf[20] = new Node(leaf[20]);
        Nodeleaf[21] = new Node(leaf[21]);
        Nodeleaf[22] = new Node(leaf[22]);
        Nodeleaf[23] = new Node(leaf[23]);
        Nodeleaf[24] = new Node(leaf[24]);
        Nodeleaf[25] = new Node(leaf[25]);
        Nodeleaf[26] = new Node(leaf[26]);




    vector<Node*> parent1(9);
    parent1[0] = new Node(leaf[0]);
    parent1[1] = new Node(leaf[3]);
    parent1[2] = new Node(leaf[6]);
    parent1[3] = new Node(leaf[9]);
    parent1[4] = new Node(leaf[12]);
    parent1[5] = new Node(leaf[15]);
    parent1[6] = new Node(leaf[18]);
    parent1[7] = new Node(leaf[21]);
    parent1[8] = new Node(leaf[24]);

    int k=1;
    for(int i = 0; i < 9; i++){
        parent1[i]->setValues(leaf[k++], 1);
        parent1[i]->setValues(leaf[k++], 2);
        k++; 
    }


    //Set parents of leaves 
    k = 0; 
    for(int i = 0; i < 27; i++){
        Nodeleaf[i] -> setParent(parent1[k]);
        parent1[k] -> setChildren(Nodeleaf[i], 0);
        Nodeleaf[++i] -> setParent(parent1[k]);
        parent1[k] -> setChildren(Nodeleaf[i], 1);
        Nodeleaf[++i] -> setParent(parent1[k]);
        parent1[k] -> setChildren(Nodeleaf[i], 2);
        k++;
    }//end for


    vector<Node*> parent2(3);
    parent2[0] = new Node(leaf[2]);
    parent2[1] = new Node(leaf[11]);
    parent2[2] = new Node(leaf[20]);

    parent2[0]->setValues(leaf[5], 1);
    parent2[0]->setValues(leaf[8], 2);
    parent2[1]->setValues(-1, 1);
    parent2[1]->setValues(leaf[17], 2);
    parent2[2]->setValues(-1, 1);
    parent2[2]->setValues(leaf[26], 2);



    k = 0;
    for(int i = 0; i < 9; i++){
        parent1[i] -> setParent(parent2[k]);
        parent2[k] -> setChildren(parent1[i], 0);
        parent1[++i] -> setParent(parent2[k]);
        parent2[k] -> setChildren(parent1[i], 1);
        parent1[++i] -> setParent(parent2[k]);
        parent2[k++] -> setChildren(parent1[i], 2);
    }


    Node * root = new Node(leaf[8]);
    root -> setValues(leaf[17], 1);
    root -> setValues(leaf[26], 2);

    for(int i = 0; i < 3; i++){
        parent2[i] -> setParent(root);
        root -> setChildren(parent2[i], i);
    }

    Tree tree = Tree();
    

    tree.setRoot(root);

    tree.treeSetNumChildren(root);

     int value = 20;
    

   Tree tree2 = Tree();
    tree2.insert(30);
    tree2.insert(40);
    tree2.insert(36);

    
    tree2.insert(35);




    tree2.insert(31); 
    tree2.insert(32);
    tree2.insert(37);
    tree2.insert(38);
    tree2.insert(34);
    tree2.insert(33);
    tree2.insert(39);
    tree2.insert(35);
    tree2.insert(15);

    tree2.insert(20);
    cout<<"\nbegin print after insert: "<<endl;
    tree2.print(tree2.getRoot());  
    cout<<"done with insert\n"<<endl;

    Tree tree3 = Tree();
    //internal nodes
    Node * root2 = new Node(5);
    Node * c1 = new Node(2);
    Node * c2 = new Node(12);
    Node * c11 = new Node(1);
    Node * c12 = new Node(3);
    Node * c21 = new Node(8);
    Node * c22 = new Node(14);
    
    //leaves
    Node * v1 = new Node(1);
    Node * v2 = new Node(2);
    Node * v3 = new Node(3);
    Node * v4 = new Node(5);
    Node * v5 = new Node(8);
    Node * v6 = new Node(12);
    Node * v7 = new Node(14);
    Node * v8 = new Node(18);
    Node * v9 = new Node(16);
    
    root2->setValues(18,2);
    root2->setChildren(c1,0);
    root2->setChildren(c2,2);
    root2->setParent(NULL);
    
    c1->setValues(5,2);
    c1->setChildren(c11,0);
    c1->setChildren(c12,2);
    c1->setParent(root2);
    
    c2->setValues(18,2);
    c2->setChildren(c21,0);
    c2->setChildren(c22,2);
    c2->setParent(root2);
    
    c11->setValues(2,2);
    c11->setChildren(v1,0);
    c11->setChildren(v2,2);
    c11->setParent(c1);
    v1->setParent(c11);
    v2->setParent(c11);
    
    c12->setValues(5,2);
    c12->setChildren(v3,0);
    c12->setChildren(v4,2);
    c12->setParent(c1);
    v3->setParent(c12);
    v4->setParent(c12);
    
    c21->setValues(12,2);
    c21->setChildren(v5,0);
    c21->setChildren(v6,2);
    c21->setParent(c2);
    v5->setParent(c21);
    v6->setParent(c21);
    
    c22->setValues(16,1);
    c22->setValues(18,2);
    c22->setChildren(v7,0);
    c22->setChildren(v9,1);
    c22->setChildren(v8,2);
    c22->setParent(c2);
    v7->setParent(c22);
    v8->setParent(c22);
    v9->setParent(c22);
    
    tree3.setRoot(root2);

    cout<<"printing before deleting some stuff"<<endl;
    tree3.print(tree3.getRoot());




    tree3.Delete(18);
    tree3.reset(tree3.getRoot());
    
    tree3.Delete(16);
    tree3.reset(tree3.getRoot());
    
    tree3.Delete(5);
    tree3.reset(tree3.getRoot());
    
    tree3.Delete(2);
    tree3.reset(tree3.getRoot());
    
    tree3.Delete(1);
    tree3.reset(tree3.getRoot());
    //tree3.reset(c22);
    cout<<"\nprinting after deleting some stuff"<<endl;
    tree3.print(tree3.getRoot());
    
    Tree tree4 = Tree();
    
    Node * root4 = new Node(5);
    Node * value1 = new Node(5);
    
    root4->setChildren(value1,0);
    
    value1->setParent(root4);
    
    tree3.setRoot(root4);
    
    tree2.Delete(20);
    tree2.Delete(15);
    tree2.Delete(38);
    tree2.Delete(34);
    
    tree2.print(tree2.getRoot());
    
    return 0;
}


/* sample run 

tree is empty
new node is 36 -1 -1 
new node is 35 -1 -1 
new node is 31 -1 -1 
new node is 32 -1 -1 
new node is 37 -1 -1 
new node is 38 -1 -1 
new node is 34 -1 -1 
new node is 33 -1 -1 
new node is 39 -1 -1 
new node is 15 -1 -1 
new node is 20 -1 -1 

begin print after insert: 
31 35 40 
20 -1 31 
15 -1 20 
15
empty
20

empty
30 -1 31 
30
empty
31


34 -1 35 
34
empty
35

37 -1 40 
36 -1 37 
36
empty
37

empty
38 39 40 
38
39
40



done with insert

printing before deleting some stuff
5 -1 18 
2 -1 5 
1 -1 2 
1
2

3 -1 5 
3
5


12 -1 18 
8 -1 12 
8
12

14 16 18 
14
16
18




printing after deleting some stuff
8 5 14 
3 -1 8 
3
8

12 -1 14 
12
14


Timothys-MacBook-Pro-3:project4 Tim$

*/
