#include "EventAnalysis.hxx"

#include <TFile.h>
#include <TObject.h>
#include <TEveManager.h>
#include <TColor.h>
#include <TChain.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TApplication.h>
#include <TH1.h>
#include <TCanvas.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <getopt.h>
#include <iostream>
#include <memory>

std::unique_ptr<TChain> gCubeReconTree = NULL;

std::unique_ptr<TH1F> deltaTNeutron = std::make_unique<TH1F> ("","#delta T, neutron",100,-10,10);
std::unique_ptr<TH1F> deltaTOther = std::make_unique<TH1F> ("","#delta T, other",100,-10,10);

void Run(std::string fileName);

int main(int argc, char** argv)
{
    std::string fileName = "";

    // Check if there is an input file on the command line.
    if (argc - optind > 0) {
        fileName = argv[optind];
    }

    Run(fileName);

    TCanvas can1;
    deltaTNeutron->Draw();
    can1.SaveAs("deltaTNeutron.pdf");

    TCanvas can2;
    deltaTOther->Draw();
    can2.SaveAs("deltaTOther.pdf");
    return 0;
}

void Run(std::string fileName)
{
    gCubeReconTree = std::make_unique<TChain> ("CubeEvents"); 
    gCubeReconTree->Add(fileName.c_str());

    if (!gCubeReconTree) 
    {
        std::cout << "Missing the event tree" << std::endl;
        return;
    }

    gCubeReconTree->SetBranchAddress("Event",&gEvent);
    std::cout<<"total number of events : "<<gCubeReconTree->GetEntries()<<std::endl;

    //for (Int_t i = 0; i< 10; i++)
    for (Int_t i = 0; i< gCubeReconTree->GetEntries(); i++)
    {
        gCubeReconTree->GetEntry(i);
        Cube::Handle<Cube::AlgorithmResult> topResult(gEvent,false);
        std::unique_ptr<EventAnalysis> tempEventAnalysis = NULL;
        try
        {
            tempEventAnalysis = std::make_unique<EventAnalysis> (topResult);
        }
        catch (const std::runtime_error& e)
        {
            std::cout << "exceptrion in event: " << i << ", " << e.what() << std::endl;
            std::cout << "--------------------------------" << std::endl;
            continue;
        }

        //CC0pi, true selection
        if (tempEventAnalysis->GetNumberOfPrimaryPionTrajectory() != 0 || tempEventAnalysis->GetNumberOfPrimaryAntiMuonTrajectory() != 1)
        {
            continue;
        }

        tempEventAnalysis->SetNumberOfPrimaryAntiMuonObject();
        //seletion of single muon track event
        if (tempEventAnalysis->GetNumberOfPrimaryAntiMuonObject() != 1)
        {
            continue;
        }
        std::cout << "event: " << i << std::endl;
        //std::cout << "number of primary muon track: " << tempEventAnalysis->GetNumberOfPrimaryAntiMuonObject() << std::endl;
        try
        {
            tempEventAnalysis->SetVertex();
            tempEventAnalysis->SetFirstObject();
        }
        catch (const std::runtime_error& e)
        {
            std::cout << e.what() << std::endl;
            std::cout << "--------------------------------" << std::endl;
            continue;
        }

        TLorentzVector vertex = tempEventAnalysis->GetVertex();
        Cube::Handle<Cube::ReconTrack> tempFirstTrack = tempEventAnalysis->GetFirstObject();
        Cube::Handle<Cube::ReconCluster> tempFirstCluster= tempEventAnalysis->GetFirstObject();

        tempEventAnalysis->ShowAllObjects();
        tempEventAnalysis->ShowFirstObject();
        tempEventAnalysis->ShowVertex();
        std::cout << "--------------------------------" << std::endl;
    }
}
