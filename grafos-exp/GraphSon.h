#ifndef snap_examples_GraphSONParser_h
#define snap_examples_GraphSONParser_h

#include <fstream>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include <iostream>
typedef TUNGraph::TNodeI SnapNode;
typedef TUNGraph::TEdgeI SnapEdge;
using namespace std;
class GraphSONParser{
    ofstream file;
    string header;
    string nodeKeyConfig;
    bool directed;
public:
    GraphSONParser(string fileName)
    {
        directed = false;
        string f = fileName+".graphson";
        const char * c = f.c_str();
        file.open(c, ofstream::out);
        file << "{\n";
    }
    ~GraphSONParser()
    {
        if(file.is_open())
            file.close();
    }
    void initGraph()
    {
        string initGraph = "    \"graph\": {\n";
        string initMode = "        \"mode\":\"NORMAL\",\n";
        if(file.is_open())
            file << initGraph << initMode;
    }
    void endGraph()
    {
        string closeGraph = "    }\n}\n";
        if(file.is_open())
            file << closeGraph;
    }
    void initNodes()
    {
        string initNode = "        \"vertices\": [\n";
        if(file.is_open())
            file << initNode;
    }
    void closeNodes()
    {
        string closeNode = "        ],\n";
        if(file.is_open())
            file << closeNode;
    }
    void initEdges()
    {
        string initEdge = "        \"edges\": [\n";
        if(file.is_open())
            file << initEdge;
    }
    void closeEdges()
    {
        string closeEdge = "        ]\n";
        if(file.is_open())
            file << closeEdge;
    }
    void writeNode(SnapNode & node, bool last)
    {
        int Number = node.GetId();
        ostringstream stream;
        stream << Number;
        string id = static_cast<ostringstream*>( &stream )->str();
        string initNode = "";
        if(last)
        {
            string indent = "            ";
            initNode = indent +"{\n"+indent+"    \"_id\": "+'"'+id+'"'+",\n"+indent+"    \"_type\": \"vertex\"\n"+indent+"}\n";
        }
        else
        {
            string indent = "            ";
            initNode = indent +"{\n"+indent+"    \"_id\": "+'"'+id+'"'+",\n"+indent+"    \"_type\": \"vertex\"\n"+indent+"},"+'\n';

        }
        if(file.is_open())
            file << initNode;
    }
    void writeEdge(SnapEdge & edge, bool last)
    {
        int edgeId = edge.GetId();
        int edgeSource = edge.GetSrcNId();
        int edgeDest = edge.GetDstNId();
        ostringstream stream1, stream2, stream3;
        stream1 << edgeId;
        stream2 << edgeSource;
        stream3 << edgeDest;
        string id = static_cast<ostringstream*>( &stream1)->str();
        string src = static_cast<ostringstream*>( &stream2 )->str();
        string dest = static_cast<ostringstream*>( &stream3 )->str();
        string initEdge = "";
        if(last)
        {
            string indent = "            ";
            initEdge = indent+"{\n"+indent+"    \"_type\": \"edge\",\n"+indent+"    \"_outV\": "+'"'+src+'"'+",\n"+indent+"    \"_inV\": "+'"'+dest+'"'+"\n"+indent+"}\n";
        }
        else
        {
            string indent = "            ";
            initEdge = indent+"{\n"+indent+"    \"_type\": \"edge\",\n"+indent+"    \"_outV\": "+'"'+src+'"'+",\n"+indent+"    \"_inV\": "+'"'+dest+'"'+"\n"+indent+"},\n";

        }
        if(file.is_open())
            file << initEdge;
    }
};

#endif
