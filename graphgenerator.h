#ifndef GRAPHGENERATOR_H
#define GRAPHGENERATOR_H

#include "mstalgorithms.h"

class GraphGenerator {
public:
    static MSTGraph* generateConnectedGraph(int n, double density = 0.3);
};

#endif
