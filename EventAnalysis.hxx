#ifndef EVENT_HXX
#define EVENT_HXX

#include <CubeEvent.hxx>
#include <CubeReconObject.hxx>
#include <CubeReconCluster.hxx>
#include <CubeReconShower.hxx>
#include <CubeReconTrack.hxx>
#include <CubeReconVertex.hxx>
#include <CubeHandle.hxx>
#include <CubeReconNode.hxx>
#include <CubeLog.hxx>
#include <CubeAlgorithmResult.hxx>
#include <CubeG4Hit.hxx>
#include <CubeG4Trajectory.hxx>
#include <CubeHit.hxx>
#include <ToolPrimaryId.hxx>
#include <ToolG4Hits.hxx>
#include <ToolMainTrajectory.hxx>
#include <ToolTrueDirection.hxx>

extern Cube::Event* gEvent;

/**
 * @brief EventAnalysis class
 * @details EventAnalysis has vector of Cube::Handle<Cube::ReconTrack>> and Cube::Handle<Cube::ReconCluster>.
 * It also has information of first object, vertex.
 * @date 2020-12-24
 */
class EventAnalysis
{
    public:
        /**
         * @brief initializer
         */
        EventAnalysis(Cube::Handle<Cube::AlgorithmResult> inResult)
            : mNumberOfPrimaryPionTrajectory(SetNumberOfPrimaryPionTrajectory())
              ,mNumberOfPrimaryAntiMuonTrajectory(SetNumberOfPrimaryAntiMuonTrajectory())
        {
            Add(inResult);
            SortObjectsByTime();
        };

        /**
         * @brief get vector of tracks.
         * @return std::vector<Cube::Handle<Cube::ReconTrack>>: vector of tracks
         * in this event.
         */
        const std::vector<Cube::Handle<Cube::ReconTrack>>& GetTrackVector() const;
        const std::vector<Cube::Handle<Cube::ReconCluster>>& GetClusterVector() const;
        const Cube::Handle<Cube::ReconObjectContainer>& GetObjects() const;

        const int GetNumberOfPrimaryAntiMuonTrajectory() const;
        const int GetNumberOfPrimaryAntiMuonObject() const;
        const int GetNumberOfPrimaryPionTrajectory() const;

        //template <typename T>
        //int GetPdg(T object);
        const int GetPdg(const Cube::Handle<Cube::ReconObject>& inObject) const;
        const int GetParentID(const Cube::Handle<Cube::ReconObject>& inObject) const;
        const int GetParentPdg(int inParentId) const;
        int SetNumberOfPrimaryAntiMuonObject();

        void SetVertex();
        const TLorentzVector& GetVertex() const;
        void SetFirstObject();
        const Cube::Handle<Cube::ReconObject> GetFirstObject() const;

        const void ShowAllObjects() const;
        const void ShowFirstObject() const;
        const void ShowVertex() const;

    private:
        void SortObjectsByTime();
        void Add(Cube::Handle<Cube::AlgorithmResult> inResult);
        std::vector<Cube::Handle<Cube::ReconTrack>> mTrack;
        std::vector<Cube::Handle<Cube::ReconCluster>> mCluster;
        Cube::Handle<Cube::ReconObjectContainer> mObjects;
        int SetNumberOfPrimaryAntiMuonTrajectory();
        int SetNumberOfPrimaryPionTrajectory();
        int mNumberOfPrimaryAntiMuonTrajectory = 0;
        int mNumberOfPrimaryAntiMuonObject = 0;
        int mNumberOfPrimaryPionTrajectory = 0;
        TLorentzVector mVertex;
        Cube::Handle<Cube::ReconObject> mFirstObject;
};

#endif
