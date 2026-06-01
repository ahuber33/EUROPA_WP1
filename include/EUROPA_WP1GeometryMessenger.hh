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
#include "G4UIcmdWithAnInteger.hh"               // for G4UIcmdWithAnInteger
#include "G4UIcmdWithoutParameter.hh"            // for G4UIcmdWithoutParameter
#include "G4UIdirectory.hh"                      // for G4UIdirectory
#include "EUROPA_WP1GeometryConstruction.hh" // for EUROPA_WP1GeometryConstruction

class G4UIcmdWithADoubleAndUnit;
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
    /// Command to set the Q1 Length
    G4UIcmdWithADoubleAndUnit *fGeometryQ1LengthCmd = nullptr;
    /// Command to set the Q2 Length
    G4UIcmdWithADoubleAndUnit *fGeometryQ2LengthCmd = nullptr;
    /// Command to set the Q3 Length
    G4UIcmdWithADoubleAndUnit *fGeometryQ3LengthCmd = nullptr;
    /// Command to set the Q4 Length
    G4UIcmdWithADoubleAndUnit *fGeometryQ4LengthCmd = nullptr;
    /// Command to set the Source -- Q1 Distance
    G4UIcmdWithADoubleAndUnit *fGeometrySourceQ1DistanceCmd = nullptr;
    /// Command to set the Q1 -- Q2 Distance
    G4UIcmdWithADoubleAndUnit *fGeometryQ1Q2DistanceCmd = nullptr;
    /// Command to set the Q2 -- Q3 Distance
    G4UIcmdWithADoubleAndUnit *fGeometryQ2Q3DistanceCmd = nullptr;
    /// Command to set the Q3 -- Q4 Distance
    G4UIcmdWithADoubleAndUnit *fGeometryQ3Q4DistanceCmd = nullptr;
    /// Command to set the Source -- Collimators Distance
    G4UIcmdWithADoubleAndUnit *fGeometrySourceCollimatorsDistanceCmd = nullptr;
    /// Command to set the Slit Opening Vertical Collimator
    G4UIcmdWithADoubleAndUnit *fGeometryVerticalSlitOpeningCmd = nullptr;
    /// Command to set the Slit Opening Horizontal Collimator
    G4UIcmdWithADoubleAndUnit *fGeometryHorizontalSlitOpeningCmd = nullptr;


};

#endif
