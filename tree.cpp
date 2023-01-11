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
    void megaRoot(Node * nodeToAdd, int spot);
};

class Tree {       
    
public:
    Node * root;
    Tree(); // constructor
    Node * search(int valToFind, Node * node);
    bool insert(int valToAdd);
    void absorb(Node * newChild, Node * successor);
    void discard(Node * removeChild);
    bool Delete(int valToKill);
    void setRoot(Node * node);
    void print(Node * start);
    void reset(Node * start);
};

//Constructor
Node::Node (int val)
{   
    parent = NULL;
    value[0] = val;
    value[1] = -1;
    value[2] = -1;


    for(int i = 0; i < 4; i++)
        child[i] = NULL;
}

void Node::setParent(Node* Parent)
{
    parent = Parent; 
}

void Node::setChildren(Node * kid, int place)
{
    child[place] = kid;
}

void Node::setValues(int Value, int place)
{
    value[place] = Value;
}

int Node::getValues(int place)
{   
    return value[place];
}

Node * Node::getChild(int spot)
{
    return child[spot];
}


void Node::printChildren()
{
    for(int i = 0; i < 3; i++){
        if(child[i] != NULL)
            child[i] -> printValues();
        else
            cout<<"child "<<i<<" is NULL"<<endl;
        }
}

Node * Node::getParent()
{
    return parent;
}

void Node::printValues()
{   Node * node = this; 
    if(node != NULL)
        for(int i = 0; i < 3; i++)
            cout<<value[i]<<" ";
}

int Node::findSpot()
{
    if(this == parent -> getChild(0))
        return 0;
    if(this == parent -> getChild(1))
        return 1;
    else
        return 2;
}

void Node::megaRoot(Node * nodeToAdd, int spot)
{   // *this* corresponds to the parent node that is being turned into the megaroot 
    for(int i = 2; i >= spot; i--)
    {   
        this -> child[i + 1] = this -> child[i];
        this -> value[i + 1] = this -> value[i];
    }
    this -> child[spot] = nodeToAdd;
    this -> value[spot] = nodeToAdd -> getValues(0);   

}



//constructor for tree
Tree::Tree()
{
    root = NULL;
}

void Tree::setRoot(Node * node)
{
    root = node;
}

void Tree::print(Node * start)
{   
    if(start != NULL){
        if(start -> getChild(0) == NULL)
            cout<<start -> getValues(0)<<endl;
        /*else if(start -> getValues(0) == -1 )
            cout<<endl;*/
        else{
            start -> printValues();
            cout<<endl;
            for(int i = 0; i < 3; i++)
                print(start -> getChild(i)); 
            cout<<endl;
            }
    }
} 

Node * Tree::search(int val, Node * node)
{   //THink about where the node values are -1

    //node -> printValues();
    //cout<<endl;


    if(node -> getChild(0) != NULL){
        if(val <= node -> getValues(0) && node -> getValues(0) != -1)
            return search(val, node -> getChild(0));
        else if(val <= node -> getValues(1) && node -> getValues(1) != -1)
            return search(val, node -> getChild(1));
        else if (val <= node -> getValues(2) && node -> getValues(2) != -1)
            return search(val, node -> getChild(2));
        else{
            cout<<"Your value is greater than the largest in the tree. \nThe value returned is 2000"<<endl;
            return NULL;
        }
    }   
    else
       return node;
 
}

bool Tree::insert(int val)
{
    Node * Search = search(val, this -> root);
    if(Search == NULL)
        return false;
    else if(Search -> getValues(0) == val)
        return false; 
    else{ //The node of the successor is returned
        Node * newNode = new Node(val);
        cout<<"new node is "<<endl;
        newNode -> printValues();
        cout<<endl;
        this -> absorb(newNode,Search);
        return true; 
    }
}

void Tree::absorb(Node * newChild, Node * successor)
{   Node * parent = successor -> getParent();

    //Create a MEGAROOT
    if(this -> root == successor -> getParent()){
        int spot = successor -> findSpot(); //Need to know the location of the successor in the parent's child array 
        //Need to turn parent of successor into MEGAROOT
        parent -> megaRoot(newChild, spot);
        newChild -> setParent(parent);
        cout<<"root values ";
        parent -> printValues();
        cout<<parent -> getValues(3)<<endl;
        cout<<"children ";
        parent -> printChildren();
        parent -> getChild(3) -> printValues();
        cout<<endl;
        //MRR: mega root right
        //MML: mega root left
        Node * MRL = new Node(parent-> getValues(0));
        Node * MRR = new Node(parent-> getValues(2));
        MRL -> setValues(-1,1);
        MRL -> setValues(parent-> getValues(1),2);
        MRR -> setValues(-1,1);
        MRR -> setValues(parent-> getValues(3),2);
        MRL -> setChildren(parent -> getChild(0),0);
        MRL -> setChildren(parent -> getChild(1),2);
        MRR -> setChildren(parent -> getChild(2),0);
        MRR -> setChildren(parent -> getChild(3),2);

        Node * empty = new Node(-1);
        Node * newRoot = new Node(parent-> getValues(1));
        newRoot -> setValues(-1, 2);
        newRoot -> setValues(parent -> getValues(3), 2);
        newRoot -> setChildren(MRL, 0);
        newRoot -> setChildren(empty, 1);
        newRoot -> setChildren(MRR, 2);
        MRR -> setParent(newRoot);
        MRR -> setParent(newRoot);

        this -> root = newRoot;

        this -> print(newRoot);
        //Mega root needs to have empty nodes in the middle positions that aren't priting out
        
    }
    
}

//this delete function should be working correctly
bool Tree::Delete(int valToKill){
    Node * b = search(valToKill, this -> root);
    if(b->getValues(0) == valToKill){
        this -> discard(search(valToKill, this -> root));
        return true;
    }
    return false;
}
 
//working on discard function
void Tree::discard(Node * childToRemove){
    
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
    
    if(totchild >= 4){ //no discard needed
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
           /*if(start->getChild(0) != NULL){
               this->reset(start->getChild(0));
           }
           if(start->getChild(1) != NULL){
               this->reset(start->getChild(1));
           }
           if(start->getChild(2) != NULL){
               this->reset(start->getChild(2));
           }*/
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
    //Node** Nodeleaf = new Node*[21];
    vector<Node *> Nodeleaf (27);



    for(int i = 0; i < 27; i++)
    {
        //Creating the leafs where the value is in the first spot of the array      
        Node newNode1 = Node(leaf[i]);
        Nodeleaf[i] = &newNode1; 

        //Nodeleaf[i] -> printValues();
    
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
/*     for(int i = 0; i < 27; i++)
        cout<<leaf[i]<<" ";
    cout<<endl;
    for(int i = 0; i < 9; i++)
        parent1[i] -> printValues();
    cout<<endl; */



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

    /* for(int i = 0; i < 21; i++)
        Nodeleaf[i] -> getParent() -> printValues(); 

    for(int i = 0; i < 7; i++)
        parent1[i] -> printChildren();
 */
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

    /* for(int i = 0; i < 3; i++)
        parent2[i] -> printValues();
     */

    k = 0;
    for(int i = 0; i < 9; i++){
        parent1[i] -> setParent(parent2[k]);
        parent2[k] -> setChildren(parent1[i], 0);
        parent1[++i] -> setParent(parent2[k]);
        parent2[k] -> setChildren(parent1[i], 1);
        parent1[++i] -> setParent(parent2[k]);
        parent2[k++] -> setChildren(parent1[i], 2);
    }

/*  for(int i = 0; i < 9; i++)
        parent1[i] -> getParent() -> printValues(); 
    cout<<endl;
    cout<<endl;
    for(int i = 0; i < 3; i++)
        parent2[i] -> printChildren();  
 */
    
    Node * root = new Node(leaf[8]);
    root -> setValues(leaf[17], 1);
    root -> setValues(leaf[26], 2);

    for(int i = 0; i < 3; i++){
        parent2[i] -> setParent(root);
        root -> setChildren(parent2[i], i);
    }

   /*  for(int i = 0; i < 3; i++)
        parent2[i] -> getParent() -> printValues();   */

    Node * n1 = Nodeleaf[8];
    Node * n2 = Nodeleaf[17];
    Node * n3 = Nodeleaf[26];


    Node * root1 = new Node(leaf[8]);
    root1 -> setValues(leaf[17], 1);
    root1 -> setValues(leaf[26], 2);
    root1 -> setChildren(n1, 0);
    root1 -> setChildren(n2,1);
    root1 -> setChildren(n3,2);
    n1 -> setParent(root1);
    n2 -> setParent(root1);
    n3 -> setParent(root1);

   

    Tree tree = Tree();
    Tree tree1 = Tree();

    //tree.setRoot(root);
    tree1.setRoot(root1);

    /* n1 -> getParent() -> printValues();
    cout<<endl;
    n2 -> getParent() -> printValues();
    cout<<endl;
    n3 -> getParent() -> printValues(); */

    //tree.print(root);
    //tree1.print(root1);

    //tree.search(50, root1);

    int value = 30;

    if(tree1.insert(value) == true)
        cout<<"value inserted"<<endl;
    else
        cout<<"value exists or value is larger than largest element"<<endl;
    
    //tree1.insert(25);

    //tree1.print(root1);
    
    //tree1.Delete(30);
    tree1.print(root1);
    
    //delete and discard testing
    
    Tree tree2 = Tree();
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
    
    tree2.setRoot(root2);

    tree2.Delete(18);
    tree2.reset(tree2.root);
    
    tree2.Delete(16);
    tree2.reset(tree2.root);
    
    tree2.Delete(5);
    tree2.reset(tree2.root);
    
    tree2.Delete(2);
    tree2.reset(tree2.root);
    
    tree2.Delete(1);
    tree2.reset(tree2.root);
    //tree2.reset(c22);
    
    tree2.print(tree2.root);
    
    Tree tree3 = Tree();
    
    Node * root3 = new Node(5);
    Node * value1 = new Node(5);
    
    root3->setChildren(value1,0);
    
    value1->setParent(root3);
    
    tree3.setRoot(root3);
    
    //tree3.Delete(5);
    
    /*delete root2;
    delete root1;
    delete v1;
    delete v2;
    delete c1;
    delete c2;
    delete v4;
    delete v4;*/
    
    return 0;
}