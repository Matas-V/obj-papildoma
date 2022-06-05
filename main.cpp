#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <regex>
using namespace std;

void nuskaityti(ifstream &fd , map<string , size_t> &zodziu_sk , map<string , map<size_t , size_t>> &references,  map<string , size_t> &linkas) {
    string eilute, zod;
    unsigned int eilNr = 1;
    int t=0;
    while(!fd.eof()) {
        getline(fd, eilute);
        stringstream line(eilute);
        while(line >> zod) {
            if (regex_match(zod, regex(
                    "[(http(s)?):\\/\\/(www\\.)?a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=]*)"))) {
                linkas.insert(pair<string, int>(zod, eilNr));
            } else {
                zod.erase(remove_if(zod.begin(), zod.end(), [](char s) {
                    return s == ',' || s == '-' || s == '.' || s == '!' || s == '?' || s == '(' || s == ')' || s == '„' || s == '“' || s == ':';
                }),
                zod.end());
            }
            zodziu_sk[zod]++;
            references[zod][eilNr]++;
        }
        eilNr++;
    }

    fd.close();
}

void isvesti(ofstream &fr , map<string , size_t> &zodziu_sk , const map<string , map<size_t , size_t>> &references, map<string , size_t> &linkas) {

    fr << "Url adresai:" <<endl;
    for (auto &linka : linkas) {
        fr<<"'" << linka.first << "' "<<" eiluteje : "<< linka.second<<endl;
    }
    fr << endl;

    fr << "Zodziai ir ju daznis:" <<endl;

    for (auto &i : zodziu_sk) {
        if (i.second > 1){
            fr << " '"<< i.first <<"' " << "is viso pasikartojo " << i.second << " kartus:" << endl;
            for (auto &j : references.at(i.first)){
                fr << j.first << " eiluteje(-ese) " << j.second << " karta(-us)" << endl;
            }
            fr << endl;
        }
    }
}

int main() {
    map<string, size_t> zodziu_sk;
    map <string, size_t> linkas;
    map<string, map<size_t, size_t>> references;
    ifstream fd("test.txt");
    ofstream fr("rez.txt");
    nuskaityti(fd , zodziu_sk , references, linkas);
    isvesti(fr , zodziu_sk , references, linkas);
    return 0;
}