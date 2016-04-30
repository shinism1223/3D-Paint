#include "parser.h"

void
Parser::setMesh(Mesh *mesh)
{
    this->mesh = mesh;
}

void
Parser::setPath(char *path)
{
    this->path = path;
}

void
Parser::parse()
{
    FILE *file = fopen(path, "r");

    if(file == NULL)
        {
        /* Unable to open file. */
        //exit(0)
        return;
    }

    while(!feof(file))
    {
        char line[MAX_LINE_LEN];
        char dataType[MAX_LINE_LEN];

        if(!fgets(line, MAX_LINE_LEN, file)) break;
        sscanf(line, "%s", dataType);

        if(strcmp(dataType, "v") == 0)
        {
            VECTOR3 tmp;
            sscanf(line, "%*s %f %f %f", &tmp.x, &tmp.y, &tmp.z);
            mesh->v.push_back(tmp);

        }
        else if(strcmp(dataType, "vt") == 0)
        {
            VECTOR2 tmp;
            sscanf(line, "%*s %f %f", &tmp.x, &tmp.y);
            mesh->vt.push_back(tmp);

        }
        else if(strcmp(dataType, "vn") == 0)
        {
            VECTOR3 tmp;
            sscanf(line, "%*s %f %f %f", &tmp.x, &tmp.y, &tmp.z);
            mesh->vn.push_back(tmp);

        }
        else if(strcmp(dataType, "f") == 0)
        {
            POLYGON polygon;
            char *token = strtok(line, " ");
            token = strtok(NULL, " ");
            while(token)
            {
                int v, vt, vn;
                switch(findTokenFormat(token))
                {
                case 0: //f v v v v v ...
                    sscanf(token, "%d", &v);
                    polygon.vIndex.push_back(v);
                    break;
                case 1: //f v/vt v/vt v/vt ...
                    sscanf(token, "%d/%d", &v, &vt);
                    polygon.vIndex.push_back(v);
                    polygon.vtIndex.push_back(vt);
                    break;
                case 2: //f v/vt/vn v/vt/vn ...
                    sscanf(token, "%d/%d/%d", &v, &vt, &vn);
                    polygon.vIndex.push_back(v);
                    polygon.vtIndex.push_back(vt);
                    polygon.vnIndex.push_back(vn);
                    break;
                case 3:  //f v//vn v//vn ...
                    sscanf(token, "%d//%d", &v, &vn);
                    polygon.vIndex.push_back(v);
                    polygon.vnIndex.push_back(vn);
                    break;
                default:
                    /* WRONG .obj FILE FORMAT */
                    break;
                }
                token = strtok(NULL, " ");
            }
            mesh->polygon.push_back(polygon);
        }
        else{
            /* other data types */
        }
    }

    toTriangle();

    fclose(file);
}

int
Parser::findTokenFormat(char *token)
{
    int cntSlashes = 0;
    int len = strlen(token);
    int ret = 0;

    bool secondElementExist = false;

    for(int i=0;i<len;i++ ) if(token[i] == '/'){
        cntSlashes++;
        if( cntSlashes == 1     &&
            i+1<len             &&
            token[i+1]!='/' )
            secondElementExist = true;
    }

    //4cases
    // 0 : v
    if(cntSlashes == 0) ret = 0;
    // 1 : v/vt
    if(cntSlashes == 1) ret = 1;
    // 2 : v/vt/vn
    if(cntSlashes == 2 && secondElementExist) ret = 2;
    // 3 : v//vn
    if(cntSlashes == 2 && !secondElementExist) ret = 3;

    return ret;
}

void
Parser::toTriangle(void)
{
    for(POLYGON& polygon : mesh->polygon)
    {
        //dividing : one polygon-> several triangles
        TRIANGLE t;

        for(int i=0;i+1<polygon.vIndex.size();i++)
        {
            t.v[0] = mesh->v[polygon.vIndex[0] - 1];
            t.v[1] = mesh->v[polygon.vIndex[i] - 1];
            t.v[2] = mesh->v[polygon.vIndex[i+1] - 1];

            if(!polygon.vtIndex.empty())
            {
                t.vt[0] = mesh->vt[polygon.vtIndex[0]-1];
                t.vt[1] = mesh->vt[polygon.vtIndex[i]-1];
                t.vt[2] = mesh->vt[polygon.vtIndex[i+1]-1];
            }else
            {
                /* In later. this place should be filled... */
            }

            if(!polygon.vnIndex.empty())
            {
                t.vn[0] = mesh->vn[polygon.vnIndex[0]-1];
                t.vn[1] = mesh->vn[polygon.vnIndex[i]-1];
                t.vn[2] = mesh->vn[polygon.vnIndex[i+1]-1];
            }else
            {
                /* In later. this place should be filled... */
            }

        }

        mesh->triangle.push_back(t);
    }
}
