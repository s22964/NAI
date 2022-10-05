#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <cmath>
using mojafunkcja_t = std::function<double(double,double)>;
void wynik(std::vector<double> arguments,mojafunkcja_t funkcja){
    using namespace std;


    cout<<funkcja(arguments[0],arguments[1])<<endl;

}

int main(int argc, char **argv) {
    using namespace std;
    vector<double> arguments;

    for (int i=2; i<argc; i++){
        arguments.push_back(stod(argv[i]));
    }


    map<string, mojafunkcja_t> formatery;

    formatery["sin"] = [](double x,double y) { return sin(x); };
    formatery["add"] = [](double x,double y) { return x+y ; };
    formatery["mod"] = [](double x,double y) { return fmod(x,y) ; };

    try{
        vector<string> argumenty(argv,argv+argc);
        auto selected_f=argumenty.at(1);
        wynik(arguments,formatery.at(selected_f));

    }catch (std::out_of_range aor) {
        cout << "Podano nieprawidlowe argumenty. Dostepne argumenty to: ";
        for (auto [k, v] : formatery) cout << " " << k;
        cout << endl;
    }



    return 0;
}