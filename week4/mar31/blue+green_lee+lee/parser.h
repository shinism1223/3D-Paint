#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#define MAX_LINE_LEN 1024

#include "mesh.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>

class Parser
{
private:
    char *path;
    Mesh *mesh;

public:
    void setMesh(Mesh *mesh);
    void setPath(char *path);
    void parse();

private:
    int findTokenFormat(char *token);
    void toTriangle(void);
};

#endif // PARSER_H_INCLUDED
