//Cobi Sabo
//01 Knapsack Backtracking Algorithm
//Fall 2018, Gupta
//Email you code and runs
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

bool promising(int index,int prof, int wei, int * maxp, Item * x,int n, int cap){
    int j,k;
    int totweight;
    float bound;
    
    if(wei >= cap)
        return false;
    else{
        j = index + 1;
        bound = prof;
        totweight = wei;
        while(j<=n && totweight+x[j].getWeight() <= cap){
            totweight += x[j].getWeight();
            bound += x[j].getProfit();
            j++;
        }
        
        k = j;
        if(k <= n)
            bound += (cap - totweight)*(x[k].getProfit()/x[k].getWeight());
        return bound > *maxp;
    }
};

void knapsack(int ind, int prof, int weight, int * maxpro, Item * y, char * best, char * inc, int * nb, int n, int w){
    if(weight <= w && prof > *maxpro){
        *maxpro = prof;
        *nb = ind;
        for(int i = 1; i <= n; i++)
            best[i] = inc[i];
    }
    
    if(promising(ind, prof, weight, maxpro, y,n,w)){
        inc[ind+1] = 'y';
        knapsack(ind+1, prof+y[ind+1].getProfit(), weight+y[ind+1].getWeight(), maxpro,y,best,inc,nb,n,w); 
        inc[ind+1] = 'n';
        knapsack(ind+1, prof, weight, maxpro,y,best,inc,nb,n,w);
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
    
    char * bestset;
    bestset = new char[numitems+1];
    
    char * include;
    include = new char[numitems+1];
    
    int * numbest;
    numbest = new int;
    *numbest = 0;
    
    for(int i = 0; i <= numitems; i++){
        if(i >= 1){
            in >> name;
            in >> profit;
            in >> weight;
            a[i].change(name, profit, weight, profit/weight);
            pq.enqueue(a[i]);
        }
        bestset[i] = 'n';
        include[i] = 'n';
        //a[i].getItem();
    }
    
    for(int i = 1; i <= numitems; i++){
        a[i] = pq.dequeue();
    }
    
    knapsack(0,0,0,maxprof,a,bestset,include,numbest,numitems,capacity);
    //cout << *numbest << endl;
    
    for(int i = 1; i<= numitems; i++){
        if(bestset[i] == 'y'){
            totw += a[i].getWeight();
            count++;
        }
    }
    cout << "The number of items in the solution: " << count << endl;
        //cout << bestset[i] << endl;
    cout << "The maximum profit is: " << *maxprof << endl;
    cout << "The total weight of items in the solution: " << totw << endl;
    cout << "Items in the solution: " << endl;
    for(int i = 1; i<= numitems; i++){
        if(bestset[i] == 'y'){
            a[i].getItem();
        }
    }
    
    delete a;
    delete maxprof;
    delete bestset;
    delete include;
    delete numbest;
   
    return 0;   
}