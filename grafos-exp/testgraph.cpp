#include <fstream>
#include <iomanip>
#include <locale>
#include <time.h>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include "Snap.h"
#include "GraphSon.h"

using namespace TSnap;
using namespace std;

class Visitor {

private:
  vector<int> nodos;

public:
  Visitor() { }
  Visitor(const int& Nodes) { }
  void FinishNode(const int& NId) { }
  void ExamineEdge(const int& NId1, const int& NId2) { }
  void TreeEdge(const int& NId1, const int& NId2) { }
  void BackEdge(const int& NId1, const int& NId2) { }
  void FwdEdge(const int& NId1, const int& NId2) { }
  void DiscoverNode(int NId){ nodos.push_back(NId); }
  vector<int> getVertices() { return nodos; }

};

PUNGraph importarGrafo(TStr nombreArchivo);
vector<int> obtenerVerticesOrdenados(PUNGraph G);
void ordenarAsc(vector<int>& v, int n);
void grafoGDF(PUNGraph G);
void grafoML(PUNGraph G);
void grafoGEXF(PUNGraph G);

int main(int argc, char* argv[]) {

    PUNGraph Graph = TUNGraph::New();
    Graph = TSnap::LoadEdgeList<PUNGraph>("facebook_combined.txt",0,1);
    PUNGraph G = importarGrafo("facebook_combined.txt");
    clock_t t;

    t = clock();
    grafoGDF(G);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Tiempo en GDF: %f ms \n", time_taken*1000);

    t = clock();
    grafoML(G);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Tiempo en GraphML: %f ms \n", time_taken*1000);

    t = clock();
    grafoGEXF(G);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Tiempo en GEXF: %f ms \n", time_taken*1000);

    t = clock();
    GraphSONParser gson("Facebook");
    int nodeCounter = 0;
    int edgeCounter = 0;
    gson.initGraph();
    gson.initNodes();
    for(SnapNode NI = Graph->BegNI(); NI!=Graph->EndNI(); NI++)
    {
        nodeCounter++;
        if(nodeCounter == Graph->GetNodes())
            gson.writeNode(NI, true);
        else
            gson.writeNode(NI, false);
    }
    gson.closeNodes();
    gson.initEdges();
    for(SnapEdge EI = Graph->BegEI(); EI!=Graph->EndEI(); EI++)
    {
        edgeCounter++;
        if(edgeCounter == Graph->GetEdges())
            gson.writeEdge(EI, true);
        else
            gson.writeEdge(EI, false);
    }
    gson.closeEdges();
    gson.endGraph();
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Tiempo en GraphSon: %f ms \n", time_taken*1000);

}

PUNGraph importarGrafo(TStr nombreArchivo) {

  PUNGraph G = TSnap::LoadEdgeList<PUNGraph>(nombreArchivo, 0, 1);

  return G;

}

vector<int> obtenerVerticesOrdenados(PUNGraph G){

  Visitor vis(G->GetNodes());
  TCnCom::GetDfsVisitor(G, vis);
  vector<int> nodos = vis.getVertices();

  ordenarAsc(nodos, int(nodos.size()));

  return nodos;

}

void ordenarAsc(vector<int>& v, int n) {

  int index;

  for (int i=1; i < n; i++)
  {
    index = v[i];

    int j = i-1;

    while (j >= 0 && v[j] > index)
    {
      v[j + 1] = v[j];
      j--;
    }

    v[j+1] = index;
  }

}

void grafoGDF(PUNGraph G) {

  ofstream myfile;
  vector<int> nodos = obtenerVerticesOrdenados(G);
  TIntV conexiones;

  myfile.open("Facebook.gdf");

  myfile << "nodedef>name VARCHAR" << "\n";
  myfile << "edgedef>node1 VARCHAR,node2 VARCHAR" << "\n";

  myfile << "nodedef>id VARCHAR\n";
		for (PUNGraph::TObj::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++)
			myfile << NI.GetId() << "\n";

		myfile << "edgedef>source VARCHAR, destination VARCHAR\n";
		for (PUNGraph::TObj::TEdgeI EI = G->BegEI(); EI < G->EndEI(); EI++)
			myfile << EI.GetSrcNId() << ", " << EI.GetDstNId() << "\n";

  myfile.close();

}

void grafoML(PUNGraph G) {

  ofstream myfile;
  vector<int> nodos = obtenerVerticesOrdenados(G);
  TIntV conexiones;

  myfile.open("Facebook.graphml");

  myfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << "\n";
  myfile << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\"" << "\n";
  myfile << "\txmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" << "\n";
  myfile << "\txsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">" << "\n";
  myfile << "  <graph id=\"G\" edgedefault=\"undirected\">" << "\n";

  for (PUNGraph::TObj::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++)
			myfile << "<node id=\"" << NI.GetId() << "\"/>\n";

		int i = 1;
	for (PUNGraph::TObj::TEdgeI EI = G->BegEI(); EI < G->EndEI(); EI++, ++i)
		myfile << "<edge id=\"e" << i << "\" source=\"" << EI.GetSrcNId() << "\" target=\"" << EI.GetDstNId() << "\"/>\n";

  myfile << "  </graph>" << "\n";
  myfile << "</graphml>" << "\n";

  myfile.close();

}

void grafoGEXF(PUNGraph G) {

  ofstream myfile;
  vector<int> nodos = obtenerVerticesOrdenados(G);
  TIntV conexiones;

  myfile.open("Facebook.gexf");

  myfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << "\n";
  myfile << "<gexf xmlns=\"http://www.gexf.net/1.2draft\" version=\"1.2\">" << "\n";
  myfile << "\t<graph mode=\"static\" defaultedgetype=\"undirected\">" << "\n";
  myfile << "\t\t<edges>" << endl;

  myfile << "<nodes>\n";
		for (PUNGraph::TObj::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++)
			myfile << "<node id=\"" << NI.GetId() << "\" />\n";
		myfile << "</nodes>\n";

		myfile << "<edges>\n";
		int i = 1;
		for (PUNGraph::TObj::TEdgeI EI = G->BegEI(); EI < G->EndEI(); EI++, ++i)
			myfile << "<edge id=\"" << i << "\" source=\"" << EI.GetSrcNId() << "\" target=\"" << EI.GetDstNId() << "\" />\n";
		myfile << "</edges>\n";

  myfile << "\t\t</edges>" << "\n";
  myfile << "\t</graph>" << "\n";
  myfile << "</gexf>" << "\n";

  myfile.close();

}
