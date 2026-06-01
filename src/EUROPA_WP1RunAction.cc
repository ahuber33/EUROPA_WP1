/**
 * @file EUROPA_WP1RunAction.cc
 * @brief Implements run-level setup, data collection, and output handling for the PALLAS simulation.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @date 2026
 * @copyright EUROPA Project - WP1
 *
 * This file contains the method definitions for the `EUROPA_WP1RunAction` class
 * declared in `EUROPA_WP1RunAction.hh`. It manages:
 *  - Initialization of run-wide ROOT files and trees
 *  - Thread-safe data collection in multi-threaded runs
 *  - Branch creation for all recorded statistics
 *  - Interaction with primary generator and geometry to populate run metadata
 *  - Begin/end-of-run hooks to prepare and finalize data storage
 *
 * The run action workflow:
 *  - **BeginOfRunAction**:
 *      - Locks file access (multi-thread safety)
 *      - Builds the ROOT output file name based on threading context
 *      - Creates TTree objects for each statistics category
 *      - Defines ROOT branches for run-wide parameters and measurements
 *      - Initializes the random seed
 *  - **During the run**:
 *      - Updates statistics via `UpdateStatistics()` and specialized variants
 *  - **EndOfRunAction**:
 *      - Finalizes statistics
 *      - Writes all TTrees to the ROOT file
 *      - Closes the file and releases resources
 *
 * Thread safety is ensured via:
 *  - `std::atomic<int> activeThreads` for counting active threads
 *  - `G4Mutex fileMutex` for synchronized file access
 *
 * @note This class uses Geant4's ROOT integration to structure physics
 *       output in an analysis-friendly format.
 */

// Include class header
#include "EUROPA_WP1RunAction.hh"

// --- Static member initialization ---
std::atomic<int> EUROPA_WP1RunAction::activeThreads(0);       ///< Counter for active threads
G4Mutex EUROPA_WP1RunAction::fileMutex = G4MUTEX_INITIALIZER; ///< Mutex for file protection

// --- Constructor ---
EUROPA_WP1RunAction::EUROPA_WP1RunAction(const char *suff, size_t N, G4bool pMT)
    : suffixe(suff), NEventsGenerated(N), flag_MT(pMT)
{
}

// --- Destructor ---
EUROPA_WP1RunAction::~EUROPA_WP1RunAction() {}

// --- Primary generator reference setter ---
void EUROPA_WP1RunAction::SetPrimaryGenerator(EUROPA_WP1PrimaryGeneratorAction *gen)
{
  fPrimaryGenerator = gen;
}

// --- Geometry reference setter ---
void EUROPA_WP1RunAction::SetGeometry(EUROPA_WP1GeometryConstruction *geom)
{
  fGeometry = geom;
}

/**
 * @brief Utility template to create simple branches in a TTree.
 * @tparam T Data type (int, float, etc.)
 * @param tree Pointer to the TTree to populate
 * @param branches List of name-pointer pairs
 */
template <typename T>
static void CreateBranches(TTree *tree, const std::vector<std::pair<const char *, T *>> &branches)
{
  for (const auto &b : branches)
  {
    tree->Branch(
        b.first,
        b.second,
        (std::string(b.first) + "/" +
         (std::is_same<T, int>::value ? "I" : std::is_same<T, float>::value ? "F"
                                                                            : ""))
            .c_str());
  }
}

/**
 * @brief Creates ROOT branches specific to YAG detector statistics.
 * @param tree ROOT tree to populate
 * @param stats YAG statistics structure
 */
static void CreateYAGBranches(TTree *tree, RunTallyYAG &stats)
{
  //tree->Branch("x_exit", "vector<float>", &stats.x_exit);
  //tree->Branch("y_exit", "vector<float>", &stats.y_exit);
  //tree->Branch("z_exit", "vector<float>", &stats.z_exit);
  tree->Branch("parentID", "vector<int>", &stats.parentID);
  //tree->Branch("particleID", "vector<int>", &stats.particleID);
  tree->Branch("energy", "vector<float>", &stats.energy);
  //tree->Branch("deposited_energy", "vector<float>", &stats.total_deposited_energy);
}

/**
 * @brief Creates ROOT branches specific to Collimators statistics.
 * @param tree ROOT tree to populate
 * @param stats Collimators statistics structure
 */
static void CreateCollimatorsBranches(TTree *tree, RunTallyCollimators &stats)
{
  tree->Branch("x_interaction", &stats.x, "x_interaction/F");
  tree->Branch("y_interaction", &stats.y, "y_interaction/F");
  tree->Branch("z_interaction", &stats.z, "z_interaction/F");
  tree->Branch("energy", &stats.energy, "energy/F");
}

//---------------------------------------------------------
//  Generic statistics update function
//---------------------------------------------------------
/**
 * @brief Thread-safe update of statistics and ROOT tree filling.
 * @tparam T Type of the statistics structure
 * @param stats Destination statistics object (persistent in the run)
 * @param newStats Source statistics (new values)
 * @param tree ROOT tree to fill
 */
template <typename T>
void EUROPA_WP1RunAction::UpdateStatistics(T &stats, const T &newStats, TTree *tree)
{
  std::lock_guard<std::mutex> lock(fileMutex);
  stats = newStats;
  if (tree)
    tree->Fill();
  else
    G4cerr << "Error: Tree is nullptr" << G4endl;
}

// --- Specific statistics update wrappers ---
void EUROPA_WP1RunAction::UpdateStatisticsGlobalInput(RunTallyGlobalInput a) { UpdateStatistics(StatsGlobalInput, a, Tree_GlobalInput); }
void EUROPA_WP1RunAction::UpdateStatisticsInput(RunTallyInput a) { UpdateStatistics(StatsInput, a, Tree_Input); }
void EUROPA_WP1RunAction::UpdateStatisticsHorizontalColl(RunTallyCollimators a) { UpdateStatistics(StatsHorizontalColl, a, Tree_HorizontalColl); }
void EUROPA_WP1RunAction::UpdateStatisticsVerticalColl(RunTallyCollimators a) { UpdateStatistics(StatsVerticalColl, a, Tree_VerticalColl); }
void EUROPA_WP1RunAction::UpdateStatisticsBSYAG(RunTallyYAG a) { UpdateStatistics(StatsBSYAG, a, Tree_BSYAG); }

/**
 * @brief Populates global input statistics from generator and geometry state.
 * @param gen Pointer to primary generator
 * @param geo Pointer to geometry construction
 * @param nEvents Total number of events in the run
 */
void RunTallyGlobalInput::FillFrom(const EUROPA_WP1GeometryConstruction *geo,
                                   int nEvents)
{
  // Extract geometry configuration
  if (geo)
  {
    NEvents = nEvents;

    Q1_Length = geo->GetQ1Length();
    Q2_Length = geo->GetQ2Length();
    Q3_Length = geo->GetQ3Length();
    Q4_Length = geo->GetQ4Length();
    SourceQ1Distance = geo->GetSourceQ1Distance();
    Q1Q2Distance = geo->GetQ1Q2Distance();
    Q2Q3Distance = geo->GetQ2Q3Distance();
    Q3Q4Distance = geo->GetQ3Q4Distance();
  }
}

//-----------------------------------------------------
//  BeginOfRunAction
//-----------------------------------------------------
/**
 * @brief Called at the start of each run to set up ROOT output structures and initialize state.
 * @param aRun Pointer to the current G4Run
 */
void EUROPA_WP1RunAction::BeginOfRunAction(const G4Run *aRun)
{
  // Populate branches for each TTree...
  G4AutoLock lock(&fileMutex); // Automatic mutex lock

  start = time(NULL); // start the timer clock to calculate run times

  int a = activeThreads;

  std::string s = flag_MT ? "_" + std::to_string(activeThreads) : "";
  fileName = suffixe + s + ".root";

  G4cout << "Filename = " << fileName << G4endl;

  f = new TFile(fileName.c_str(), "RECREATE");

  // Creating trees for different types of run information
  Tree_GlobalInput = new TTree("GlobalInput", "Global Input Information");                 // Tree to access Input information
  Tree_Input = new TTree("Input", "Input Information");                                    // Tree to access Input information
  Tree_HorizontalColl = new TTree("Horizontal_Coll", "Horizontal Collimator Information"); // Tree to access Horizontal Collimator information
  Tree_VerticalColl = new TTree("Vertical_Coll", "Vertical Collimator Information");       // Tree to access Vertical Collimator information
  Tree_BSYAG = new TTree("BSYAG", "BS YAG Information");                                   // Tree to access Back Collimator infos

  //*****************************INFORMATIONS FROM THE GLOBAL INPUT*******************************************
  std::vector<std::pair<const char *, int *>> globalIntBranches = {
      {"NEvents", &StatsGlobalInput.NEvents}};

  std::vector<std::pair<const char *, float *>> globalFloatBranches = {
      {"Q1_Length", &StatsGlobalInput.Q1_Length},
      {"Q2_Length", &StatsGlobalInput.Q2_Length},
      {"Q3_Length", &StatsGlobalInput.Q3_Length},
      {"Q4_Length", &StatsGlobalInput.Q4_Length},
      {"SourceQ1Distance", &StatsGlobalInput.SourceQ1Distance},
      {"Q1Q2Distance", &StatsGlobalInput.Q1Q2Distance},
      {"Q2Q3Distance", &StatsGlobalInput.Q2Q3Distance},
      {"Q3Q4Distance", &StatsGlobalInput.Q3Q4Distance}};

  CreateBranches(Tree_GlobalInput, globalIntBranches);
  CreateBranches(Tree_GlobalInput, globalFloatBranches);

  //*****************************INFORMATIONS FROM THE INPUT*******************************************
  std::vector<std::pair<const char *, float *>> inputBranches = {
      {"x", &StatsInput.x}, {"xp", &StatsInput.xp}, {"y", &StatsInput.y}, {"yp", &StatsInput.yp}, {"z", &StatsInput.z}, {"zp", &StatsInput.zp}, {"energy", &StatsInput.energy}};
  CreateBranches(Tree_Input, inputBranches);

  
  //************************************INFORMATIONS FROM THE HORIZONTAL COLLIMATOR*****************************************
  CreateCollimatorsBranches(Tree_HorizontalColl, StatsHorizontalColl);

  //************************************INFORMATIONS FROM THE VERTICAL COLLIMATOR*****************************************
  CreateCollimatorsBranches(Tree_VerticalColl, StatsVerticalColl);

  //************************************INFORMATIONS FROM THE YAGs*****************************************
  //************************************INFORMATIONS FROM THE BS YAG*****************************************
  CreateYAGBranches(Tree_BSYAG, StatsBSYAG);

  // set the random seed to the CPU clock
  // G4Random::setTheEngine(new CLHEP::HepJamesRandom);
  G4long seed = time(NULL) + a;
  G4Random::setTheSeed(seed);
  // G4Random::setTheSeed(1712670533);
  G4cout << "seed = " << seed << G4endl;

  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  if (G4VVisManager::GetConcreteInstance())
  {
    G4UImanager *UI = G4UImanager::GetUIpointer();
    UI->ApplyCommand("/vis/scene/notifyHandlers");
  }

  activeThreads++;
}

//-----------------------------------------------------
//  EndOfRunAction
//-----------------------------------------------------
/**
 * @brief Called at the end of each run to write collected data to the ROOT file and clean up.
 * @param aRun Pointer to the current G4Run
 */
void EUROPA_WP1RunAction::EndOfRunAction(const G4Run *aRun)
{
  StatsGlobalInput.FillFrom(fGeometry, NEventsGenerated);
  UpdateStatisticsGlobalInput(StatsGlobalInput);

  G4AutoLock lock(&fileMutex);

  // Write all trees to ROOT file
  f->cd();
  Tree_GlobalInput->Write();
  Tree_Input->Write();
  Tree_HorizontalColl->Write();
  Tree_VerticalColl->Write();
  Tree_BSYAG->Write();
  f->Close();
  delete f;
  f = nullptr;

  if (G4VVisManager::GetConcreteInstance())
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");

  G4cout << "Leaving Run Action" << G4endl;
}
