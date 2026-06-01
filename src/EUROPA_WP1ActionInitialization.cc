/**
 * @file EUROPA_WP1ActionInitialization.cc
 * @brief Implementation of the ActionInitialization class for the PALLAS PlasmaMLP simulation.
 *
 * This file defines the EUROPA_WP1ActionInitialization class, which is responsible
 * for setting up all user actions required for a Geant4 simulation of the PALLAS PlasmaMLP.
 * It manages the creation and assignment of the following actions:
 *   - Primary generator action
 *   - Run action
 *   - Event action
 *   - Stepping action
 *
 * In multithreaded mode, this class also defines master-thread-specific actions such as RunAction.
 * It stores configuration parameters such as the number of events, number of threads, and output suffix,
 * and provides access to the simulation geometry.
 *
 * This class inherits from G4VUserActionInitialization and overrides Build() and BuildForMaster()
 * to provide thread-aware initialization of user actions.
 *
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @date 2026
 * @copyright EUROPA Project - WP1
 */


#include "EUROPA_WP1ActionInitialization.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Constructor for ActionInitialization class
 * @param suff Suffix string to append to output files
 * @param N Number of events to generate
 * @param Ncores Number of threads to use in multithreaded mode
 * @param pMT Flag indicating whether multithreading is enabled
 * @param geometry Pointer to the simulation geometry
 */
EUROPA_WP1ActionInitialization::EUROPA_WP1ActionInitialization(
    const char *suff, 
    size_t N, 
    size_t Ncores, 
    G4bool pMT, 
    EUROPA_WP1GeometryConstruction* geometry)
    : G4VUserActionInitialization(), 
      suffixe(suff),
      NEventsGenerated(N),
      numThreads(Ncores),
      flag_MT(pMT),
      fGeometry(geometry)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Destructor for ActionInitialization class
 */
EUROPA_WP1ActionInitialization::~EUROPA_WP1ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Build actions for the master thread
 *
 * This function is called in multithreaded mode to define actions
 * that are executed only in the master thread, such as RunAction.
 */
void EUROPA_WP1ActionInitialization::BuildForMaster() const
{
    SetUserAction(new EUROPA_WP1RunAction(suffixe, NEventsGenerated, flag_MT));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Build actions for worker threads or single-threaded runs
 *
 * This function creates and assigns all user actions required for the simulation:
 * - PrimaryGeneratorAction
 * - RunAction
 * - EventAction
 * - SteppingAction
 */
void EUROPA_WP1ActionInitialization::Build() const
{
    // Create primary generator action
    auto *generator = new EUROPA_WP1PrimaryGeneratorAction(NEventsGenerated, numThreads, flag_MT);
    
    // Create run action
    auto *runAction = new EUROPA_WP1RunAction(suffixe, NEventsGenerated, flag_MT);
    
    // Create event action
    auto *eventAction = new EUROPA_WP1EventAction(suffixe);

    // Provide the run action with pointers to the primary generator and geometry
    runAction->SetPrimaryGenerator(generator);
    runAction->SetGeometry(fGeometry);
    
    // Assign user actions to the simulation
    SetUserAction(generator);
    SetUserAction(runAction);
    SetUserAction(eventAction);
    SetUserAction(new EUROPA_WP1SteppingAction());
}
