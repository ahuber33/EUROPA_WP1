#include "EUROPA_WP1GeometryMessenger.hh"

/**
 * @file EUROPA_WP1GeometryMessenger.cc
 * @brief Implementation of the GeometryMessenger class for the PALLAS PlasmaMLP simulation.
 *
 * This file defines the EUROPA_WP1GeometryMessenger class, which inherits from G4UImessenger.
 * It provides an interface for the Geant4 UI to control geometry and field parameters 
 * for the EUROPA_WP1 simulation.
 *
 * Responsibilities include:
 *  - Creating UI directories and commands for geometry, display, and magnetic field controls.
 *  - Enabling/disabling visualization of geometry components (full geometry, quadrupoles, collimators).
 *  - Setting geometry parameters such as quadrupole lengths and distances between elements.
 *  - Setting magnetic field gradients and dipole field options.
 *  - Passing user-specified values to the EUROPA_WP1GeometryConstruction class.
 *
 * Each UI command is linked to a setter function in the geometry construction to update
 * the simulation configuration dynamically.
 *
 * The messenger also provides functions to:
 *  - Retrieve current command values for UI display.
 *  - Delete all UI commands upon destruction to free memory.
 *
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @date 2026
 * @copyright EUROPA Project - WP1
 */


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Constructor
 * @param aDetector Pointer to the EUROPA_WP1 geometry construction class
 *
 * This constructor initializes all UI commands and directories for geometry, display, and field control.
 */
EUROPA_WP1GeometryMessenger::EUROPA_WP1GeometryMessenger(EUROPA_WP1GeometryConstruction *aDetector)
    : G4UImessenger(), fGeometry(aDetector)
{
    // Create a main directory for all UI commands related to EUROPA_WP1
    fExampleDir = new G4UIdirectory("/EUROPA_WP1/");
    fExampleDir->SetGuidance("UI commands specific to this example");

    // Create sub-directory for geometry-related commands
    fGeometryDir = new G4UIdirectory("/EUROPA_WP1/geometry/");
    fGeometryDir->SetGuidance("Geometry construction UI commands");

    // Command to print current geometry and field settings
    fPrintCmd = new G4UIcmdWithoutParameter("/EUROPA_WP1/print", this);
    fPrintCmd->SetGuidance("Print current settings.");


    //=====================================
    // Geometry Commands
    //=====================================

    /**
     * @brief Command to set the Q1 Length.
     *
     * Parameter: Q1Length (double) unit
     */
    fGeometryQ1LengthCmd = new G4UIcmdWithADoubleAndUnit("/EUROPA_WP1/geometry/setQ1Length", this);
    fGeometryQ1LengthCmd->SetGuidance("Set Q1 Length");
    fGeometryQ1LengthCmd->SetParameterName("Q1Length", false);
    fGeometryQ1LengthCmd->SetRange("Q1Length>0.");
    fGeometryQ1LengthCmd->SetUnitCategory("Length");
    fGeometryQ1LengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryQ1LengthCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Q2 Length.
     *
     * Parameter: Q2Length (double) unit
     */
    fGeometryQ2LengthCmd = new G4UIcmdWithADoubleAndUnit("/EUROPA_WP1/geometry/setQ2Length", this);
    fGeometryQ2LengthCmd->SetGuidance("Set Q2 Length");
    fGeometryQ2LengthCmd->SetParameterName("Q2Length", false);
    fGeometryQ2LengthCmd->SetRange("Q2Length>0.");
    fGeometryQ2LengthCmd->SetUnitCategory("Length");
    fGeometryQ2LengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryQ2LengthCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Q3 Length.
     *
     * Parameter: Q3Length (double) unit
     */
    fGeometryQ3LengthCmd = new G4UIcmdWithADoubleAndUnit("/EUROPA_WP1/geometry/setQ3Length", this);
    fGeometryQ3LengthCmd->SetGuidance("Set Q3 Length");
    fGeometryQ3LengthCmd->SetParameterName("Q3Length", false);
    fGeometryQ3LengthCmd->SetRange("Q3Length>0.");
    fGeometryQ3LengthCmd->SetUnitCategory("Length");
    fGeometryQ3LengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryQ3LengthCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Q4 Length.
     *
     * Parameter: Q4Length (double) unit
     */
    fGeometryQ4LengthCmd = new G4UIcmdWithADoubleAndUnit("/EUROPA_WP1/geometry/setQ4Length", this);
    fGeometryQ4LengthCmd->SetGuidance("Set Q4 Length");
    fGeometryQ4LengthCmd->SetParameterName("Q4Length", false);
    fGeometryQ4LengthCmd->SetRange("Q4Length>0.");
    fGeometryQ4LengthCmd->SetUnitCategory("Length");
    fGeometryQ4LengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryQ4LengthCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Source - Q1 Distance.
     *
     * Parameter: SourceQ1Distance (double) unit
     */
    fGeometrySourceQ1DistanceCmd = new G4UIcmdWithADoubleAndUnit("/EUROPA_WP1/geometry/setSourceQ1Distance", this);
    fGeometrySourceQ1DistanceCmd->SetGuidance("Set Source -- Q1 Distance");
    fGeometrySourceQ1DistanceCmd->SetParameterName("SourceQ1Distance", false);
    fGeometrySourceQ1DistanceCmd->SetRange("SourceQ1Distance>0.");
    fGeometrySourceQ1DistanceCmd->SetUnitCategory("Length");
    fGeometrySourceQ1DistanceCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometrySourceQ1DistanceCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Q1 - Q2 Distance.
     *
     * Parameter: Q1Q2Distance (double) unit
     */
    fGeometryQ1Q2DistanceCmd = new G4UIcmdWithADoubleAndUnit("/EUROPA_WP1/geometry/setQ1Q2Distance", this);
    fGeometryQ1Q2DistanceCmd->SetGuidance("Set Q1 -- Q2 Distance");
    fGeometryQ1Q2DistanceCmd->SetParameterName("Q1Q2Distance", false);
    fGeometryQ1Q2DistanceCmd->SetRange("Q1Q2Distance>0.");
    fGeometryQ1Q2DistanceCmd->SetUnitCategory("Length");
    fGeometryQ1Q2DistanceCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryQ1Q2DistanceCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Q2 - Q3 Distance.
     *
     * Parameter: Q2Q3Distance (double) unit
     */
    fGeometryQ2Q3DistanceCmd = new G4UIcmdWithADoubleAndUnit("/EUROPA_WP1/geometry/setQ2Q3Distance", this);
    fGeometryQ2Q3DistanceCmd->SetGuidance("Set Q2 -- Q3 Distance");
    fGeometryQ2Q3DistanceCmd->SetParameterName("Q2Q3Distance", false);
    fGeometryQ2Q3DistanceCmd->SetRange("Q2Q3Distance>0.");
    fGeometryQ2Q3DistanceCmd->SetUnitCategory("Length");
    fGeometryQ2Q3DistanceCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryQ2Q3DistanceCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Q3 - Q4 Distance.
     *
     * Parameter: Q3Q4Distance (double) unit
     */
    fGeometryQ3Q4DistanceCmd = new G4UIcmdWithADoubleAndUnit("/EUROPA_WP1/geometry/setQ3Q4Distance", this);
    fGeometryQ3Q4DistanceCmd->SetGuidance("Set Q3 -- Q4 Distance");
    fGeometryQ3Q4DistanceCmd->SetParameterName("Q3Q4Distance", false);
    fGeometryQ3Q4DistanceCmd->SetRange("Q3Q4Distance>0.");
    fGeometryQ3Q4DistanceCmd->SetUnitCategory("Length");
    fGeometryQ3Q4DistanceCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryQ3Q4DistanceCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Source - Collimators Distance.
     *
     * Parameter: SourceCollimatorsDistance (double) unit
     */
    fGeometrySourceCollimatorsDistanceCmd = new G4UIcmdWithADoubleAndUnit("/EUROPA_WP1/geometry/setSourceCollimatorsDistance", this);
    fGeometrySourceCollimatorsDistanceCmd->SetGuidance("Set Source -- Collimators Distance");
    fGeometrySourceCollimatorsDistanceCmd->SetParameterName("SourceCollimatorsDistance", false);
    fGeometrySourceCollimatorsDistanceCmd->SetRange("SourceCollimatorsDistance>0.");
    fGeometrySourceCollimatorsDistanceCmd->SetUnitCategory("Length");
    fGeometrySourceCollimatorsDistanceCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometrySourceCollimatorsDistanceCmd->SetToBeBroadcasted(false);


    /**
     * @brief Command to set the slit opening of Vertical Collimators.
     *
     * Parameter: Vertical Slit opening (double) unit
     */
    fGeometryVerticalSlitOpeningCmd = new G4UIcmdWithADoubleAndUnit("/EUROPA_WP1/geometry/setVerticalSlitOpening", this);
    fGeometryVerticalSlitOpeningCmd->SetGuidance("Set Vertical Slit Opening");
    fGeometryVerticalSlitOpeningCmd->SetParameterName("VerticalSlitOpening", false);
    fGeometryVerticalSlitOpeningCmd->SetRange("VerticalSlitOpening>0.");
    fGeometryVerticalSlitOpeningCmd->SetUnitCategory("Length");
    fGeometryVerticalSlitOpeningCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryVerticalSlitOpeningCmd->SetToBeBroadcasted(false);


    /**
     * @brief Command to set the slit opening of Horizontal Collimators.
     *
     * Parameter: Horizontal Slit opening (double) unit
     */
    fGeometryHorizontalSlitOpeningCmd = new G4UIcmdWithADoubleAndUnit("/EUROPA_WP1/geometry/setHorizontalSlitOpening", this);
    fGeometryHorizontalSlitOpeningCmd->SetGuidance("Set Horizontal Slit Opening");
    fGeometryHorizontalSlitOpeningCmd->SetParameterName("HorizontalSlitOpening", false);
    fGeometryHorizontalSlitOpeningCmd->SetRange("HorizontalSlitOpening>0.");
    fGeometryHorizontalSlitOpeningCmd->SetUnitCategory("Length");
    fGeometryHorizontalSlitOpeningCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryHorizontalSlitOpeningCmd->SetToBeBroadcasted(false);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Destructor.
 *
 * Deletes all UI commands to free memory.
 */
EUROPA_WP1GeometryMessenger::~EUROPA_WP1GeometryMessenger()
{
    delete fPrintCmd;
    delete fGeometryQ1LengthCmd;
    delete fGeometryQ2LengthCmd;
    delete fGeometryQ3LengthCmd;
    delete fGeometryQ4LengthCmd;
    delete fGeometrySourceQ1DistanceCmd;
    delete fGeometryQ1Q2DistanceCmd;
    delete fGeometryQ2Q3DistanceCmd;
    delete fGeometryQ3Q4DistanceCmd;
    delete fGeometrySourceCollimatorsDistanceCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Apply a new value from the UI command.
 * @param aCommand Pointer to the UI command used.
 * @param aNewValue New value as string from the UI.
 *
 * Converts the string to the appropriate type and sets it in the
 * geometry construction.
 */
void EUROPA_WP1GeometryMessenger::SetNewValue(G4UIcommand *aCommand, G4String aNewValue)
{
    if (aCommand == fPrintCmd)
    {
        fGeometry->Print();
    }
    else if (aCommand == fGeometryQ1LengthCmd)
    {
        fGeometry->SetQ1Length(fGeometryQ1LengthCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryQ2LengthCmd)
    {
        fGeometry->SetQ2Length(fGeometryQ2LengthCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryQ3LengthCmd)
    {
        fGeometry->SetQ3Length(fGeometryQ3LengthCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryQ4LengthCmd)
    {
        fGeometry->SetQ4Length(fGeometryQ4LengthCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometrySourceQ1DistanceCmd)
    {
        fGeometry->SetSourceQ1Distance(fGeometrySourceQ1DistanceCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryQ1Q2DistanceCmd)
    {
        fGeometry->SetQ1Q2Distance(fGeometryQ1Q2DistanceCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryQ2Q3DistanceCmd)
    {
        fGeometry->SetQ2Q3Distance(fGeometryQ2Q3DistanceCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryQ3Q4DistanceCmd)
    {
        fGeometry->SetQ3Q4Distance(fGeometryQ3Q4DistanceCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometrySourceCollimatorsDistanceCmd)
    {
        fGeometry->SetSourceCollimatorsDistance(fGeometrySourceCollimatorsDistanceCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryVerticalSlitOpeningCmd)
    {
        fGeometry->SetVerticalSlitOpening(fGeometryVerticalSlitOpeningCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryHorizontalSlitOpeningCmd)
    {
        fGeometry->SetHorizontalSlitOpening(fGeometryHorizontalSlitOpeningCmd->GetNewDoubleValue(aNewValue));
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Get the current value of a UI command.
 * @param aCommand Pointer to the UI command.
 * @return Current value of the command as a string.
 *
 * Retrieves the current value from the geometry construction and
 * converts it to a string for display in the UI.
 */
G4String EUROPA_WP1GeometryMessenger::GetCurrentValue(G4UIcommand *aCommand)
{
    G4String cv;

    if (aCommand == fGeometryQ1LengthCmd)
    {
        cv = fGeometryQ1LengthCmd->ConvertToString(fGeometry->GetQ1Length(), "m");
    }
        else if (aCommand == fGeometryQ2LengthCmd)
    {
        cv = fGeometryQ2LengthCmd->ConvertToString(fGeometry->GetQ2Length(), "m");
    }
        else if (aCommand == fGeometryQ3LengthCmd)
    {
        cv = fGeometryQ3LengthCmd->ConvertToString(fGeometry->GetQ3Length(), "m");
    }
        else if (aCommand == fGeometryQ4LengthCmd)
    {
        cv = fGeometryQ4LengthCmd->ConvertToString(fGeometry->GetQ4Length(), "m");
    }
    else if (aCommand == fGeometrySourceQ1DistanceCmd)
    {
        cv = fGeometrySourceQ1DistanceCmd->ConvertToString(fGeometry->GetSourceQ1Distance(), "m");
    }
    else if (aCommand == fGeometryQ1Q2DistanceCmd)
    {
        cv = fGeometryQ1Q2DistanceCmd->ConvertToString(fGeometry->GetQ1Q2Distance(), "m");
    }
    else if (aCommand == fGeometryQ2Q3DistanceCmd)
    {
        cv = fGeometryQ2Q3DistanceCmd->ConvertToString(fGeometry->GetQ2Q3Distance(), "m");
    }
    else if (aCommand == fGeometryQ3Q4DistanceCmd)
    {
        cv = fGeometryQ3Q4DistanceCmd->ConvertToString(fGeometry->GetQ3Q4Distance(), "m");
    }
    else if (aCommand == fGeometrySourceCollimatorsDistanceCmd)
    {
        cv = fGeometrySourceCollimatorsDistanceCmd->ConvertToString(fGeometry->GetSourceCollimatorsDistance(), "m");
    }
    else if (aCommand == fGeometryVerticalSlitOpeningCmd)
    {
        cv = fGeometryVerticalSlitOpeningCmd->ConvertToString(fGeometry->GetVerticalSlitOpening(), "m");
    }
    else if (aCommand == fGeometryHorizontalSlitOpeningCmd)
    {
        cv = fGeometryHorizontalSlitOpeningCmd->ConvertToString(fGeometry->GetHorizontalSlitOpening(), "m");
    }
    return cv;
}
