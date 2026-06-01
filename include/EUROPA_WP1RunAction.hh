#ifndef EUROPA_WP1RunAction_h
#define EUROPA_WP1RunAction_h 1

/**
 * @class EUROPA_WP1RunAction
 * @brief Defines the run-level actions and statistics handling for the PALLAS simulation.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @date 2026
 * @copyright EUROPA Project - WP1
 *
 * This header declares the `EUROPA_WP1RunAction` class, which inherits from 
 * Geant4's `G4UserRunAction` to manage actions at the beginning and end of each run. 
 * It handles:
 *  - Collection and storage of run-wide statistics
 *  - ROOT file and tree creation for data output
 *  - Synchronization in multithreaded runs
 *  - Coordination with primary generator and geometry configuration
 *
 * The associated `RunTallyGlobalInput` struct stores simulation configuration
 * parameters and beamline element settings for the entire run.
 *
 * Data recorded here typically includes:
 *  - Initial beam parameters
 *  - Magnet gradients and lengths
 *  - Detector and collimator configurations
 *  - Collected measurements from various beamline elements
 *
 * @note This class interacts closely with:
 *       - `EUROPA_WP1PrimaryGeneratorAction`
 *       - `EUROPA_WP1GeometryConstruction`
 *       - Event-level statistics classes such as `RunTallyQuadrupoles`
 */

// Include base classes and Geant4 utilities
#include "G4UserRunAction.hh"       // Base class for user-defined run actions
#include "G4Run.hh"                 // Run object for event accumulation
#include "G4UImanager.hh"           // UI manager (for commands)
#include "G4VVisManager.hh"         // Visualization manager
#include "G4RunManager.hh"
#include "TFile.h"                  // ROOT file I/O
#include "TTree.h"
#include "TBranch.h"
#include <mutex>
#include "EUROPA_WP1PrimaryGeneratorAction.hh"
#include "EUROPA_WP1GeometryConstruction.hh"
#include "EUROPA_WP1EventAction.hh" 


// Forward declarations
class EUROPA_WP1PrimaryGeneratorAction;
class EUROPA_WP1GeometryConstruction;

/**
 * @struct RunTallyGlobalInput
 * @brief Stores configuration parameters and beamline settings for a simulation run.
 */
struct RunTallyGlobalInput {
  // --- General run settings ---
  int NEvents = 0;                 

  // --- Physical parameters ---
  float Q1_Length = 0.0;          
  float Q2_Length = 0.0;          
  float Q3_Length = 0.0;          
  float Q4_Length = 0.0;          
  float SourceQ1Distance = 0.0;   
  float Q1Q2Distance = 0.0;       
  float Q2Q3Distance = 0.0;       
  float Q3Q4Distance = 0.0;       

  /**
   * @brief Populate structure from generator and geometry settings.
   * @param gen Pointer to primary generator
   * @param geo Pointer to geometry construction
   * @param nEvents Number of events to simulate
   */
  void FillFrom(const EUROPA_WP1GeometryConstruction* geo,
                int nEvents);
};

/**
 * @class EUROPA_WP1RunAction
 * @brief Manages run-level actions, statistics, and ROOT output.
 */
class EUROPA_WP1RunAction : public G4UserRunAction
{
public:
  /**
   * @brief Constructor
   * @param fileName Base name for ROOT output file
   * @param nEventsGenerated Number of events to process
   * @param multithreaded True if running with multithreading
   */
  EUROPA_WP1RunAction(const char* fileName, size_t nEventsGenerated, G4bool multithreaded);

  /// Destructor
  ~EUROPA_WP1RunAction();

  /// Called at the start of each run
  void BeginOfRunAction(const G4Run* run) override;

  /// Called at the end of each run
  void EndOfRunAction(const G4Run* run) override;

  /// Generic template to update cumulative statistics in a ROOT tree
  template<typename T>
  static void UpdateStatistics(T& stats, const T& newStats, TTree* tree);

  // --- Specific statistics update methods ---
  void UpdateStatisticsGlobalInput(RunTallyGlobalInput);
  void UpdateStatisticsInput(RunTallyInput);
  void UpdateStatisticsHorizontalColl(RunTallyCollimators);
  void UpdateStatisticsVerticalColl(RunTallyCollimators);
  void UpdateStatisticsBSYAG(RunTallyYAG);

  /// Set the primary generator reference
  void SetPrimaryGenerator(EUROPA_WP1PrimaryGeneratorAction* gen);

  /// Set the geometry reference
  void SetGeometry(EUROPA_WP1GeometryConstruction* geom);

private:
  // --- Output configuration ---
  G4String suffixe;     ///< File suffix for ROOT outputs
  G4String fileName;    ///< Base file name for ROOT outputs

  // --- Stored run statistics ---
  RunTallyGlobalInput StatsGlobalInput;
  RunTallyInput StatsInput;
  RunTallyCollimators StatsHorizontalColl;
  RunTallyCollimators StatsVerticalColl;
  RunTallyYAG StatsBSYAG;

  size_t NEventsGenerated; ///< Number of events generated in the run
  G4bool flag_MT;          ///< Multithreading enabled flag

  // --- ROOT file and trees ---
  TFile *f = nullptr;
  TTree *Tree_GlobalInput = nullptr;
  TTree *Tree_Input = nullptr;
  TTree *Tree_HorizontalColl = nullptr;
  TTree *Tree_VerticalColl = nullptr;
  TTree *Tree_BSYAG = nullptr;
  TBranch *RunBranch = nullptr;

  time_t start; ///< Start time of the run

  // --- Thread-safety ---
  static std::atomic<int> activeThreads;
  static G4Mutex fileMutex;

protected:
  EUROPA_WP1PrimaryGeneratorAction* fPrimaryGenerator = nullptr; ///< Primary generator reference
  EUROPA_WP1GeometryConstruction* fGeometry = nullptr; ///< Geometry reference
};

#endif // EUROPA_WP1RunAction_h