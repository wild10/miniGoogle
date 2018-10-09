#include "Parse.h"


int main() {
    Parse *parse = new Parse("../../../test/");//spanishText_10000_15000
    parse->processFile();
   
    return 0;
}
