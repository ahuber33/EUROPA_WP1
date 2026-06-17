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
     * @brief Command to set the Spectrum Temperature.
     *
     * Parameter: SpectrumTemperature (double) unit
     */
    fGeometrySpectrumTemperatureCmd = new G4UIcmdWithADoubleAndUnit("/EUROPA_WP1/geometry/setSpectrumTemperature", this);
    fGeometrySpectrumTemperatureCmd->SetGuidance("Set Spectrum Temperature");
    fGeometrySpectrumTemperatureCmd->SetParameterName("SpectrumTemperature", false);
    //fGeometrySpectrumTemperatureCmd->SetRange("SpectrumTemperature>3. && SpectrumTemperature<300.");
    fGeometrySpectrumTemperatureCmd->SetUnitCategory("Energy");
    fGeometrySpectrumTemperatureCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometrySpectrumTemperatureCmd->SetToBeBroadcasted(false);


    /**
     * @brief Command to set the Spectrum Temperature.
     *
     * Parameter: SpectrumTemperature (double) unit
     */
    fGeometrySpectrumEcutCmd = new G4UIcmdWithADoubleAndUnit("/EUROPA_WP1/geometry/setSpectrumEcut", this);
    fGeometrySpectrumEcutCmd->SetGuidance("Set Spectrum Ecut");
    fGeometrySpectrumEcutCmd->SetParameterName("SpectrumEcut", false);
    fGeometrySpectrumEcutCmd->SetRange("SpectrumEcut>3. && SpectrumEcut<600.");
    fGeometrySpectrumEcutCmd->SetUnitCategory("Energy");
    fGeometrySpectrumEcutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometrySpectrumEcutCmd->SetToBeBroadcasted(false);


    /**
     * @brief Command to set the Convertor Material.
     *
     * Parameter: ConvertorMaterial (string)
     */
    fGeometryConvertorMaterialCmd = new G4UIcmdWithAString("/EUROPA_WP1/geometry/setConvertorMaterial", this);
    fGeometryConvertorMaterialCmd->SetGuidance("Set Convertor Material");
    fGeometryConvertorMaterialCmd->SetParameterName("ConvertorMaterial", false);
    fGeometryConvertorMaterialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryConvertorMaterialCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Convertor Radius.
     *
     * Parameter: ConvertorRadius (double) unit
     */
    fGeometryConvertorRadiusCmd = new G4UIcmdWithADoubleAndUnit("/EUROPA_WP1/geometry/setConvertorRadius", this);
    fGeometryConvertorRadiusCmd->SetGuidance("Set Convertor Radius");
    fGeometryConvertorRadiusCmd->SetParameterName("ConvertorRadius", false);
    fGeometryConvertorRadiusCmd->SetRange("ConvertorRadius>0.");
    fGeometryConvertorRadiusCmd->SetUnitCategory("Length");
    fGeometryConvertorRadiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryConvertorRadiusCmd->SetToBeBroadcasted(false);


    /**
     * @brief Command to set the Convertor Thickness.
     *
     * Parameter: ConvertorThickness (double) unit
     */
    fGeometryConvertorThicknessCmd = new G4UIcmdWithADoubleAndUnit("/EUROPA_WP1/geometry/setConvertorThickness", this);
    fGeometryConvertorThicknessCmd->SetGuidance("Set Convertor Thickness");
    fGeometryConvertorThicknessCmd->SetParameterName("ConvertorThickness", false);
    fGeometryConvertorThicknessCmd->SetRange("ConvertorThickness>0.");
    fGeometryConvertorThicknessCmd->SetUnitCategory("Length");
    fGeometryConvertorThicknessCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryConvertorThicknessCmd->SetToBeBroadcasted(false);


    /**
     * @brief Command to set the Cible Radius.
     *
     * Parameter: CibleRadius (double) unit
     */
    fGeometryCibleRadiusCmd = new G4UIcmdWithADoubleAndUnit("/EUROPA_WP1/geometry/setCibleRadius", this);
    fGeometryCibleRadiusCmd->SetGuidance("Set Cible Radius");
    fGeometryCibleRadiusCmd->SetParameterName("CibleRadius", false);
    fGeometryCibleRadiusCmd->SetRange("CibleRadius>0.");
    fGeometryCibleRadiusCmd->SetUnitCategory("Length");
    fGeometryCibleRadiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryCibleRadiusCmd->SetToBeBroadcasted(false);


    /**
     * @brief Command to set the Cible Thickness.
     *
     * Parameter: CibleThickness (double) unit
     */
    fGeometryCibleThicknessCmd = new G4UIcmdWithADoubleAndUnit("/EUROPA_WP1/geometry/setCibleThickness", this);
    fGeometryCibleThicknessCmd->SetGuidance("Set Cible Thickness");
    fGeometryCibleThicknessCmd->SetParameterName("CibleThickness", false);
    fGeometryCibleThicknessCmd->SetRange("CibleThickness>0.");
    fGeometryCibleThicknessCmd->SetUnitCategory("Length");
    fGeometryCibleThicknessCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryCibleThicknessCmd->SetToBeBroadcasted(false);


    /**
     * @brief Command to set the Cible Density Fraction.
     *
     * Parameter: CibleDensityFraction (double)
     */
    fGeometryCibleDensityFractionCmd = new G4UIcmdWithADouble("/EUROPA_WP1/geometry/setCibleDensityFraction", this);
    fGeometryCibleDensityFractionCmd->SetGuidance("Set Cible Density Fraction");
    fGeometryCibleDensityFractionCmd->SetParameterName("CibleDensityFraction", false);
    fGeometryCibleDensityFractionCmd->SetRange("CibleDensityFraction >0 && CibleDensityFraction <=1.");
    fGeometryCibleDensityFractionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryCibleDensityFractionCmd->SetToBeBroadcasted(false);

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
    delete fGeometrySpectrumTemperatureCmd;
    delete fGeometrySpectrumEcutCmd;
    delete fGeometryConvertorMaterialCmd;
    delete fGeometryConvertorRadiusCmd;
    delete fGeometryConvertorThicknessCmd;
    delete fGeometryCibleRadiusCmd;
    delete fGeometryCibleThicknessCmd;
    delete fGeometryCibleDensityFractionCmd;
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
    else if (aCommand == fGeometrySpectrumTemperatureCmd)
    {
        fGeometry->SetSpectrumTemperature(fGeometrySpectrumTemperatureCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometrySpectrumEcutCmd)
    {
        fGeometry->SetSpectrumEcut(fGeometrySpectrumEcutCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryConvertorMaterialCmd)
    {
        fGeometry->SetConvertorMaterial(aNewValue);
    }
    else if (aCommand == fGeometryConvertorRadiusCmd)
    {
        fGeometry->SetConvertorRadius(fGeometryConvertorRadiusCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryConvertorThicknessCmd)
    {
        fGeometry->SetConvertorThickness(fGeometryConvertorThicknessCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryCibleRadiusCmd)
    {
        fGeometry->SetCibleRadius(fGeometryCibleRadiusCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryCibleThicknessCmd)
    {
        fGeometry->SetCibleThickness(fGeometryCibleThicknessCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryCibleDensityFractionCmd)
    {
        fGeometry->SetCibleDensityFraction(fGeometryCibleDensityFractionCmd->GetNewDoubleValue(aNewValue));
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

    if (aCommand == fGeometrySpectrumTemperatureCmd)
    {
        cv = fGeometrySpectrumTemperatureCmd->ConvertToString(fGeometry->GetSpectrumTemperature(), "MeV");
    }
    else if (aCommand == fGeometrySpectrumEcutCmd)
    {
        cv = fGeometrySpectrumEcutCmd->ConvertToString(fGeometry->GetSpectrumEcut(), "MeV");
    }
    else if (aCommand == fGeometryConvertorMaterialCmd)
    {
        cv = fGeometry->GetConvertorMaterial();
    }
    else if (aCommand == fGeometryConvertorRadiusCmd)
    {
        cv = fGeometryConvertorRadiusCmd->ConvertToString(fGeometry->GetConvertorRadius(), "mm");
    }
    else if (aCommand == fGeometryConvertorThicknessCmd)
    {
        cv = fGeometryConvertorThicknessCmd->ConvertToString(fGeometry->GetConvertorThickness(), "mm");
    }
    else if (aCommand == fGeometryCibleRadiusCmd)
    {
        cv = fGeometryCibleRadiusCmd->ConvertToString(fGeometry->GetCibleRadius(), "mm");
    }
    else if (aCommand == fGeometryCibleThicknessCmd)
    {
        cv = fGeometryCibleThicknessCmd->ConvertToString(fGeometry->GetCibleThickness(), "mm");
    }
    else if (aCommand == fGeometryCibleDensityFractionCmd)
    {
        cv = fGeometryCibleDensityFractionCmd->ConvertToString(fGeometry->GetCibleDensityFraction());
    }
    return cv;
}
