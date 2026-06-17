/**
 * @file EUROPA_WP1SteppingAction.cc
 * @brief Implements step-level tracking, data extraction, and event updates for the PALLAS simulation.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @date 2026
 * @copyright EUROPA Project - WP1
 *
 * This file contains the method definitions for the `EUROPA_WP1SteppingAction` class
 * declared in `EUROPA_WP1SteppingAction.hh`. It manages:
 *  - Extraction of particle positions, momentum, energy, and track metadata at each Geant4 step
 *  - Handling of quadrupole crossing information for beamline statistics
 *  - Detection of interactions with collimators and updating of corresponding tallies
 *  - Updating of YAG screen statistics including energy deposition and exit positions
 *  - Optional termination of particles when leaving specific volumes or reaching collimators
 *
 * The stepping action workflow:
 *  - **UserSteppingAction**:
 *      - Extracts pre-step and post-step positions and momenta
 *      - Computes kinetic energy and deposited energy per step
 *      - Stores initial beam parameters at first step of primary particles
 *      - Updates quadrupole statistics at entry/exit
 *      - Updates collimator tallies and stops tracks if necessary
 *      - Updates YAG screen tallies and deposited energies
 *      - Kills particles leaving the world volume
 *
 *
 * @note This class relies on Geant4's step action mechanism to record fine-grained
 *       tracking information suitable for later beamline analysis.
 */


#include "EUROPA_WP1SteppingAction.hh"

/**
 * @brief Constructor.
 *
 * Initializes the Geant4 generic messenger and declares user commands
 * for controlling tracking status (global and for collimators).
 */
EUROPA_WP1SteppingAction::EUROPA_WP1SteppingAction()
{
    sMessenger = new G4GenericMessenger(this, "/EUROPA_WP1/step/", "Control commands for my application");

    sMessenger->DeclareProperty("SetTrackingStatus", TrackingStatus)
        .SetGuidance("Enable or disable general tracking.")
        .SetParameterName("TrackingStatus", false)
        .SetDefaultValue("true");

    sMessenger->DeclareProperty("SetTrackingStatusCollimators", TrackingStatusCollimators)
        .SetGuidance("Enable or disable collimator tracking.")
        .SetParameterName("TrackingStatusCollimators", false)
        .SetDefaultValue("true");
}

/**
 * @brief Destructor.
 *
 * Cleans up the allocated messenger.
 */
EUROPA_WP1SteppingAction::~EUROPA_WP1SteppingAction()
{
    delete sMessenger;
}

/**
 * @brief Store the initial beam parameters at the beginning of the event.
 *
 * @param evtac Pointer to the event action where the values are stored.
 */
void EUROPA_WP1SteppingAction::SetInputInformations(EUROPA_WP1EventAction *evtac) const
{
    evtac->SetXStart(preStep.x);
    evtac->SetXpStart(preStep.px);
    evtac->SetYStart(preStep.y);
    evtac->SetYpStart(preStep.py);
    evtac->SetZStart(preStep.z);
    evtac->SetZpStart(preStep.pz);
    evtac->SetEnergyStart(energy);
}

/**
 * @brief Update collimator interaction information in the tally.
 *
 * @param tally Collimator tally to update.
 * @param x Interaction X position [mm].
 * @param y Interaction Y position [mm].
 * @param z Interaction Z position [mm].
 * @param energy Kinetic energy at interaction [MeV].
 */
void UpdateConverter(RunTallyConverter &tally, G4float x, G4float y, G4float z, G4float energy)
{
    tally.SetXInteraction(x);
    tally.SetYInteraction(y);
    tally.SetZInteraction(z);
    tally.SetEnergy(energy);
    tally.ActiveFlag();
}

/**
 * @brief Update YAG (Yttrium Aluminum Garnet screen) tally with particle data.
 *
 * @param tally YAG tally to update.
 * @param x Exit X position [mm].
 * @param y Exit Y position [mm].
 * @param z Exit Z position [mm].
 * @param energy Particle kinetic energy [MeV].
 */
void UpdateCible(RunTallyCible &tally, G4float x, G4float y, G4float z, G4float energy)
{
    tally.AddXCreation(x);
    tally.AddYCreation(y);
    tally.AddZCreation(z);
    tally.AddEnergy(energy);
}

/**
 * @brief Main Geant4 stepping action executed at each simulation step.
 *
 * Collects track and step information (positions, momenta, energies, volumes).
 * Manages input beam initialization, quadrupole crossing, collimator detection,
 * YAG screens, and particle termination.
 *
 * @param aStep Pointer to the current Geant4 step.
 */
void EUROPA_WP1SteppingAction::UserSteppingAction(const G4Step *aStep)
{
    // --- Preparation of variables ---
    auto evtac  = static_cast<EUROPA_WP1EventAction *>(G4EventManager::GetEventManager()->GetUserEventAction());
    theTrack    = aStep->GetTrack();
    auto pre    = aStep->GetPreStepPoint();
    auto post   = aStep->GetPostStepPoint();

    // General info
    particleName   = theTrack->GetDefinition()->GetParticleName();
    particleID     = theTrack->GetDefinition()->GetPDGEncoding();
    endproc        = post->GetProcessDefinedStep()->GetProcessName();
    parentID       = theTrack->GetParentID();
    trackID        = theTrack->GetTrackID();
    stepNo         = theTrack->GetCurrentStepNumber();
    energy         = pre->GetKineticEnergy() / MeV;
    energyDeposited= aStep->GetTotalEnergyDeposit() / CLHEP::keV;

    // Positions
    auto prePos  = pre->GetPosition()  / CLHEP::mm;
    auto postPos = post->GetPosition() / CLHEP::mm;
    preStep.x = prePos.x();  preStep.y = prePos.y();  preStep.z = prePos.z();
    postStep.x = postPos.x(); postStep.y = postPos.y(); postStep.z = postPos.z();

    // Directions
    auto preMom  = pre->GetMomentumDirection();
    auto postMom = post->GetMomentumDirection();
    preStep.px = preMom.x();  preStep.py = preMom.y();  preStep.pz = preMom.z();
    postStep.px = postMom.x(); postStep.py = postMom.y(); postStep.pz = postMom.z();

    // Volumes
    volumeNamePreStep  = pre->GetPhysicalVolume()->GetName();
    volumeNamePostStep = post->GetPhysicalVolume()->GetName();

    pd = theTrack->GetDefinition();
    auto ion = dynamic_cast<G4Ions*>(pd);

    // --- Begin main logic ---

    // Initial beam info (step 1, primary particle only)
    if (parentID == 0 && stepNo == 1)
        SetInputInformations(evtac);


    // Collimators
    if (parentID == 0)
    {
        if (volumeNamePostStep == "Converter")
        {
            UpdateConverter(evtac->GetConverter(), postStep.x, postStep.y, postStep.z, energy);
        }
    }

    if (ion)
    {
        Z = ion->GetAtomicNumber();
        A = ion->GetAtomicMass();

        if (volumeNamePostStep == "Cible"
        && Z == 42
        && A == 99
        && stepNo ==1
        && theTrack->GetCreatorProcess()->GetProcessName() == "photonNuclear")
        {
            //G4cout << "Creation 99Mo" << G4endl;
            //G4cout << "Creator process = " << theTrack->GetCreatorProcess()->GetProcessName() << G4endl;
            UpdateCible(evtac->GetCible(), postStep.x, postStep.y, postStep.z, energy);
        }
    }
    


    // Kill particles leaving the world
    if (volumeNamePostStep == "World")
        theTrack->SetTrackStatus(fStopAndKill);

        //G4cout << "y = " << prePos.y() << G4endl;
}
