// Copyright 2010-2012 UT-Battelle, LLC.  See LICENSE.txt for more information.
#include "eavl.h"
#include "eavlCUDA.h"
#include "eavlFilter.h"
#include "eavlDataSet.h"
#include "eavlTimer.h"
#include "eavlException.h"

#include "eavlImporterFactory.h"
#include "eavlVTKExporter.h"

#include "eavlIsosurfaceFilter.h"
#include "eavlExecutor.h"


eavlDataSet *ReadMeshFromFile(const string &filename, int meshindex)
{
    eavlImporter *importer = eavlImporterFactory::GetImporterForFile(filename);
    
    if (!importer)
        THROW(eavlException,"Didn't determine proper file reader to use");

    vector<string> allmeshes = importer->GetMeshList();
    if (meshindex >= (int)allmeshes.size())
        return NULL;

    string meshname = allmeshes[meshindex];
    // always read the first domain for now
    eavlDataSet *out = importer->GetMesh(meshname, 0);
    vector<string> allvars = importer->GetFieldList(meshname);
    for (size_t i=0; i<allvars.size(); i++)
        out->fields.push_back(importer->GetField(allvars[i], meshname, 0));

    return out;
}
 
int main(int argc, char *argv[])
{
    try
    {   
        if (argc != 2)
            THROW(eavlException,"Incorrect number of arguments");

        int meshindex = 0;
        eavlDataSet *data = ReadMeshFromFile(argv[1], meshindex);
        while (data)
        {
            data->PrintSummary(cout);
            ++meshindex;
            data = ReadMeshFromFile(argv[1], meshindex);
            if (data)
                cout << "\n\n";
        }
    }
    catch (const eavlException &e)
    {
        cerr << e.GetErrorText() << endl;
        cerr << "\nUsage: "<<argv[0]<<" <infile>\n";
        return 1;
    }


    return 0;
}
