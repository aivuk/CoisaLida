//
//  ofPoem.cpp
//  CoisaLida
//
//  Created by walrus on 10/21/14.
//
//

#include "ofPoem.h"
#include <sstream>
#include <string>

void ofPoem::setup(std::string _text) {
    istringstream itext(_text);
    copy(istream_iterator<std::string>(itext),
         istream_iterator<string>(),
         back_inserter(text));

}