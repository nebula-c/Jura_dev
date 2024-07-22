#include "iostream"
#include <fstream>
#include <sstream>
#include <string>
#include <json.hpp> // Using nolmann/json.hpp
#include <typeinfo>

#include "TH1.h"
#include "TF1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TMarker.h"
#include "TLegend.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"

#include "Jura.hpp"

using namespace std;
using json = nlohmann::json;

struct Time {
    int hours;
    int minutes;
    int seconds;
    int milliseconds;
};

Time parseTime(const std::string& timeStr) {
    Time time;
    // ss >> time.hours >> discard >> time.minutes >> discard >> time.seconds >> discard >> time.microseconds;
    std::string timePart = timeStr.substr(7);
    sscanf(timePart.c_str(), "%2d%2d%2d.%3d", &time.hours, &time.minutes, &time.seconds, &time.milliseconds);
    return time;
}

Time parseTime2(const std::string& timeStr) {
    Time time;
    // std::string timePart = timeStr.substr(7);
    sscanf(timeStr.c_str(), "%2d:%2d:%2d.%6d", &time.hours, &time.minutes, &time.seconds, &time.milliseconds);
    time.milliseconds /= 1000;
    return time;
}

long long timeDifference(const Time& t1, const Time& t2) {
    std::chrono::milliseconds time1 = std::chrono::hours(t1.hours) + std::chrono::minutes(t1.minutes) +
                                      std::chrono::seconds(t1.seconds) + std::chrono::milliseconds(t1.milliseconds);
    std::chrono::milliseconds time2 = std::chrono::hours(t2.hours) + std::chrono::minutes(t2.minutes) +
                                      std::chrono::seconds(t2.seconds) + std::chrono::milliseconds(t2.milliseconds);

    return (time2 - time1).count();
}

void test1(char* jsonfilename)
{
    Jura Jura(jsonfilename);
    json myjson = *(Jura.ReturnJson());

    for (auto filename : Jura.ReturnFilenames())
    {
        TGraph *g1 = new TGraph();
        
        ifstream datafile(filename);

        if (!datafile.is_open()) {
            cerr << "Unable to open file data.txt" << endl;
        }

        int mstime = 0, accum_time=0, rmsval = 0, timeval=0;
        bool is_firstLine = true;
        Time previousTime = {0, 0, 0, 0};

        string line;
        while (getline(datafile, line)) 
        {
            istringstream iss(line);
            string token;
            while (iss >> token) 
            {
                if (token == "RMS:") 
                {
                    if (iss >> token) 
                    {
                        rmsval = stod(token);
                    }
                }
                if (token == "Time:") 
                {
                    if (iss >> token) 
                        {  // Read the RMS value
                            Time currentTime = parseTime2(token);
                            if (!is_firstLine) {
                                long long deltaTime = timeDifference(previousTime, currentTime);
                                mstime = deltaTime;
                                accum_time+=mstime;
                            } else {
                                is_firstLine = false;
                            }
                            previousTime = currentTime;
                        }
                        g1->SetPoint(g1->GetN(),accum_time,rmsval*0.142857+8057.143);
                }
            }
        }
    Jura.Add2mg(g1);
    datafile.close();
    }
    Jura.Drawing_mg();
}

void Juratest(char* jsonfilename)
{
    // Jura Jura(jsonfilename);
    // Jura.BuildJsonExample();
    test1(jsonfilename);
    







    // if(myjson["config"]["xlimit"]["set"]==true)
    // {
    //     int xmin = myjson["config"]["xlimit"]["range"][0];
    //     int xmax = myjson["config"]["xlimit"]["range"][1];    
    //     g1->GetXaxis()->SetRangeUser(xmin,xmax);
    // }
    
    // if(myjson["config"]["ylimit"]["set"]==true)
    // {
    //     int ymin = myjson["config"]["ylimit"]["range"][0];
    //     int ymax = myjson["config"]["ylimit"]["range"][1];
    //     g1->SetMinimum(ymin);
    //     g1->SetMaximum(ymax);
    // }

    // g1->SetTitle(myconsttitle);
    // g1->GetXaxis()->SetTitle(xlabel.c_str());
    // g1->GetYaxis()->SetTitle(ylabel.c_str());
    // g1->GetYaxis() -> SetTitleOffset(1.5);

    // // GraphSet(g1,myjson["dataconfig"],"data1");
    // // GraphSet(g2,myjson["dataconfig"],"data2");
    // // GraphSet(g3,myjson["dataconfig"],"data3");
    
    // // TF1 *mysin = new TF1("mysin","[0]*sin([1]*x+[2])+[3]",5000,60000);
    // TF1 *mysin = new TF1("mysin","[0]/([1]*sin([2]*x+[3])+[4])+[5]",5000,60000);

    // mysin->SetParameters(0,13000);
    // // mysin->SetParameters(1,0.0064);
    // // mysin->SetParameters(2,1);
    // // mysin->SetParameters(3,12000);
    // mysin->SetParameters(5,9000);
    // // mysin->SetParLimits(0,1700,1900);
    // // mysin->SetParLimits(1,0.00636,0.00638);
    // // // mysin->SetParLimits(2,11000,13000);
    // // mysin->SetParLimits(3,11115,13000);

    // // mysin->FixParameter(1,0.0065);
    // // g1->Fit(mysin,"R0L");

    
    // TFitResultPtr fit1 = g1->Fit(mysin,"R0LS");

    // // TFitResultPtr fit2 = h1->Fit(gaus2,"R0S");
    // cout << fit1 << endl;

    // // l1->AddEntry("",Form("(R0L)#chi^{2}/NDF=%4.3f/%d",fit1->Chi2(),fit1->Ndf()),"");
    // // l1->AddEntry(g1,"Test");

    // g1->Draw("apl");
    // // g2->Draw("SAME");
    // // g3->Draw("SAME");

    // l1->AddEntry(g1,"Total");

    // double *g2x = g2->GetX();
    // double *g2y = g2->GetY();
    // double *g3x = g3->GetX();
    // double *g3y = g3->GetY();

    
    // bool is_first = true;
    // for (int j=0; j<g2->GetN(); j++) {
    //     TMarker *m2 = new TMarker(g2x[j], g2y[j], 22);
    //     if(g2x[j] < myjson["config"]["xlimit"]["range"][0]) continue;
    //     if(g2x[j] > myjson["config"]["xlimit"]["range"][1]) break;
    //     m2->SetMarkerSize(0.9);
    //     m2->SetMarkerColor(3);    
    //     m2->Draw();
    //     if(is_first==true)
    //     {
    //         l1->AddEntry(m2, "Interval  < 2ms");
    //         is_first=false;
    //     }
    // }
    
    // is_first=true;
    // for (int j=0; j<g3->GetN(); j++) {
    //     TMarker *m3 = new TMarker(g3x[j], g3y[j], 22);
    //     if(g3x[j] < myjson["config"]["xlimit"]["range"][0]) continue;
    //     if(g3x[j] > myjson["config"]["xlimit"]["range"][1]) break;
    //     m3->SetMarkerSize(0.9);
    //     m3->SetMarkerColor(2);
    //     m3->Draw();
    //     if(is_first==true)
    //     {
    //         l1->AddEntry(m3, "Interval #geq 2ms");
    //         is_first=false;
    //     }
    // }
        
    // // m2->SetMarkerSize(0.9);
    // // m2->SetMarkerColor(3);
    // // m3->SetMarkerSize(0.9);
    // // m3->SetMarkerColor(2);
    
    // // m2->Draw();
    // // m3->Draw();

    // // mysin->Draw("SAME");
    // // mysin->Draw("apl");
    // l1->Draw();

    

    // if(myjson["save"]==true)
    // {
    //     string pngpath = "./PNGS/";
    //     string output = pngpath + myconsttitle + ".pdf";
    //     cout << output << endl;
    //     const char* myconstoutput = output.c_str();
    //     c1->SaveAs(myconstoutput);
    // }
}
