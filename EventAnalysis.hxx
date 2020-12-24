#ifndef EVENT_HXX
#define EVENT_HXX

#include <CubeEvent.hxx>
#include <CubeReconObject.hxx>
#include <CubeReconCluster.hxx>
#include <CubeReconTrack.hxx>
#include <CubeHandle.hxx>
#include <CubeG4Trajectory.hxx>
#include <ToolMainTrajectory.hxx>

extern Cube::Event* gEvent;

/**
 * @brief EventAnalysis class
 * @details EventAnalysis has vector of Cube::Handle<Cube::ReconTrack>> 
 * and Cube::Handle<Cube::ReconCluster>. \n
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
         * @brief get vector of tracks in this event
         * @return std::vector<Cube::Handle<Cube::ReconTrack>>: vector of tracks
         * in this event.
         */
        const std::vector<Cube::Handle<Cube::ReconTrack>>& GetTrackVector() const;

        /**
         * @brief get vector of clusters in this event
         * @return std::vector<Cube::Handle<Cube::ReconCluster>>: vector of clusters
         * in this event.
         */
        const std::vector<Cube::Handle<Cube::ReconCluster>>& GetClusterVector() const;

        /**
         * @brief get ReconObjectContainer
         * @return Cube::Handle<Cube::ReconObjectContainer>
         */
        const Cube::Handle<Cube::ReconObjectContainer>& GetObjects() const;

        /**
         * @brief set number of primary anti muon object
         * @details count how many primary anti muons are in this event 
         * based on reconstruected information.
         * @return int number of primary anti muon object
         */
        int SetNumberOfPrimaryAntiMuonObject();

        /**
         * @brief get number of primary anti muon
         * @details count how many primary anti muons are in this event 
         * based on true information.
         * @return int mNumberOfPrimaryAntiMuonTrajectory
         */
        const int GetNumberOfPrimaryAntiMuonTrajectory() const;

        /**
         * @brief get number of primary anti muon object
         * @details count how many primary anti muons are in this event \n
         * based on reconstruected information.
         * @return int mNumberOfPrimaryAntiMuonObject
         */
        const int GetNumberOfPrimaryAntiMuonObject() const;

        /**
         * @brief get number of primary pion
         * @details count how many primary pions are in this event 
         * based on true information.
         * @return int mNumberOfPrimaryPionTrajectory
         */
        const int GetNumberOfPrimaryPionTrajectory() const;

        /**
         * @brief get pdg code of object
         * @param const Cube::Handle<Cube::ReconObject>& inObject: input object
         */
        const int GetPdg(const Cube::Handle<Cube::ReconObject>& inObject) const;

        /**
         * @brief get parent id of object
         * @param const Cube::Handle<Cube::ReconObject>& inObject: input object
         */
        const int GetParentID(const Cube::Handle<Cube::ReconObject>& inObject) const;

        /**
         * @brief get parent pdg code of object
         * @param int inParentId: input parentId of the object
         */
        const int GetParentPdg(int inParentId) const;

        /**
         * @brief set interaction vertex
         * @details vertex is TLorentzVector, earliest primary muon track's front point
         */
        void SetVertex();

        /**
         * @brief get interaction vertex
         */
        const TLorentzVector& GetVertex() const;

        /**
         * @brief set first object in time
         * @detials The first object should not be muon or muon induced \n
         * (assume that muon PID is very good)
         */
        void SetFirstObject();

        /**
         * @brief get first object in time
         * @return Cube::Handle<Cube::ReconObject>.
         */
        const Cube::Handle<Cube::ReconObject> GetFirstObject() const;

        /**
         * @brief show all object information in this event
         * @details informations: (x, y, z, t), pdg, parentId
         */
        const void ShowAllObjects() const;

        /**
         * @brief show first object information in this event
         * @details informations: (x, y, z, t), pdg, parentId
         */
        const void ShowFirstObject() const;

        /**
         * @brief show interaction vertex information of this event
         * @details informations: (x, y, z, t)
         */
        const void ShowVertex() const;

    private:
        /**
         * @brief sort object in this event by time
         */
        void SortObjectsByTime();

        /**
         * @brief add reconstructed object from data file to this event
         */
        void Add(Cube::Handle<Cube::AlgorithmResult> inResult);

        /**
         * @brief vector of tracks of this event
         */
        std::vector<Cube::Handle<Cube::ReconTrack>> mTrack;

        /**
         * @brief vector of clusters of this event
         */
        std::vector<Cube::Handle<Cube::ReconCluster>> mCluster;

        /**
         * @brief object in this event
         */
        Cube::Handle<Cube::ReconObjectContainer> mObjects;

        /**
         * @brief set number of primary anti muon
         * @details count how many primary anti muons are in this event 
         * based on true information.
         * @return int number of primary anti muon
         */
        int SetNumberOfPrimaryAntiMuonTrajectory();

        /**
         * @brief set number of primary pion
         * @details count how many primary pions are in this event 
         * based on true information. \n
         * The pion includes pi+- pi0
         * @return int number of primary pion
         */
        int SetNumberOfPrimaryPionTrajectory();

        /**
         * @brief number of primary anti muon (beasd on true information)
         */
        int mNumberOfPrimaryAntiMuonTrajectory = 0;

        /**
         * @brief number of primary anti muon (beasd on reconstructed information)
         */
        int mNumberOfPrimaryAntiMuonObject = 0;

        /**
         * @brief number of primary pion (beasd on true information)
         */
        int mNumberOfPrimaryPionTrajectory = 0;
        
        /**
         * @brief interaction vertex
         */
        TLorentzVector mVertex;

        /**
         * @brief first object in time
         */
        Cube::Handle<Cube::ReconObject> mFirstObject;
};

#endif
