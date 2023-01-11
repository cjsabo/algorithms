#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>

using namespace std;

class Item{  //Item class
    public:
    
    int name;
    int weight;
    int profit;
    double ratio;
    
    Item(){
        name = 0;
        weight = 0;
        profit = 0;
        ratio = 0;
    }
    Item(int n, int w, int p, int r){
        name = n;
        weight = w;
        profit = p;
        ratio = r;
    }
    
    int getName() {return name;}
    int getWeight() {return weight;}
    int getProfit() {return profit;}
    double getRatio() {return ratio;}
    void change(int n1, int p1, int w1, double r1){name = n1, profit = p1, weight = w1, ratio = (double)p1/(double)w1;}
    void getItem() {cout << "name: " << name << " profit: " << profit << " weight: " << weight << " ratio: " << setprecision(3) << ratio << endl;}
};

struct node{ //node structure
    node * left;
    Item x;
    node * right;
};

struct node1{
    node1 * right;
    int level;
    int prof;
    int weight;
    int * include;
    float bound;
    node1 * left;
};

float bound(node1 x,Item * i, int cap,int n){ //this should be correct
    int j;
    int k;
    int totweight;
    float result;
    if(x.weight >= cap)
        return 0;
    else{
        result = x.prof;
        j = x.level + 1;
        totweight = x.weight;
        while(j <= n && (totweight + i[j].getWeight() <= cap)){
            totweight += i[j].getWeight();
            result += i[j].getProfit();
            j++;
        }
        k = j;
        if(k<=n)
            result += (cap - totweight)*(i[k].getProfit()/i[k].getWeight());
        return result;
    }
};

class tree{ //Tree class that is implemented as a Priority queue
    private:
    Item save;
    
    public:
    node * root;
    int count;
    
    tree(){
        root = nullptr;
        count = 0;
    }
    
    void enqueue(Item z){  //Enqueue function
        root = ehelp(z,root);
    }
    
    node *ehelp(Item q, node *r){  //Enqueue helper function 
        if(r == nullptr){
            r = new node;
            r->x = q;
            r->right = nullptr;
            r->left = nullptr;
            count++;
            return r;
        }
        else{
            if(q.getRatio() >= r->x.getRatio()){
                r->left = ehelp(q,r->left);
                return r;
            }
            if(q.getRatio() < r->x.getRatio()){
                r->right = ehelp(q,r->right);
                return r;
            }   
        }
    }
    
    Item dequeue(){ //Dequeue function
        if(isEmpty()){
            cout << "The queue is empty." << endl;
            return save;
        }
        root = dhelp(root);
        count--;
        return save;
    }
    
    node * dhelp(node *r){ //Dequeue helper function 
        if(r->left == nullptr && r->right == nullptr){
            save = r->x;
            r = nullptr;
            return r;
        }
        if(r->left == nullptr && r->right != nullptr){
            save = r->x;
            r = r->right;
            return r;
        }
        else{
            r->left = dhelp(r->left);
            return r;
        }
    }
    
    int getCount() {return count;}
    bool isEmpty() {return count == 0;}
};

class nodeq{
    public:
    node1 * root;
    int count;
    node1 save;
    
    nodeq(){
        root = nullptr;
        count = 0;
    }
    
    void enqueue(node1 z, int n){  //Enqueue function
        root = ehelp(z,root,n);
    }
    
    node1 *ehelp(node1 x, node1 *r,int n){  //Enqueue helper function 
        if(r == nullptr){
            r = new node1;
            r->weight = x.weight;
            r->prof = x.prof;
            r->level = x.level;
            r->bound = x.bound;
            r->include = new int[n];
            for(int h = 1; h <= n; h++){
                r->include[h] = x.include[h];
            }
            r->left = nullptr;
            r->right = nullptr;
            count++;
            return r;
        }
        else{
            if(x.bound >= r->bound){
                r->left = ehelp(x,r->left,n);
                return r;
            }
            if(x.bound < r->bound){
                r->right = ehelp(x,r->right,n);
                return r;
            }
        }
    }
    
    node1 dequeue(){ //Dequeue function
        if(isEmpty()){
            cout << "The queue is empty." << endl;
            return save;
        }
        root = dhelp(root);
        count--;
        return save;
    }
    
    node1 * dhelp(node1 *r){ //Dequeue helper function 
        if(r->left == nullptr && r->right == nullptr){
            save = *r;
            r = nullptr;
            return r;
        }
        if(r->left == nullptr && r->right != nullptr){
            save = *r;
            r = r->right;
            return r;
        }
        else{
            r->left = dhelp(r->left);
            return r;
        }
    }
    
    int getCount() {return count;}
    bool isEmpty() {return count == 0;}
};

void ks3(int numi, Item * a, int cap, int * maxpro,int * bs){
    nodeq nq;
    node1 v;
    node1 u;
    v.level = 0;
    v.prof = 0;
    v.weight = 0;
    *maxpro = 0;
    v.include = new int[numi];
    for(int b = 1; b <= numi; b++){
        v.include[b] = 0;
    }
    v.left = nullptr;
    v.right = nullptr;
    v.bound = bound(v,a,cap,numi);
    nq.enqueue(v,numi);
    while(!nq.isEmpty()){
        v = nq.dequeue();
        if(v.bound > *maxpro){
            u.level = v.level + 1;
            u.weight = v.weight + a[u.level].getWeight();
            u.prof = v.prof + a[u.level].getProfit();
            u.include = new int[numi];
            u.left = nullptr;
            u.right = nullptr;
            for(int c = 1; c <= numi; c++){
                u.include[c] = v.include[c];
            }
            u.include[u.level] = 1;
            if(u.weight <= cap && u.prof > *maxpro){
                *maxpro = u.prof;
                u.include[u.level] = 1;
                for(int d = 1; d <= numi; d++){
                    bs[d] = u.include[d];
                }
            }
            
            u.bound = bound(u,a,cap,numi);
            if(u.bound > *maxpro)
                nq.enqueue(u,numi);
            u.weight = v.weight;
            u.prof = v.prof;
            u.bound = bound(u,a,cap,numi);
            if(u.bound > *maxpro){
                u.include[u.level] = 0;
                nq.enqueue(u,numi);
            }
        }
    }
};

int main(){ //main program 
    
    int name;
    int profit;
    int weight;
    int numitems;
    int capacity;
    int count = 0;
    int totw = 0;
    string file;
    ifstream in;
    
    cout << "Please type the name of the file you would like to read: ";
    cin >> file;
    cout << "Thank you!" << endl;
    in.open(file);
    
    in >> numitems;
    in >> capacity;
    
    tree pq;

    Item *a;
    a = new Item[numitems+1];
    
    int * maxprof;
    maxprof = new int;
    *maxprof = 0;
    
    int * bestset;
    bestset = new int[numitems+1];
    
    for(int i = 0; i <= numitems; i++){
        if(i >= 1){
            in >> name;
            in >> profit;
            in >> weight;
            a[i].change(name, profit, weight, profit/weight);
            pq.enqueue(a[i]);
        }//a[i].getItem();
    }
    
    for(int i = 1; i <= numitems; i++){
        a[i] = pq.dequeue();
        //a[i].getItem();
    }
    
    ks3(numitems,a,capacity,maxprof,bestset);
    for(int i = 1; i <= numitems; i++){
        if(bestset[i] == 1){
            count++;
            totw += a[i].getWeight();
        }
    }
    cout << "The number of items in the solution: " << count << endl;
    cout << "The maximum profit is: " << *maxprof << endl;
    cout << "The total weight of all the items in solution: " << totw << endl;
    cout << "Items in the solution: " << endl;
    for(int q = 1; q <= numitems; q++){
        if(bestset[q] == 1)
            a[q].getItem();
    }
    
    delete a;
    delete maxprof;
    delete bestset;
   
    return 0;   
}