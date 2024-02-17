#include<iostream>
#include<fstream>
#include<algorithm>
#include<cmath>
#define V 10
using namespace std;

class Planet{
public:
    string p_name;
    int p_number;
    int x,y,z;
    int weight,profit;
};

typedef struct {
    int planet1;
    int planet2;
    int distance;
}Edge;

class Tree {
   Edge edges[V-1];    //as a tree has vertex-1 edges
   int n;
   char map[19][16];
public:
    Tree() {
        n = 0;
    }

    void addEdge(Edge e){
        edges[n]=e;   //add edge e into the tree
        n++;
    }

    void printEdges(){    //print edge, distance and total distance
        int total_distance=0;

        for(int i=0; i<n; i++) {
            cout<<"Edge: "<<char(edges[i].planet1+'A')<<"--"<<char(edges[i].planet2+'A');
            cout<<" & Distance: "<<edges[i].distance<<endl;
            total_distance += edges[i].distance;
        }

        cout<<"\nTotal Distance: "<<total_distance<<endl;
    }

    void drawGraph(){
        inimap(map);
        planets(map);
        connection();
        display(map);
    }

    void inimap(char m[19][16]){
        for(int i=0; i<19; i++){
            for(int j=0; j<16; j++){
                m[i][j]=' ';
            }
        }
    }

    void planets(char m[19][16]){
        m[9][9]='A';
        m[6][12]='B';
        m[6][6]='C';
        m[15][9]='D';
        m[9][15]='E';
        m[0][12]='F';
        m[0][6]='G';
        m[9][3]='H';
        m[18][6]='I';
        m[18][12]='J';
    }

    void connection(){
        for(int i=0; i<n; i++){
            connect(map,edges[i].planet1+1,edges[i].planet2+1);
        }
    }

    void connect(char m[19][16],int a, int b){
        switch(a){
        case 1:
            if(b==2){
                m[8][10]='/';
                m[7][11]='/';
            }
            if(b==3){
                m[8][8]='\\';
                m[7][7]='\\';
            }
            if(b==4){
                m[10][9]='|';
                m[11][9]='|';
                m[12][9]='|';
                m[13][9]='|';
                m[14][9]='|';
            }
            break;
        case 2:
            if(b==5){
                m[7][13]='\\';
                m[8][14]='\\';
            }
            if(b==6){
                m[1][12]='|';
                m[2][12]='|';
                m[3][12]='|';
                m[4][12]='|';
                m[5][12]='|';
            }
            if(b==7){
                m[1][7]='\\';
                m[2][8]='\\';
                m[3][9]='\\';
                m[4][10]='\\';
                m[5][11]='\\';
            }
            if(b==10){
                m[7][12]='|';
                m[8][12]='|';
                m[9][12]='|';
                m[10][12]='|';
                m[11][12]='|';
                m[12][12]='|';
                m[13][12]='|';
                m[14][12]='|';
                m[15][12]='|';
                m[16][12]='|';
                m[17][12]='|';
            }
            break;
        case 3:
            if(b==8){
                m[7][5]='/';
                m[8][4]='/';
            }
            if(b==9){
                m[7][6]='|';
                m[8][6]='|';
                m[9][6]='|';
                m[10][6]='|';
                m[11][6]='|';
                m[12][6]='|';
                m[13][6]='|';
                m[14][6]='|';
                m[15][6]='|';
                m[16][6]='|';
                m[17][6]='|';
            }
            break;
        case 4:
            if(b==9){
                m[16][8]='/';
                m[17][7]='/';
            }
        }
    }

    void display(char m[19][16]){
        cout<<endl;
        for(int i=0; i<19; i++){
            cout<<"";
            for(int j=0; j<16; j++){
                cout<<m[i][j];
            }
            cout<<endl;
        }
    }
};

class CloudSet{
    int n;
    int set[V];    //a set can hold maximum V vertices
public:
    CloudSet(){
        n = -1;
    }

    void addCloud(int vert){
        set[++n] = vert;    //add planet to the set
    }

    int deleteCloud() {
        return set[n--];
    }

    friend int findCloud(CloudSet *cSetArr, int vert);
    friend void merge(CloudSet &set1, CloudSet &set2);
};

void merge(CloudSet &set1, CloudSet &set2){
    //merge two vertex sets together
    while(set2.n >= 0)
        set1.addCloud(set2.deleteCloud());
        //addToSet(vSet1, delFromSet(vSet2));
}

int findCloud(CloudSet *cSetArr, int vert){
    //find the planet in different planet sets
    for(int i = 0; i<V; i++){
        for(int j = 0; j<=cSetArr[i].n; j++){
            if(vert == cSetArr[i].set[j]){
                return i;   //node found in i-th planet set
            }
        }
   }
}

void readPlanets(string filePlanet,Planet p[]){
    ifstream MyReadFile;
    MyReadFile.open(filePlanet);

    for(int i=0; i<V; i++){
        MyReadFile>>p[i].p_name>>p[i].x>>p[i].y>>p[i].z>>p[i].weight>>p[i].profit;
    }
}

void readGraph(string fileEdge,Planet p[],Edge e[],int &edgecount){
    ifstream MyReadFile;
    MyReadFile.open(fileEdge);
    int i;

    for(i=0; i<15; i++){
        MyReadFile>>e[i].planet1>>e[i].planet2;

        int p1=e[i].planet1;
        int p2=e[i].planet2;

        e[i].distance=sqrt(pow(p[p2].x-p[p1].x,2)+pow(p[p2].y-p[p1].y,2)+pow(p[p2].z-p[p1].z,2));
    }

    edgecount=i;
}

void swapping(Edge &e1, Edge &e2){
    Edge temp;
    temp=e1;
    e1=e2;
    e2=temp;
}

void sortEdge(Edge *e, int edgecount){
    //sort the edges of graph in ascending order of cost
    int flag = 1, i, j;

    for(i = 0; i<(edgecount-1) && flag; i++){   //modified bubble sort is used
        flag = 0;
        for(j = 0; j<(edgecount-i-1); j++){
            if(e[j].distance > e[j+1].distance){
                swapping(e[j], e[j+1]);
                flag = 1;
            }
        }
   }
}

void kruskal(Edge e[],int edgecount,Tree &tr){
    Edge ed;
    int p1_loc, p2_loc;
    CloudSet cSetArray[V];

    for(int i = 0; i < V; i++){
        cSetArray[i].addCloud(i); //each set contains one element
    }

    sortEdge(e, edgecount);//ecount number of edges in the graph

    int count = 0;

    while(count <= V-1){
        ed = e[count];
        p1_loc = findCloud(cSetArray, ed.planet1);
        p2_loc = findCloud(cSetArray, ed.planet2);

        if(p1_loc != p2_loc){  //check whether planet1 and planet2 is in same set or not
            merge(cSetArray[p1_loc], cSetArray[p2_loc]);
            tr.addEdge(ed);
        }
        count++;
    }
}

int main() {
    Planet p[V];

    readPlanets("A2planets.txt",p); //read data sets

    const int maxedge=20;
    Edge e[maxedge];
    int edgecount;

    readGraph("edges.txt",p,e,edgecount); //read edges and calculate distance

    Tree tr;

    kruskal(e,edgecount-1,tr); //kruskal's algorithm

    cout<<"------------------------------"<<endl;
    cout<<"      Edge of the Tree"<<endl;
    cout<<"------------------------------\n"<<endl;
    tr.printEdges(); //display edges

    cout<<"\n------------------------------"<<endl;
    cout<<"           Graph of"<<endl;
    cout<<"     Minimum Spanning Tree"<<endl;
    cout<<"------------------------------\n"<<endl;
    tr.drawGraph(); //draw graph
}
