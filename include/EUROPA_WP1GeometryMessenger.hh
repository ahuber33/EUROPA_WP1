#ifndef EUROPA_WP1GeometryMessenger_H
#define EUROPA_WP1GeometryMessenger_H

/**
 * @class EUROPA_WP1GeometryMessenger
 * @brief UI messenger for the Geometry constrcution in PALLAS simulation
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @date 2026
 * @copyright EUROPA Project - WP1
 * 
 * Provides UI commands to setup detector and readout geometry (prior to
 * initialization). Length, distance, gradients and display can be changed.
 */

#include "G4UIcmdWithADoubleAndUnit.hh"          // for G4UIcmdWithADoubleAndUnit
#include "G4UIcmdWithADouble.hh"          // for G4UIcmdWithADouble
#include "G4UIcmdWithAString.hh"          // for G4UIcmdWithADouble
#include "G4UIcmdWithAnInteger.hh"               // for G4UIcmdWithAnInteger
#include "G4UIcmdWithoutParameter.hh"            // for G4UIcmdWithoutParameter
#include "G4UIdirectory.hh"                      // for G4UIdirectory
#include "EUROPA_WP1GeometryConstruction.hh" // for EUROPA_WP1GeometryConstruction

class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;
class G4UIdirectory;
class EUROPA_WP1GeometryConstruction;

class EUROPA_WP1GeometryMessenger : public G4UImessenger
{
public:
    EUROPA_WP1GeometryMessenger(EUROPA_WP1GeometryConstruction *);
    ~EUROPA_WP1GeometryMessenger();

    /// Invokes appropriate methods based on the typed command
    virtual void SetNewValue(G4UIcommand *, G4String) final;
    /// Retrieves the current settings
    virtual G4String GetCurrentValue(G4UIcommand *) final;

private:
    /// Detector construction to setup
    EUROPA_WP1GeometryConstruction *fGeometry = nullptr;
    /// Command to set the directory common to all messengers in this example
    /// /Par04
    G4UIdirectory *fExampleDir = nullptr;
    /// Command to set the directory for detector settings /EUROPA_WP1/geometry
    G4UIdirectory *fGeometryDir = nullptr;
    /// Command printing current settings
    G4UIcmdWithoutParameter *fPrintCmd;


    ///GEOMETRY
    /// Command to set the Spectrum Temperature
    G4UIcmdWithADoubleAndUnit *fGeometrySpectrumTemperatureCmd = nullptr;
    /// Command to set the Spectrum Ecut
    G4UIcmdWithADoubleAndUnit *fGeometrySpectrumEcutCmd = nullptr;
    /// Command to set the Convertor Material
    G4UIcmdWithAString *fGeometryConvertorMaterialCmd = nullptr;
    /// Command to set the Convertor Radius
    G4UIcmdWithADoubleAndUnit *fGeometryConvertorRadiusCmd = nullptr;
    /// Command to set the Convertor Thickness
    G4UIcmdWithADoubleAndUnit *fGeometryConvertorThicknessCmd = nullptr;
    /// Command to set the Cible Radius
    G4UIcmdWithADoubleAndUnit *fGeometryCibleRadiusCmd = nullptr;
    /// Command to set the Cible Thickness
    G4UIcmdWithADoubleAndUnit *fGeometryCibleThicknessCmd = nullptr;
    /// Command to set the Cible Density Fraction
    G4UIcmdWithADouble *fGeometryCibleDensityFractionCmd = nullptr;


};

#endif
