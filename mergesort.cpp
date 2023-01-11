#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
#include <cstdlib>

using namespace std;

void merge(int * a, int first, int last, int middle){
    int b[last-first+1];
    int x = first;
    int y = 0;
    int z = middle+1;
    
    while(x <= middle && z <= last){
        if(a[x] < a[z]){
            b[y] = a[x];
            y++;
            x++;
        }
        else{
            b[y] = a[z];
            y++;
            z++;
        }
    }
    
    while(x <= middle){
        b[y] = a[x];
        y++;
        x++;
    }
    while(z <= last){
        b[y] = a[z];
        y++;
        z++;
    }
    
    for(int s = first; s <= last; s++)
        a[s] = b[s-first];
}

void mergesort(int * a, int first, int last){
    if(first < last){
        int middle = (first+last)/2;
        mergesort(a,first,middle);
        mergesort(a,middle+1,last);
        merge(a,first,last,middle);
    }
}

int main(){
    
    //ask the user for a number
    int n;
    cout << "Welcome to MergeSort! Please put in the size of your array: ";
    cin >> n;
    cout << "Thank you for your input!" << endl;
    //cout << n << endl;
    
    //allocate the array
    int * a = new int[n];
    for(int i = 0; i < n; i++){
        a[i] = (rand()%1000001);
        //cout << a[i] << endl;
    }
    
    //sort the array
    mergesort(a,0,n-1);
    
    //print the array
    for(int i = 0; i < n; i++)
        cout << a[i] << endl;
    
    
    delete [] a;
    return 0;
}
