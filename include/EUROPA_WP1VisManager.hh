#ifndef EUROPA_WP1VisManager_h
#define EUROPA_WP1VisManager_h 1

/**
 * @brief Custom visualization manager for the PALLAS project.
 * @class EUROPA_WP1VisManager
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @date 2026
 * @copyright EUROPA Project - WP1
 * 
 * This class extends Geant4's `G4VisManager` to configure and register
 * the set of visualization drivers used in the PALLAS simulation.
 * It allows the simulation to support various graphics systems such as
 * OpenGL, HepRApp, VRML, or other Geant4 visualization backends.
 * The class primarily overrides the `RegisterGraphicsSystems()` method
 * to add the desired set of graphics systems when the visualization
 * manager is initialized.
 */

#include "G4VisManager.hh"

class EUROPA_WP1VisManager : public G4VisManager
{
public:
    /// Constructor: prepares the visualization manager
    EUROPA_WP1VisManager();

private:
    /**
     * @brief Registers the visualization drivers to be used.
     *
     * This method is called internally by `G4VisManager::Initialize()`.
     * Override this method to register custom or specific graphics systems.
     */
    void RegisterGraphicsSystems() override;
};

#endif // EUROPA_WP1VisManager_h
