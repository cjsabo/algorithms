//Cobi Sabo
//Dynamic Programming Algorithm Program for 0-1 Knapsack Problem (Part 1)
//Algorithms, F2018, Gupta
//Sent runs to you in email with txt files.
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

int max(int x, int y){
    if(x > y)
        return x;
    else
        return y;
}

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

    //Item a[numitems];
    Item *a;
    a = new Item[numitems];
    Item *b;
    b = new Item[numitems];
    int P[numitems+1][capacity+1]; //this is indexed differently, starting at 0
    
    for(int i = 0; i < numitems; i++){
        in >> name;
        in >> profit;
        in >> weight;
        a[i].change(name, profit, weight, profit/weight);
        //a[i].getItem();
    }
    for(int j = 1; j < numitems+1; j++){
        for(int k = 0; k < capacity+1; k++){
            P[0][k] = 0;
            P[j][0] = 0;
            /*if(j == 0){
                if(k < a[j].getWeight()-1)
                    P[j][k] = 0;
                else
                    P[j][k] = a[j].getProfit();
                cout << P[j][k] << " " << endl;
            }*/
            //else{
            if(a[j-1].getWeight() <= k)
                P[j][k] = max(P[j-1][k], a[j-1].getProfit() + P[j-1][k-a[j-1].getWeight()]);
            else
                P[j][k] = P[j-1][k];
                //cout << P[j][k] << " ";
            //}
        }
    }
    
    int n = numitems;
    int c = capacity;
    while(n > 0){
        if(P[n][c] == P[n-1][c]){
            n--;
        }
        else{
            b[count] = a[n-1];
            c -= a[n-1].getWeight();
            totw += a[n-1].getWeight();
            n--;
            count++;
        }
    }
    cout << "The total number of items in the solution: " << count << endl;
    cout << "The total number of profit of items in the solution: " << P[numitems][capacity] << endl;
    cout << "The total weight of all the items in the solution: " << totw << endl;
    cout << "Items in the solution:" << endl;
    for(int x =0; x < count; x++)
        b[x].getItem();
            
    delete[] b;
    return 0;   
}