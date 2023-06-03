#include "lang_gpp_impl/analyzer.h"
#include "lang_gpp_impl/syntax.h"
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "incorrent format\n\nformat: ./execute.exe -file";
        return 1;
    }
    try {
        StateM programm1;
        programm1.StartDKA(argv[1]);
        PrintVec(vecToken);
        if (programm1.flagAnalyzer) {
            cout << "SET_CONST = ";
            print_set(set_const.begin(), set_const.end());
            cout << "SET_VARIABLE = ";
            print_set(set_var.begin(), set_var.end());
            std::cout << "SET_LABLE = {";
            for(auto & [const_lable, __ignore] : set_lable) {
                std::cout << const_lable << " ";
            }
            std::cout << "}" << std::endl;
            cout << "ANALYZER: accept" << endl;
            Syntax syntax2;   
            std::cout << "++++++\n";
            syntax2.StartSyntax(); 
            std::cout << "syntax: " << syntax2.flagSyntax << std::endl;
            std::ofstream buff("test.txt");
            auto li_atom = syntax2.get_atoms();
            li_atom.match_labels();
            buff << li_atom << std::endl;
            std::cout << "created file for stack language: test.txt" << std::endl;
        }
        else {
            PrintErrors();
            cout << "Analyzer: acception error" << endl;
            
            return -1;
        }
    } catch(std::bad_any_cast & e) {
        std::cerr << e.what() << std::endl;
    } catch(exception e) {
        std::cerr <<  e.what() << std::endl;
    } catch (const char* e) {
        std::cerr << e << std::endl;
    } catch(std::string & e) {
        std::cerr << e << std::endl;
    }



    
    return 0;
}