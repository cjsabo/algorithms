#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>

using namespace std;

class node{
    
    public:
    node * parent;
    int value[6];
    node * child[6];
    
    node(int val){
        for(int i = 0; i < 6; i++){
            value[i] = 0;
            child[i] = nullptr;
        }
        
        value[0] = val;
    };
    //bool numChildren();
    //void absorb(node * newChild);
    //void discard(node * badChild);
    
};

class tree{
    
    public:
    node * root;
    void print(node * start);
    
    tree(){
        root = new node(0);
    };
    //node * search(int valToFind);
    //bool insert(int valToAdd);
    //bool del(int valToPunish);
    //void print();
};


int main(){ //main program 
    
    ifstream in;
    string file;
    int size;
    int temp;
    tree t;
    node * n;
    
    cout << "Type in the name of the file: " ;
    cin >> file;
    in.open(file);
    
    in >> size;

    for(int i = 0; i < size; i++){
        in >> temp;
        n = new node(temp);
        if(t.root->child[0] == nullptr){
            t.root->child[0] = n;
            t.root->value[0] = temp;
        }
    }
    
    delete n;
    return 0;   
}