#include <iostream>
#include <vector>
#include <random>
#include <functional>
#include <map>
#include <iomanip>
#include <bits/stdc++.h>
#include <chrono>


using domain_t = std::vector<double>;
using fun = std::function<double(double, double)>;

FILE *fp = NULL;
FILE *gnupipe = NULL;
std::random_device rd;
std::mt19937 mt_generator(rd()); //generator liczb losowych

domain_t hill_climbing(std::function<double(double, double)> &f, const domain_t minimal_d, const domain_t maximal_d,
                       const int max_iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    double time_taken;
    double r;

    std::ios_base::sync_with_stdio(false);
    fp = fopen("plot.dat", "r");

    domain_t current_p(minimal_d.size());
    domain_t new_p(minimal_d.size());
    for (int i = 0; i < minimal_d.size(); i++) { //losowanie punktu początkowego
        std::uniform_real_distribution<double> dist(minimal_d[i], maximal_d[i]);
        current_p[i] = dist(mt_generator);

    }

    for (int it = 0; it < max_iterations; it++) {

        for (int i = 0; i < minimal_d.size(); i++) { //losowanie nowego punktu
            std::uniform_real_distribution<double> dist(minimal_d[i], maximal_d[i]);
            new_p[i] = dist(mt_generator);

        }
        std::cout << std::endl << "wynik: " << f(new_p[0], new_p[1]) << std::endl;
        if (minimal_d.size() == 2) {
            if (f(current_p[0], current_p[1]) > f(new_p[0], new_p[1])) { //porównywanie punktów

                current_p[0] = new_p[0];
                current_p[1] = new_p[1];
            }
        } else if (minimal_d.size() == 1) {
            if (f(current_p[0], 0) > f(new_p[0], 0)) { //porównywanie punktów

                current_p[0] = new_p[0];


            }
        } else {
            throw std::out_of_range("");
        }
        auto end = std::chrono::high_resolution_clock::now();
        time_taken=std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        time_taken *= 1e-9;
        r=f(current_p[0], 0);
        fprintf(fp,"%f %f\n",time_taken,r);
    }
    if (minimal_d.size() == 1) {
        if (f(current_p[0], 0) == 1e+10) {
            throw std::runtime_error("");
        }
    } else if (minimal_d.size() == 2) {
        if (f(current_p[0], current_p[1]) == 1e+10) {
            throw std::runtime_error("");
        }
    }
    domain_t result = {current_p[0], current_p[1]};//zwrócenie wartości
    return result;
}

int main(int argc, char **argv) {
    using namespace std;


    char *GnuCommands[] = {"set title \"Demo\"","set xlabel \"Czas\"","set ylabel \"Dokladnosc\"", "plot 'plot.dat' with points"};

    gnupipe = _popen("C:/gnuplot/bin/gnuplot.exe -persist", "w");



    map<std::string, fun> Wf;
    Wf["McCormick"] = [&](double x, double y) {

        if ((x >= -1.5) && (x <= 4) && (y >= -3) && (y <= 4)) {
            return (sin((x + y)) + pow((x - y), 2.0) - 1.5 * x + 2.5 * y + 1);
        }
        return pow(10, 10);
    };
    Wf["Sphere"] = [&](double x, double y) {


        return x*x;
    };
    Wf["Booth"] = [&](double x, double y) {
        if (x <= 10 && x >= -10 && y <= 10 && y >= -10) {
            return (pow(x + 2 * y - 7, 2) + pow(2 * x + y - 5, 2));
        }
        return pow(10, 10);


    };


    try {
        vector<string> funkcja(argv, argv + argc);  //wybieranie argumentów do funkcji
        vector<double> arguments;
        for (int i = 2; i < argc; i++) {
            arguments.push_back(stod(argv[i]));
        }
        domain_t selected_min, selected_max;

        if (arguments.size() == 3) {        //przypisanie dziedziny tylko dla x
            selected_min = {arguments[0]};
            selected_max = {arguments[1]};

        } else if (arguments.size() == 5) {     //przypisanie dziedziny dla x i y
            selected_min = {arguments[0], arguments[1]};
            selected_max = {arguments[2], arguments[3]};
            if ((selected_min[1] > selected_max[1])) {
                throw std::range_error("");
            }
        } else {
            throw std::out_of_range("");        //bledna ilosc argumentow
        }
        if (selected_min[0] > selected_max[0]) {
            throw std::range_error("");
        }


        auto selected_f = funkcja.at(1);
        auto selected_it = arguments[arguments.size() - 1];

        //auto best = hill_climbing(Wf.at(selected_f), selected_min, selected_max, selected_it);
        for (int i=0; i<4;i++){
            fprintf(gnupipe,"%s\n",GnuCommands[i]);

        }
        //std::cout << "best x = " << best[0] << std::endl;
//        if (arguments.size() == 5) {
//
//            std::cout << "best y = " << best[1] << std::endl;
//        }


    } catch (out_of_range aor) {
        cout << "Podano nieprawidlowe argumenty. Dostepne funkcje to: ";
        for (auto[k, v]: Wf) cout << " " << k;
        cout << endl;
        cout << "Szablon argumentow: [funkcja] [przedzial minimalny](max 2) [przedzial maksymalny](max 2) [iteracje]"
             << endl;
    } catch (range_error r) {
        cout << "Bledna dziedzina";
    } catch (runtime_error er) {
        std::cout << "Nie wylosowano punktu spelniajacego wymagania funkcji.";
    }
    return 0;
}
