#include "iostream"
#include <fstream>
#include <sstream>
#include <string>
#include <nlohmann/json.hpp> // Using nolmann/json.hpp
#include <typeinfo>

#include "TH1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMultiGraph.h"

using namespace std;
using json = nlohmann::json;


class Jura {
private :
    json myjson;
    vector <TGraph*> vec_tgraph;
    vector <TH1*> vec_thist;
    vector <string> vec_filename;
    vector <string> vec_dataconfig;
    TMultiGraph *mg = new TMultiGraph();;
    THStack *hs = new THStack("hs","Temporary ttitle");
    string xlabel, ylabel, mytitle;
    TCanvas *c1 = new TCanvas();
    TLegend *l1 = new TLegend();

public:

    Jura(){}

    Jura(char* jsonfilename) { LoadingJson(jsonfilename); }

    void BuildJsonExample()
    {
        json j;
        j["mode"] = "Graph or Hist";
        j["config"] = {
            {"title", "mytitle"},
            {"xlabel", "X_axis_title"},
            {"ylabel", "Y_axis_title"},
            {"xscale", "linear"},
            {"yscale", "linear"},
            {"xlimit", {
                {"set", true},
                {"range", {50000, 60000}}
            }},
            {"ylimit", {
                {"set", true},
                {"range", {9000, 14000}}
            }},
            {"hist",{
                {"xmin", 0},
                {"xmax", 200},
                {"bins", 100}
            }},
            {"legend",{
                {"xrange", {0.75, 0.95}},
                {"yrange", {0.80, 0.95}}
            }}
        };
        j["datafiles"] = {"./mydata.txt"};
        j["dataconfigs"] = {
            {"data1", {
                {"set", true},
                {"linecolor", 4},
                {"linewidth", 1},
                {"linestyle", 1},
                {"markercolor", 4},
                {"markersize", 0.5},
                {"markerstyle", 8},
                {"label", "Data-1"}
            }},
            {"data2", {
                {"set", true},
                {"linecolor", 3},
                {"linewidth", 1},
                {"linestyle", 1},
                {"markercolor", 3},
                {"markersize", 0.9},
                {"markerstyle", 8},
                {"label", "Data-2"}
            }},
            {"data3", {
                {"set", true},
                {"linecolor", 2},
                {"linewidth", 1},
                {"linestyle", 1},
                {"markercolor", 2},
                {"markersize", 0.9},
                {"markerstyle", 8},
                {"label", "Data-3"}
            }}
        };
        j["save"] = true;

        std::ofstream file("ExampleJson.json");
        if (file.is_open()) {
            file << std::setw(4) << j << std::endl;
            file.close();
            std::cout << "JSON data saved to config.json" << std::endl;
        } else {
            std::cerr << "Unable to open file for writing" << std::endl;
        }
    }

    vector<string> ReturnFilenames() { return vec_filename; }

    void DefaultRead()
    {   
        if (myjson.contains("datafiles")) {
            for (const auto& item : myjson["datafiles"].items())
            {
                vec_filename.push_back(item.value());
            }
        }
        else { cerr << "Please check your json file" << endl;}

        if (myjson.contains("dataconfigs")) {
            for (const auto& item : myjson["dataconfigs"].items())
            {
                vec_dataconfig.push_back(item.key());
            }
        }
        else { cerr << "Please check your json file" << endl;}

        if(myjson.contains("config"))
        {
            xlabel = myjson["config"]["xlabel"];
            ylabel = myjson["config"]["ylabel"];
            mytitle = myjson["config"]["title"];
            float l_xmin = myjson["config"]["legend"]["xrange"][0];
            float l_xmax = myjson["config"]["legend"]["xrange"][1];
            float l_ymin = myjson["config"]["legend"]["yrange"][0];
            float l_ymax = myjson["config"]["legend"]["yrange"][1];
            l1 = new TLegend(l_xmin,l_ymin,l_xmax,l_ymax);
        }
        const char* myconsttitle = mytitle.c_str();
        c1 = new TCanvas("c1",myconsttitle,1600,2000);
    }

    void LoadingJson(char* jsonfilename)
    {
        ifstream myjsonfile(jsonfilename);
        myjsonfile >> myjson;
        DefaultRead();
    }

    json *ReturnJson() { return &myjson; }


    void TotalSet_mg()
    {
        if(myjson["config"]["xlimit"]["set"]==true)
        {
            int xmin = myjson["config"]["xlimit"]["range"][0];
            int xmax = myjson["config"]["xlimit"]["range"][1];    
            mg->GetXaxis()->SetRangeUser(xmin,xmax);
        }

        if(myjson["config"]["ylimit"]["set"]==true)
        {
            int ymin = myjson["config"]["ylimit"]["range"][0];
            int ymax = myjson["config"]["ylimit"]["range"][1];
            mg->SetMinimum(ymin);
            mg->SetMaximum(ymax);
        }
        string mytitle = myjson["config"]["title"];
        const char* myconsttitle = mytitle.c_str();
        mg->SetTitle(myconsttitle);
        mg->GetXaxis()->SetTitle(xlabel.c_str());
        mg->GetYaxis()->SetTitle(ylabel.c_str());
        mg->GetYaxis()->SetTitleOffset(1.5);
    }

    void TotalSet_hs()
    {
        if(myjson["config"]["xlimit"]["set"]==true)
        {
            int xmin = myjson["config"]["xlimit"]["range"][0];
            int xmax = myjson["config"]["xlimit"]["range"][1];    
            hs->GetXaxis()->SetRangeUser(xmin,xmax);
        }

        if(myjson["config"]["ylimit"]["set"]==true)
        {
            int ymin = myjson["config"]["ylimit"]["range"][0];
            int ymax = myjson["config"]["ylimit"]["range"][1];
            hs->SetMinimum(ymin);
            hs->SetMaximum(ymax);
        }
        string mytitle = myjson["config"]["title"];
        const char* myconsttitle = mytitle.c_str();
        hs->SetTitle(myconsttitle);
        hs->GetXaxis()->SetTitle(xlabel.c_str());
        hs->GetYaxis()->SetTitle(ylabel.c_str());
        hs->GetYaxis()->SetTitleOffset(1.5);
    }


    void HistSet(TH1 *myhist, string datalabel)
    {
        if(myjson["dataconfigs"][datalabel]["set"]==true)
        {
            int mylinecolor =   myjson["dataconfigs"][datalabel]["linecolor"];
            float mylinewidth = myjson["dataconfigs"][datalabel]["linewidth"];
            int mylinestyle =   myjson["dataconfigs"][datalabel]["linestyle"];
            int mymarkercolor = myjson["dataconfigs"][datalabel]["markercolor"];
            float mymarkersize= myjson["dataconfigs"][datalabel]["markersize"];
            int mymarkerstyle = myjson["dataconfigs"][datalabel]["markerstyle"];
        
            myhist->SetLineColor(mylinecolor);
            myhist->SetLineWidth(mylinewidth);
            myhist->SetLineStyle(mylinestyle);
            myhist->SetMarkerColor(mymarkercolor);
            myhist->SetMarkerSize(mymarkersize);
            myhist->SetMarkerStyle(mymarkerstyle);
        }
    }


    void Drawing_mg()
    {
        for (int i=0;i<vec_tgraph.size();i++)
        {
            TGraph *eachTGraph = vec_tgraph[i];
            string eachconfig = vec_dataconfig[i];
            cout << eachconfig << endl;
            string mylabel;

            HistSet((TH1*)eachTGraph,eachconfig);
            mg->Add(eachTGraph);
            
            mylabel = myjson["dataconfigs"][eachconfig]["label"];
            l1->AddEntry(eachTGraph,mylabel.c_str());
        }
        mg->Draw("ALP");
        TotalSet_mg();
        l1->Draw();
    }

    void Drawing_hs()
    {
        for (int i=0;i<vec_thist.size();i++)
        {
            TH1 *eachTH1 = vec_thist[i];
            string eachconfig = vec_dataconfig[i];
            string mylabel;

            HistSet((TH1*)eachTH1,eachconfig);
            hs->Add(eachTH1);

            mylabel = myjson["dataconfigs"][eachconfig]["label"];
            l1->AddEntry(eachTH1,mylabel.c_str());
        }

        hs->Draw("nostack");
        const char* myconsttitle = mytitle.c_str();
        hs->SetTitle(myconsttitle);
        TotalSet_hs();
        l1->Draw();
    }
    void Add2mg(TGraph *g1) { vec_tgraph.push_back(g1); }
    void Add2hs(TH1 *h1)    { vec_thist.push_back(h1); }
    vector<TGraph*> ReturnVecTGraph() { return vec_tgraph; }
};





