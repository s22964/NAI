//
// Created by Ezr0 on 2023-01-24.
//

#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <sstream>
#include <fstream>
#include <chrono>
std::chrono::time_point <std::chrono::system_clock, std::chrono::duration<double>> czas_start;
struct node
{
    int x;
    int y;
    node* parent;
    int cost=std::numeric_limits<int>::max();; ///< cost
    std::vector<node*> neighbours;
    bool visited=false;
    bool obstacle=false;
};
node *nodeStart = nullptr;
node *nodeEnd = nullptr;
std::pair<std::vector<int>, std::vector<std::vector<int>>> readFile(std::string Filename) {
    using namespace std;
    ifstream source; ///< odczytywany plik
    source.open(Filename);
    vector<std::string> lines; ///< linie w odczytywanym pliku
    string line;
    while (std::getline(source, line)) {
        lines.push_back(line);
    }
    if (lines.size() < 1) {
        cout << "zla ilosc zmiennych "<<lines.size();
        exit(3);
    }
    vector<int> matrix;

    int x,y;
    vector<int> c;
    std::vector<vector<int>> myObstacles;
    for(auto l:lines){
        c.clear();
        stringstream iss(l);
        iss>>x>>y;
        if(l==lines[0]){
            matrix.push_back(x);
            matrix.push_back(y);
            continue;
        }
        c.push_back(x);
        c.push_back(y);
        myObstacles.push_back(c);
    }
    return make_pair(matrix, myObstacles);
}




int main(int argc, char **argv) {
    using namespace std;
    cout<<"1.Filename 2.Start-x 3.Start-y 4.End-x 5.End-y"<<endl;
    node* nodes= nullptr;
    int max_x,max_y,sX,sY,eX,eY;
    string Filename(argv[1]);
    readFile(Filename);
    max_x=readFile(Filename).first[0];
    max_y=readFile(Filename).first[1];
    sX=stoi(argv[2]);
    sY=stoi(argv[3]);
    eX=stoi(argv[4]);
    eY=stoi(argv[5]);
    vector<vector<int>> obstacles=readFile(Filename).second;

    for(auto o:obstacles){
        if(((o[0]==sX&&o[1]==sY)||(o[0]==eX&&o[1]==eY))||(o[0]>=max_x||o[1]>=max_y)){
            cout<<"bad obstacle";
            exit(3);
        }
    }
    if(sX>=max_x||sY>=max_y||eX>=max_x||eY>=max_y){
        cout<<"bad points";
        exit(3);
    }
    auto czas_start = std::chrono::system_clock::now();
    bool t=false;
    nodes=new node[max_x*max_y]; //zajecie miejsca dla nodow
    for (int b=0;b<max_x;b++){ //tworzenie nodow
        for(int a=0;a<max_y;a++){
            if(a==sX&&b==max_y-1-sY){
                cout<<"[S]";
            }else if(a==eX&&b==max_y-1-eY){
                cout<<"[E]";
            }else{
                for(auto o:obstacles){
                    if(a==o[0]&&b==max_y-1-o[1]){
                        cout<<"[=]";
                        t=true;
                        break;
                    }
                }
                if(!t) {
                    cout << "[X]";
                }
                t=false;

            }
            nodes[b*max_x+a].x=a;
            nodes[b*max_x+a].y=b;
            nodes[b*max_x+a].parent= nullptr;
            nodes[b*max_x+a].visited=false;
            for(auto o:obstacles){
                if(a==o[0]&&b==o[1]) {
                    nodes[b * max_x + a].obstacle = true;
                    break;
                }
            }
        }
        cout<<endl;
    }


    for (int y = 0; y < max_x; y++) { //tworzenie sasiadow
        for (int x = 0; x < max_y; x++) {
            if (y > 0)
                nodes[y * max_x + x].neighbours.push_back(&nodes[(y - 1) * max_x + (x + 0)]);
            if (y < max_y - 1)
                nodes[y * max_x + x].neighbours.push_back(&nodes[(y + 1) * max_x + (x + 0)]);
            if (x > 0)
                nodes[y * max_x + x].neighbours.push_back(&nodes[(y + 0) * max_x + (x - 1)]);
            if (x < max_x - 1)
                nodes[y * max_x + x].neighbours.push_back(&nodes[(y + 0) * max_x + (x + 1)]);
        }
    }



    nodeStart=&nodes[sY*max_x+sX];
    nodeEnd=&nodes[eY*max_x+eX];
    list<node*> toVisit; //lista bo mozna latwiej modifikowac przod
    nodeStart->cost=0;
    toVisit.push_back(nodeStart);
    node* current=nodeStart;
    while(!toVisit.empty()){
        toVisit.sort([](node *a, node *b){ return a->cost < b->cost; });//sortowanie listy od najmniejszego kosztu

        current=toVisit.front();
        for(node* neighbour:current->neighbours){
            if(!neighbour->visited&&neighbour->obstacle==0) { //jeśli był, albo jest przeszkodą to skipujemy
                toVisit.push_back(neighbour);
            }
            int actualCost=current->cost+1;
            if(actualCost<neighbour->cost){
                neighbour->cost=actualCost;
                neighbour->parent=current;
            }
        }
        current->visited= true;
        while(!toVisit.empty()&&toVisit.front()->visited){ //jeśli był sprawdzany to wypada
            toVisit.pop_front();
        }
    }

    vector<node*> path;
    current=nodeEnd;
    while(current->parent!= nullptr){
        path.push_back(current->parent);
        current=current->parent;
    }
    if(current!=nodeStart){
        cout<<"path not found!"<<endl;
    }else{
        cout<<"path found!"<<endl;
    }
    auto czas_stop = std::chrono::system_clock::now();

    for (int b=0;b<max_x;b++) { //tworzenie nodow
        for (int a = 0; a < max_y; a++) {
            if (a ==sX && b == max_y-1-sY) {
                cout << "[S]";
            } else if (a == eX && b ==max_y-1- eY) {
                cout << "[E]";
            }else {
                for(auto o:obstacles){
                    if(a==o[0]&&b==max_y-1-o[1]){
                        cout<<"[=]";
                        t=true;
                        break;
                    }
                }
                if(t){
                    t=false;
                    continue;
                }
                for(auto n:path){
                    if(a ==n->x&&b ==max_y-1- n->y){
                        cout<<"[*]";
                        t=true;
                        break;
                    }
                }
                if(t){
                    t=false;
                    continue;
                }else{
                    cout << "[X]";}
            }
        }
        cout << endl;
    }
    std::chrono::duration<double> czas_obliczen = czas_stop-czas_start;
    std::cout << "Czas Obliczen: " << czas_obliczen.count() << "s\n";



    return 0;
}
