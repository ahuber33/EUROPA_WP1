/**
 * @file EUROPA_WP1EventAction.cc
 * @brief Implementation of the EventAction class for the PALLAS PlasmaMLP simulation.
 *
 * This file defines the EUROPA_WP1EventAction class, which handles per-event
 * actions in the Geant4 simulation of the PALLAS PlasmaMLP setup.
 * 
 * The responsibilities of this class include:
 *  - Resetting all per-event statistics at the beginning of an event.
 *  - Collecting input, collimator, beam stop, YAG detector, and quadrupole statistics during the event.
 *  - Passing per-event statistics to the run-level action (EUROPA_WP1RunAction) at the end of the event.
 *
 * The class works in conjunction with:
 *  - EUROPA_WP1SteppingAction: for per-step updates within an event.
 *  - EUROPA_WP1RunAction: to accumulate run-level statistics.
 *
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @date 2026
 * @copyright EUROPA Project - WP1
 */


#include "EUROPA_WP1SteppingAction.hh"    ///< Stepping action header (per-step updates)
#include "EUROPA_WP1EventAction.hh"       ///< Event action header
#include "EUROPA_WP1RunAction.hh"         ///< Run action header (for statistics accumulation)

/**
 * @brief Constructor for EUROPA_WP1EventAction
 * @param suff Suffix used for naming ROOT branches or output files
 *
 * Initializes the event action and stores the provided suffix.
 */
EUROPA_WP1EventAction::EUROPA_WP1EventAction(const char *suff) 
    : suffixe(suff) 
{}

/**
 * @brief Destructor for EUROPA_WP1EventAction
 *
 * Cleans up any resources used by the event action.
 */
EUROPA_WP1EventAction::~EUROPA_WP1EventAction() {}

/**
 * @brief Called at the beginning of each event
 * @param evt Pointer to the current G4Event
 *
 * Resets all per-event statistics and counters to initial empty states.
 * This includes:
 * - Input-related counters
 * - Horizontal and vertical collimator statistics
 * - Beam Stop (BS) and BSPEC YAG detector statistics
 * - Quadrupole statistics
 */
void EUROPA_WP1EventAction::BeginOfEventAction(const G4Event *evt)
{
    /** Reset input statistics */
    StatsInput = {};

    /** Reset horizontal and vertical collimator statistics */
    StatsHorizontalColl = {};
    StatsVerticalColl = {};

    /** Reset Beam Stop (BS) and BSPEC YAG detector statistics */
    StatsBSYAG = {};

}

/**
 * @brief Called at the end of each event
 * @param evt Pointer to the current G4Event
 *
 * Updates run-level statistics by passing the per-event data to the
 * EUROPA_WP1RunAction. Only non-empty or relevant data are updated
 * for input, BS YAG, and BSPEC YAG statistics, while quadrupole and
 * collimator statistics are always updated.
 */
void EUROPA_WP1EventAction::EndOfEventAction(const G4Event *evt)
{
    /** Get pointer to current run action */
    EUROPA_WP1RunAction *runac = 
        (EUROPA_WP1RunAction *)(G4RunManager::GetRunManager()->GetUserRunAction());

    /** Update input energy statistics if valid */
    if (StatsInput.energy > 0) 
        //runac->UpdateStatisticsInput(StatsInput);

    /** Update Beam Stop YAG statistics if not empty */
    if (!StatsBSYAG.energy.empty()) 
        runac->UpdateStatisticsBSYAG(StatsBSYAG);

    /** Always update quadrupole and collimator statistics */
    //runac->UpdateStatisticsQuadrupoles(StatsQuadrupoles);

    if(StatsHorizontalColl.GetFlag()==true)
    {
      //runac->UpdateStatisticsHorizontalColl(StatsHorizontalColl);
      StatsHorizontalColl.ResetFlag();
    }

    if(StatsVerticalColl.GetFlag()==true)
    {
      //runac->UpdateStatisticsVerticalColl(StatsVerticalColl);
      StatsVerticalColl.ResetFlag();
    }

}
