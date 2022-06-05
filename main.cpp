#include <iostream>
#include <map>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <regex>
#include <set>
using namespace std;

bool isUrl(std::string Temp)
{
    bool index = 0;
    cout << Temp << endl;

    if (std::regex_match (Temp, std::regex("https?:\\/\\/(www\\.)?[-a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=]*)")))
        index = 1;
    if (std::regex_match (Temp, std::regex("[-a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=]*)")))
        index = 1;

    return index;
}

void nuskaityti(ifstream &fd , map<string , size_t> &zodziu_sk , map<string , map<size_t , size_t>> &references,  map<string , size_t> &linkas) {
    string eilute, zod;
    unsigned int eilNr = 1;
    int t=0;
    while(!fd.eof()) {
        getline(fd, eilute);
        stringstream line(eilute);
        while(line >> zod) {
            if (regex_match(zod, std::regex(
                    "[(http(s)?):\\/\\/(www\\.)?a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=]*)"))) {
                linkas.insert(std::pair<std::string, int>(zod, eilNr));
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
        fr<<"'" << linka.first << "' "<<" eiluteje : "<< linka.second<<std::endl;
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
    map<std::string, size_t> zodziu_sk;
    std::map <std::string, size_t> linkas;
    map<std::string, std::map<size_t, size_t>> references;
    ifstream fd("test.txt");
    ofstream fr("rez.txt");
    nuskaityti(fd , zodziu_sk , references, linkas);
    isvesti(fr , zodziu_sk , references, linkas);
    return 0;
}