#include "EventAnalysis.hxx"

Cube::Event* gEvent = NULL;

void EventAnalysis::Add(Cube::Handle<Cube::AlgorithmResult> inResult)
{
    if (inResult->GetObjectContainers().size() == 0)
    {
        throw std::runtime_error("Object container size = 0");
    }
    for (Cube::AlgorithmResult::ReconObjects::reverse_iterator o = inResult->GetObjectContainers().rbegin();
            o != inResult->GetObjectContainers().rend(); 
            ++o) 
    {
        Cube::Handle<Cube::ReconObjectContainer> objects
            = gEvent->GetObjectContainer((*o)->GetName());

        if (objects)
        {
            this->mObjects = objects;
        }

        for (Cube::ReconObjectContainer::iterator obj = objects->begin();
                obj != objects->end(); ++obj) 
        {
            Cube::Handle<Cube::ReconTrack> track = (*obj);
            if (track)
            {
                this->mTrack.push_back(track);
            }

            Cube::Handle<Cube::ReconCluster> cluster = (*obj);
            if (cluster)
            {
                this->mCluster.push_back(cluster);
            }
        }
    }
}

const std::vector<Cube::Handle<Cube::ReconTrack>>& EventAnalysis::GetTrackVector() const
{
    return this->mTrack;
}

const std::vector<Cube::Handle<Cube::ReconCluster>>& EventAnalysis::GetClusterVector() const
{
    return this->mCluster;
}

const Cube::Handle<Cube::ReconObjectContainer>& EventAnalysis::GetObjects() const
{
        return this->mObjects;
}

int EventAnalysis::SetNumberOfPrimaryAntiMuonTrajectory()
{
    int tempNumberOfPrimaryAntiMuonTrajectory = 0;
    for (Cube::Event::G4TrajectoryContainer::iterator g4Trajectory
            = gEvent->G4Trajectories.begin();
            g4Trajectory != gEvent->G4Trajectories.end();
            ++g4Trajectory) 
    {
        Cube::Handle<Cube::G4Trajectory> gt = g4Trajectory->second; //trajectory
        if (gt->GetParentId() == -1)
        {
            if (gt->GetPDGCode() == -13)
            {
                tempNumberOfPrimaryAntiMuonTrajectory++;
            }
        }
    }

    return tempNumberOfPrimaryAntiMuonTrajectory;
}

int EventAnalysis::SetNumberOfPrimaryPionTrajectory()
{
    int tempNumberOfPrimaryPionTrajectory = 0;
    for (Cube::Event::G4TrajectoryContainer::iterator g4Trajectory
            = gEvent->G4Trajectories.begin();
            g4Trajectory != gEvent->G4Trajectories.end();
            ++g4Trajectory) 
    {
        Cube::Handle<Cube::G4Trajectory> gt = g4Trajectory->second; //trajectory
        if (gt->GetParentId() == -1)
        {
            if (std::abs(gt->GetPDGCode()) == 211 || gt->GetPDGCode() == 111)
            {
                tempNumberOfPrimaryPionTrajectory++;
            }
        }
    }
    return tempNumberOfPrimaryPionTrajectory;
}

const int EventAnalysis::GetNumberOfPrimaryAntiMuonTrajectory() const
{
    return this->mNumberOfPrimaryAntiMuonTrajectory;
}

const int EventAnalysis::GetNumberOfPrimaryPionTrajectory() const
{
    return this->mNumberOfPrimaryPionTrajectory;
}

bool tSort(Cube::Handle<Cube::ReconObject> a, Cube::Handle<Cube::ReconObject> b)
{
    double aTime;
    double bTime;
    Cube::Handle<Cube::ReconTrack> tempTrackA = a;
    Cube::Handle<Cube::ReconCluster> tempClusterA = a;
    Cube::Handle<Cube::ReconTrack> tempTrackB = b;
    Cube::Handle<Cube::ReconCluster> tempClusterB = b;
    if (tempTrackA)
    {
        aTime = tempTrackA->GetPosition().T();
    }
    if (tempClusterA)
    {
        aTime = tempClusterA->GetPosition().T();
    }
    if (tempTrackB)
    {
        bTime = tempTrackB->GetPosition().T();
    }
    if (tempClusterB)
    {
        bTime = tempClusterB->GetPosition().T();
    }

    return aTime < bTime;
}

void EventAnalysis::SortObjectsByTime()
{
    if (this->GetObjects())
    {
        std::sort(this->mObjects->begin(), this->mObjects->end(), tSort);
    }
}

void EventAnalysis::SetFirstObject()
{
    Cube::Handle<Cube::ReconObject> tempObject;
    for (auto t : *mObjects)
    {
        if (GetPdg(t) == -13 || GetPdg(t) == 0 || std::abs(GetParentPdg(GetParentID(t))) == 13)
        {
            continue;
        }
        tempObject = t;
        break;
    }
    if (!tempObject)
    {
        throw std::runtime_error("no first object candidate");
    }
    this->mFirstObject = tempObject;
}

const Cube::Handle<Cube::ReconObject> EventAnalysis::GetFirstObject() const
{
    return this->mFirstObject;
}

int EventAnalysis::SetNumberOfPrimaryAntiMuonObject()
{
    int tempNumberOfPrimaryAntiMuonObject = 0;
    for (auto tempObject : *this->GetObjects())
    {
        Cube::Handle<Cube::ReconTrack> tempTrack = tempObject;
        if (tempTrack)
        {
            if (GetPdg(tempTrack) == -13 && GetParentID(tempTrack) == -1)
            {
                tempNumberOfPrimaryAntiMuonObject++;
            }
        }
    }
    this->mNumberOfPrimaryAntiMuonObject = tempNumberOfPrimaryAntiMuonObject;
    return tempNumberOfPrimaryAntiMuonObject;
}

const int EventAnalysis::GetNumberOfPrimaryAntiMuonObject() const
{
    return mNumberOfPrimaryAntiMuonObject;
}

const int EventAnalysis::GetPdg(const Cube::Handle<Cube::ReconObject>& inObject) const
{
    int tempPDGCode = 0;
    Cube::Handle<Cube::ReconTrack> inTrack = inObject;
    Cube::Handle<Cube::ReconCluster> inCluster = inObject;
    if (inTrack)
    {
        for (Cube::Event::G4TrajectoryContainer::iterator g4Trajectory
                = gEvent->G4Trajectories.begin();
                g4Trajectory != gEvent->G4Trajectories.end();
                ++g4Trajectory) 
        {
            if(g4Trajectory->first == Cube::Tool::MainTrajectory(*gEvent, *inTrack))
            {
                Cube::Handle<Cube::G4Trajectory> gt = g4Trajectory->second; //trajectory
                tempPDGCode = gt->GetPDGCode();
            }
        }
    }
    if (inCluster)
    {
        for (Cube::Event::G4TrajectoryContainer::iterator g4Trajectory
                = gEvent->G4Trajectories.begin();
                g4Trajectory != gEvent->G4Trajectories.end();
                ++g4Trajectory) 
        {
            if(g4Trajectory->first == Cube::Tool::MainTrajectory(*gEvent, *inCluster))
            {
                Cube::Handle<Cube::G4Trajectory> gt = g4Trajectory->second; //trajectory
                tempPDGCode = gt->GetPDGCode();
            }
        }
    }
    return tempPDGCode;
}

const int EventAnalysis::GetParentID(const Cube::Handle<Cube::ReconObject>& inObject) const
{
    int tempParentId = 0;
    Cube::Handle<Cube::ReconTrack> inTrack = inObject;
    Cube::Handle<Cube::ReconCluster> inCluster = inObject;
    if (inTrack)
    {
        for (Cube::Event::G4TrajectoryContainer::iterator g4Trajectory
                = gEvent->G4Trajectories.begin();
                g4Trajectory != gEvent->G4Trajectories.end();
                ++g4Trajectory) 
        {
            if(g4Trajectory->first == Cube::Tool::MainTrajectory(*gEvent, *inTrack))
            {
                Cube::Handle<Cube::G4Trajectory> gt = g4Trajectory->second; //trajectory
                tempParentId = gt->GetParentId();
            }
        }
    }
    if (inCluster)
    {
        for (Cube::Event::G4TrajectoryContainer::iterator g4Trajectory
                = gEvent->G4Trajectories.begin();
                g4Trajectory != gEvent->G4Trajectories.end();
                ++g4Trajectory) 
        {
            if(g4Trajectory->first == Cube::Tool::MainTrajectory(*gEvent, *inCluster))
            {
                Cube::Handle<Cube::G4Trajectory> gt = g4Trajectory->second; //trajectory
                tempParentId = gt->GetParentId();
            }
        }
    }
    return tempParentId;
}

void EventAnalysis::SetVertex()
{
    TLorentzVector tempVertex;
    for (auto tempObject : *this->GetObjects())
    {
        Cube::Handle<Cube::ReconTrack> tempTrack = tempObject;
        if (tempTrack)
        {
            Cube::Handle<Cube::TrackState> frontState = tempTrack->GetState();
            if (GetPdg(tempTrack) == -13)
            {
                tempVertex = frontState->GetPosition();
                break;
            }
        }
    }
    if (tempVertex.X() == 0 && tempVertex.Y() == 0 && tempVertex.Z() == 0)
    {
        throw std::runtime_error("no vertex candidate");
    }
    this->mVertex = tempVertex;
}

const TLorentzVector& EventAnalysis::GetVertex() const
{
    return this->mVertex;
}

const void EventAnalysis::ShowAllObjects() const
{
    std::cout << "number of tracks: " << this->mTrack.size() << std::endl;
    std::cout << "number of clusters: " << this->mCluster.size() << std::endl;
    for (auto tempObject : *this->GetObjects())
    {
        Cube::Handle<Cube::ReconTrack> tempTrack = tempObject;
        if (tempTrack)
        {
            std::cout << "track:   (" << tempTrack->GetPosition().X();
            std::cout << ", " << tempTrack->GetPosition().Y();
            std::cout << ", " << tempTrack->GetPosition().Z();
            std::cout << ", " << tempTrack->GetPosition().T();
            std::cout << ") , pdg: " << GetPdg(tempTrack);
            std::cout << ", parentId: " << GetParentID(tempTrack) <<std::endl;
        }
        Cube::Handle<Cube::ReconCluster> tempCluster = tempObject;
        if (tempCluster)
        {
            std::cout << "cluster: (" << tempCluster->GetPosition().X();
            std::cout << ", " << tempCluster->GetPosition().Y();
            std::cout << ", " << tempCluster->GetPosition().Z();
            std::cout << ", " << tempCluster->GetPosition().T();
            std::cout << ") , pdg: " << GetPdg(tempCluster);
            std::cout << ", parentId: " << GetParentID(tempCluster) <<std::endl;
        }
    }
}

const void EventAnalysis::ShowFirstObject() const
{
    Cube::Handle<Cube::ReconTrack> tempFirstTrack = this->GetFirstObject();
    Cube::Handle<Cube::ReconCluster> tempFirstCluster= this->GetFirstObject();
    if (tempFirstTrack)
    {
        std::cout << "first object, track:   (" << tempFirstTrack->GetPosition().X();
        std::cout << ", " << tempFirstTrack->GetPosition().Y();
        std::cout << ", " << tempFirstTrack->GetPosition().Z();
        std::cout << ", " << tempFirstTrack->GetPosition().T();
        std::cout << ") , pdg: " << GetPdg(tempFirstTrack);
        std::cout << ", parentId: " << GetParentID(tempFirstTrack) <<std::endl;
    }
    else if (tempFirstCluster)
    {
        std::cout << "first object, cluster: (" << tempFirstCluster->GetPosition().X();
        std::cout << ", " << tempFirstCluster->GetPosition().Y();
        std::cout << ", " << tempFirstCluster->GetPosition().Z();
        std::cout << ", " << tempFirstCluster->GetPosition().T();
        std::cout << ") , pdg: " << GetPdg(tempFirstCluster);
        std::cout << ", parentId: " << GetParentID(tempFirstCluster) <<std::endl;
    }
    else
    {
        std::cout << "no first object candidate" << std::endl;
    }
}

const void EventAnalysis::ShowVertex() const
{
    TLorentzVector vertex = this->GetVertex();
    std::cout << "vertex: " << vertex.X()
        << ", " << vertex.Y()
        << ", " << vertex.Z()
        << ", " << vertex.T();
    std::cout << std::endl;
}

const int EventAnalysis::GetParentPdg(int inParentId) const
{
    int tempParentPdg = 0;
    for (Cube::Event::G4TrajectoryContainer::iterator g4Trajectory
            = gEvent->G4Trajectories.begin();
            g4Trajectory != gEvent->G4Trajectories.end();
            ++g4Trajectory) 
    {
        if(g4Trajectory->first == inParentId)
        {
            Cube::Handle<Cube::G4Trajectory> gt = g4Trajectory->second;
            tempParentPdg = gt->GetPDGCode();
            break;
        }
    }
    return tempParentPdg;
}
